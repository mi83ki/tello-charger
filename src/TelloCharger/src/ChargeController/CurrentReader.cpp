/**
 * @file CurrentReader.cpp
 * @brief 電流取得クラス
 * @author Tatsuya Miyazaki
 * @date 2023-09-23
 *
 */

#include "CurrentReader.h"

#include <Adafruit_INA219.h>
#include <Log.h>
#include <M5Unified.h>
#include <Wire.h>

#include "pinConfig.h"

Adafruit_INA219 ina219;

/**
 * @brief Constdsruct a new CurrentReader::CurrentReader object
 *
 */
CurrentReader::CurrentReader(uint32_t cycleTime)
    : _isConnect(false),
      _timer(Timer(cycleTime)),
      _current(0.0),
      _movAveFilter(MovAveFilter(10, 0)) {
  Wire.begin(INA_SDA_PIN, INA_SCL_PIN);
  if (!ina219.begin()) {
    logger.error("CurrentReader(): Failed to find INA219 chip");
  } else {
    _isConnect = true;
    _movAveFilter.setData(FLOAT_TO_FIX(_getCurrent()));
    logger.info("CurrentReader(): INA219 chip found!");
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
float CurrentReader::_getCurrent(void) {
  if (isConnect()) {
    return ina219.getCurrent_mA();
  }
  return 0.0;
}

/**
 * @brief 電流値を取得する
 *
 * @return float 電流値[mA]
 */
float CurrentReader::getCurrent(void) { return _current; }

/**
 * @brief ループ処理
 *
 */
void CurrentReader::loop() {
  if (_timer.isCycleTime()) {
    float current = _getCurrent();
    _current = FIX_TO_FLOAT(_movAveFilter.movingAverage(FLOAT_TO_FIX(current)));
    // logger.trace("CurrentReader.loop(): current = " + String(current) +
    //              ", movave = " + String(_current));
  }
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
