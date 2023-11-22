/**
 * @file CheckServoCurrent.cpp
 * @brief サーボ電流を監視するクラス
 * @author Tatsuya Miyazaki
 * @date 2023/11/20
 *
 * @details サーボ電流を監視するクラス
 */

#include "CheckServoCurrent.h"

#include <Log.h>

/**
 * @brief Construct a new Check Servo Current:: Check Servo Current object
 *
 * @param servo
 * @param fet
 * @param current
 */
CheckServoCurrent::CheckServoCurrent(ServoController *servo, FETController *fet,
                                     CurrentReader *current)
    : ControlBase(servo, fet, current, "CheckServoCurrent"),
      _isServoMovingPrevious(false),
      _haveToEmargencyStopServo(false) {}

// この電流[mA]より大きければサーボモータ稼働中
const float CheckServoCurrent::SERVO_CURRENT_MOVING_THREASHOLD = 150.0;
// 以下の時間過電流を検知すればサーボを止める
const uint32_t CheckServoCurrent::SERVO_MOVING_TIMEOUT = 5000;

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool CheckServoCurrent::isServoOverCurrent(void) {
  return !_fet->read() && _servo->isTargetAngle() &&
         isServoMoving(_current->getCurrent());
}

/**
 * @brief 非常停止が必要かどうか
 *
 * @return true
 * @return false
 */
bool CheckServoCurrent::haveToEmargencyStopServo(void) {
  if (_haveToEmargencyStopServo) {
    _haveToEmargencyStopServo = false;
    return true;
  }
  return false;
}

/**
 * @brief アームを初期位置に戻すループ処理
 *
 * @return true 処理完了
 * @return false 処理中
 */
bool CheckServoCurrent::loop(void) {
  static Timer timer = Timer(500);
  if (timer.isCycleTime()) {
    logger.debug(
        "checkServoTimeout(): current = " + String(_current->getCurrent()) +
        ", isMoving = " + String(isServoOverCurrent()));
  }
  switch (_step) {
    case 0:
      if (isServoOverCurrent()) {
        // サーボ指示値が収束した状態で過電流を検知した
        _timer.startTimer();
        _step++;
        logger.debug("checkServoTimeout(): start timer");
      }
      break;
    case 1:
      if (isServoOverCurrent() && _timer.getTime() >= SERVO_MOVING_TIMEOUT) {
        // 過電流がタイムアウトした場合
        logger.error("checkServoTimeout(): servo moving timeout. time = " +
                     String(_timer.getTime()) +
                     ", current = " + String(_current->getCurrent()));
        _haveToEmargencyStopServo = true;
        if (_servo->isCatchDrone())
          _step = 10;
        else
          _step = 20;
      } else if (!isServoOverCurrent()) {
        // サーボの稼働が終了した
        logger.debug("checkServoTimeout(): finish servo moving. time = " +
                     String(_timer.getTime()) +
                     ", current = " + String(_current->getCurrent()));
        _step = 99;
      }
      break;
    case 10:
      // 非常停止処理（アームを開く）
      if (_servo->isConnectUsb()) {
        // まずはUSBアームを動かす
        _servo->disconnectUsb();
        logger.info("emargencyStopServo(): Set USB servo angle from " +
                    String(_servo->servoUsb()->getPresentAngle()) + " to " +
                    String(_servo->servoUsb()->getTargetAngle()));
      } else if (_servo->isDisconnectUsb()) {
        // USBアームが閉じてから捕獲アームを動かす
        _servo->releaseDrone();
        logger.info("emargencyStopServo(): Set Catch servo angle from " +
                    String(_servo->servoCatch()->getPresentAngle()) + " to " +
                    String(_servo->servoCatch()->getTargetAngle()));
        _step = 99;
      }
      break;
    case 20:
      // 非常停止処理（アームを閉じる）
      if (_servo->isReleaseDrone()) {
        // まずは捕獲アームを動かす
        _servo->catchDrone();
        logger.info("emargencyStopServo(): Set Catch servo angle from " +
                    String(_servo->servoCatch()->getPresentAngle()) + " to " +
                    String(_servo->servoCatch()->getTargetAngle()));
      } else if (_servo->isCatchDrone()) {
        // 捕獲アームが閉じてからUSBアームを動かす
        _servo->connectUsb();
        logger.info("emargencyStopServo(): Set USB servo angle from " +
                    String(_servo->servoUsb()->getPresentAngle()) + " to " +
                    String(_servo->servoUsb()->getTargetAngle()));
        _step = 99;
      }
      break;
    default:
      _step = 0;
      return true;
  }
  return false;
}