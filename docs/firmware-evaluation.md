# Firmware evaluation and release gates

The goal is a modular handheld for authorized assessment, running first on the identified ESP32-D0WD-V3 with 4 MB flash. Individual capability claims require evidence for the actual hardware and configuration. No checklist item is complete merely because a dependency compiles.

## Rating

Each feature has a maturity level: **missing**, **implemented**, **host-tested**, **device-tested**, or **peripheral-validated**. Report failures and skipped tests alongside passes. A release for use on hardware requires the gates below; it does not promise immunity from power loss, damaged hardware, or every flash security configuration.

| Area | Acceptance criteria |
| --- | --- |
| Build | Reproducible pinned build; generated bootloader, partition table, and application fit within the detected flash. Record build size and versions. |
| Recovery | Detect chip, flash size and security state; refuse uncertain or incompatible hardware. Back up and verify readable existing flash before changes; store backups privately outside the repository. Never burn eFuses. Confirm ROM download mode and a recovery procedure. Esptool verifies writes, but a successful write does not prove that the application boots. |
| Boot and command interface | Cold boot and reset repeatedly, accept valid commands, reject malformed/oversized input, recover after errors, and report honest feature status. Host test parsing and edge cases. |
| Stability | Repeat Wi-Fi and BLE scans, alternate them, and check responsiveness, errors and free heap on the device. Exercise disconnects, missing peripherals and storage-full conditions when those parts exist. |
| Display and controls | Identify controller, voltage, resolution, interface and exact pins. Verify display rendering, navigation and button debounce on the actual hardware; handle missing display without boot failure. |
| Wi-Fi / Bluetooth | Device tests must independently confirm passive discovery against known test devices, repeated use, and no unintended transmissions from the chosen scan modes. BLE support is not Bluetooth Classic support. |
| IR | Known receiver and transmitter driver; test receiving and transmitting supported protocols against owned equipment. A generic obstacle sensor is not sufficient. |
| NFC | Identify PN532 variant and interface; detect module and known tags, test supported operations independently. Never equate 13.56 MHz NFC with 125 kHz. |
| 125 kHz | Confirm reader's protocol, voltage, and format, and read known compatible tags. Writing or emulation needs separate hardware and tests. |
| Sub-GHz | Identify radio, antenna, power and allowed local bands; verify configuration and receive/transmit between owned test devices. Test missing radio and bus sharing. No generic claim of support for arbitrary frequencies or secure protocols. |
| microSD | Test mount, file write/read and power-cycle persistence on actual 3.3 V-safe module and card, including shared bus with radio and failure cases. |
| Security and resilience | Limit input and result memory, sanitize terminal output, avoid hard-coded credentials, and prevent invalid pin use. Verify behavior under no memory, invalid configuration, and reset. |

## Current evidence

On 2026-11-06 the board was re-identified as ESP32-D0WD-V3 with 4 MB flash. Security fuse inspection showed secure boot and flash encryption disabled and UART download enabled. Its complete pre-existing flash was read to a private local backup and compared against the chip using esptool's digest verification. The baseline custom image was flashed with verified write hashes, booted, handled serial commands and rejected oversized input. Three alternating Wi-Fi/BLE scan cycles completed and serial remained responsive.

The newer default image was subsequently flashed with verified write hashes. It booted; all unconfigured SD, LF, IR, NFC and RF commands returned disabled rather than activating pins. Multiple additional Wi-Fi/BLE cycles completed, with reported post-cycle free heap approximately 209.8 KB. A temporary advertisement from the computer's Bluetooth adapter was observed by its known MAC in a passive BLE scan; the advertisement was stopped afterwards. No Wi-Fi networks were returned. The computer's active network was 5 GHz, outside the original ESP32's 2.4 GHz range; a known 2.4 GHz test network is still needed. This is a short smoke test, not an endurance test or a guarantee against heap leaks.

Host tests cover the command parser, conservative GPIO checks, ID-12LA frame checksums and malformed frames, and image layout checks. SD operations, the ID-12LA reader, NEC infrared, PN532 UID reading and CC1101 SPI probe are experimental and disabled by default. The physical modules, OLED, buttons, pinout, recovery restoration and shared buses have not been validated. CC1101 receive/transmit and display/button interfaces are missing. Maintain this distinction when reporting progress.

## Execution order

1. Establish testable command parsing and safe baseline with host tests and firmware compile.
2. When the board is connected, identify it again, inspect security state, back up flash where readable, install a verified image and exercise boot, commands, repeated scans and recovery.
3. Add drivers and tests incrementally, with explicit hardware profiles and no guessed pin assignments. Validate each module on the real hardware and repeat interaction tests as modules are combined.
4. Only declare a full device release once all required hardware and recovery gates pass. The current hardware inventory cannot establish full-device operation.

References: [Espressif flashing](https://docs.espressif.com/projects/esptool/en/latest/esp32/esptool/flashing-firmware.html), [esptool flash verification](https://docs.espressif.com/projects/esptool/en/latest/esp32/esptool/advanced-commands.html).
