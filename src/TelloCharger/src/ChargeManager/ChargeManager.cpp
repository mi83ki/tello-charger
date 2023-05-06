/**
 * @file ChargeManager.cpp
 * @brief 充電管理クラス
 * @author Tatsuya Miyazaki
 * @date 2023/5/6
 *
 * @details 充電指示や電流量を管理するクラス
 */

#include "ChargeManager.h"

#include <Log.h>

/**
 * @brief Construct a new Servo Controller:: Servo Controller object
 *
 */
ChargeManager::ChargeManager()
  : _controller(ChargeController())
{
}

/**
 * @brief Destroy the Servo Controller:: Servo Controller object
 *
 */
ChargeManager::~ChargeManager() {}

/**
 * @brief 充電を開始する
 * 
 */
void ChargeManager::startCharge(void)
{
  _controller.startCharge();
}

/**
 * @brief 充電を停止する
 * 
 */
void ChargeManager::stopCharge(void)
{
  _controller.stopCharge();
}

/**
 * @brief Telloの電源をONにする
 * 
 */
void ChargeManager::powerOnTello(void)
{
  _controller.powerOnTello();
}

/**
 * @brief WASDでサーボの角度を微調整する
 *
 * @param input キーボード入力
 */
void ChargeManager::wasdControl(char input)
{
  _controller.wasdControl(input);
}

/**
 * @brief 充電中かどうか
 * 
 * @return true 充電中
 * @return false 充電中でない
 */
bool ChargeManager::isCharging(void)
{
  return _controller.isCharging();
}

/**
 * @brief ループ処理
 *
 */
void ChargeManager::loop(void)
{
  _controller.loop();
}

/**
 * @brief サーボ角度の文字列を取得する
 *
 * @return String サーボ角度の文字列
 */
String ChargeManager::toString(void)
{
  return _controller.toString();
}