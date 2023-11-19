/**
 * @file ControlArmCharge.cpp
 * @brief アーム充電位置制御ベースクラス
 * @author Tatsuya Miyazaki
 * @date 2023/11/18
 *
 * @details アーム充電位置制御のベースクラス
 */

#include "ControlArmCharge.h"

#include <Log.h>

ControlArmCharge::ControlArmCharge(ServoController *servo, FETController *fet,
                                   CurrentReader *current, Timer *chargeTimer)
    : ControlBase(servo, fet, current), _chargeTimer(chargeTimer) {}

/**
 * @brief 処理を開始する
 *
 */
void ControlArmCharge::start(void) {
  _step = 1;
  logger.info("ControlArmCharge.start(): _step = " + String(_step));
}

/**
 * @brief 処理を終了する
 *
 */
void ControlArmCharge::stop(void) { _step = 0; }

/**
 * @brief 充電接続するループ処理
 *
 * @param catchCnt 捕獲時に繰り返す回数
 * @param retryCnt USB接続するまでの動作をリトライする回数
 * @return true 処理完了
 * @return false 処理中
 */
bool ControlArmCharge::loop(void) {
  uint8_t catchCnt = 1;
  uint8_t retryCnt = 0;
  static uint8_t _catchCnt = 0;
  static uint8_t _retryCnt = 0;
  switch (_step) {
    case 0:
      // 何もしない
      break;
    case 1:
      // 初期化
      _catchCnt = 0;
      _retryCnt = 0;
      _step++;
    case 2:
      // 捕獲する
      if (_servo->isReleaseDrone()) {
        if (catchCnt - _catchCnt <= 1) {
          _servo->catchDrone();
        } else {
          _servo->catchDroneOnce();
        }
        _catchCnt++;
      } else if (_servo->isCatchDrone()) {
        if (_catchCnt >= catchCnt)
          // 指定した回数捕獲したら次に進む
          _step++;
        else
          _servo->releaseDrone();
      }
      break;
    case 3:
      // USBを接続する
      if (_servo->isDisconnectUsb())
        _servo->connectUsb();
      else if (_servo->isConnectUsb()) {
        if (_retryCnt < retryCnt) {
          _catchCnt = 0;
          _step = 5;
          _retryCnt++;
        } else {
          _step++;
        }
      }
      break;
    case 4:
      // MOSFETをONにする
      if (!_fet->read()) {
        _fet->on();
        _chargeTimer->startTimer();
        logger.info("chargeLoop(): start charge timer");
      } else {
        _step = 99;
      }
      break;
    case 5:
      if (_servo->isConnectUsb())
        _servo->disconnectUsb();
      else if (_servo->isDisconnectUsb())
        _step = 2;
      break;
    default:
      // 初期位置に戻す処理完了
      _step = 0;
      return true;
  }
  return false;
}