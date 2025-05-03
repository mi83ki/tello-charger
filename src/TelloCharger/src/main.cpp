#include <Arduino.h>
#include <M5Unified.h>

#ifdef M5STACK_M5ATOMS3
#define USE_M5ATOM_S3
#endif
#include <Log.h>
#include <MQTTClientESP32.h>
#include <MacUtils.h>
#include <WiFiESP32.h>

#include "ChargeController/ChargeController.h"
#include "HttpServer/DroneChargerProtocol.h"
#include "HttpServer/HttpServer.h"
#include "HttpServer/MqttHandler.h"
#include "ssid.h"

/** WiFiのSSID（ssid.h（git管理対象外）にて定義） */
const char *wifiSSID = WIFI_SSID;
/** WiFiのパスワード（ssid.h（git管理対象外）にて定義） */
const char *wifiPassword = WIFI_PASSWORD;
/** WebAPIの待ち受けポート */
const uint8_t httpPort = 80;

/** WiFi接続インスタンス */
WiFiESP32 *wifi;
/** MQTTクライアントインスタンス */
MQTTClientESP32 *mqttClient;
/** MQTTハンドラインスタンス */
MqttHandler *mqttHandler;
/** WebAPIインスタンス */
HttpServer *server;
/** 充電制御インスタンス */
ChargeController *charger;

/**
 * @brief WiFi通信用Task
 */
void taskWifi(void *) {
  wifi = new WiFiESP32(wifiSSID, wifiPassword);

  // for fixed IP Address
  IPAddress ip(MY_FIXED_IP_1, MY_FIXED_IP_2, MY_FIXED_IP_3, MY_FIXED_IP_4);
  IPAddress gateway(MY_FIXED_IP_1, MY_FIXED_IP_2, MY_FIXED_IP_3, GATEWAY_IP_4);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);

  // WiFi接続
  if (!wifi->begin()) {
    logger.info("WiFi Init Fail");
    // ESP.restart();
  }

  server = new HttpServer(httpPort, charger);
  mqttClient = new MQTTClientESP32(MQTT_HOST, MQTT_PORT, MQTT_BUFFER_SIZE);
  mqttHandler = new MqttHandler(mqttClient, charger, DEFAULT_MAC_ADDRESS);

  while (true) {
    if (wifi->healthCheck()) {
      server->begin();
      mqttHandler->loop();
    } else {
      server->end();
    }
  }
  vTaskDelay(1);
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  charger = new ChargeController();
  // WiFi通信用Taskを起動 Core 0
  xTaskCreatePinnedToCore(taskWifi, "taskWifi", 4096, NULL, 1, NULL, 0);
}

void loop() {
  // ボタン押下時の処理
  M5.update();
  if (M5.BtnA.wasPressed()) {
    if (charger->isReleaseDrone())
      charger->startCharge();
    else
      charger->stopCharge();
  }

  // キーボードからのWASD入力時の処理
  if (Serial.available()) {
    // 文字が届いていればを読み込む
    char input = Serial.read();
    charger->wasdControl(input);
  }

  // 制御部のループ処理
  charger->loop();
  delay(10);
}
