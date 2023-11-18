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
