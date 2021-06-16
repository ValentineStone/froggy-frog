#pragma once
#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

OneWire oneWire(A0);
DallasTemperature dallasTemperature(&oneWire);

struct __attribute__((packed)) Reading {
  uint8_t port;
  float value;
};

void readings_setup() {
  dallasTemperature.begin();
}

Reading get_reading_from_port(uint8_t port) {
  dallasTemperature.requestTemperatures();
  auto temp = dallasTemperature.getTempCByIndex(port);
  return Reading{port, temp};
}
