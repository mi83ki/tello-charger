/**
 * @file ChargeController.cpp
 * @brief 充電制御クラス
 * @author Tatsuya Miyazaki
 * @date 2023/5/5
 *
 * @details サーボモータとMOSFETを制御するクラス
 */

#include "ChargeController.h"

#include <Log.h>

/** 捕獲サーボのピン番号 */
const uint8_t ChargeController::SERVO_CATCH_PIN = 19;
/** USBサーボのピン番号 */
const uint8_t ChargeController::SERVO_USB_PIN = 23;
/** MOSFETのピン番号 */
const uint8_t ChargeController::CHARGE_CONTROL_PIN = G22;

// この電流[mA]より大きければ充電中
const float ChargeController::CHARGE_CURRENT_CHARGING_THREASHOLD = 100.0;
// この電流[mA]より小さくなると充電を終了する
const float ChargeController::CHARGE_CURRENT_STOP_THREASHOLD = 200.0;
// この電流[mA]より大きければサーボモータ稼働中
const float ChargeController::SERVO_CURRENT_MOVING_THREASHOLD = 250.0;
// 以下の時間過電流を検知すればサーボを止める
const uint32_t ChargeController::SERVO_MOVING_TIMEOUT = 3000;

/**
 * @brief Construct a new Servo Controller:: Servo Controller object
 *
 */
ChargeController::ChargeController()
    : _servo(ServoController(SERVO_CATCH_PIN, SERVO_USB_PIN)),
      _fet(FETController(CHARGE_CONTROL_PIN)),
      _current(CurrentReader(500)),
      _chargeTimer(Timer()),
      _controlStartCharge(
          ControlStartCharge(&_servo, &_fet, &_current, &_chargeTimer)),
      _controlStopCharge(
          ControlStopCharge(&_servo, &_fet, &_current, &_chargeTimer)),
      _controlPowerOnDrone(
          ControlPowerOnDrone(&_servo, &_fet, &_current, &_chargeTimer)),
      _isServoMovingPrevious(false),
      _checkServoStep(0),
      _checkServoTimer(Timer()) {}

/**
 * @brief Destroy the Servo Controller:: Servo Controller object
 *
 */
ChargeController::~ChargeController() {}

/**
 * @brief 充電を開始する
 *
 */
void ChargeController::startCharge(void) {
  _controlStartCharge.start();
}

/**
 * @brief 充電を開始する
 *
 * @param catchCnt 捕獲繰り返し回数
 * @param retryCnt USB接続するまでの動作をリトライする回数
 */
void ChargeController::startCharge(uint8_t catchCnt, uint8_t retryCnt) {
  _controlStartCharge.start(catchCnt, retryCnt);
}

/**
 * @brief 充電開始処理中かどうか
 *
 * @return true
 * @return false
 */
bool ChargeController::isStartChargeExecuting(void) {
  return _controlStartCharge.isExecuting();
}

/**
 * @brief 充電を停止する
 *
 */
void ChargeController::stopCharge(void) { _controlStopCharge.start(); }

/**
 * @brief 充電終了処理中かどうか
 *
 * @return true
 * @return false
 */
bool ChargeController::isStopChargeExecuting(void) {
  return _controlStopCharge.isExecuting();
}

/**
 * @brief ドローンの電源をONにする
 *
 */
void ChargeController::powerOnDrone(void) { _controlPowerOnDrone.start(); }

/**
 * @brief ドローンの起動処理中かどうか
 *
 * @return true
 * @return false
 */
bool ChargeController::isPowerOnExecuting(void) {
  return _controlPowerOnDrone.isExecuting();
}

/**
 * @brief WASDでサーボの角度を微調整する
 *
 * @param input キーボード入力
 */
void ChargeController::wasdControl(char input) { _servo.wasdControl(input); }

/**
 * @brief 充電中かどうか
 *
 * @return true 充電中
 * @return false 充電中でない
 */
bool ChargeController::isCharging(void) {
  return _servo.isCatchDrone() && _servo.isConnectUsb() && _fet.read();
}

/**
 * @brief 初期位置かどうか
 *
 * @return true 初期位置
 * @return false 初期位置でない
 */
bool ChargeController::isInitPos(void) {
  return _servo.isReleaseDrone() && _servo.isDisconnectUsb() && !_fet.read();
}

/**
 * @brief 充電時間を取得する
 *
 * @return uint32_t 充電時間[ms]
 */
uint32_t ChargeController::getChargeTimeMillis(void) {
  return _chargeTimer.getTime();
}

/**
 * @brief ドローンをキャッチしているかどうか
 *
 * @return true
 * @return false
 */
bool ChargeController::isCatchDrone(void) { return _servo.isCatchDrone(); }

/**
 * @brief ドローンをリリースしているかどうか
 *
 * @return true
 * @return false
 */
