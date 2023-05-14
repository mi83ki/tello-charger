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
const uint8_t ChargeController::CATCH_CNT = 2;

/**
 * @brief Construct a new Servo Controller:: Servo Controller object
 *
 */
ChargeController::ChargeController()
  : _servo(ServoController(SERVO_CATCH_PIN, SERVO_USB_PIN))
  , _fet(FETController(CHARGE_CONTROL_PIN))
  , _initStep(0)
  , _chargeStep(0)
  , _startChargeStep(0)
  , _stopChargeStep(0)
  , _powerOnTelloStep(0)
  , _powerOnTelloTimer(Timer())
  , _isPowerOnTelloFinished(false)
  , _catchCnt(0)
  , _chargeRetryCnt(0)
  , _catchCntTarget(1)
  , _chargeTimer(Timer())
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
 * @return true 処理完了
 * @return false 処理中
 */
bool ChargeController::_initLoop()
{
  switch (_initStep)
  {
    case 0:
      // 何もしない
      break;
    case 1:
      // MOSFETをOFFする
      if (_fet.read())
      {
        _fet.off();
        _chargeTimer.stopTimer();
        logger.info("chargeLoop(): stop charge timer, " + String(getChargeTimeMillis()));
      }
      else
      {
        _initStep++;
      }
      break;
    case 2:
      // USBアームが接続中であれば離す
      if (_servo.isConnectUsb())
        _servo.disconnectUsb();
      else if (_servo.isDisconnectUsb())
        _initStep++;
      break;
    case 3:
      // 捕獲アームが捕獲中の場合は戻す
      if (_servo.isCatchDrone())
        _servo.releaseDrone();
      else if (_servo.isReleaseDrone())
        _initStep++;
      break;
    default:
      // 初期位置に戻す処理完了
      _initStep = 0;
      return true;
  }
  return false;
}

/**
 * @brief 充電接続するループ処理
 *
 * @param catchCnt 捕獲時に繰り返す回数
 * @return true 処理完了
 * @return false 処理中
 */
bool ChargeController::_chargeLoop(uint8_t catchCnt)
{
  switch (_chargeStep)
  {
    case 0:
      // 何もしない
      break;
    case 1:
      // 初期化
      _catchCnt = 0;
      _chargeRetryCnt = 0;
      _chargeStep++;
    case 2:
      // 捕獲する
      if (_servo.isReleaseDrone())
      {
        if (catchCnt - _catchCnt <= 1)
        {
          _servo.catchDrone();
        }
        else
        {
          _servo.catchDroneOnce();
        }
        _catchCnt++;
      }
      else if (_servo.isCatchDrone())
      {
        if (_catchCnt >= catchCnt)
          // 指定した回数捕獲したら次に進む
          _chargeStep++;
        else
          _servo.releaseDrone();
      }
      break;
    case 3:
      // USBを接続する
      if (_servo.isDisconnectUsb())
        _servo.connectUsb();
      else if (_servo.isConnectUsb())
      {
        if (_chargeRetryCnt < 1)
        {
          _catchCnt = 0;
          _chargeStep = 5;
          _chargeRetryCnt++;
        }
        else
        {
          _chargeStep++;
        }
      }
      break;
    case 4:
      // MOSFETをONにする
      if (!_fet.read())
      {
        _fet.on();
        _chargeTimer.startTimer();
        logger.info("chargeLoop(): start charge timer");
      }
      else
      {
        _chargeStep = 99;
      }
      break;
    case 5:
      if (_servo.isConnectUsb())
        _servo.disconnectUsb();
      else if (_servo.isDisconnectUsb())
        _chargeStep = 2;
      break;
    default:
      // 初期位置に戻す処理完了
      _chargeStep = 0;
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
  _catchCntTarget = CATCH_CNT;
}

/**
 * @brief 充電を開始する
 *
 * @param catchCnt 捕獲繰り返し回数
 */
void ChargeController::startCharge(uint8_t catchCnt)
{
  _startChargeStep = 1;
  _catchCntTarget = catchCnt;
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
      // 初期処理
      _initStep = 1;
      _startChargeStep++;
      break;
    case 2:
      // アームを初期位置に戻す
      if (_initLoop())
      {
        _chargeStep = 1;
        _startChargeStep++;
      }
      break;
    case 3:
      // 充電接続する
      if (_chargeLoop(_catchCntTarget))
      {
        _startChargeStep++;
      }
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
  switch (_stopChargeStep)
  {
    case 0:
      // 何もしない
      break;
    case 1:
      // 初期処理
      _initStep = 1;
      _stopChargeStep++;
      break;
    case 2:
      // アームを初期位置に戻す
      if (_initLoop())
      {
        _stopChargeStep++;
      }
      break;
    default:
      // 充電停止完了
      _stopChargeStep = 0;
      return true;
  }
  return false;
}

/**
 * @brief Telloの電源をONにする
 *
 */
void ChargeController::powerOnTello(void)
{
  _powerOnTelloStep = 1;
  _isPowerOnTelloFinished = false;
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
      startCharge(1);
      _powerOnTelloStep++;
      break;
    case 2:
      // USB接続を待つ
      if (_startChargeStep == 0)
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
    case 4:
      // 終了を待つ
      if (_stopChargeStep == 0)
      {
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
 * @brief 初期位置かどうか
 *
 * @return true 初期位置
 * @return false 初期位置でない
 */
bool ChargeController::isInitPos(void)
{
  return _servo.isReleaseDrone() && _servo.isDisconnectUsb() && !_fet.read();
}

/**
 * @brief Tello電源ON処理が完了したかどうか
 *
 * @return true 完了
 * @return false 完了していない
 */
bool ChargeController::isPowerOnFinished(void)
{
  return _isPowerOnTelloFinished;
}

/**
 * @brief 充電時間を取得する
 *
 * @return uint32_t 充電時間[ms]
 */
uint32_t ChargeController::getChargeTimeMillis(void)
{
  return _chargeTimer.getTime();
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
    _isPowerOnTelloFinished = true;
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
