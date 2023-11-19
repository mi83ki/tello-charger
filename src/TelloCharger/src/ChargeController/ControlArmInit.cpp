/**
 * @file ControlArmInit.cpp
 * @brief アーム初期位置制御ベースクラス
 * @author Tatsuya Miyazaki
 * @date 2023/11/18
 *
 * @details アーム初期位置制御のベースクラス
 */

#include "ControlArmInit.h"

#include <Log.h>

ControlArmInit::ControlArmInit(ServoController *servo, FETController *fet,
                               CurrentReader *current, Timer *chargeTimer)
    : ControlBase(servo, fet, current, "ControlArmInit"),
      _chargeTimer(chargeTimer) {}

/**
 * @brief アームを初期位置に戻すループ処理
 *
 * @return true 処理完了
 * @return false 処理中
 */
bool ControlArmInit::loop(void) {
  switch (_step) {
    case 0:
      // 何もしない
      break;
    case 1:
      // MOSFETをOFFする
      if (_fet->read()) {
        _fet->off();
        _chargeTimer->stopTimer();
        logger.info("chargeLoop(): stop charge timer, " +
                    String(_chargeTimer->getTime()));
      } else {
        _step++;
      }
      break;
    case 2:
      // USBアームが接続中であれば離す
      if (_servo->isConnectUsb())
        _servo->disconnectUsb();
      else if (_servo->isDisconnectUsb())
        _step++;
      break;
    /*
    case 3:
      if (!isServoOverCurrent()) {
        _step++;
      } else {
      }
      */
    case 3:
      // 捕獲アームが捕獲中の場合は戻す
      if (_servo->isCatchDrone())
        _servo->releaseDrone();
      else if (_servo->isReleaseDrone())
        _step++;
      break;
    default:
      // 初期位置に戻す処理完了
      _step = 0;
      return true;
  }
  return false;
}