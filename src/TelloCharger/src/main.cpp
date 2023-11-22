#include <Arduino.h>
#include <Log.h>
#include <M5Atom.h>
#include <WiFiESP32.h>

#include "ChargeController/ChargeController.h"
#include "HttpServer/HttpServer.h"
#include "ssid.h"

/** WiFiのSSID（ssid.h（git管理対象外）にて定義） */
const char *wifiSSID = WIFI_SSID;
/** WiFiのパスワード（ssid.h（git管理対象外）にて定義） */
const char *wifiPassword = WIFI_PASSWORD;
/** WebAPIの待ち受けポート */
const uint8_t httpPort = 80;

/** WiFi接続インスタンス */
WiFiESP32 *wifi;
/** WebAPIインスタンス */
HttpServer *server;
/** 充電制御インスタンス */
ChargeController *charger;

void setup() {
  M5.begin();
  // for fixed IP Address
  IPAddress ip(MY_FIXED_IP_1, MY_FIXED_IP_2, MY_FIXED_IP_3, MY_FIXED_IP_4);
  IPAddress gateway(MY_FIXED_IP_1, MY_FIXED_IP_2, MY_FIXED_IP_3, GATEWAY_IP_4);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);
  charger = new ChargeController();
  wifi = new WiFiESP32(wifiSSID, wifiPassword);
  server = new HttpServer(httpPort, charger);
}

void loop() {
  // WebAPIサーバー接続監視
  if (wifi->healthCheck())
    server->begin();
  else
    server->end();

  // ボタン押下時の処理
  M5.update();
  if (M5.Btn.wasPressed()) {
    if (charger->isCatchDrone())
      charger->stopCharge();
    else
      charger->startCharge();
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
