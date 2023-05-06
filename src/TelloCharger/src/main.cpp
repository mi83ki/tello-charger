#include <Arduino.h>

#include <Log.h>
#include <M5Atom.h>
#include <WiFiESP32.h>

#include "HttpServer.h"
#include "ServoController.h"

#define HTTP_PORT 80


WiFiESP32 *wifi;
HttpServer *server;

ServoController *servo;
static uint8_t chargeStep = 0;

// MOSFET
#define CHARGE_CONTROL_PIN G22
Timer chargeTimer = Timer(3000);
bool enableCharge = false;

void chargeOn(void)
{
  digitalWrite(CHARGE_CONTROL_PIN, 1);
  enableCharge = true;
}

void chargeOff(void)
{
  digitalWrite(CHARGE_CONTROL_PIN, 0);
  enableCharge = false;
}

bool isEnableCharge(void)
{
  return enableCharge;
}

void setup()
{
  wifi = new WiFiESP32(wifi_ssid, wifi_password);
  server = new HttpServer(HTTP_PORT);
  servo = new ServoController();
  
  pinMode(CHARGE_CONTROL_PIN, OUTPUT);
  chargeOff();
  delay(3000);
  chargeOn();
  logger.info("chargeOn");
  servo->releaseDrone();
  servo->disconnectUsb();
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
    chargeStep = (chargeStep + 1) % 4;
    switch (chargeStep)
    {
      case 0:
        // ドローンをリリースする
        servo->releaseDrone();
        break;
      case 1:
        // ドローンをキャッチする
        servo->catchDrone();
        break;
      case 2:
        // 充電開始する
        servo->connectUsb();
        break;
      case 3:
        // 充電終了する
        servo->disconnectUsb();
        break;
      default:
        chargeStep = 0;
        break;
    }
    logger.info("chargeStep = " + String(chargeStep) + ", " + servo->toString());
  }
  if (Serial.available())
  {                             // シリアルデータが利用可能であれば
    char input = Serial.read(); // シリアルデータを読み込む
    if (input == 'w')
    {                  // もし 'w' キーが押されたら
      servo->setServoUsb(servo->getServoUsb() + 1);
    }
    else if (input == 's')
    {                  // もし 's' キーが押されたら
      servo->setServoUsb(servo->getServoUsb() - 1);
    }
    else if (input == 'a')
    {                   // もし 'a' キーが押されたら
      servo->setServoCatch(servo->getServoCatch() - 1);
    }
    else if (input == 'd')
    {                   // もし 'd' キーが押されたら
      servo->setServoCatch(servo->getServoCatch() + 1);
    }
    logger.info("key = " + String(input) + ", " + servo->toString());
  }
  servo->loop();
  delay(10);
}
