# maen-zer0

A beginner's modular, handheld electronics learning tool built around an ESP32 DevKit V1. Inspired by Flipper Zero, it is an independent project: it will not run Flipper firmware or automatically reproduce all of its features. The first goal is a USB-powered breadboard prototype that can display a menu, read buttons, and learn about infrared remotes and NFC tags that we own.

## What success looks like

- A small display and physical buttons provide a simple menu.
- An infrared receiver observes a known TV remote; an infrared LED can later control the owner's device.
- A 13.56 MHz NFC module identifies and reads supported, deliberately purchased test tags.
- A later CC1101 radio experiment exchanges packets between two owned modules after local frequency rules are checked.
- Optional 125 kHz RFID, microSD storage, a battery, enclosure, and other peripherals can be added after the basics work.
- Firmware reports missing devices and keeps working when a module is disconnected.

## Design boundaries

The original ESP32 has Wi-Fi and Bluetooth built in, but no integrated sub-GHz radio, NFC, 125 kHz RFID front end, or IR optical parts. Its usual USB connection is a USB-to-serial bridge: native USB keyboard emulation like Flipper's requires additional hardware or a different MCU. Hardware capabilities do not imply universal protocol, tag, or secure-system compatibility. Use only equipment and credentials you own or have permission to test; do not assume captured signals can be replayed against rolling-code systems.

```text
                USB power + programming
                         |
                    ESP32 DevKit V1
              /          |          \
        I2C display   buttons      IR receiver / LED
              \
              PN532 NFC (later)
                         \
                SPI: CC1101 / microSD (later)
```

The diagram is conceptual: NFC shares the I2C bus with the display only if the specific PN532 board supports compatible 3.3 V I2C and its address does not conflict. The optional SPI devices share signal lines and need separate chip-select pins. Exact connections and constraints are in the build guide.

## Project documents

1. [How Flipper Zero works](docs/flipper-how-it-works.md): subsystems, protocols, and realistic comparisons.
2. [ESP32 and electronics primer](docs/esp32-and-electronics.md): essential concepts and electrical limits.
3. [Build and shopping guide](docs/build-guide.md): staged purchases, wiring, setup, first test, and roadmap.
4. [Research sources and decisions](docs/research.md): primary references, unknowns, and next decisions.

## Scope and current status

Research and build plan only, researched 2026-11-05; no hardware, code, wiring, library compatibility, prices, or radio compliance has been tested on the user's specific board. Confirm the exact board pin labels, module variants, country of operation, and budget before finalizing hardware purchases beyond the starter set. Build on a breadboard first; a polished compact product is a later project.
