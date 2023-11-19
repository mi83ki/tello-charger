/**
 * @file ControlStartCharge.h
 * @brief 充電開始制御ベースクラス
 * @author Tatsuya Miyazaki
 * @date 2023/11/18
 *
 * @details 充電開始制御のベースクラス
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

class ControlStartCharge : public ControlBase {
 public:
  ControlStartCharge(ServoController *, FETController *, CurrentReader *,
                     Timer *);
  bool loop(void) override;

 private:
  ControlArmInit _controlArmInit;
  ControlArmCharge _controlArmCharge;
};
