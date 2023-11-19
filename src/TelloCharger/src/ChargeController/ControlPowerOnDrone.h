/**
 * @file ControlPowerOnDrone.h
 * @brief ドローン電源ON制御クラス
 * @author Tatsuya Miyazaki
 * @date 2023/11/18
 *
 * @details ドローン電源ON制御のクラス
 */

#pragma once
#include <Arduino.h>
#include <Timer.h>

#include "ControlArmCharge.h"
#include "ControlArmInit.h"
#include "ControlBase.h"
#include "CurrentReader.h"
#include "FETController.h"
#include "ServoController.h"

class ControlPowerOnDrone : public ControlBase {
 public:
  ControlPowerOnDrone(ServoController *, FETController *, CurrentReader *,
                      Timer *);
  bool loop(void) override;

 private:
  ControlArmInit _controlArmInit;
  ControlArmCharge _controlArmCharge;
  static const uint16_t POWER_ON_WAIT;
};
