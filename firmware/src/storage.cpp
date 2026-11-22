#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "board_config.h"
#include "board_pins.h"
#include "pin_validation.h"
#include "storage.h"

namespace {
SPIClass storageBus(VSPI);
constexpr char kTestPath[] = "/maen-zer0-test.tmp";
constexpr char kPayload[] = "maen-zer0 storage test\n";

bool mountCard() {
  if (!maen::sdPinsValid(MAEN_SD_SCK, MAEN_SD_MISO, MAEN_SD_MOSI, MAEN_SD_CS) || boardPinsConflict()) {
    Serial.println("SD disabled: configure distinct, safe pins for the actual board first.");
    return false;
  }
  storageBus.begin(MAEN_SD_SCK, MAEN_SD_MISO, MAEN_SD_MOSI, MAEN_SD_CS);
  if (!SD.begin(MAEN_SD_CS, storageBus, 4000000, "/sd", 5, false)) {
    Serial.println("SD mount failed: check card, format, power, and wiring.");
    SD.end();
    storageBus.end();
    return false;
  }
  if (SD.cardType() == CARD_NONE) {
    Serial.println("No SD card detected.");
    SD.end();
    storageBus.end();
    return false;
  }
  return true;
}
void unmountCard() { SD.end(); storageBus.end(); }
}  // namespace

void storageInfo() {
  if (!mountCard()) return;
  Serial.printf("SD card size: %llu bytes; filesystem total: %llu, used: %llu\n",
                static_cast<unsigned long long>(SD.cardSize()),
                static_cast<unsigned long long>(SD.totalBytes()),
                static_cast<unsigned long long>(SD.usedBytes()));
  unmountCard();
}

void storageTest() {
  if (!mountCard()) return;
  if (SD.exists(kTestPath)) {
    Serial.println("SD test refused: test file already exists; preserve or remove it manually.");
    unmountCard();
    return;
  }
  File file = SD.open(kTestPath, FILE_WRITE);
  if (!file) {
    Serial.println("SD test failed: cannot create test file.");
    unmountCard();
    return;
  }
  const size_t written = file.write(reinterpret_cast<const uint8_t *>(kPayload), sizeof(kPayload) - 1);
  file.close();
  file = SD.open(kTestPath, FILE_READ);
  bool verified = file && written == sizeof(kPayload) - 1;
  if (verified) {
    for (size_t i = 0; i < sizeof(kPayload) - 1; ++i) {
      if (file.read() != static_cast<unsigned char>(kPayload[i])) verified = false;
    }
    if (file.available()) verified = false;
  }
  if (file) file.close();
  const bool removed = SD.remove(kTestPath);
  Serial.printf("SD write/read: %s; temporary file cleanup: %s\n",
                verified ? "pass" : "fail", removed ? "pass" : "fail");
  unmountCard();
}
