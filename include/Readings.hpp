#pragma once
#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define CACHED_ADDRESSES_COUNT 8

OneWire oneWire(A0);
DallasTemperature dallasTemperature(&oneWire);
DeviceAddress deviceAddresses[CACHED_ADDRESSES_COUNT];
uint8_t deviceAddressesMap = 0;

struct __attribute__((packed)) Reading {
  uint8_t port;
  float value;
};

void try_cache_address(uint8_t port) {
  if (dallasTemperature.getAddress(deviceAddresses[port], port)) {
    deviceAddressesMap |= 1 << port;
    Serial.print("For port ");
    Serial.print(port);
    Serial.print(" got addres ");
    for (uint8_t j = 0; j < 8; j++) {
      Serial.print((int)deviceAddresses[port][j]);
      Serial.print(":");
    }
    Serial.println();
  }
}

void readings_setup() {
  dallasTemperature.begin();
  Serial.println("Caching addresses...");
  for (uint8_t i = 0; i < CACHED_ADDRESSES_COUNT; i++)
    try_cache_address(i);
}

Reading get_reading_from_port(uint8_t port) {
  float temp;
  if (deviceAddressesMap & (1 << port)) {
    Serial.println("Getting optimized...");
    dallasTemperature.requestTemperaturesByAddress(deviceAddresses[port]);
    temp = dallasTemperature.getTempC(deviceAddresses[port]);
    if (temp == -127) {
      Serial.print("Clearing cache for");
      Serial.println(port);
      deviceAddressesMap ^= 1 << port;
    }
  } else {
    Serial.println("Getting slow...");
    dallasTemperature.requestTemperaturesByIndex(port);
    temp = dallasTemperature.getTempCByIndex(port);
    if (port < CACHED_ADDRESSES_COUNT && temp != -127) {
      Serial.print("Caching ");
      Serial.println(port);
      try_cache_address(port);
    }
  }
  return Reading{port, temp};
}
