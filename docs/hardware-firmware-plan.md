# Hardware and firmware plan

Research snapshot: 2026-11-06. This is a proposed build, not a tested wiring diagram or a promise that a prebuilt firmware supports our parts. Use devices and systems you own or are authorized to assess.

**Firmware decision update:** The active direction is [custom firmware](../firmware/README.md), evaluated against the [firmware rubric](firmware-evaluation.md). Bruce remains a reference and alternative; the Bruce-specific recommendations below are historical and are not verified wiring for the custom firmware. Confirm the exact display, build and peripheral support before ordering feature modules. The [Marauder plan](marauder-plan.md) records another superseded decision.

## Recommendation

Build a USB-powered prototype around the ESP32 and 1.3-inch OLED already on hand. Identify both first, then test display and buttons. Use Bruce as the primary firmware; only commit to a Bruce-specific display or replacement board after checking its current board configurations and compiling a suitable target. If adapting Bruce to the existing OLED proves impractical, choose a display and board configuration supported by Bruce. Add peripherals individually and defer the battery and enclosure.

On 2026-11-06, esptool identified the connected device as an **ESP32-D0WD-V3 (revision 3.1)**, with 4 MB of flash, Wi-Fi and Bluetooth, connected through a CP2102 USB-to-serial bridge. This confirms the original ESP32 chip family, but not the exact module or development-board model, pin labels, or OLED controller. ESP32 variants differ in flash, pins, Bluetooth and USB. A 1.3-inch OLED may use different controllers, interfaces and resolutions: physical size alone identifies none of them.

The official Bruce ESP-General headless configuration inspected on 2026-11-06 targets an ESP32-S3 and specifies an 8 MB partition layout; it is **not** a suitable image for this 4 MB original ESP32. A compatible original-ESP32 target or custom build still needs to be identified and validated before flashing. No Bruce build has been flashed or tested on this board.

## Firmware and existing builds

