/**
 * @file CurrentReader.cpp
 * @brief 電流取得クラス
 * @author Tatsuya Miyazaki
 * @date 2023-09-23
 *
 */

#include "CurrentReader.h"

#include <Adafruit_INA219.h>

#include "Log.h"

Adafruit_INA219 ina219;

/**
 * @brief Constdsruct a new CurrentReader::CurrentReader object
 *
 */
CurrentReader::CurrentReader() : _isConnect(false) {
  Wire1.begin(26, 32);
  if (!ina219.begin()) {
    logger.error("CurrentReader(): Failed to find INA219 chip");
  } else {
    _isConnect = true;
  }
}

/**
 * @brief Destroy the CurrentReader::CurrentReader object
 *
 */
CurrentReader::~CurrentReader() {}

/**
 * @brief 電流センサに接続しているかどうか
 *
 * @return true 接続している
 * @return false
 */
bool CurrentReader::isConnect(void) { return _isConnect; }

/**
 * @brief 電流値を取得する
 *
 * @return float 電流値[mA]
 */
float CurrentReader::getCurrent(void) {
  if (isConnect()) {
    return ina219.getCurrent_mA();
  }
  return 0.0;
}

/**
 * @brief 電流センサ読込テスト
 *
 */
void CurrentReader::readIna219(void) {
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  Serial.print("Bus Voltage:   ");
  Serial.print(busvoltage);
  Serial.println(" V");
  Serial.print("Shunt Voltage: ");
  Serial.print(shuntvoltage);
  Serial.println(" mV");
  Serial.print("Load Voltage:  ");
  Serial.print(loadvoltage);
  Serial.println(" V");
  Serial.print("Current:       ");
  Serial.print(current_mA);
  Serial.println(" mA");
  Serial.print("Power:         ");
  Serial.print(power_mW);
  Serial.println(" mW");
  Serial.println("");
}
