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
  void start(void);
  void start(uint8_t, uint8_t);
  bool loop(void) override;

 private:
  ControlArmInit _controlArmInit;
  ControlArmCharge _controlArmCharge;
  /** 充電開始時の捕獲目標回数 */
  uint8_t _catchCntTarget;
  /** USBを接続するまでの動作を行う目標回数 */
  uint8_t _retryCntTarget;

  static const uint8_t CATCH_CNT;
  static const uint8_t RETRY_CNT;
};
