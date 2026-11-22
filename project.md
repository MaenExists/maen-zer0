# maen-zer0

A modular, handheld pentesting and electronics platform built around an ESP32 DevKit V1. Inspired by Flipper Zero, it is an independent project: it will not run Flipper firmware or automatically reproduce all of its features. The first goal is a USB-powered breadboard prototype that can display a menu, read buttons, and learn about infrared remotes and NFC tags that we own.

**Firmware direction:** Build and validate [custom maen-zer0 firmware](firmware/README.md) for this board. Bruce remains a useful reference and possible alternative, but its features are not automatically present in our firmware. See the [evaluation rubric](docs/firmware-evaluation.md) for release gates and current limitations.

## Hardware capability checklist

These are project targets. Check an item only after the hardware is integrated and its stated function is verified on the actual device. The ESP32 and proposed modules have not yet been tested together.

- [ ] **Sub-1 GHz transceiver:** Integrate a CC1101 with a suitable antenna and demonstrate receiving and transmitting between owned devices in a permitted band.
- [ ] **Customizable radio platform:** Provide configurable radio settings and an application interface for supported digital protocols, then demonstrate communication with an owned IoT or access-control test device. Supported frequencies, modulation, protocols, and hardware determine compatibility; this is not universal digital communication.
- [ ] **125 kHz RFID:** Integrate a suitable low-frequency front end and read a known compatible test tag. Writing and emulation require separate hardware and protocol verification.
- [ ] **NFC:** Integrate a compatible PN532 module and detect a known 13.56 MHz test tag. Reading, writing, and emulation support must be verified separately for each tag and mode.
- [ ] **Bluetooth:** Verify Bluetooth operation on the specific ESP32 board and implement and test a useful Bluetooth application. The proposed original ESP32 has Bluetooth hardware built in.
- [ ] **Infrared transceiver:** Integrate a receiver and driven IR LED, and demonstrate receiving and sending a supported remote protocol with owned equipment.
- [ ] **microSD card:** Integrate a compatible microSD module and verify mounting, reading, and writing files.

## What success looks like

- A small display and physical buttons provide a simple menu.
- An infrared receiver observes a known TV remote; an infrared LED can later control the owner's device.
- A 13.56 MHz NFC module identifies and reads supported, deliberately purchased test tags.
- A later CC1101 radio experiment exchanges packets between two owned modules after local frequency rules are checked.
- Add 125 kHz RFID and microSD storage after the basics work; battery, enclosure, and other peripherals can follow.
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
5. [Hardware and firmware plan](docs/hardware-firmware-plan.md): firmware comparison, existing builds, staged shopping list and integration milestones.
6. [Breadboard shopping list](docs/breadboard-shopping-list.md): quantities, selected parts and decisions needed before ordering.
7. [Bruce board research](docs/bruce-board-research.md): findings for the connected 4 MB ESP32, available firmware references and peripheral compatibility.
8. [Experimental custom firmware](firmware/README.md): an early serial-controlled ESP32 prototype, developed alongside the Bruce plan.
9. [Firmware evaluation rubric](docs/firmware-evaluation.md): capability, recovery, safety and hardware validation gates.
10. [Board recovery record](docs/board-recovery.md): the private flash backup, device-specific recovery information and its verification limits.

## Scope and current status

Research and build plan only, researched 2026-11-05; no hardware, code, wiring, library compatibility, prices, or radio compliance has been tested on the user's specific board. Confirm the exact board pin labels, module variants, country of operation, and budget before finalizing hardware purchases beyond the starter set. Build on a breadboard first; a polished compact product is a later project.