| Project | What it offers | Fit for maen-zer0 |
| --- | --- | --- |
| [Bruce](https://github.com/BruceDevices/firmware) | Broad suite; supported targets can use CC1101, PN532, IR and storage alongside Wi-Fi/BLE. Feature availability varies by target and build. | Best first candidate for breadth. Exact board, display, flash size, pinout and module support require validation; 125 kHz RFID support is not established for our proposed hardware. |
| [ESP32 Marauder](https://github.com/justcallmekoko/ESP32Marauder) | Focused Wi-Fi/Bluetooth research suite with several hardware-specific builds. | Valuable alternative for wireless work; not established as the integrated solution for the full checklist. Its [target index](https://github.com/justcallmekoko/ESP32Marauder/wiki/Supported-Hardware) warns to match the exact hardware and binary. |
| [CapibaraZero](https://github.com/CapibaraZero/fw) | Alternative ESP32-based handheld firmware. | Compare if choosing a supported ESP32-S3 design; do not assume compatibility with our original ESP32 or display. Its [project description](https://projecthub.arduino.cc/andreockx/15a31898-553e-45bd-9573-d899d8fbc3ae) identifies 125 kHz as unsupported. |
| [ESP32-Flipper community build](https://github.com/Cyclik-227/ESP32-Flipper) | Documents an ESP32-WROOM, 240×240 ST7789 screen, CC1101, PN532, IR and a custom Bruce board target. | A useful reference for an original ESP32, but its wiring and binary are specific to its parts. Treat its compatibility claims as unverified for our build. |
| [Brucey S3 community build](https://github.com/nistordarius26h-ship-it/esp32s3-brucey) | Describes a custom ESP32-S3, 128×64 SSD1306 OLED and Bruce adaptation. | A possible reference for small-screen adaptation, not evidence that our OLED or original ESP32 is supported. |

Do not flash a binary for a different board merely because it uses an ESP32. A firmware build must match the chip, flash layout, display, pins, inputs and connected peripherals. Verify the source revision and build from a suitable official target or a reviewed adaptation. Some Bruce lite builds omit features; check the specific build before choosing it. Firmware cannot compensate for missing radio front ends or unsupported protocols.

## Shopping list

### Buy first, after checking what you already own

| Qty | Item | Purpose and selection |
| --- | --- | --- |
| 1 | Full-size solderless breadboard, or two | Some wide DevKit boards block access to the rows on a narrow board. |
| 1 set | Jumper wires, male-to-male and male-to-female | Prototype and connect breakout headers. |
| 1 | Correct USB **data** cable | Power, serial and flashing; match the actual board connector. |
| 5 | Momentary pushbuttons | Navigation; a supported firmware target may use another input arrangement. |
| 1 | Resistor assortment | Include 330 Ω, 1 kΩ, 4.7 kΩ and 10 kΩ for basic circuits and IR driver. |
| 1 | Digital multimeter, if not already owned | Check continuity, polarity and voltage before connecting modules. |

Use the existing OLED for initial tests. Do not order another display until the controller, interface and firmware path are known. If your existing IR module is a 3.3 V-compatible **demodulating remote-control receiver**, use it for receive testing; a typical obstacle sensor or IR transmitter alone is not equivalent. Read its part number and pinout.

### Buy next as capabilities are added

| Qty | Item | Requirement / caveat |
| --- | --- | --- |
| 1 | PN532 NFC breakout with antenna | Choose a documented 3.3 V-safe interface and board mode supported by the chosen firmware; obtain 2 compatible test tags, e.g. NTAG213. This is 13.56 MHz NFC, not 125 kHz RFID. |
| 1 | 38 kHz demodulating IR receiver | Only if the existing module is unsuitable; confirm 3.3 V supply, output and pinout. |
| 1–2 | 940 nm IR LEDs and a suitable NPN transistor | IR transmission needs a driver and resistors, with current checked against the actual LED and transistor. A receiver module alone cannot transmit. |
| 1 | 3.3 V-compatible SPI microSD breakout and microSD card | Confirm logic voltage and shared-SPI behavior. Some breakouts do not release MISO correctly. |
| 2 | CC1101 modules with matching antennas | Two modules make a controlled test possible. Select the breakout's RF matching and antenna for the intended locally permitted band; determine country and rules before ordering for transmission. One module suffices for initial receive-only work. |
| 1 | 125 kHz reader plus matching test tags | Select a documented tag format and a 3.3 V-safe interface or appropriate level shifting. A basic UART reader may only read; firmware integration, writing and emulation are separate unresolved work. |

Optional later: nRF24L01+ with suitable 3.3 V supply for supported proprietary 2.4 GHz experiments; it is not needed for the ESP32's Wi-Fi/Bluetooth. Add perfboard, enclosure and a complete protected battery/charger/power-path design only after the USB prototype works and power requirements are measured. A USB power bank is a practical interim portable supply.

## Capability and integration status

Wi-Fi and Bluetooth hardware may be built into the board, depending on its exact ESP32 variant. The original ESP32 has Wi-Fi and Bluetooth Classic/BLE, but normally uses USB-to-serial rather than native USB device functionality. CC1101 covers supported sub-GHz frequencies and modulations, not arbitrary radio or every protocol. PN532 and an LF reader are separate systems. A receiver plus transmitter is needed for IR. Storage requires a compatible card interface and working filesystem support. The checklist in [project.md](../project.md) remains unchecked until demonstrated on this device.

## Build sequence and decision gates

1. **Identify:** Record ESP32 module marking, DevKit model, flash size and exposed pins; OLED controller, resolution, supply, pin labels and interface; IR module part number and pins. Check country of use and budget before choosing an RF band or shopping vendor.
2. **Prove the basics:** USB power; test the board, display and button inputs independently. Use an appropriate display driver and verify the OLED's logic voltage. Save actual wiring and results.
3. **Configure Bruce:** Check current Bruce board targets for the verified hardware. Try a reproducible compile for an exact or explicitly adapted target, including flash and display constraints; document what is missing. Compare the community build if helpful. If it cannot fit or support the OLED, decide between a supported display/board and a Bruce adaptation. Do not assume a compiled build is hardware-tested.
4. **Add IR and NFC:** Verify IR receiving and transmission on an owned remote/device; detect a known NFC test tag. Check firmware support separately from chip capabilities.
5. **Add storage and sub-GHz:** Test SD alone, then CC1101 receive and a two-module packet exchange with an appropriate antenna, allowed frequency, power and duty cycle. If sharing SPI, check independent chip selects and MISO release. Validate earlier functions after each addition.
6. **Add LF RFID and finish:** Integrate a documented 125 kHz reader, implement support if the selected firmware lacks it, and test matching tags. Measure the power rail under load before selecting battery circuitry; build a durable enclosure after wiring stabilizes.

The earlier [build guide](build-guide.md) contains provisional example pins and a simple LED test; none of its connections are validated for the identified parts or the eventual firmware. Resolve pin conflicts, boot strapping, peripheral voltage and the board's regulator capacity against the actual hardware before wiring.
