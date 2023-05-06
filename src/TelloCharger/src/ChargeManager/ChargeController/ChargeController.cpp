/**
 * @file ChargeController.cpp
 * @brief 充電制御クラス
 * @author Tatsuya Miyazaki
 * @date 2023/5/5
 *
 * @details サーボモータとMOSFETを制御するクラス
 */

#include "ChargeController.h"

#include <Log.h>

/** 捕獲サーボのピン番号 */
const uint8_t ChargeController::SERVO_CATCH_PIN = 19;
/** USBサーボのピン番号 */
const uint8_t ChargeController::SERVO_USB_PIN = 23;
/** MOSFETのピン番号 */
const uint8_t ChargeController::CHARGE_CONTROL_PIN = G22;
/** 充電開始時に何回捕獲するか */
const uint8_t ChargeController::CATCH_CNT = 3;

/**
 * @brief Construct a new Servo Controller:: Servo Controller object
 *
 */
ChargeController::ChargeController()
  : _servo(ServoController(SERVO_CATCH_PIN, SERVO_USB_PIN))
  , _fet(FETController(CHARGE_CONTROL_PIN))
  , _startChargeStep(0)
  , _stopChargeStep(0)
  , _powerOnTelloStep(0)
  , _powerOnTelloTimer(Timer())
  , _catchCnt(0)
{
}

/**
 * @brief Destroy the Servo Controller:: Servo Controller object
 *
 */
ChargeController::~ChargeController() {}

/**
 * @brief アームを初期位置に戻すループ処理
 *
 * @param step 処理ステップ
 * @return true 処理完了
 * @return false 処理中
 */
bool ChargeController::_initLoop(uint8_t *step)
{
  switch (*step)
  {
    case 0:
      // 何もしない
      break;
    case 1:
      // MOSFETをOFFする
      if (_fet.read())
        _fet.off();
      else
        (*step)++;
      break;
    case 2:
      // USBアームが接続中であれば離す
      if (_servo.isConnectUsb())
        _servo.disconnectUsb();
      else if (_servo.isDisconnectUsb())
        (*step)++;
      break;
    case 3:
      // 捕獲アームが捕獲中の場合は戻す
      if (_servo.isCatchDrone())
        _servo.releaseDrone();
      else if (_servo.isReleaseDrone())
        (*step)++;
      break;
    default:
      // 充電開始完了
      (*step) = 0;
      return true;
  }
  return false;
}

/**
 * @brief 充電を開始する
 *
 */
void ChargeController::startCharge(void)
{
  _startChargeStep = 1;
}

/**
 * @brief 充電を開始するループ処理
 *
 * @return true 処理完了
 * @return false 処理中
 */
bool ChargeController::_startChargeLoop(void)
{
  switch (_startChargeStep)
  {
    case 0:
      // 何もしない
      break;
    case 1:
    case 2:
    case 3:
      // 初期位置に戻す
      _initLoop(&_startChargeStep);
      if (_startChargeStep == 4)
        _catchCnt = 0;
      break;
    case 4:
      // 捕獲する
      if (_servo.isReleaseDrone())
      {
        _servo.catchDrone();
        _catchCnt++;
      }
      else if (_servo.isCatchDrone())
      {
        if (_catchCnt >= CATCH_CNT)
          // 指定した回数捕獲したら次に進む
          _startChargeStep++;
        else
          _servo.releaseDrone();
      }
      break;
    case 5:
      // USBを接続する
      if (_servo.isDisconnectUsb())
        _servo.connectUsb();
      else if (_servo.isConnectUsb())
        _startChargeStep++;
      break;
    case 6:
      // MOSFETをONにする
      if (!_fet.read())
        _fet.on();
      else
        _startChargeStep++;
      break;
    default:
      // 充電開始完了
      _startChargeStep = 0;
      return true;
  }
  return false;
}

/**
 * @brief 充電を停止する
 *
 */
void ChargeController::stopCharge(void)
{
  _stopChargeStep = 1;
}

/**
 * @brief 充電を停止するループ処理
 *
 * @return true 処理完了
 * @return false 処理中
 */
bool ChargeController::_stopChargeLoop(void)
{
  return _initLoop(&_stopChargeStep);
}

/**
 * @brief Telloの電源をONにする
 *
 */
void ChargeController::powerOnTello(void)
{
  _powerOnTelloStep = 1;
}

/**
 * @brief Telloの電源をONするループ処理
 *
 * @return true 処理完了
 * @return false 処理中
 */
bool ChargeController::_powerOnTelloLoop(void)
{
  switch (_powerOnTelloStep)
  {
    case 0:
      // 何もしない
      break;
    case 1:
      // USBを接続する
      startCharge();
      _powerOnTelloStep++;
      break;
    case 2:
      // USB接続を待つ
      if (isCharging())
      {
        _powerOnTelloTimer.startTimer();
        _powerOnTelloStep++;
      }
      break;
    case 3:
      // 1秒後に充電を終了する
      if (_powerOnTelloTimer.getTime() >= 1000)
      {
        stopCharge();
        _powerOnTelloStep++;
      }
      break;
    default:
      _powerOnTelloStep = 0;
      return true;
  }
  return false;
}

/**
 * @brief WASDでサーボの角度を微調整する
 *
 * @param input キーボード入力
 */
void ChargeController::wasdControl(char input)
{
  _servo.wasdControl(input);
}

/**
 * @brief 充電中かどうか
 *
 * @return true 充電中
 * @return false 充電中でない
 */
bool ChargeController::isCharging(void)
{
  return _servo.isCatchDrone() && _servo.isConnectUsb() && _fet.read();
}

/**
 * @brief ループ処理
 *
 */
void ChargeController::loop(void)
{
  if (_startChargeLoop())
  {
    logger.info("Finish to start charge");
  }
  if (_stopChargeLoop())
  {
    logger.info("Finish to stop charge");
  }
  if (_powerOnTelloLoop())
  {
    logger.info("Finish to power on Tello");
  }
  _servo.loop();
}

/**
 * @brief サーボ角度の文字列を取得する
 *
 * @return String サーボ角度の文字列
 */
String ChargeController::toString(void)
{
  return _servo.toString();
}
