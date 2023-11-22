/**
 * @file CheckServoCurrent.h
 * @brief サーボ電流を監視するクラス
 * @author Tatsuya Miyazaki
 * @date 2023/11/20
 *
 * @details サーボ電流を監視するクラス
 */

#pragma once
#include <Arduino.h>
#include <Timer.h>

#include "ControlBase.h"

class CheckServoCurrent : public ControlBase {
 public:
  CheckServoCurrent(ServoController *, FETController *, CurrentReader *);
  static bool isServoMoving(float current) {
    return current >= SERVO_CURRENT_MOVING_THREASHOLD;
  }
  bool isServoOverCurrent(void);
  bool haveToEmargencyStopServo(void);
  bool loop(void) override;

 private:
  /** サーボモータの過電流対策 */
  bool _isServoMovingPrevious;
  /** サーボモータを異常停止すべきかどうか */
  bool _haveToEmargencyStopServo;

  static const float SERVO_CURRENT_MOVING_THREASHOLD;
  static const uint32_t SERVO_MOVING_TIMEOUT;
};
