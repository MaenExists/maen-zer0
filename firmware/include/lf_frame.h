#pragma once
#include <stddef.h>
#include <stdint.h>

namespace maen {
// ID-12LA ASCII: STX, ten hex ID characters, two hex XOR checksum
// characters, CR, LF, ETX. A new STX always resynchronizes the parser.
class LfFrameParser {
 public:
  bool feed(uint8_t byte) {
    if (byte == 0x02) { active_ = true; length_ = 0; return false; }
    if (!active_) return false;
    if (length_ < 12) {
      int value = hex(byte);
      if (value < 0) { active_ = false; return false; }
      digits_[length_++] = static_cast<uint8_t>(value);
      return false;
    }
    if ((length_ == 12 && byte == '\r') || (length_ == 13 && byte == '\n')) {
      ++length_;
      return false;
    }
    if (length_ != 14 || byte != 0x03) { active_ = false; return false; }
    active_ = false;
    uint8_t checksum = 0;
    for (size_t i = 0; i < 10; i += 2) checksum ^= (digits_[i] << 4) | digits_[i + 1];
    if (checksum != ((digits_[10] << 4) | digits_[11])) return false;
    for (size_t i = 0; i < 10; ++i) id_[i] = "0123456789ABCDEF"[digits_[i]];
    id_[10] = '\0';
    return true;
  }
  const char *id() const { return id_; }

 private:
  static int hex(uint8_t c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
  }
  uint8_t digits_[12] = {};
  char id_[11] = {};
  size_t length_ = 0;
  bool active_ = false;
};
}  // namespace maen
