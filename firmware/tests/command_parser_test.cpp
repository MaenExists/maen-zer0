#include "command_parser.h"
#include "pin_validation.h"
#include "lf_frame.h"
#include <cassert>
#include <string>

using maen::Command;

Command submit(maen::CommandParser &p, const std::string &s) {
  Command output = Command::None;
  for (char c : s) {
    Command next = p.feed(c);
    if (next != Command::None) output = next;
  }
  return output;
}

int main() {
  maen::CommandParser p;
  assert(submit(p, "  HeLP  \r\n") == Command::Help);
  assert(submit(p, "?\n") == Command::Help);
  assert(submit(p, "wifi scan\n") == Command::WifiScan);
  assert(submit(p, "BLE SCAN\n") == Command::BleScan);
  assert(submit(p, "sd info\n") == Command::SdInfo);
  assert(submit(p, "sd test\n") == Command::SdTest);
  assert(submit(p, "lf read\n") == Command::LfRead);
  assert(submit(p, "ir read\n") == Command::IrRead);
  assert(submit(p, "ir send\n") == Command::IrSend);
  assert(submit(p, "nfc read\n") == Command::NfcRead);
  assert(submit(p, "rf info\n") == Command::RfInfo);
  assert(submit(p, "status\n") == Command::Status);
  assert(submit(p, "info\n") == Command::Info);
  assert(submit(p, "\n  \r\n") == Command::None);
  assert(submit(p, "wifi scan now\n") == Command::Unknown);
  assert(submit(p, std::string(64, 'a') + "\n") == Command::Unknown);
  assert(submit(p, std::string(65, 'a') + "help\n") == Command::TooLong);
  assert(submit(p, "info\n") == Command::Info);
  assert(submit(p, "he\x1b" "lp\n") == Command::Help);
  assert(maen::sdPinsValid(18, 19, 23, 27));
  assert(maen::sdPinsValid(18, 34, 23, 27));
  assert(!maen::sdPinsValid(-1, -1, -1, -1));
  assert(!maen::sdPinsValid(18, 19, 23, 18));
  assert(!maen::sdPinsValid(6, 19, 23, 27));
  assert(!maen::sdPinsValid(34, 19, 23, 27));
  assert(!maen::sdPinsValid(18, 19, 23, 5));
  const int pinsOk[] = {-1, -1, 18, 19};
  const int pinsConflict[] = {-1, 18, 19, 18};
  assert(!maen::pinsConflict(pinsOk, 4));
  assert(maen::pinsConflict(pinsConflict, 4));

  maen::LfFrameParser lf;
  auto frame = [&lf](const std::string &bytes) {
    bool found = false;
    for (unsigned char c : bytes) found = lf.feed(c) || found;
    return found;
  };
  assert(frame("\x02" "0C000621A58E\r\n\x03"));
  assert(std::string(lf.id()) == "0C000621A5");
  assert(frame("garbage\x02" "0c000621a58e\r\n\x03"));
  assert(!frame("\x02" "0C000621A58F\r\n\x03"));
  assert(!frame("\x02" "0C000621A58E\n\r\x03"));
  assert(!frame("\x02" "0C0Z0621A58E\r\n\x03"));
  assert(frame("\x02" "bad\x02" "0C000621A58E\r\n\x03"));

  // Feed deterministic noise through both parsers under sanitizers.
  unsigned int random = 0x12345678;
  for (int i = 0; i < 200000; ++i) {
    random = random * 1664525u + 1013904223u;
    const auto byte = static_cast<unsigned char>(random >> 24);
    p.feed(static_cast<char>(byte));
    lf.feed(byte);
  }
  p.feed('\n');
  assert(submit(p, "info\n") == Command::Info);
  assert(frame("\x02" "0C000621A58E\r\n\x03"));
}
