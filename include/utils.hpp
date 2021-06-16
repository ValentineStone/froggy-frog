#pragma once
#include <Arduino.h>
#include <util/crc16.h>

uint8_t __hex_buffer[3] = {0, 0, 0};
char* hex(uint8_t a) {
  uint8_t lo = a & 0b00001111;
  uint8_t hi = a >> 4;
  __hex_buffer[0] = hi < 10 ? hi + 48 : hi - 10 + 97;
  __hex_buffer[1] = lo < 10 ? lo + 48 : lo - 10 + 97;
  return (char*)__hex_buffer;
}

void print_uuid(Stream& stream, uint8_t* uuid) {
  for (uint8_t i = 0; i < 16; i++) {
    if (i == 4 || i == 6 || i == 8 || i == 10)
      stream.print('-');
    stream.print(hex(uuid[i]));
  }
}

bool uuid_equals(uint8_t* a, uint8_t* b) {
  for (uint8_t i = 0; i < 16; i++)
    if (a[i] != b[i])
      return false;
  return true;
};

bool uuid_null(uint8_t* a) {
  for (uint8_t i = 0; i < 16; i++)
    if (a[i])
      return false;
  return true;
};

void uuid_parse(const char* uuid, uint8_t* parsed) {
  for (uint8_t i = 0, j = 0; i < 36; i++, j++) {
    if (uuid[i] == '-') {
      j--;
      continue;
    }
    uint8_t dec = uuid[i] - (uuid[i] > 57 ? ('a' - 10) : '0');
    if (j % 2 == 0) {
      parsed[j / 2] = dec << 4;
    } else {
      parsed[j / 2] += dec;
    }
  }
}