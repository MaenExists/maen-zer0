#pragma once
#include <stddef.h>

namespace maen {
// Conservative original-ESP32 GPIO allow-list: excludes flash, UART, boot
// strapping pins and input-only pins for outputs. Physical board wiring still
// needs verification; the firmware cannot detect electrical conflicts.
inline bool outputPin(int pin) {
  return pin == 16 || pin == 17 || pin == 18 || pin == 19 || pin == 21 ||
         pin == 22 || pin == 23 || pin == 25 || pin == 26 || pin == 27 ||
         pin == 32 || pin == 33;
}
inline bool inputPin(int pin) {
  return outputPin(pin) || pin == 34 || pin == 35 || pin == 36 || pin == 39;
}
inline bool sdPinsValid(int sck, int miso, int mosi, int cs) {
  return outputPin(sck) && inputPin(miso) && outputPin(mosi) && outputPin(cs) &&
         sck != miso && sck != mosi && sck != cs && miso != mosi && miso != cs && mosi != cs;
}
inline bool pinsConflict(const int *pins, size_t count) {
  for (size_t i = 0; i < count; ++i)
    for (size_t j = i + 1; j < count; ++j)
      if (pins[i] >= 0 && pins[i] == pins[j]) return true;
  return false;
}
}  // namespace maen
