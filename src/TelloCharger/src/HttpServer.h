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

class HttpServer
{
public:
  HttpServer(uint16_t);
  ~HttpServer();
  void begin(void);
  void end(void);

private:
  static void _notFound(AsyncWebServerRequest *);
  static void _onChargeGet(AsyncWebServerRequest *);
  static void _onChargePut(AsyncWebServerRequest *, JsonVariant &);
  static void _onPowerPut(AsyncWebServerRequest *, JsonVariant &);
  void _defineApi(void);

  /** HTTPサーバーインスタンス */
  AsyncWebServer _server;
  /** サーバーが待ち受け中かどうか */
  bool _bAvailable;
};
