#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include "board_config.h"
#include "board_pins.h"
#include "infrared.h"
#include "pin_validation.h"

namespace {
bool hasCode = false;
uint64_t lastCode = 0;

}  // namespace

void infraredRead() {
  if (!maen::inputPin(MAEN_IR_RX) || boardPinsConflict()) {
    Serial.println("IR receive disabled: configure a safe, unshared receiver pin.");
    return;
  }
  IRrecv receiver(MAEN_IR_RX);
  decode_results results;
  receiver.enableIRIn();
  Serial.println("Waiting up to 10 seconds for a 32-bit NEC remote signal...");
  const unsigned long start = millis();
  bool found = false;
  while (millis() - start < 10000UL && !found) {
    if (receiver.decode(&results)) {
      if (results.decode_type == NEC && results.bits == 32 && !results.repeat) {
        lastCode = results.value;
        hasCode = found = true;
        Serial.printf("NEC 32-bit code: %08lX\n", static_cast<unsigned long>(lastCode));
      }
      receiver.resume();
    }
    delay(5);
  }
  receiver.disableIRIn();
  if (!found) Serial.println("No supported IR signal received; last valid code is unchanged.");
}

void infraredSend() {
  if (!maen::outputPin(MAEN_IR_TX) || boardPinsConflict()) {
    Serial.println("IR transmit disabled: configure a safe, unshared transmitter pin.");
    return;
  }
  if (!hasCode) {
    Serial.println("No captured NEC code in memory; use ir read first.");
    return;
  }
  IRsend sender(MAEN_IR_TX);
  sender.begin();
  sender.sendNEC(lastCode, 32);
  Serial.println("Sent last captured NEC code.");
}
