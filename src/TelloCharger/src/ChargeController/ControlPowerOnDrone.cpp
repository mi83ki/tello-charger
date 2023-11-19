/**
 * @file ControlPowerOnDrone.cpp
 * @brief ドローン電源ON制御クラス
 * @author Tatsuya Miyazaki
 * @date 2023/11/18
 *
 * @details ドローン電源ON制御のクラス
 */

#include "ControlPowerOnDrone.h"

#include <Log.h>

/** 電源ON時に接続したアームを離すまでの時間[ms] */
const uint16_t ControlPowerOnDrone::POWER_ON_WAIT = 2000;

/**
 * @brief Construct a new Control Power On Drone:: Control Power On Drone object
 *
 * @param servo
 * @param fet
 * @param current
 * @param chargeTimer
 */
ControlPowerOnDrone::ControlPowerOnDrone(ServoController *servo,
                                         FETController *fet,
                                         CurrentReader *current,
                                         Timer *chargeTimer)
    : ControlBase(servo, fet, current, "ControlPowerOnDrone"),
      _controlArmInit(ControlArmInit(servo, fet, current, chargeTimer)),
      _controlArmCharge(ControlArmCharge(servo, fet, current, chargeTimer)) {}

/**
 * @brief ドローンの電源をONするループ処理
 *
 * @return true 処理完了
 * @return false 処理中
 */
bool ControlPowerOnDrone::loop(void) {
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
        _timer.startTimer();
        _step++;
      }
      break;
    case 4:
      // 1秒後に充電を終了する
      if (_timer.getTime() >= POWER_ON_WAIT) {
        _controlArmInit.start();
        _step++;
      }
      break;
    case 5:
      // アームを初期位置に戻す
      if (_controlArmInit.loop()) {
        _step++;
      }
      break;
    default:
      _step = 0;
      return true;
  }
  return false;
}