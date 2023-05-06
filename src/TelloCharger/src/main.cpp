#include <Arduino.h>

#include <Log.h>
#include <M5Atom.h>
#include <WiFiESP32.h>

#include "HttpServer.h"
#include "ChargeController.h"
#include "ssid.h"


#define HTTP_PORT 80

const char *wifi_ssid = WIFI_SSID;
const char *wifi_password = WIFI_PASSWORD;

WiFiESP32 *wifi;
HttpServer *server;
ChargeController *controller;

void setup()
{
  wifi = new WiFiESP32(wifi_ssid, wifi_password);
  server = new HttpServer(HTTP_PORT);
  controller = new ChargeController();
}

void loop()
{
  if (wifi->healthCheck())
  {
    server->begin();
  }
  else
  {
    server->end();
  }

  M5.update();
  if (M5.Btn.wasPressed())
  {
    controller->advanceChargeStep();
  }
  if (Serial.available())
  {
    // 文字が届いていればを読み込む
    char input = Serial.read();
    controller->wasdControl(input);
  }
  controller->loop();
  delay(10);
}
