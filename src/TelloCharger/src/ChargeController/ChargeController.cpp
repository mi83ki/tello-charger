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
/** 充電開始時に何回捕獲するか */
const uint8_t ChargeController::CATCH_CNT = 2;
/** 充電開始時に何回USB接続をリトライするか */
const uint8_t ChargeController::RETRY_CNT = 1;
/** 電源ON時に接続したアームを離すまでの時間[ms] */
const uint16_t ChargeController::POWER_ON_WAIT = 2000;

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
      _controlArmInit(ControlArmInit(&_servo, &_fet, &_current, &_chargeTimer)),
      _controlArmCharge(ControlArmCharge(&_servo, &_fet, &_current, &_chargeTimer)),
      _startChargeStep(0),
      _stopChargeStep(0),
      _powerOnDroneStep(0),
      _powerOnDroneTimer(Timer()),
      _catchCnt(0),
      _catchCntTarget(1),
      _retryCnt(0),
      _retryCntTarget(0),
      _chargeTimer(Timer()),
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
  _startChargeStep = 1;
  _catchCntTarget = CATCH_CNT;
  _retryCntTarget = RETRY_CNT;
}

/**
 * @brief 充電を開始する
 *
 * @param catchCnt 捕獲繰り返し回数
 * @param retryCnt USB接続するまでの動作をリトライする回数
 */
void ChargeController::startCharge(uint8_t catchCnt, uint8_t retryCnt) {
  _startChargeStep = 1;
  _catchCntTarget = catchCnt;
  _retryCntTarget = retryCnt;
}

/**
 * @brief 充電を開始するループ処理
 *
 * @return true 処理完了
 * @return false 処理中
 */
bool ChargeController::_startChargeLoop(void) {
  switch (_startChargeStep) {
    case 0:
      // 何もしない
      break;
    case 1:
      // 初期処理
      _controlArmInit.start();
      _startChargeStep++;
      break;
    case 2:
      // アームを初期位置に戻す
      if (_controlArmInit.loop()) {
        _controlArmCharge.start();
        _startChargeStep++;
      }
      break;
    case 3:
      // 充電接続する
      if (_controlArmCharge.loop()) {
        _startChargeStep++;
      }
      break;
    default:
      // 充電開始完了
      _startChargeStep = 0;
      return true;
  }
  return false;
}

/**
 * @brief 充電開始処理中かどうか
 *
 * @return true
 * @return false
 */
bool ChargeController::isStartChargeExecuting(void) {
  return _startChargeStep != 0;
}

/**
 * @brief 充電を停止する
 *
 */
void ChargeController::stopCharge(void) { _stopChargeStep = 1; }

/**
 * @brief 充電を停止するループ処理
 *
 * @return true 処理完了
 * @return false 処理中
 */
bool ChargeController::_stopChargeLoop(void) {
  switch (_stopChargeStep) {
    case 0:
      // 何もしない
      break;
    case 1:
      // 初期処理
      _controlArmInit.start();
      _stopChargeStep++;
      break;
    case 2:
      // アームを初期位置に戻す
      if (_controlArmInit.loop()) {
        _stopChargeStep++;
      }
      break;
    default:
      // 充電停止完了
      _stopChargeStep = 0;
      return true;
  }
  return false;
}

/**
 * @brief 充電終了処理中かどうか
 *
 * @return true
 * @return false
 */
bool ChargeController::isStopChargeExecuting(void) {
  return _stopChargeStep != 0;
}

/**
 * @brief ドローンの電源をONにする
 *
 */
void ChargeController::powerOnDrone(void) { _powerOnDroneStep = 1; }

/**
 * @brief ドローンの電源をONするループ処理
 *
 * @return true 処理完了
 * @return false 処理中
 */
bool ChargeController::_powerOnDroneLoop(void) {
  switch (_powerOnDroneStep) {
    case 0:
      // 何もしない
      break;
    case 1:
      // 初期処理
      _controlArmInit.start();
      _powerOnDroneStep++;
      break;
    case 2:
      // アームを初期位置に戻す
      if (_controlArmInit.loop()) {
        _controlArmCharge.start();
        _powerOnDroneStep++;
      }
      break;
    case 3:
      // 充電接続する
      if (_controlArmCharge.loop()) {
        _powerOnDroneTimer.startTimer();
        _powerOnDroneStep++;
      }
      break;
    case 4:
      // 1秒後に充電を終了する
      if (_powerOnDroneTimer.getTime() >= POWER_ON_WAIT) {
        _controlArmInit.start();
        _powerOnDroneStep++;
      }
      break;
    case 5:
      // アームを初期位置に戻す
      if (_controlArmInit.loop()) {
        _powerOnDroneStep++;
      }
      break;
    default:
      _powerOnDroneStep = 0;
      return true;
  }
  return false;
}

/**
 * @brief ドローンの起動処理中かどうか
 *
 * @return true
 * @return false
 */
bool ChargeController::isPowerOnExecuting(void) {
  return _powerOnDroneStep != 0;
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
      if (isServoMoving() && _checkServoTimer.getTime() >= SERVO_MOVING_TIMEOUT) {
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

  if (_startChargeLoop()) {
    logger.info("Finish to start charge");
  }
  if (_stopChargeLoop()) {
    logger.info("Finish to stop charge");
  }
  if (_powerOnDroneLoop()) {
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
