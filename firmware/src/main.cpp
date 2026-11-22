#include <Arduino.h>
#include <BLEDevice.h>
#include <WiFi.h>
#include <esp_bt_main.h>
#include <atomic>
#include "command_parser.h"
#include "storage.h"
#include "lf_reader.h"
#include "infrared.h"
#include "nfc.h"
#include "radio.h"

namespace {
maen::CommandParser commandParser;

// Radio broadcasts are untrusted input; keep control characters off the terminal.
void printSafe(const String &value) {
  for (size_t i = 0; i < value.length(); ++i) {
    const uint8_t c = static_cast<uint8_t>(value[i]);
    Serial.write(c >= 32 && c < 127 ? c : '?');
  }
}

void help() {
  Serial.println("Commands: help | info | status | wifi scan | ble scan | sd info | sd test | lf read | ir read | ir send | nfc read | rf info");
  Serial.println("Scans observe nearby broadcasts; no credentials are needed.");
}

void info() {
  Serial.println("maen-zer0 experimental firmware 0.1.0");
  Serial.printf("Chip: %s rev %d, cores: %d\n", ESP.getChipModel(),
                ESP.getChipRevision(), ESP.getChipCores());
  Serial.printf("Flash: %u bytes, free heap: %u bytes, uptime: %lu s\n",
                ESP.getFlashChipSize(), ESP.getFreeHeap(), millis() / 1000UL);
}

void status() {
  Serial.println("Wi-Fi scan: available; BLE scan: available");
  Serial.println("microSD: experimental; disabled until pins are configured and tested");
  Serial.println("125 kHz ID-12LA reader: experimental, disabled until pin and reader are verified");
  Serial.println("IR NEC capture/send: experimental, disabled until pins and hardware are verified");
  Serial.println("PN532 NFC read: experimental, disabled until pins and hardware are verified");
  Serial.println("CC1101: experimental SPI probe only; RF receive/transmit not implemented");
  Serial.println("OLED/buttons: not implemented");
}

void wifiScan() {
  Serial.println("Scanning Wi-Fi passively; this may take several seconds...");
  if (!WiFi.mode(WIFI_STA)) {
    Serial.println("Wi-Fi initialization failed");
    return;
  }
  WiFi.disconnect();
  const int count = WiFi.scanNetworks(false, true, true, 150);
  if (count < 0) {
    Serial.printf("Wi-Fi scan failed (%d)\n", count);
  } else {
    Serial.printf("Found %d networks\n", count);
    for (int i = 0; i < count; ++i) {
      Serial.printf("%2d  ch %-2d  %4d dBm  %s  ", i + 1, WiFi.channel(i),
                    WiFi.RSSI(i), WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "open" : "secured");
      const String ssid = WiFi.SSID(i);
      if (ssid.isEmpty()) Serial.print("<hidden>");
      else printSafe(ssid);
      Serial.println();
    }
  }
  WiFi.scanDelete();
  WiFi.mode(WIFI_OFF);
}

class ScanReporter : public BLEAdvertisedDeviceCallbacks {
 public:
  unsigned int shown = 0;
  void onResult(BLEAdvertisedDevice device) override {
    if (shown >= 32) return;
    ++shown;
    Serial.printf("%2u  %4d dBm  ", shown, device.getRSSI());
    printSafe(String(device.getAddress().toString().c_str()));
    if (device.haveName()) {
      Serial.print("  ");
      printSafe(String(device.getName().c_str()));
    }
    Serial.println();
  }
};

ScanReporter scanReporter;
std::atomic<bool> scanComplete(false);
void onScanComplete(BLEScanResults) { scanComplete.store(true); }

void bleScan() {
  Serial.println("Scanning BLE passively for 5 seconds...");
  BLEDevice::init("");
  if (esp_bluedroid_get_status() != ESP_BLUEDROID_STATUS_ENABLED) {
    Serial.println("BLE initialization failed");
    BLEDevice::deinit(false);
    return;
  }
  BLEScan *scan = BLEDevice::getScan();
  scan->setActiveScan(false);
  scanReporter.shown = 0;
  // Duplicate mode avoids accumulating unbounded device records in the BLE library.
  scan->setAdvertisedDeviceCallbacks(&scanReporter, true);
  scanComplete.store(false);
  if (!scan->start(5, onScanComplete, false)) {
    Serial.println("BLE scan failed to start");
    BLEDevice::deinit(false);
    return;
  }
  const unsigned long started = millis();
  while (!scanComplete.load() && millis() - started < 10000UL) delay(10);
  if (!scanComplete.load()) {
    scan->stop();
    Serial.println("BLE scan timed out");
    scan->clearResults();
    BLEDevice::deinit(false);
    return;
  }
  Serial.printf("Displayed %u advertisements (limit 32; duplicates possible)\n", scanReporter.shown);
  scan->clearResults();
  // Releasing controller memory is irreversible until reset; retain it so a
  // second scan in the same session can initialize Bluetooth again.
  BLEDevice::deinit(false);
}

void runCommand(maen::Command command) {
  switch (command) {
    case maen::Command::Help: help(); break;
    case maen::Command::Info: info(); break;
    case maen::Command::Status: status(); break;
    case maen::Command::WifiScan: wifiScan(); break;
    case maen::Command::BleScan: bleScan(); break;
    case maen::Command::SdInfo: storageInfo(); break;
    case maen::Command::SdTest: storageTest(); break;
    case maen::Command::LfRead: lfRead(); break;
    case maen::Command::IrRead: infraredRead(); break;
    case maen::Command::IrSend: infraredSend(); break;
    case maen::Command::NfcRead: nfcRead(); break;
    case maen::Command::RfInfo: radioInfo(); break;
    case maen::Command::Unknown: Serial.println("Unknown command. Type help."); break;
    case maen::Command::TooLong: Serial.println("Command too long; discarded."); break;
    case maen::Command::None: break;
  }
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\nmaen-zer0 ready. Type help.");
}

void loop() {
  while (Serial.available()) {
    runCommand(commandParser.feed(static_cast<char>(Serial.read())));
  }
  delay(5);
}
