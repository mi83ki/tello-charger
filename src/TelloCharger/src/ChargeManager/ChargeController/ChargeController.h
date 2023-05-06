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
  void stopCharge(void);
  void powerOnTello(void);
  void wasdControl(char);
  bool isCharging(void);
  void loop(void);
  String toString(void);

private:
  bool _initLoop(uint8_t *);
  bool _startChargeLoop(void);
  bool _stopChargeLoop(void);
  bool _powerOnTelloLoop(void);

  /** サーボ制御部 */
  ServoController _servo;
  /** MOSFET制御部 */
  FETController _fet;
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

  static const uint8_t SERVO_CATCH_PIN;
  static const uint8_t SERVO_USB_PIN;
  static const uint8_t CHARGE_CONTROL_PIN;
  static const uint8_t CATCH_CNT;
};
