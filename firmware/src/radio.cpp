#include <Arduino.h>
#include <SPI.h>
#include "board_config.h"
#include "board_pins.h"
#include "pin_validation.h"
#include "radio.h"

namespace {
SPIClass radioBus(HSPI);

bool readStatus(uint8_t address, uint8_t &value) {
  radioBus.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(MAEN_RF_CS, LOW);
  const unsigned long start = micros();
  while (digitalRead(MAEN_RF_MISO) == HIGH && micros() - start < 10000UL) delayMicroseconds(10);
  if (digitalRead(MAEN_RF_MISO) == HIGH) {
    digitalWrite(MAEN_RF_CS, HIGH);
    radioBus.endTransaction();
    return false;
  }
  radioBus.transfer(address | 0xC0);  // CC1101 status registers require burst-read bit.
  value = radioBus.transfer(0);
  digitalWrite(MAEN_RF_CS, HIGH);
  radioBus.endTransaction();
  return true;
}
}  // namespace

void radioInfo() {
  if (!maen::sdPinsValid(MAEN_RF_SCK, MAEN_RF_MISO, MAEN_RF_MOSI, MAEN_RF_CS) || boardPinsConflict()) {
    Serial.println("RF disabled: configure safe, unshared CC1101 SPI pins first.");
    return;
  }
  pinMode(MAEN_RF_CS, OUTPUT);
  digitalWrite(MAEN_RF_CS, HIGH);
  radioBus.begin(MAEN_RF_SCK, MAEN_RF_MISO, MAEN_RF_MOSI, MAEN_RF_CS);
  uint8_t part = 0, version = 0;
  const bool read = readStatus(0x30, part) && readStatus(0x31, version);
  radioBus.end();
  if (!read || part != 0 || version == 0 || version == 0xff) {
    Serial.println("CC1101 not detected or returned an invalid status.");
    return;
  }
  Serial.printf("CC1101 status: part=0x%02X version=0x%02X (unverified hardware)\n", part, version);
}
