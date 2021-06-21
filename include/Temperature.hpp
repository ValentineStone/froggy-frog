#pragma once
#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define TEMPERATURE_CACHED_ADDRESSES_COUNT 8
OneWire temperatureOneWire(A0);
DallasTemperature dallasTemperature(&temperatureOneWire);
DeviceAddress temperatureDeviceAddresses[TEMPERATURE_CACHED_ADDRESSES_COUNT];
uint8_t temperatureDeviceAddressesMap = 0;

void temperature_try_cache_address(uint8_t port) {
  if (dallasTemperature.getAddress(temperatureDeviceAddresses[port], port)) {
    temperatureDeviceAddressesMap |= 1 << port;
  }
}

float temperature_reading_on_port(uint8_t port) {
  float value;
  if (temperatureDeviceAddressesMap & (1 << port)) {
    dallasTemperature.requestTemperaturesByAddress(
        temperatureDeviceAddresses[port]);
    value = dallasTemperature.getTempC(temperatureDeviceAddresses[port]);
    if (value == -127)
      temperatureDeviceAddressesMap ^= 1 << port;
  } else {
    dallasTemperature.requestTemperaturesByIndex(port);
    value = dallasTemperature.getTempCByIndex(port);
    if (port < TEMPERATURE_CACHED_ADDRESSES_COUNT && value != -127)
      temperature_try_cache_address(port);
  }
  return value;
}

void temperature_setup() {
  dallasTemperature.begin();
  for (uint8_t i = 0; i < TEMPERATURE_CACHED_ADDRESSES_COUNT; i++)
    temperature_try_cache_address(i);
}