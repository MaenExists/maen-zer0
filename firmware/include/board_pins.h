#pragma once
#include "board_config.h"
#include "pin_validation.h"

inline bool boardPinsConflict() {
  // Until a shared-bus implementation is verified, every enabled peripheral
  // must use distinct pins. Negative values mean disabled.
  const int pins[] = {MAEN_SD_SCK, MAEN_SD_MISO, MAEN_SD_MOSI, MAEN_SD_CS,
                      MAEN_LF_RX, MAEN_IR_RX, MAEN_IR_TX,
                      MAEN_NFC_SDA, MAEN_NFC_SCL, MAEN_NFC_IRQ, MAEN_NFC_RESET,
                      MAEN_RF_SCK, MAEN_RF_MISO, MAEN_RF_MOSI, MAEN_RF_CS};
  return maen::pinsConflict(pins, sizeof(pins) / sizeof(pins[0]));
}
