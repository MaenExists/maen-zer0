# maen-zer0 experimental firmware

An early custom firmware prototype for an original ESP32 with 4 MB flash. This is a separate experiment from the Bruce plan, not a full replacement or a Flipper/Bruce equivalent. It uses USB serial at 115200 baud and does not require a screen or external modules.

Commands: `help`, `info`, `status`, `wifi scan`, `ble scan`, `sd info`, `sd test`, `lf read`, `ir read`, `ir send`, `nfc read` and `rf info`. Wi-Fi and BLE scans listen for nearby broadcasts; scans block the command interface for their duration. The device does not connect to Wi-Fi or store credentials. BLE output is limited to 32 advertisements and can include duplicates; devices that do not advertise during the scan may not appear. Output sanitizes non-ASCII characters.

The OLED and buttons are not implemented. Peripheral commands are experimental and disabled by default: `board_config.h` needs valid pins for the actual 3.3 V-safe hardware. `sd info` reports card information; `sd test` creates, reads and removes a temporary file and refuses to overwrite one already present. `lf read` validates ID-12LA ASCII frames; it does not write or emulate tags. IR is limited to capture and resend of a 32-bit NEC code, and requires both a demodulating receiver and a driven IR LED. `nfc read` identifies ISO14443-A tag UIDs using PN532 I2C with IRQ and reset. `rf info` only reads CC1101 status registers; it does not receive or transmit radio signals. None of these peripheral functions has been hardware-tested. Exact parts, pin assignments, compatibility, and electrical characteristics must be established before integration. Use authorized devices and environments.

## Build

Install PlatformIO Core, then run from this directory:

```sh
pio run -e esp32dev
```

`pio run -e esp32dev-peripherals-build` compiles a profile with illustrative pins to exercise code excluded by the disabled defaults. **It is not a wiring plan and must not be flashed as-is.** The current collision guard requires separate pins; shared SPI/I2C buses are future work.

The resulting application is `.pio/build/esp32dev/firmware.bin`. The selected `huge_app.csv` layout has one large application slot and does not provide a second slot for OTA updates. Flashing replaces existing firmware; back up flash if its contents matter. The board-specific upload command is `pio run -e esp32dev -t upload --upload-port /dev/ttyUSB0`. Use the actual port and verify the full image/partition arrangement first. For serial interaction use `pio device monitor -p /dev/ttyUSB0 -b 115200` and send commands with a newline. USB serial can reset the board when opened.

## Verification

Run `python -m unittest discover -s tests -p 'test_*.py'` for image-check tests, and `g++ -std=c++17 -Wall -Wextra -Werror -fsanitize=address,undefined -I include tests/command_parser_test.cpp -o /tmp/maen-parser-test && /tmp/maen-parser-test` for parser and pin checks. Run `python tools/check_image.py .pio/build/esp32dev` after building to check basic placement. These checks do not establish that a board can safely be flashed or that its firmware works. After installation, `python tools/device_smoke.py --port /dev/ttyUSB0` (with pyserial installed) exercises serial, disabled SD, and repeated scans without printing nearby scan results.

See [the evaluation rubric](../docs/firmware-evaluation.md) for hardware and release gates.
