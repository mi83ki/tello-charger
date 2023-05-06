/**
 * @file ChargeManager.h
 * @brief 充電管理クラス
 * @author Tatsuya Miyazaki
 * @date 2023/5/6
 *
 * @details 充電指示や電流量を管理するクラス
 */

#pragma once
#include <Arduino.h>

#include <Timer.h>
#include "ChargeController/ChargeController.h"

class ChargeManager
{
public:
  ChargeManager();
  ~ChargeManager();
  void startCharge(void);
  void stopCharge(void);
  void powerOnTello(void);
  void wasdControl(char);
  bool isCharging(void);
  void loop(void);
  String toString(void);

private:
  ChargeController _controller;
};
