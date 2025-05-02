/**
 * @file ChargeController.h
 * @brief 充電制御クラス
 * @author Tatsuya Miyazaki
 * @date 2023/5/5
 *
 * @details サーボモータとMOSFETを制御するクラス
 */

#pragma once
#include <Arduino.h>
#include <Timer.h>

#include "CheckServoCurrent.h"
#include "ControlPowerOnDrone.h"
#include "ControlStartCharge.h"
#include "ControlStopCharge.h"
#include "CurrentReader.h"
#include "FETController.h"
#include "ServoController.h"

class ChargeController {
 public:
  ChargeController();
  ~ChargeController();
  void stop(void);
  void startCharge(void);
  void startCharge(uint8_t, uint8_t);
  bool isStartChargeExecuting(void);
  void stopCharge(void);
  bool isStopChargeExecuting(void);
  void powerOnDrone(void);
  bool isPowerOnExecuting(void);
  void wasdControl(char);
  bool isCharging(void);
  bool isInitPos(void);
  bool isCatchDrone(void);
  bool isReleaseDrone(void);
  bool isConnectUsb(void);
  bool isDisconnectUsb(void);
  bool isTargetAngle(void);
  uint32_t getChargeTimeMillis(void);
  float getChargeTimeSec(void);

  float getCurrent(void);
  bool isChargingCurrent(void);
  bool isFullCharge(void);
  bool haveToRelease(void);

  void loop(void);
  String toString(void);

 private:
  typedef enum eCharge {
    IDLE,
    START_CHARGE,
    STOP_CHARGE,

  } ChargeStepType;
  /** サーボ制御部 */
  ServoController _servo;
  /** MOSFET制御部 */
  FETController _fet;
  /** 電流計測部 */
  CurrentReader _current;
  /** 充電時間計測タイマー */
  Timer _chargeTimer;

  /** 充電開始制御部 */
  ControlStartCharge _controlStartCharge;
  /** 充電停止制御部 */
  ControlStopCharge _controlStopCharge;
  /** ドローン電源ON制御部 */
  ControlPowerOnDrone _controlPowerOnDrone;
  /** サーボ電流監視部 */
  CheckServoCurrent _checkServoCurrent;

  static const float CHARGE_CURRENT_CHARGING_THREASHOLD;
  static const float CHARGE_CURRENT_STOP_THREASHOLD;
};
