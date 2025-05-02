#pragma once

#include <Arduino.h>  // String 型など
#include <ArduinoJson.h>

String buildChargeStatusJson(bool charge, float current, float chargingTime,
                             bool isStartChargeExecuting,
                             bool isStopChargeExecuting,
                             bool isPowerOnExecuting);
