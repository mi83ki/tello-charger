/**
 * @file FETController.cpp
 * @brief MOSFET制御クラス
 * @author Tatsuya Miyazaki
 * @date 2023/5/6
 *
 * @details MOSFETを制御するクラス
 */

#include "FETController.h"

#include "Log.h"


/**
 * @brief Construct a new FETController::FETController object
 *
 * @param pin ピン番号
 */
FETController::FETController(uint8_t pin) : _pin(pin)
{
  pinMode(_pin, OUTPUT);
  off();
}

/**
 * @brief Destroy the FETController::FETController object
 *
 */
FETController::~FETController() {}

/**
 * @brief FETをONにする
 *
 */
void FETController::on(void)
{
  digitalWrite(_pin, 1);
  logger.info("FETController::on()");
}

/**
 * @brief FETをOFFにする
 *
 */
void FETController::off(void)
{
  digitalWrite(_pin, 0);
  logger.info("FETController::off()");
}

/**
 * @brief FETの状態を取得する
 *
 * @return true ON
 * @return false OFF
 */
bool FETController::read(void)
{
  return digitalRead(_pin) != 0;
}
