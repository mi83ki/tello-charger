/**
 * @file ChargeManager.cpp
 * @brief 充電管理クラス
 * @author Tatsuya Miyazaki
 * @date 2023/5/6
 *
 * @details 充電指示や電流量を管理するクラス
 */

#include "ChargeManager.h"

#include <Log.h>

/**
 * @brief Construct a new Servo Controller:: Servo Controller object
 *
 */
ChargeManager::ChargeManager()
    : _controller(ChargeController()), _ina(CurrentReader()) {}

/**
 * @brief Destroy the Servo Controller:: Servo Controller object
 *
 */
ChargeManager::~ChargeManager() {}

/**
 * @brief 充電を開始する
 *
 */
void ChargeManager::startCharge(void) { _controller.startCharge(); }

/**
 * @brief 充電を停止する
 *
 */
void ChargeManager::stopCharge(void) { _controller.stopCharge(); }

/**
 * @brief ドローンの電源をONにする
 *
 */
void ChargeManager::powerOnDrone(void) { _controller.powerOnDrone(); }

/**
 * @brief WASDでサーボの角度を微調整する
 *
 * @param input キーボード入力
 */
void ChargeManager::wasdControl(char input) { _controller.wasdControl(input); }

/**
 * @brief 充電中かどうか
 *
 * @return true 充電中
 * @return false 充電中でない
 */
bool ChargeManager::isCharging(void) { return _controller.isCharging(); }

/**
 * @brief 初期位置かどうか
 *
 * @return true 初期位置
 * @return false 初期位置でない
 */
bool ChargeManager::isInitPos(void) { return _controller.isInitPos(); }

/**
 * @brief 充電開始処理中かどうか
 *
 * @return true
 * @return false
 */
bool ChargeManager::isStartChargeExecuting(void) {
  return _controller.isStartChargeExecuting();
}

/**
 * @brief 充電終了処理中かどうか
 *
 * @return true
 * @return false
 */
bool ChargeManager::isStopChargeExecuting(void) {
  return _controller.isStopChargeExecuting();
}

/**
 * @brief ドローンの起動処理中かどうか
 *
 * @return true
 * @return false
 */
bool ChargeManager::isPowerOnExecuting(void) {
  return _controller.isPowerOnExecuting();
}

/**
 * @brief 充電時間を取得する
 *
 * @return uint32_t 充電時間[ms]
 */
uint32_t ChargeManager::getChargeTimeMillis(void) {
  return _controller.getChargeTimeMillis();
}

/**
 * @brief 電流値を取得する
 *
 * @return float 電流値[mA]
 */
float ChargeManager::getCurrent(void) { return _ina.getCurrent(); }

/**
 * @brief ループ処理
 *
 */
void ChargeManager::loop(void) { _controller.loop(); }

/**
 * @brief サーボ角度の文字列を取得する
 *
 * @return String サーボ角度の文字列
 */
String ChargeManager::toString(void) { return _controller.toString(); }
