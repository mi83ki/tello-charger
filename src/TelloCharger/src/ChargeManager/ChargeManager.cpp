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
    : _controller(ChargeController()),
      _current(CurrentReader(500)),
      _isServoMovingPrevious(false),
      _checkServoStep(0),
      _checkServoTimer(Timer()) {}

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
bool ChargeManager::isCharging(void) {
  return _controller.isCharging() /* && isChargingCurrent() */;
}

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
float ChargeManager::getCurrent(void) { return _current.getCurrent(); }

/**
 * @brief 充電中の電流かどうか
 *
 * @return true 充電中である
 * @return false
 */
bool ChargeManager::isChargingCurrent(void) {
  return getCurrent() >= CHARGE_CURRENT_CHARGING_THREASHOLD;
}

/**
 * @brief 満充電かどうか
 *
 * @return true 満充電である
 * @return false
 */
bool ChargeManager::isFullCharge(void) {
  return _controller.isCharging() && isChargingCurrent() &&
         getCurrent() < CHARGE_CURRENT_STOP_THREASHOLD &&
         _controller.getChargeTimeMillis() > 30000;
}

/**
 * @brief アームをリリースすべきかどうか
 *
 * @return true
 * @return false
 */
bool ChargeManager::haveToRelease(void) {
  return _controller.isCharging() && !isChargingCurrent() &&
         _controller.getChargeTimeMillis() > 60000;
}

/**
 * @brief サーボモータが稼働中かどうか
 *
 * @return true
 * @return false
 */
bool ChargeManager::isServoMoving(void) {
  return !_controller.isCharging() &&
         getCurrent() >= SERVO_CURRENT_MOVING_THREASHOLD;
}

/**
 * @brief サーボモータが過電流タイムアウトしたかどうか
 *
 */
bool ChargeManager::checkServoTimeout(void) {
  bool ret = false;
  static Timer timer = Timer(500);
  if (timer.isCycleTime()) {
    logger.debug("checkServoTimeout(): current = " + String(getCurrent()) +
                 ", isMoving = " + String(isServoMoving()));
  }
  switch (_checkServoStep) {
    case 0:
      if (_controller.isTargetAngle() && isServoMoving()) {
        // サーボ指示値が収束した状態で過電流を検知した
        _checkServoTimer.startTimer();
        _checkServoStep++;
        logger.debug("checkServoTimeout(): start timer");
      }
      break;
    case 1:
      if (_checkServoTimer.getTime() >= SERVO_MOVING_TIMEOUT) {
        // 過電流がタイムアウトした場合
        logger.error("checkServoTimeout(): servo moving timeout. time = " +
                     String(_checkServoTimer.getTime()) +
                     ", current = " + String(getCurrent()));
        ret = true;
      } else if (!_controller.isTargetAngle() || !isServoMoving()) {
        // サーボの稼働が終了した
        logger.debug("checkServoTimeout(): finish servo moving. time = " +
                     String(_checkServoTimer.getTime()) +
                     ", current = " + String(getCurrent()));
        _checkServoStep = 0;
      }
      break;
    default:
      _checkServoStep = 0;
      break;
  }
  return ret;
}

/**
 * @brief ループ処理
 *
 */
void ChargeManager::loop(void) {
  static bool requestedStopCharge = false;
  _current.loop();
  if (!requestedStopCharge && isFullCharge()) {
    // 満充電になったら止める
    logger.info(
        "ChargeManager.loop(): Stop charging due to full charge. current = " +
        String(_current.getCurrent()));
    _controller.stopCharge();
    requestedStopCharge = true;
  } else if (!requestedStopCharge && haveToRelease()) {
    // 充電開始したが電流が流れなかった場合
    logger.info(
        "ChargeManager.loop(): Stop charging due to no current. current = " +
        String(_current.getCurrent()));
    _controller.stopCharge();
    requestedStopCharge = true;
  } else if (requestedStopCharge && !isFullCharge()) {
    requestedStopCharge = false;
  }

  if (checkServoTimeout()) {
    if (_controller.isCatchDrone()) {
      stopCharge();
    } else if (_controller.isReleaseDrone()) {
      startCharge();
    }
  }

  _controller.loop();
}

/**
 * @brief サーボ角度の文字列を取得する
 *
 * @return String サーボ角度の文字列
 */
String ChargeManager::toString(void) { return _controller.toString(); }
