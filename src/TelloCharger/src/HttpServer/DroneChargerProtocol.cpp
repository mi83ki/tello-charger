/**
 * @file DroneChargerProtocol.cpp
 * @brief Drone‑Charger MQTT プロトコル － JSON 変換ユーティリティ（実装）
 */
#include "DroneChargerProtocol.h"

/* ==============================================================
 *  内部ユーティリティ
 * ==============================================================*/
namespace {
template<size_t CAP = 192>
bool deserialize(const String& json, StaticJsonDocument<CAP>& doc)
{
  return !deserializeJson(doc, json);
}
}

/* ==============================================================
 *  ChargeStatus
 * ==============================================================*/
/**
 * @brief ChargeStatus 構造体を JSON 文字列にシリアライズ
 * @param[in] s 送信する ChargeStatus
 * @return シリアライズ済み JSON
 */
String buildChargeStatusJson(const ChargeStatus& s)
{
  StaticJsonDocument<192> doc;
  doc["charge"]                 = s.charge;
  doc["current"]                = s.current;
  doc["chargingTime"]           = s.chargingTime;
  doc["isStartChargeExecuting"] = s.isStartChargeExecuting;
  doc["isStopChargeExecuting"]  = s.isStopChargeExecuting;
  doc["isPowerOnExecuting"]     = s.isPowerOnExecuting;

  String out;
  serializeJson(doc, out);
  return out;
}

/**
 * @brief /charge/status JSON を ChargeStatus 構造体へデシリアライズ
 * @param[in] json 受信した JSON 文字列
 * @retval ChargeStatus.valid = true  パース成功
 * @retval ChargeStatus.valid = false パース失敗
 */
ChargeStatus parseChargeStatusJson(const String& json)
{
  ChargeStatus s{false, 0, 0, false, false, false, false};
  StaticJsonDocument<192> doc;
  if (!deserialize(json, doc)) return s;

  s.charge                 = doc["charge"]                 | false;
  s.current                = doc["current"]                | 0.0;
  s.chargingTime           = doc["chargingTime"]           | 0.0;
  s.isStartChargeExecuting = doc["isStartChargeExecuting"] | false;
  s.isStopChargeExecuting  = doc["isStopChargeExecuting"]  | false;
  s.isPowerOnExecuting     = doc["isPowerOnExecuting"]     | false;
  s.valid = true;
  return s;
}

/* ==============================================================
 *  Request 共通
 * ==============================================================*/
/**
 * @brief 共通 RequestHeader → JSON （内部ヘルパ）
 */
static String buildRequestJson(const RequestHeader& h)
{
  StaticJsonDocument<128> doc;
  doc["timestamp"] = h.timestamp;
  doc["req_id"]    = h.req_id;
  String out;
  serializeJson(doc, out);
  return out;
}

/**
 * @brief 共通 RequestHeader パーサ（内部ヘルパ）
 */
static RequestHeader parseRequestJson(const String& json)
{
  RequestHeader h{"", "", false};
  StaticJsonDocument<128> doc;
  if (!deserialize(json, doc)) return h;
  if (!doc.containsKey("req_id")) return h;
  h.req_id    = doc["req_id"].as<String>();
  if (!doc.containsKey("timestamp")) return h;
  h.timestamp = doc["timestamp"].as<String>();
  h.valid     = true;
  return h;
}

/* ---- build wrappers ---- */
String buildChargeStartRequestJson(const RequestHeader& h){ return buildRequestJson(h); }
String buildChargeStopRequestJson (const RequestHeader& h){ return buildRequestJson(h); }
String buildPowerOnRequestJson    (const RequestHeader& h){ return buildRequestJson(h); }

/* ---- parse wrappers ---- */
RequestHeader parseChargeStartRequestJson(const String& j){ return parseRequestJson(j); }
RequestHeader parseChargeStopRequestJson (const String& j){ return parseRequestJson(j); }
RequestHeader parsePowerOnRequestJson    (const String& j){ return parseRequestJson(j); }

/* ==============================================================
 *  Response 共通
 * ==============================================================*/
/**
 * @brief 共通 ResponseHeader → JSON （内部ヘルパ）
 */
static String buildResponseJson(const ResponseHeader& h)
{
  StaticJsonDocument<128> doc;
  doc["req_id"] = h.req_id;
  doc["status"] = h.status;
  doc["error"]  = h.error;
  String out;
  serializeJson(doc, out);
  return out;
}

/**
 * @brief 共通 ResponseHeader パーサ（内部ヘルパ）
 */
static ResponseHeader parseResponseJson(const String& json)
{
  ResponseHeader h{"", "", "", false};
  StaticJsonDocument<128> doc;
  if (!deserialize(json, doc)) return h;
  if (!doc.containsKey("req_id") || !doc.containsKey("status")) return h;

  h.req_id = doc["req_id"].as<String>();
  h.status = doc["status"].as<String>();
  h.error  = doc["error"] | "";
  h.valid  = true;
  return h;
}

/* ---- build wrappers ---- */
String buildChargeStartResponseJson(const ResponseHeader& h){ return buildResponseJson(h); }
String buildChargeStopResponseJson (const ResponseHeader& h){ return buildResponseJson(h); }
String buildPowerOnResponseJson    (const ResponseHeader& h){ return buildResponseJson(h); }

/* ---- parse wrappers ---- */
ResponseHeader parseChargeStartResponseJson(const String& j){ return parseResponseJson(j); }
ResponseHeader parseChargeStopResponseJson (const String& j){ return parseResponseJson(j); }
ResponseHeader parsePowerOnResponseJson    (const String& j){ return parseResponseJson(j); }
