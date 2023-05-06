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

#include "FETController.h"
#include "ServoController.h"

class ChargeController
{
public:
  ChargeController();
  ~ChargeController();
  void startCharge(void);
  void startCharge(uint8_t);
  void stopCharge(void);
  void powerOnTello(void);
  void wasdControl(char);
  bool isCharging(void);
  uint32_t getChargeTimeMillis(void);
  void loop(void);
  String toString(void);

private:
  typedef enum eCharge
  {
    IDLE,
    START_CHARGE,
    STOP_CHARGE,
    
  } ChargeStepType;
  bool _initLoop();
  bool _chargeLoop(uint8_t);
  bool _startChargeLoop(void);
  bool _stopChargeLoop(void);
  bool _powerOnTelloLoop(void);

  /** サーボ制御部 */
  ServoController _servo;
  /** MOSFET制御部 */
  FETController _fet;
  /** 初期位置に戻す処理のステップ */
  uint8_t _initStep;
  /** 充電接続する処理のステップ */
  uint8_t _chargeStep;
  /** 充電開始制御用の処理ステップ */
  uint8_t _startChargeStep;
  /** 充電停止制御用の処理ステップ */
  uint8_t _stopChargeStep;
  /** Tello電源ON制御用の処理ステップ */
  uint8_t _powerOnTelloStep;
  /** Tello電源ON制御用のタイマー */
  Timer _powerOnTelloTimer;
  /** 充電開始時の捕獲回数 */
  uint8_t _catchCnt;
  /** 充電開始時の捕獲目標回数 */
  uint8_t _catchCntTarget;
  /** 充電時間計測タイマー */
  Timer _chargeTimer;

  static const uint8_t SERVO_CATCH_PIN;
  static const uint8_t SERVO_USB_PIN;
  static const uint8_t CHARGE_CONTROL_PIN;
  static const uint8_t CATCH_CNT;
};