bool ChargeController::isReleaseDrone(void) { return _servo.isReleaseDrone(); }

/**
 * @brief USBを接続しているかどうか
 *
 * @return true
 * @return false
 */
bool ChargeController::isConnectUsb(void) { return _servo.isConnectUsb(); }

/**
 * @brief USBを切断しているかどうか
 *
 * @return true
 * @return false
 */
bool ChargeController::isDisconnectUsb(void) {
  return _servo.isDisconnectUsb();
}

/**
 * @brief サーボモータの指示値が目標角度に到達したかどうか
 *
 * @return true
 * @return false
 */
bool ChargeController::isTargetAngle(void) { return _servo.isTargetAngle(); }

/**
 * @brief 電流値を取得する
 *
 * @return float 電流値[mA]
 */
float ChargeController::getCurrent(void) { return _current.getCurrent(); }

/**
 * @brief 充電中の電流かどうか
 *
 * @return true 充電中である
 * @return false
 */
bool ChargeController::isChargingCurrent(void) {
  return getCurrent() >= CHARGE_CURRENT_CHARGING_THREASHOLD;
}

/**
 * @brief 満充電かどうか
 *
 * @return true 満充電である
 * @return false
 */
bool ChargeController::isFullCharge(void) {
  return isCharging() && isChargingCurrent() &&
         getCurrent() < CHARGE_CURRENT_STOP_THREASHOLD &&
         getChargeTimeMillis() > 30000;
}

/**
 * @brief アームをリリースすべきかどうか
 *
 * @return true
 * @return false
 */
bool ChargeController::haveToRelease(void) {
  return isCharging() && !isChargingCurrent() && getChargeTimeMillis() > 60000;
}

/**
 * @brief サーボモータが稼働中かどうか
 *
 * @return true
 * @return false
 */
bool ChargeController::isServoMoving(void) {
  return !isCharging() && getCurrent() >= SERVO_CURRENT_MOVING_THREASHOLD;
}

/**
 * @brief サーボモータが過電流かどうか
 *
 * @return true
 * @return false
 */
bool ChargeController::isServoOverCurrent(void) {
  return !_fet.read() && isTargetAngle() &&
         getCurrent() >= SERVO_CURRENT_MOVING_THREASHOLD;
}

/**
 * @brief サーボモータが過電流タイムアウトしたかどうか
 *
 */
bool ChargeController::checkServoTimeout(void) {
  bool ret = false;
  static Timer timer = Timer(500);
  if (timer.isCycleTime()) {
    logger.debug("checkServoTimeout(): current = " + String(getCurrent()) +
                 ", isMoving = " + String(isServoMoving()));
  }
  switch (_checkServoStep) {
    case 0:
      if (isTargetAngle() && isServoMoving()) {
        // サーボ指示値が収束した状態で過電流を検知した
        _checkServoTimer.startTimer();
        _checkServoStep++;
        logger.debug("checkServoTimeout(): start timer");
      }
      break;
    case 1:
      if (isServoMoving() &&
          _checkServoTimer.getTime() >= SERVO_MOVING_TIMEOUT) {
        // 過電流がタイムアウトした場合
        logger.error("checkServoTimeout(): servo moving timeout. time = " +
                     String(_checkServoTimer.getTime()) +
                     ", current = " + String(getCurrent()));
        ret = true;
      } else if (!isTargetAngle() || !isServoMoving()) {
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
void ChargeController::loop(void) {
  static bool requestedStopCharge = false;
  _current.loop();
  if (!requestedStopCharge && isFullCharge()) {
    // 満充電になったら止める
    logger.info(
        "ChargeController.loop(): Stop charging due to full charge. current "
        "= " +
        String(_current.getCurrent()));
    stopCharge();
    requestedStopCharge = true;
  } else if (!requestedStopCharge && haveToRelease()) {
    // 充電開始したが電流が流れなかった場合
    logger.info(
        "ChargeController.loop(): Stop charging due to no current. current = " +
        String(_current.getCurrent()));
    stopCharge();
    requestedStopCharge = true;
  } else if (requestedStopCharge && !isFullCharge()) {
    requestedStopCharge = false;
  }

  if (checkServoTimeout()) {
    if (isCatchDrone()) {
      stopCharge();
    } else if (isReleaseDrone()) {
      startCharge();
    }
  }

  if (_controlStartCharge.loop()) {
    logger.info("Finish to start charge");
  }
  if (_controlStopCharge.loop()) {
    logger.info("Finish to stop charge");
  }
  if (_controlPowerOnDrone.loop()) {
    logger.info("Finish to power on Tello");
  }
  _servo.loop();
}

/**
 * @brief サーボ角度の文字列を取得する
 *
 * @return String サーボ角度の文字列
 */
String ChargeController::toString(void) { return _servo.toString(); }
