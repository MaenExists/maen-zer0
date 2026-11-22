#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PN532.h>
#include <new>
#include "board_config.h"
#include "board_pins.h"
#include "nfc.h"
#include "pin_validation.h"

void nfcRead() {
  if (!maen::outputPin(MAEN_NFC_SDA) || !maen::outputPin(MAEN_NFC_SCL) ||
      !maen::inputPin(MAEN_NFC_IRQ) || !maen::outputPin(MAEN_NFC_RESET) || boardPinsConflict()) {
    Serial.println("NFC disabled: configure safe, unshared SDA, SCL, IRQ and reset pins.");
    return;
  }
  if (!Wire.begin(MAEN_NFC_SDA, MAEN_NFC_SCL, 100000)) {
    Serial.println("NFC I2C initialization failed.");
    return;
  }
  // The upstream class allocates an I2C helper without a destructor. Reuse a
  // single instance instead of leaking one on every scan.
  static Adafruit_PN532 *reader = nullptr;
  if (!reader) reader = new (std::nothrow) Adafruit_PN532(MAEN_NFC_IRQ, MAEN_NFC_RESET, &Wire);
  if (!reader || !reader->begin() || !reader->getFirmwareVersion() || !reader->SAMConfig() ||
      !reader->setPassiveActivationRetries(2)) {
    Serial.println("PN532 unavailable; check wiring, I2C mode and power.");
    Wire.end();
    return;
  }
  Serial.println("Waiting up to 10 seconds for an ISO14443-A tag...");
  // The library API takes no destination size; accommodate its uint8_t length.
  uint8_t uid[255] = {};
  uint8_t length = 0;
  if (reader->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &length, 10000) &&
      (length == 4 || length == 7)) {
    Serial.print("NFC UID: ");
    for (uint8_t i = 0; i < length; ++i) Serial.printf("%02X", uid[i]);
    Serial.println();
  } else {
    Serial.println("No supported NFC tag detected.");
  }
  Wire.end();
}
