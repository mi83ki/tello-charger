/**
 * @file HttpServer.h
 * @brief HTTPサーバー管理クラス
 * @author Tatsuya Miyazaki
 * @date 2023/5/5
 *
 * @details HTTPサーバーを管理するクラス
 */

#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Log.h>

#include "../ChargeManager/ChargeManager.h"

class HttpServer {
 public:
  HttpServer(uint16_t, ChargeManager *);
  ~HttpServer();
  void begin(void);
  void end(void);

 private:
  static void _notFound(AsyncWebServerRequest *);
  static void _onChargeGet(AsyncWebServerRequest *);
  static void _onChargePut(AsyncWebServerRequest *, JsonVariant &);
  static void _onPowerOnPut(AsyncWebServerRequest *);
  void _defineApi(void);

  /** HTTPサーバーインスタンス */
  AsyncWebServer _server;
  /** 充電管理部のインスタンス */
  static ChargeManager *_charger;
  /** サーバーが待ち受け中かどうか */
  bool _bAvailable;
};
