#include <Arduino.h>
#include "board_config.h"
#include "board_pins.h"
#include "lf_frame.h"
#include "pin_validation.h"
#include "lf_reader.h"

void lfRead() {
  if (!maen::inputPin(MAEN_LF_RX) || boardPinsConflict()) {
    Serial.println("LF reader disabled: configure a safe, unshared RX pin for the actual reader.");
    return;
  }
  HardwareSerial reader(2);
  reader.begin(9600, SERIAL_8N1, MAEN_LF_RX, -1);
  Serial.println("Waiting up to 10 seconds for an ID-12LA ASCII tag...");
  maen::LfFrameParser parser;
  const unsigned long start = millis();
  bool found = false;
  while (millis() - start < 10000UL && !found) {
    while (reader.available() && !found) {
      if (parser.feed(static_cast<uint8_t>(reader.read()))) {
        Serial.printf("LF tag: %s (checksum valid)\n", parser.id());
        found = true;
      }
    }
    delay(5);
  }
  if (!found) Serial.println("No valid LF tag received.");
  reader.end();
}
