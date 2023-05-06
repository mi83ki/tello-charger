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

/**
 * @brief Construct a new Servo Controller:: Servo Controller object
 *
 */
ChargeController::ChargeController()
  : _servo(ServoController(SERVO_CATCH_PIN, SERVO_USB_PIN))
  , _fet(FETController(CHARGE_CONTROL_PIN))
{
}

/**
 * @brief Destroy the Servo Controller:: Servo Controller object
 *
 */
ChargeController::~ChargeController() {}

void ChargeController::startCharge(void)
{

}

void ChargeController::stopCharge(void)
{
  
}

void ChargeController::powerOnTello(void)
{
  
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
 * @brief 充電処理のサーボ動作を1ステップ進める
 *
 * @return uint8_t 充電処理のステップ
 * 0: リリース状態
 * 1: ドローンをキャッチ
 * 2: 充電開始
 * 3: 充電終了
 */
uint8_t ChargeController::advanceChargeStep(void)
{
  return _servo.advanceChargeStep();
}

/**
 * @brief ループ処理
 *
 */
void ChargeController::loop(void)
{
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
