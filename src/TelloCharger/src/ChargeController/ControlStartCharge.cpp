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

ControlStartCharge::ControlStartCharge(ServoController *servo,
                                       FETController *fet,
                                       CurrentReader *current,
                                       Timer *chargeTimer)
    : ControlBase(servo, fet, current),
      _controlArmInit(ControlArmInit(servo, fet, current, chargeTimer)),
      _controlArmCharge(
          ControlArmCharge(servo, fet, current, chargeTimer)) {}

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
        _controlArmCharge.start();
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