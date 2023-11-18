/**
 * @file FETController.h
 * @brief MOSFET制御クラス
 * @author Tatsuya Miyazaki
 * @date 2023/5/6
 *
 * @details MOSFETを制御するクラス
 */

#pragma once
#include <Arduino.h>

class FETController {
 public:
  FETController(uint8_t);
  ~FETController();
  void on(void);
  void off(void);
  bool read(void);

 private:
  /** 制御ピン */
  uint8_t _pin;
};
