#include "MessageBuilder.h"

/**
 * @brief 充電状態 JSON を生成する
 *
 * @param charge                 充電中フラグ
 * @param current                充電電流 [mA]
 * @param chargingTime           充電経過時間 [秒]
 * @param isStartChargeExecuting 充電開始処理実行中フラグ
 * @param isStopChargeExecuting  充電停止処理実行中フラグ
 * @param isPowerOnExecuting     電源 ON 処理実行中フラグ
 * @return String                シリアライズ済み JSON
 */
String buildChargeStatusJson(bool charge, float current, float chargingTime,
                             bool isStartChargeExecuting,
                             bool isStopChargeExecuting,
                             bool isPowerOnExecuting) {
  // 生成する JSON が 100 バイト以下なので 128 バイト確保で十分
  DynamicJsonDocument doc(128);

  doc["charge"] = charge;
  doc["current"] = current;
  doc["chargingTime"] = chargingTime;
  doc["isStartChargeExecuting"] = isStartChargeExecuting;
  doc["isStopChargeExecuting"] = isStopChargeExecuting;
  doc["isPowerOnExecuting"] = isPowerOnExecuting;

  String json;
  serializeJson(doc, json);
  return json;
}