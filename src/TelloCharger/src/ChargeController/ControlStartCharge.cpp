/**
 * @file ControlStartCharge.cpp
 * @brief 充電開始制御ベースクラス
 * @author Tatsuya Miyazaki
 * @date 2023/11/18
 *
 * @details 充電開始制御のベースクラス
 */

#include "ControlStartCharge.h"

#include <Log.h>

/** 充電開始時に何回捕獲するか */
const uint8_t ControlStartCharge::CATCH_CNT = 2;
/** 充電開始時に何回USB接続をリトライするか */
const uint8_t ControlStartCharge::RETRY_CNT = 1;

/**
 * @brief Construct a new Control Start Charge:: Control Start Charge object
 *
 * @param servo
 * @param fet
 * @param current
 * @param chargeTimer
 */
ControlStartCharge::ControlStartCharge(ServoController *servo,
                                       FETController *fet,
                                       CurrentReader *current,
                                       Timer *chargeTimer)
    : ControlBase(servo, fet, current, "ControlStartCharge"),
      _controlArmInit(ControlArmInit(servo, fet, current, chargeTimer)),
      _controlArmCharge(ControlArmCharge(servo, fet, current, chargeTimer)),
      _catchCntTarget(1),
      _retryCntTarget(0) {}

/**
 * @brief 処理を開始する
 *
 */
void ControlStartCharge::start(void) {
  ControlBase::start();
  _catchCntTarget = CATCH_CNT;
  _retryCntTarget = RETRY_CNT;
}

/**
 * @brief 充電を開始する
 *
 * @param catchCnt 捕獲繰り返し回数
 * @param retryCnt USB接続するまでの動作をリトライする回数
 */
void ControlStartCharge::start(uint8_t catchCnt, uint8_t retryCnt) {
  ControlBase::start();
  _catchCntTarget = catchCnt;
  _retryCntTarget = retryCnt;
}

/**
 * @brief 処理を終了する
 *
 */
void ControlStartCharge::stop(void) {
  ControlBase::stop();
  _controlArmInit.stop();
  _controlArmCharge.stop();
}

/**
 * @brief 充電を開始するループ処理
 *
 * @return true 処理完了
 * @return false 処理中
 */
bool ControlStartCharge::loop(void) {
  switch (_step) {
    case 0:
      // 何もしない
      break;
    case 1:
      // 初期処理
      _controlArmInit.start();
      _step++;
      break;
    case 2:
      // アームを初期位置に戻す
      if (_controlArmInit.loop()) {
        _controlArmCharge.start(_catchCntTarget, _retryCntTarget);
        _step++;
      }
      break;
    case 3:
      // 充電接続する
      if (_controlArmCharge.loop()) {
        _step++;
      }
      break;
    default:
      // 充電開始完了
      _step = 0;
      return true;
  }
  return false;
}