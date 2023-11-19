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

#include "CurrentReader.h"
#include "FETController.h"
#include "ServoController.h"
#include "ControlArmInit.h"
#include "ControlArmCharge.h"

class ChargeController {
 public:
  ChargeController();
  ~ChargeController();
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

  float getCurrent(void);
  bool isChargingCurrent(void);
  bool isFullCharge(void);
  bool haveToRelease(void);

  bool isServoMoving(void);
  bool isServoOverCurrent(void);
  bool checkServoTimeout(void);

  void loop(void);
  String toString(void);

 private:
  typedef enum eCharge {
    IDLE,
    START_CHARGE,
    STOP_CHARGE,

  } ChargeStepType;
  bool _chargeLoop(uint8_t, uint8_t);
  bool _chargeLoop();
  bool _startChargeLoop(void);
  bool _stopChargeLoop(void);
  bool _powerOnDroneLoop(void);

  /** サーボ制御部 */
  ServoController _servo;
  /** MOSFET制御部 */
  FETController _fet;
  /** 電流計測部 */
  CurrentReader _current;

  /** アーム初期位置駆動制御部 */
  ControlArmInit _controlArmInit;
  /** アーム充電位置駆動制御部 */
  ControlArmCharge _controlArmCharge;

  /** 充電開始制御用の処理ステップ */
  uint8_t _startChargeStep;
  /** 充電停止制御用の処理ステップ */
  uint8_t _stopChargeStep;
  /** Tello電源ON制御用の処理ステップ */
  uint8_t _powerOnDroneStep;
  /** Tello電源ON制御用のタイマー */
  Timer _powerOnDroneTimer;
  /** 充電開始時の捕獲回数 */
  uint8_t _catchCnt;
  /** 充電開始時の捕獲目標回数 */
  uint8_t _catchCntTarget;
  /** USBを接続するまでの動作を何回行うか */
  uint8_t _retryCnt;
  /** USBを接続するまでの動作を行う目標回数 */
  uint8_t _retryCntTarget;
  /** 充電時間計測タイマー */
  Timer _chargeTimer;
  /** サーボモータの過電流対策 */
  bool _isServoMovingPrevious;
  uint8_t _checkServoStep;
  Timer _checkServoTimer;

  static const uint8_t SERVO_CATCH_PIN;
  static const uint8_t SERVO_USB_PIN;
  static const uint8_t CHARGE_CONTROL_PIN;
  static const uint8_t CATCH_CNT;
  static const uint8_t RETRY_CNT;
  static const uint16_t POWER_ON_WAIT;
  static const float CHARGE_CURRENT_CHARGING_THREASHOLD;
  static const float CHARGE_CURRENT_STOP_THREASHOLD;
  static const float SERVO_CURRENT_MOVING_THREASHOLD;
  static const uint32_t SERVO_MOVING_TIMEOUT;
};
