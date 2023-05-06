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
const float ServoController::SERVO_CATCH_VEL = 60;
/** USBサーボの速度[deg/s] */
const float ServoController::SERVO_USB_VEL = 150;

/**
 * @brief Construct a new Servo Controller:: Servo Controller object
 *
 * @param catchPin 捕獲サーボの制御PIN
 * @param usbPin USBサーボの制御PIN
 */
ServoController::ServoController(uint8_t catchPin, uint8_t usbPin)
  : _servoCatch(ServoESP32(0, catchPin, DRONE_RELEASE_ANGLE, SERVO_CATCH_VEL))
  , _servoUsb(ServoESP32(2, usbPin, USB_OFF_ANGLE, SERVO_USB_VEL))
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
  return _servoCatch.getTargetAngle();
}

/**
 * @brief 捕獲サーボの角度を指定する
 *
 * @param angle 捕獲サーボの角度
 */
void ServoController::setServoCatch(float angle)
{
  _servoCatch.setTargetAngle(angle);
}

/**
 * @brief USBサーボの角度を指定する
 *
 * @return float USBサーボの角度
 */
float ServoController::getServoUsb(void)
{
  return _servoUsb.getTargetAngle();
}

/**
 * @brief USBサーボの角度を指定する
 *
 * @param angle USBサーボの角度
 */
void ServoController::setServoUsb(float angle)
{
  _servoUsb.setTargetAngle(angle);
}

/**
 * @brief ドローンを捕獲する
 *
 */
void ServoController::catchDrone(void)
{
  _servoCatch.setTargetAngle(DRONE_CATCH_ANGLE);
}

/**
 * @brief ドローンをリリースする
 *
 */
void ServoController::releaseDrone(void)
{
  _servoCatch.setTargetAngle(DRONE_RELEASE_ANGLE);
}

/**
 * @brief USBを接続する
 *
 */
void ServoController::connectUsb(void)
{
  _servoUsb.setTargetAngle(USB_ON_ANGLE);
}

/**
 * @brief USBを切断する
 *
 */
void ServoController::disconnectUsb(void)
{
  _servoUsb.setTargetAngle(USB_OFF_ANGLE);
}

/**
 * @brief ドローンをキャッチしているかどうか
 * 
 * @return true 
 * @return false 
 */
bool ServoController::isCatchDrone(void)
{
  return _servoCatch.getPresentAngle() == (float)(DRONE_CATCH_ANGLE);
}

/**
 * @brief ドローンをリリースしているかどうか
 * 
 * @return true 
 * @return false 
 */
bool ServoController::isReleaseDrone(void)
{
  return _servoCatch.getPresentAngle() == (float)(DRONE_RELEASE_ANGLE);
}

/**
 * @brief USBを接続しているかどうか
 * 
 * @return true 
 * @return false 
 */
bool ServoController::isConnectUsb(void)
{
  return _servoUsb.getPresentAngle() == (float)(USB_ON_ANGLE);
}

/**
 * @brief USBを切断しているかどうか
 * 
 * @return true 
 * @return false 
 */
bool ServoController::isDisconnectUsb(void)
{
  return _servoUsb.getPresentAngle() == (float)(USB_OFF_ANGLE);
}

/**
 * @brief WASDでサーボの角度を微調整する
 *
 * @param input キーボード入力
 */
void ServoController::wasdControl(char input)
{
  switch (input)
  {
    case 'w':
      setServoUsb(getServoUsb() + 1);
      break;
    case 's':
      setServoUsb(getServoUsb() - 1);
      break;
    case 'a':
      setServoCatch(getServoCatch() - 1);
      break;
    case 'd':
      setServoCatch(getServoCatch() + 1);
      break;
    default:
      break;
  }
  logger.info("key = " + String(input) + ", " + toString());
}

/**
 * @brief ループ処理
 *
 */
void ServoController::loop(void)
{
  _servoCatch.loop();
  _servoUsb.loop();
}

/**
 * @brief サーボ角度の文字列を取得する
 *
 * @return String サーボ角度の文字列
 */
String ServoController::toString(void)
{
  return ("Servo Angle (catch, usb) = (" + String(_servoCatch.getTargetAngle()) + ", " +
          String(_servoUsb.getTargetAngle()) + ")\n");
}
