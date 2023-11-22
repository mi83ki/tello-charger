/**
 * @file ControlStopCharge.cpp
 * @brief 充電終了制御クラス
 * @author Tatsuya Miyazaki
 * @date 2023/11/18
 *
 * @details 充電終了制御のクラス
 */

#include "ControlStopCharge.h"

#include <Log.h>

/**
 * @brief Construct a new Control Stop Charge:: Control Stop Charge object
 * 
 * @param servo 
 * @param fet 
 * @param current 
 * @param chargeTimer 
 */
ControlStopCharge::ControlStopCharge(ServoController *servo, FETController *fet,
                                     CurrentReader *current, Timer *chargeTimer)
    : ControlBase(servo, fet, current, "ControlStopCharge"),
      _controlArmInit(ControlArmInit(servo, fet, current, chargeTimer)) {}

/**
 * @brief 処理を終了する
 *
 */
void ControlStopCharge::stop(void) {
  ControlBase::stop();
  _controlArmInit.stop();
}

/**
 * @brief 充電を停止するループ処理
 *
 * @return true 処理完了
 * @return false 処理中
 */
bool ControlStopCharge::loop(void) {
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
        _step++;
      }
      break;
    default:
      // 充電停止完了
      _step = 0;
      return true;
  }
  return false;
}