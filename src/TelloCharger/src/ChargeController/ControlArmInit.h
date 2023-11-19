/**
 * @file ControlArmInit.h
 * @brief アーム初期位置制御ベースクラス
 * @author Tatsuya Miyazaki
 * @date 2023/11/18
 *
 * @details アーム初期位置制御のベースクラス
 */

#pragma once
#include <Arduino.h>
#include <Timer.h>

#include "ControlBase.h"
#include "CurrentReader.h"
#include "FETController.h"
#include "ServoController.h"

class ControlArmInit : public ControlBase {
 public:
  ControlArmInit(ServoController *, FETController *, CurrentReader *, Timer *);
  bool loop(void) override;

 private:
  Timer *_chargeTimer;
};
