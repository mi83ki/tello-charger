/**
 * @file ControlStopCharge.h
 * @brief 充電終了制御クラス
 * @author Tatsuya Miyazaki
 * @date 2023/11/18
 *
 * @details 充電終了制御のクラス
 */

#pragma once
#include <Arduino.h>
#include <Timer.h>

#include "ControlArmInit.h"
#include "ControlBase.h"
#include "CurrentReader.h"
#include "FETController.h"
#include "ServoController.h"

class ControlStopCharge : public ControlBase {
 public:
  ControlStopCharge(ServoController *, FETController *, CurrentReader *,
                    Timer *);
  void stop(void);
  bool loop(void) override;

 private:
  ControlArmInit _controlArmInit;
};
