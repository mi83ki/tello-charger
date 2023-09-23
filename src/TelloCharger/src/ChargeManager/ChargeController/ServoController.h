/**
 * @file ServoController.h
 * @brief サーボモータ制御クラス
 * @author Tatsuya Miyazaki
 * @date 2023/5/5
 *
 * @details サーボモータを管理するクラス
 */

#pragma once
#include <Arduino.h>
#include <ServoESP32.h>

#include "Log.h"

class ServoController {
 public:
  ServoController(uint8_t, uint8_t);
  ~ServoController();
  float getServoCatch(void);
  void setServoCatch(float);
  float getServoUsb(void);
  void setServoUsb(float);
  void catchDrone(void);
  void catchDroneOnce(void);
  void releaseDrone(void);
  void connectUsb(void);
  void disconnectUsb(void);
  bool isCatchDrone(void);
  bool isReleaseDrone(void);
  bool isConnectUsb(void);
  bool isDisconnectUsb(void);
  void wasdControl(char);
  void loop(void);
  String toString(void);

 private:
  /** ドローン捕獲サーボ */
  ServoESP32 _servoCatch;
  /** USBを動かすサーボ */
  ServoESP32 _servoUsb;

  static const int16_t DRONE_RELEASE_ANGLE;
  static const int16_t DRONE_CATCH_ANGLE;
  static const int16_t DRONE_CATCH_ANGLE_ONCE;
  static const int16_t USB_OFF_ANGLE;
  static const int16_t USB_ON_ANGLE;
  static const float SERVO_CATCH_VEL;
  static const float SERVO_USB_VEL;
};
