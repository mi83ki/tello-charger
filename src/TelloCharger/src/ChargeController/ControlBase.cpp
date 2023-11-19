/**
 * @file ControlBase.cpp
 * @brief 充電器制御ベースクラス
 * @author Tatsuya Miyazaki
 * @date 2023/11/18
 *
 * @details 充電器制御のベースクラス
 */

#include "ControlBase.h"

#include <Log.h>

/**
 * @brief Construct a new Control Base:: Control Base object
 *
 * @param servo
 * @param fet
 * @param current
 */
ControlBase::ControlBase(ServoController *servo, FETController *fet,
                         CurrentReader *current)
    : _servo(servo), _fet(fet), _current(current), _step(0), _timer(Timer()) {}

/**
 * @brief Destroy the Control Base:: Control Base object
 *
 */
ControlBase::~ControlBase() {}

/**
 * @brief 処理を開始する
 *
 */
void ControlBase::start(void) {
  _step = 1;
  logger.info("start(): _step = " + String(_step));
}

/**
 * @brief 処理を終了する
 *
 */
void ControlBase::stop(void) {
  _step = 0;
  logger.info("stop(): _step = " + String(_step));
}

/**
 * @brief 実行中かどうか
 *
 * @return true
 * @return false
 */
bool ControlBase::isExecuting(void) { return _step != 0; }
