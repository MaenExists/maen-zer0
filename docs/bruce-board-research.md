# Bruce firmware selection for the connected ESP32

Research snapshot: 2026-11-06. The connected chip identifies as ESP32-D0WD-V3 rev 3.1 with 4 MB flash; the development-board model and OLED controller are unknown. Nothing below has been flashed or tested on this device.

## Selection

**Recommended starting point: a custom, source-built Bruce target for the original ESP32 and 4 MB flash, using the community `nemo` target as a reference.** No exact version or prebuilt binary can yet be recommended for the complete device. The 1.3-inch OLED controller, resolution and interface must be identified first; the board's pin labels and controls also need to be checked.

The [latest official release inspected](https://github.com/BruceDevices/firmware/releases/tag/1.16.1) is 1.16.1 (2026-08-11). It has original-ESP32 binaries for specific complete devices, but no identified bare original-ESP32-plus-our-OLED target. The [official headless ESP-General configuration](https://github.com/BruceDevices/firmware/blob/main/boards/ESP-General/ESP-General.ini) is for an ESP32-S3 and uses an 8 MB partition layout; it is not compatible with this board as-is. Do not flash it here. A matching chip family alone does not make another device's display, GPIO mapping or image safe to assume compatible.

The [ESP32-Flipper community project](https://github.com/Cyclik-227/ESP32-Flipper) provides a [`nemo` configuration](https://github.com/Cyclik-227/ESP32-Flipper/blob/main/boards/nemo/nemo.ini) using `esp32dev` and the 4 MB full partition layout. Its [pin configuration](https://github.com/Cyclik-227/ESP32-Flipper/blob/main/boards/nemo/pins_arduino.h) is for a 240×240 ST7789 SPI color display and four buttons, not our unidentified OLED. It disables SD and generic SPI pins in that file. Its author reports screen, controls, IR and WebUI working, while CC1101 and PN532 remain unchecked in the README. The repository contains a binary, but its provenance, exact upstream version, partition image, peripheral support and safety have not been independently verified. Do not flash it as our device image or copy its wiring uncritically.

The upstream [WROOM/ST7735 proposal](https://github.com/BruceDevices/firmware/pull/2832) is another reference for 4 MB, but is closed without merge and targets a different display. The [Brucey S3 project](https://github.com/nistordarius26h-ship-it/esp32s3-brucey) describes an SSD1306 OLED adaptation, but uses an ESP32-S3; it is a display-porting reference only, not a binary for this chip.

Pin the exact Bruce source revision only after testing whether this reference port can be reproduced against it. The official main branch inspected was commit `a59213f3ec6cd302fa32c2042b2700c9c24d991a`; that observation does not imply that `nemo` compiles against it. Its `custom_4Mb_full.csv` provides a 0x3c0000-byte factory application partition and a 0x30000-byte filesystem partition, with no second OTA application slot. Compile-time feature cuts may be necessary and must be measured, not assumed.

## Planned parts

| Part | Bruce compatibility and next check |
| --- | --- |
| 1.3-inch OLED | Unknown controller, resolution, voltage and I2C/SPI. The available `nemo` binary uses ST7789 instead; screen support and usability on a small monochrome panel need development and verification. |
| Wi-Fi / Bluetooth | Chip reports both. Which functions fit and run depends on the build. Upstream documents that its `LITE_VERSION` removes a number of capabilities, including BLE scan; inspect the actual revision and flags. |
| CC1101 | Upstream supports CC1101 on appropriate builds. `nemo`'s README lists it as pending and its generic SPI defaults are disabled. Design and test the pin map and 3.3 V supply; choose RF band/antenna for the actual location. |
| PN532 | Upstream includes PN532 support; interface and board-specific pins require verification. It handles 13.56 MHz NFC, not 125 kHz. The community `nemo` project lists its PN532 as pending. |
| 125 kHz reader | Upstream source at the inspected revision has a non-lite **Read 125kHz** menu using UART at 9600 baud with a 14-byte framed packet. ID-12LA integration is not confirmed: compare exact output mode, framing, voltage and wiring, and test real tags. This is not proof of writing or emulation. |
| IR | The reference limits enabled IR protocols to save space and reports its own receiver and transmitter tested. Identify the owned IR sensor; a demodulating remote receiver and a separately driven LED are needed for receive/transmit. |
| microSD | Supported on appropriate Bruce targets, but disabled in the `nemo` pin configuration. Select pins and test storage separately, then shared-SPI behavior with CC1101. |

## Next steps

1. Identify OLED markings and pin labels, and the development board's exact layout; record the existing IR module's part number if available.
2. Build a pinned, reviewed custom original-ESP32 Bruce configuration. Verify partition offsets, application and filesystem sizes and included capabilities. Compare the OLED integration against upstream's display abstractions before promising support.
3. If an appropriate custom build is established, back up existing flash where possible, flash the matching image, verify boot and controls, and test each connected peripheral. Successful compilation alone is not hardware validation.
4. If the OLED adaptation or 4 MB constraints make the desired suite impractical, consider a known-supported color screen or a supported ESP32-S3 with more flash; do not purchase either until the tradeoff is decided.
