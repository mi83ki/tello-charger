/**
 * @file HttpServer.cpp
 * @brief HTTPサーバー管理クラス
 * @author Tatsuya Miyazaki
 * @date 2023/5/5
 *
 * @details HTTPサーバーを管理するクラス
 */

#include "HttpServer.h"

ChargeManager *HttpServer::_charger = nullptr;

/**
 * @brief Construct a new Http Server:: Http Server object
 *
 * @param httpPort 待ち受けポート番号
 */
HttpServer::HttpServer(uint16_t httpPort, ChargeManager *charger)
  : _server(AsyncWebServer(httpPort)), _bAvailable(false)
{
  _charger = charger;
  _defineApi();
  _server.onNotFound(_notFound);
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
}

/**
 * @brief Destroy the Http Server:: Http Server object
 *
 */
HttpServer::~HttpServer() {}

/**
 * @brief HTTPサーバーの待ち受けを開始する
 *
 */
void HttpServer::begin(void)
{
  if (!_bAvailable)
  {
    logger.info("HttpServer::begin(): Start Http Server");
    _server.begin();
    _bAvailable = true;
  }
}

/**
 * @brief HTTPサーバーの待ち受けを停止する
 *
 */
void HttpServer::end(void)
{
  if (_bAvailable)
  {
    logger.info("HttpServer::end(): Stop Http Server");
    _server.end();
    _bAvailable = false;
  }
}

/**
 * @brief URLで指定されたパスに合致するものが見つからなかったときの振る舞い
 * HTTP_OPTIONSのときだけ200を返すようにしています。これは、CORS対応であり、
 * CORSプリフライトリクエストを正常処理とするためのものです。
 *
 * @param request
 */
void HttpServer::_notFound(AsyncWebServerRequest *request)
{
  if (request->method() == HTTP_OPTIONS)
  {
    request->send(200);
  }
  else
  {
    request->send(404);
  }
}

/**
 * @brief 充電状態取得要求
 *
 * @param request
 */
void HttpServer::_onChargeGet(AsyncWebServerRequest *request)
{
  AsyncJsonResponse *response = new AsyncJsonResponse();
  JsonObject root = response->getRoot();
  root["charge"] = _charger->isCharging();
  root["current"] = 0;
  root["chargingTime"] = _charger->getChargeTimeMillis();
  response->setLength();
  request->send(response);
  String str = "";
  serializeJson(root, str);
  logger.info("onChargeGet: send " + str);
}

/**
 * @brief 充電開始/停止要求
 *
 * @param request
 * @param json
 */
void HttpServer::_onChargePut(AsyncWebServerRequest *request, JsonVariant &json)
{
  JsonObject jsonObj = json.as<JsonObject>();
  String str = "";
  serializeJson(jsonObj, str);
  logger.info("onChargePut: recieve " + str);
  if (jsonObj.containsKey("charge"))
  {
    bool charge = jsonObj["charge"];
    if (charge)
    {
      _charger->startCharge();
      while (!_charger->isCharging())
      {
        delay(500);
        logger.info(".");
      }
    }
    else
    {
      _charger->stopCharge();
      while (!_charger->isInitPos())
      {
        delay(500);
        logger.info(".");
      }
    }
    // レスポンス
    AsyncJsonResponse *response = new AsyncJsonResponse();
    JsonObject root = response->getRoot();
    root["charge"] = _charger->isCharging();
    root["current"] = 0;
    root["chargingTime"] = _charger->getChargeTimeMillis();
    response->setLength();
    request->send(response);
    str = "";
    serializeJson(root, str);
    logger.info("onChargePut: send " + str);
  }
  else
  {
    // chargeのキーがない
    request->send(400);
    logger.info("onChargePut: send 400 Bad Request");
  }
}

/**
 * @brief Tello電源ON要求
 *
 * @param request
 * @param json
 */
void HttpServer::_onPowerPut(AsyncWebServerRequest *request, JsonVariant &json)
{
  JsonObject jsonObj = json.as<JsonObject>();
  String str = "";
  serializeJson(jsonObj, str);
  logger.info("onPowerPut: recieve " + str);
  if (jsonObj.containsKey("power"))
  {
    bool power = jsonObj["power"];
    if (power)
    {
      _charger->powerOnTello();
      while (!_charger->isPowerOnFinished())
      {
        delay(500);
        logger.info(".");
      }
    }

    AsyncJsonResponse *response = new AsyncJsonResponse();
    JsonObject root = response->getRoot();
    root["power"] = power;
    response->setLength();
    request->send(response);
    str = "";
    serializeJson(root, str);
    logger.info("onPowerPut: send " + str);
  }
  else
  {
    // powerのキーがない
    request->send(400);
    logger.info("onPowerPut: send 400 Bad Request");
  }
}

/**
 * @brief APIの定義
 *
 */
void HttpServer::_defineApi(void)
{
  _server.on("/charge", HTTP_GET, _onChargeGet);
  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/charge", _onChargePut);
  _server.addHandler(handler);
  handler = new AsyncCallbackJsonWebHandler("/power", _onPowerPut);
  _server.addHandler(handler);
}
