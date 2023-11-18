/**
 * @file ControlBase.h
 * @brief 充電器制御ベースクラス
 * @author Tatsuya Miyazaki
 * @date 2023/11/18
 *
 * @details 充電器制御のベースクラス
 */

#pragma once
#include <Arduino.h>
#include <Timer.h>

#include "CurrentReader.h"
#include "FETController.h"
#include "ServoController.h"

class ControlBase {
 public:
  ControlBase(ServoController *, FETController *, CurrentReader *);
  ~ControlBase();
  virtual void start(void) = 0;
  virtual void stop(void) = 0;
  virtual void loop(void) = 0;

 private:
  /** サーボ制御部 */
  ServoController *_servo;
  /** MOSFET制御部 */
  FETController *_fet;
  /** 電流計測部 */
  CurrentReader *_current;
  /** 処理のステップ */
  uint8_t _step;
  /** 制御用のタイマー */
  Timer _timer;
};
