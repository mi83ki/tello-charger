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
  root["charge"] = true;
  root["current"] = 10;
  root["chargingTime"] = millis();
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
  bool charge = jsonObj["charge"] | false;
  if (charge)
    _charger->startCharge();
  else
    _charger->stopCharge();
  String str = "";
  serializeJson(jsonObj, str);
  logger.info("onChargePut: recieve " + str);

  AsyncJsonResponse *response = new AsyncJsonResponse();
  JsonObject root = response->getRoot();
  root["charge"] = true;
  root["current"] = 10;
  root["chargingTime"] = millis();
  response->setLength();
  request->send(response);
  str = "";
  serializeJson(root, str);
  logger.info("onChargePut: send " + str);
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
  bool power = jsonObj["power"] | false;
  if (power)
    _charger->powerOnTello();
  String str = "";
  serializeJson(jsonObj, str);
  logger.info("onPowerPut: recieve " + str);

  AsyncJsonResponse *response = new AsyncJsonResponse();
  JsonObject root = response->getRoot();
  root["power"] = true;
  response->setLength();
  request->send(response);
  str = "";
  serializeJson(root, str);
  logger.info("onPowerPut: send " + str);
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