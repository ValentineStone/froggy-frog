#pragma once
#include <Arduino.h>

struct VoltageSensor {
  VoltageSensor(uint8_t _pin, uint16_t _interval, float _top, uint16_t _volts)
      : pin(_pin), interval(_interval), top(_top), volts(_volts) {}
  uint8_t pin;
  uint16_t interval;
  float top;
  uint16_t volts;
  float reading() {
    float inst_voltage;
    double squared_voltage;
    double sum_squared_voltage = 0;
    uint64_t started = millis();
    uint64_t samples = 0;
    while (millis() - started < interval) {
      samples++;
      inst_voltage = analogRead(pin) / 1024. - 0.5;
      squared_voltage = inst_voltage * inst_voltage;
      sum_squared_voltage += squared_voltage;
    }
    double mean_square_voltage = sum_squared_voltage / samples;
    double root_mean_square_voltage = sqrt(mean_square_voltage);
    float value = root_mean_square_voltage / top * volts;
    return value;
  }
};