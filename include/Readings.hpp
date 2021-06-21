#pragma once
#include <Arduino.h>
#include "Temperature.hpp"
#include "Voltage.hpp"

struct __attribute__((packed)) Reading {
  uint8_t port;
  float value;
};

VoltageSensor volatge_sensor_a6(A6, 100, 0.156, 220);
VoltageSensor volatge_sensor_a7(A7, 100, 0.156, 220);

void readings_setup() {
  temperature_setup();
}
void readings_loop() {
}
Reading reading_on_port(uint8_t port) {
  float value;
  if (port == 255)
    value = volatge_sensor_a7.reading();
  else if (port == 254)
    value = volatge_sensor_a6.reading();
  else
    value = temperature_reading_on_port(port);
  return Reading{port, value};
}