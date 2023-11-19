/**
 * @file ControlArmCharge.h
 * @brief アーム充電位置制御ベースクラス
 * @author Tatsuya Miyazaki
 * @date 2023/11/18
 *
 * @details アーム充電位置制御のベースクラス
 */

#pragma once
#include <Arduino.h>
#include <Timer.h>

#include "ControlBase.h"
#include "CurrentReader.h"
#include "FETController.h"
#include "ServoController.h"

class ControlArmCharge : public ControlBase {
 public:
  ControlArmCharge(ServoController *, FETController *, CurrentReader *,
                   Timer *);
  void start(void);
  void start(uint8_t, uint8_t);
  bool loop(void) override;

 private:
  Timer *_chargeTimer;
  /** 充電開始時の捕獲回数 */
  uint8_t _catchCnt;
  /** 充電開始時の捕獲目標回数 */
  uint8_t _catchCntTarget;
  /** USBを接続するまでの動作を何回行うか */
  uint8_t _retryCnt;
  /** USBを接続するまでの動作を行う目標回数 */
  uint8_t _retryCntTarget;

  static const uint8_t CATCH_CNT;
  static const uint8_t RETRY_CNT;
};
