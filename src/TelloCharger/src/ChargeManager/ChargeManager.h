/**
 * @file ChargeManager.h
 * @brief 充電管理クラス
 * @author Tatsuya Miyazaki
 * @date 2023/5/6
 *
 * @details 充電指示や電流量を管理するクラス
 */

#pragma once
#include <Arduino.h>
#include <Timer.h>

#include "ChargeController/ChargeController.h"
#include "ChargeManager/CurrentReader.h"

// この電流[mA]より大きければ充電中
#define CHARGE_CURRENT_CHARGING_THREASHOLD (100.0)
// この電流[mA]より小さくなると充電を終了する
#define CHARGE_CURRENT_STOP_THREASHOLD (200.0)
// この電流[mA]より大きければサーボモータ稼働中
#define SERVO_CURRENT_MOVING_THREASHOLD (150.0)
// 以下の時間過電流を検知すればサーボを止める
#define SERVO_MOVING_TIMEOUT (3000)

class ChargeManager {
 public:
  ChargeManager();
  ~ChargeManager();
  void startCharge(void);
  void stopCharge(void);
  void powerOnDrone(void);
  void wasdControl(char);
  bool isCharging(void);
  bool isInitPos(void);
  bool isStartChargeExecuting(void);
  bool isStopChargeExecuting(void);
  bool isPowerOnExecuting(void);
  uint32_t getChargeTimeMillis(void);
  float getCurrent(void);
  bool isChargingCurrent(void);
  bool isFullCharge(void);
  bool haveToRelease(void);
  bool isServoMoving(void);
  bool checkServoTimeout(void);
  void loop(void);
  String toString(void);

 private:
  ChargeController _controller;
  CurrentReader _current;
  bool _isServoMovingPrevious;
  uint8_t _checkServoStep;
  Timer _checkServoTimer;
};
