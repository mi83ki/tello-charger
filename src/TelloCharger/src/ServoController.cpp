/**
 * @file ServoController.cpp
 * @brief サーボモータ制御クラス
 * @author Tatsuya Miyazaki
 * @date 2023/5/5
 *
 * @details サーボモータを管理するクラス
 */

#include "ServoController.h"

/** ドローンを離すときのサーボ角度[deg] */
const int16_t ServoController::DRONE_RELEASE_ANGLE = -31;
/** ドローンを捕まえるときのサーボ角度[deg] */
const int16_t ServoController::DRONE_CATCH_ANGLE = 8;
/** USBを離すときのサーボ角度[deg] */
const int16_t ServoController::USB_OFF_ANGLE = -56;
/** USBを接触させるときのサーボ角度[deg] */
const int16_t ServoController::USB_ON_ANGLE = 50;
/** 捕獲サーボの速度[deg/s] */
const float ServoController::SERVO_CATCH_VEL = 45;
/** USBサーボの速度[deg/s] */
const float ServoController::SERVO_USB_VEL = 150;

/**
 * @brief Construct a new Servo Controller:: Servo Controller object
 * 
 */
ServoController::ServoController()
  : servoCatch(ServoESP32(0, 19, DRONE_RELEASE_ANGLE, SERVO_CATCH_VEL))
  , servoUsb(ServoESP32(2, 23, USB_OFF_ANGLE, SERVO_USB_VEL))
{
}

/**
 * @brief Destroy the Servo Controller:: Servo Controller object
 * 
 */
ServoController::~ServoController() {}

/**
 * @brief 捕獲サーボの現在角度を取得する
 * 
 * @return float 捕獲サーボの角度
 */
float ServoController::getServoCatch(void)
{
  return servoCatch.getTargetAngle();
}

/**
 * @brief 捕獲サーボの角度を指定する
 * 
 * @param angle 捕獲サーボの角度
 */
void ServoController::setServoCatch(float angle)
{
  servoCatch.setTargetAngle(angle);
}

/**
 * @brief USBサーボの角度を指定する
 * 
 * @return float USBサーボの角度
 */
float ServoController::getServoUsb(void)
{
  return servoUsb.getTargetAngle();
}

/**
 * @brief USBサーボの角度を指定する
 * 
 * @param angle USBサーボの角度
 */
void ServoController::setServoUsb(float angle)
{
  servoUsb.setTargetAngle(angle);
}

/**
 * @brief ドローンを捕獲する
 * 
 */
void ServoController::catchDrone(void)
{
  servoCatch.setTargetAngle(DRONE_CATCH_ANGLE);
}

/**
 * @brief ドローンをリリースする
 * 
 */
void ServoController::releaseDrone(void)
{
  servoCatch.setTargetAngle(DRONE_RELEASE_ANGLE);
}

/**
 * @brief USBを接続する
 * 
 */
void ServoController::connectUsb(void)
{
  servoUsb.setTargetAngle(USB_ON_ANGLE);
}

/**
 * @brief USBを切断する
 * 
 */
void ServoController::disconnectUsb(void)
{
  servoUsb.setTargetAngle(USB_OFF_ANGLE);
}

/**
 * @brief ループ処理
 *
 */
void ServoController::loop(void)
{
  servoCatch.loop();
  servoUsb.loop();
}

/**
 * @brief サーボ角度の文字列を取得する
 *
 * @return String サーボ角度の文字列
 */
String ServoController::toString(void)
{
  return ("Servo Angle (catch, usb) = (" + String(servoCatch.getTargetAngle()) + ", " +
          String(servoUsb.getTargetAngle()) + ")\n");
}
