#include "MqttHandler.h"

#include <Log.h>

#include "../HttpServer/DroneChargerProtocol.h"

//--------------------------------------------------------------
// コールバック用に“いま動いている唯一のハンドラ”を保持
namespace {
MqttHandler* g_handler = nullptr;
}  // namespace

/**
 * @brief コンストラクタ
 * @param client    初期化済み MQTTClientESP32
 * @param charger   充電制御オブジェクト
 * @param macAddr   MAC アドレス文字列（例：DEFAULT_MAC_ADDRESS）
 *
 * - 必要なトピックを Subscribe
 * - コールバックを登録
 * - ステータス Publish 周期は 500 ms 固定
 */
MqttHandler::MqttHandler(MQTTClientESP32* client, ChargeController* charger,
                         const String& macAddr)
    : client_(client),
      charger_(charger),
      timer_(Timer(CYCLE_MS)),
      base_("drone-charger/" + macAddr + "/") {
  g_handler = this;
  subscribe_();
  attachCallback_();
}

//--------------------------------------------------------------
/**
 * @brief 周期処理（loop から呼び出し）
 *
 * - 500 ms ごとに MQTT 健全性を確認
 * - OK なら `/charge/status` を Publish
 * - 高速に呼んでも問題ない非ブロッキング実装
 */
void MqttHandler::loop() {
  if (timer_.isCycleTime()) {
    if (client_ && client_->healthCheck()) {
      publishStatus_();
    }
  }
}

// =====================  private  ==============================
/**
 * @brief 各種トピックを Subscribe
 */
void MqttHandler::subscribe_() {
  client_->subscribe(base_ + "charge/start/request");
  client_->subscribe(base_ + "charge/stop/request");
  client_->subscribe(base_ + "power/on/request");
}

/**
 * @brief MQTT ライブラリにコールバックを登録
 */
void MqttHandler::attachCallback_() {
  client_->registOnMessageCallback(onMessageStatic_);
}

/**
 * @brief /charge/status を Publish
 */
void MqttHandler::publishStatus_() {
  ChargeStatus st{
      charger_->isCharging(),
      charger_->getCurrent(),
      charger_->getChargeTimeSec(),
      charger_->isStartChargeExecuting(),
      charger_->isStopChargeExecuting(),
      charger_->isPowerOnExecuting(),
  };
  String payload = buildChargeStatusJson(st);
  client_->publish(base_ + "charge/status", payload);
  logger.debug("Publish status: " + payload);
}

// ---------- メッセージコールバック関連 -------------------------
/**
 * @brief MQTT ライブラリが呼び出すスタティックコールバック
 *
 * - 登録表から対応する MqttHandler インスタンスを探し
 *   `onMessage_()` へデリゲート
 */
void MqttHandler::onMessageStatic_(String topic, String payload) {
  if (!g_handler) return;
  g_handler->onMessage_(topic, payload);
}

/**
 * @brief トピックごとの処理をディスパッチ
 */
void MqttHandler::onMessage_(const String& topic, const String& payload) {
  logger.info("Received: " + topic + " | " + payload);

  if (topic == base_ + "charge/start/request") {
    handleChargeStart_(payload);
  } else if (topic == base_ + "charge/stop/request") {
    handleChargeStop_(payload);
  } else if (topic == base_ + "power/on/request") {
    handlePowerOn_(payload);
  } else {
    logger.warn("Unhandled topic: " + topic);
  }
}

// -------------- 個別ハンドラ -------------------
/**
 * @brief 充電開始要求を処理
 */
void MqttHandler::handleChargeStart_(const String& payload) {
  RequestHeader req = parseChargeStartRequestJson(payload);
  ResponseHeader res = {req.req_id, "SUCCESS", "", true};

  if (!req.valid) {
    res.status = "FAILURE";
    res.error = "ChargeStartRequest JSON が不正";
    logger.error(res.error + ": " + payload);
  } else {
    charger_->startCharge();
  }

  client_->publish(base_ + "charge/start/response",
                   buildChargeStartResponseJson(res));
}

/**
 * @brief 充電停止要求を処理
 */
void MqttHandler::handleChargeStop_(const String& payload) {
  RequestHeader req = parseChargeStopRequestJson(payload);
  ResponseHeader res = {req.req_id, "SUCCESS", "", true};

  if (!req.valid) {
    res.status = "FAILURE";
    res.error = "ChargeStopRequest JSON が不正";
    logger.error(res.error + ": " + payload);
  } else {
    charger_->stopCharge();
  }

  client_->publish(base_ + "charge/stop/response",
                   buildChargeStopResponseJson(res));
}

/**
 * @brief 電源 ON 要求を処理
 */
void MqttHandler::handlePowerOn_(const String& payload) {
  RequestHeader req = parsePowerOnRequestJson(payload);
  ResponseHeader res = {req.req_id, "SUCCESS", "", true};

  if (!req.valid) {
    res.status = "FAILURE";
    res.error = "PowerOnRequest JSON が不正";
    logger.error(res.error + ": " + payload);
  } else {
    charger_->powerOnDrone();
  }

  client_->publish(base_ + "power/on/response", buildPowerOnResponseJson(res));
}
