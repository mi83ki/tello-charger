#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

/* ---------- ペイロード構造体 ---------- */
/**
 * @brief 充電状態
 */
struct ChargeStatus {
  bool charge;
  float current;
  float chargingTime;
  bool isStartChargeExecuting;
  bool isStopChargeExecuting;
  bool isPowerOnExecuting;
  bool valid;
};

/**
 * @brief リクエストヘッダ
 */
struct RequestHeader {
  String timestamp;
  String req_id;
  bool valid;
};

/**
 * @brief レスポンスヘッダ
 */
struct ResponseHeader {
  String req_id;
  String status;
  String error;
  bool valid;
};

/* ---------- 送信用ビルド関数（構造体 → JSON） ---------- */
String buildChargeStatusJson(const ChargeStatus& src);
String buildChargeStartRequestJson(const RequestHeader& src);
String buildChargeStopRequestJson(const RequestHeader& src);
String buildPowerOnRequestJson(const RequestHeader& src);
String buildChargeStartResponseJson(const ResponseHeader& src);
String buildChargeStopResponseJson(const ResponseHeader& src);
String buildPowerOnResponseJson(const ResponseHeader& src);

/* ---------- 受信用パース関数（JSON →構造体） ---------- */
ChargeStatus parseChargeStatusJson(const String& json);
RequestHeader parseChargeStartRequestJson(const String& json);
RequestHeader parseChargeStopRequestJson(const String& json);
RequestHeader parsePowerOnRequestJson(const String& json);
ResponseHeader parseChargeStartResponseJson(const String& json);
ResponseHeader parseChargeStopResponseJson(const String& json);
ResponseHeader parsePowerOnResponseJson(const String& json);
