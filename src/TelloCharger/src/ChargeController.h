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
  uint8_t advanceChargeStep(void);
  void loop(void);
  String toString(void);

private:
  /** サーボ制御部 */
  ServoController _servo;
  /** MOSFET制御部 */
  FETController _fet;

  static const uint8_t SERVO_CATCH_PIN;
  static const uint8_t SERVO_USB_PIN;
  static const uint8_t CHARGE_CONTROL_PIN;
};
