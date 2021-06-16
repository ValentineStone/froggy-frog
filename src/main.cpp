#include <Arduino.h>
#include <SoftwareSerial.h>

#include "Message.hpp"
#include "Network.hpp"
#include "Readings.hpp"
#include "keys.h"
#include "utils.hpp"

#define LED_EXTERNAL 5
uint8_t DEVICE_ID[16];

#define HC12_TX 2
#define HC12_RX 3
SoftwareSerial HC12 = SoftwareSerial(HC12_RX, HC12_TX);

struct Network : public NetworkBase {
  using NetworkBase::NetworkBase;
  void handleMessage() {
    switch (parser.message.message_id) {
      case MESSAGE_ID_REQUEST_VALUE: {
        digitalWrite(LED_EXTERNAL, LOW);
        uint8_t port = *inbound.payload;
        Reading reading = get_reading_from_port(port);
        if (reading.value == -127) {
          Serial.print("Error reading value on port ");
          Serial.print(port);
        } else {
          Serial.print("On port ");
          Serial.print(port);
          Serial.print(" value=");
          Serial.println(reading.value);
          respond(MESSAGE_ID_VALUE, sizeof(Reading), (uint8_t*)&reading);
        }
      }
    }
  }
  void handleBadMessage() {}
  void handleTimeout() {}
  void handleAnyMessage() {}
  void handleSent(uint8_t* data, uint8_t len) {
    digitalWrite(LED_EXTERNAL, HIGH);
  }
  void handleBroadcast() {}
};

Network network(HC12, DEVICE_ID);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_EXTERNAL, OUTPUT);
  pinMode(HC12_RX, INPUT);
  pinMode(HC12_TX, OUTPUT);
  HC12.begin(9600);
  Serial.begin(9600);
  readings_setup();

  uuid_parse(DEVICE_UUID, DEVICE_ID);

  Serial.print("Device UUID: ");
  Serial.println(DEVICE_UUID);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LED_EXTERNAL, HIGH);
}

void loop() {
  network.loop();
}