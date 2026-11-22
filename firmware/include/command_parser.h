#pragma once

#include <stddef.h>
#include <string.h>

namespace maen {
enum class Command { None, Help, Info, Status, WifiScan, BleScan, SdInfo, SdTest, LfRead, IrRead, IrSend, NfcRead, RfInfo, Unknown, TooLong };

class CommandParser {
 public:
  static constexpr size_t MaxLength = 64;

  Command feed(char c) {
    if (c == '\r' || c == '\n') {
      Command result = Command::None;
      if (overflow_) result = Command::TooLong;
      else if (length_) {
        buffer_[length_] = '\0';
        result = parse();
      }
      length_ = 0;
      overflow_ = false;
      return result;
    }
    if (overflow_) return Command::None;
    if (c < 32 || c > 126) return Command::None;
    if (length_ == MaxLength) {
      overflow_ = true;
      return Command::None;
    }
    buffer_[length_++] = c >= 'A' && c <= 'Z' ? static_cast<char>(c + ('a' - 'A')) : c;
    return Command::None;
  }

 private:
  Command parse() {
    size_t start = 0;
    while (start < length_ && buffer_[start] == ' ') ++start;
    while (length_ > start && buffer_[length_ - 1] == ' ') buffer_[--length_] = '\0';
    if (start == length_) return Command::None;
    const char *word = buffer_ + start;
    if (!strcmp(word, "help") || !strcmp(word, "?")) return Command::Help;
    if (!strcmp(word, "info")) return Command::Info;
    if (!strcmp(word, "status")) return Command::Status;
    if (!strcmp(word, "wifi scan")) return Command::WifiScan;
    if (!strcmp(word, "ble scan")) return Command::BleScan;
    if (!strcmp(word, "sd info")) return Command::SdInfo;
    if (!strcmp(word, "sd test")) return Command::SdTest;
    if (!strcmp(word, "lf read")) return Command::LfRead;
    if (!strcmp(word, "ir read")) return Command::IrRead;
    if (!strcmp(word, "ir send")) return Command::IrSend;
    if (!strcmp(word, "nfc read")) return Command::NfcRead;
    if (!strcmp(word, "rf info")) return Command::RfInfo;
    return Command::Unknown;
  }

  char buffer_[MaxLength + 1] = {};
  size_t length_ = 0;
  bool overflow_ = false;
};
}  // namespace maen
