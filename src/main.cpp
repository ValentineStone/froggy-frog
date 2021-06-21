#include <Arduino.h>
#include <SoftwareSerial.h>

#include "Message.hpp"
#include "Network.hpp"
#include "Readings.hpp"

uint8_t DEVICE_ID = 1;

#define LED_EXTERNAL 5
#define HC12_TX 2
#define HC12_RX 3
SoftwareSerial HC12 = SoftwareSerial(HC12_RX, HC12_TX);

struct Network : public NetworkBase {
  using NetworkBase::NetworkBase;
  void handleMessage() {
    // Serial.println("Got message");
    switch (inbound.message_id) {
      case MESSAGE_ID_REQUEST_VALUE: {
        digitalWrite(LED_EXTERNAL, LOW);
        uint8_t port = *inbound.payload;
        Reading reading = reading_on_port(port);
        if (reading.value == -127) {
          // Serial.print("Error reading value on port ");
          // Serial.println(port);
        } else {
          // Serial.print("On port ");
          // Serial.print(port);
          // Serial.print(" value=");
          // Serial.println(reading.value);
          respond(MESSAGE_ID_VALUE, sizeof(Reading), (uint8_t*)&reading);
        }
      }
    }
  }
  void handleBroadcast() {
    // Serial.println("Got broadcast");
  }
  void handleBadMessage() {
    // Serial.println("Got BAD message");
  }
  void handleSent() { digitalWrite(LED_EXTERNAL, HIGH); }
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

  Serial.print("Device ID: ");
  Serial.println(DEVICE_ID);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LED_EXTERNAL, HIGH);
}

void loop() {
  readings_loop();
  network.loop();
}