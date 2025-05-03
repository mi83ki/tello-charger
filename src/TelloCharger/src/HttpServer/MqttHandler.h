#pragma once
#include <Arduino.h>
#include <MQTTClientESP32.h>
#include <Timer.h>

#include "../ChargeController/ChargeController.h"

/**
 * @class MqttHandler
 * @brief MQTT 通信を一元管理するクラス
 *
 * - コマンドトピックの Subscribe / 解析
 * - ChargeController への指示出し
 * - 応答メッセージおよび充電ステータスの Publish
 */
class MqttHandler {
 public:
  /**
   * @brief コンストラクタ
   * @param client   初期化済み MQTTClientESP32
   * @param charger  充電制御オブジェクト
   * @param macAddr  MAC アドレス文字列（トピックプレフィクス生成用）
   */
  MqttHandler(MQTTClientESP32* client, ChargeController* charger,
              const String& macAddr);

  /**
   * @brief 周期処理（非ブロッキング）
   * @note loop() から頻繁に呼び出してください
   */
  void loop();

 private:
  /* ---------------- 内部状態 ---------------- */
  MQTTClientESP32* client_;    ///< MQTT クライアント
  ChargeController* charger_;  ///< 充電制御オブジェクト
  Timer timer_;                ///< タイマ
  String base_;                ///< "drone-charger/<MAC>/" プレフィクス
  uint32_t prevMs_{0};
  static constexpr uint32_t CYCLE_MS = 500;  ///< ステータス送信周期[ms]

  /* -------------- 内部ユーティリティ -------------- */
  void publishStatus_();   ///< /charge/status を Publish
  void subscribe_();       ///< 必要トピックを Subscribe
  void attachCallback_();  ///< MQTT コールバック登録

  /* -------------- コールバック -------------- */
  static void onMessageStatic_(String topic, String payload);
  void onMessage_(const String& topic, const String& payload);

  /* -------------- 個別ハンドラ -------------- */
  void handleChargeStart_(const String& payload);  ///< 充電開始要求
  void handleChargeStop_(const String& payload);   ///< 充電停止要求
  void handlePowerOn_(const String& payload);      ///< 電源 ON 要求
};
