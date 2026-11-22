# Marauder firmware plan

**Historical plan, superseded:** Bruce is now the primary firmware. The following Marauder evaluation is retained for reference; use the [hardware and firmware plan](hardware-firmware-plan.md) for current decisions. Research snapshot: 2026-11-06. No target has been compiled or tested on our hardware.

## Hardware decision comes first

Identify the user's ESP32 chip and flash size and the OLED controller, resolution and interface. Marauder's [hardware index](https://github.com/justcallmekoko/ESP32Marauder/wiki/Supported-Hardware) lists distinct targets and warns against substituting binaries for other boards. Its [POOM target](https://github.com/justcallmekoko/ESP32Marauder/wiki/POOM) supports a 128×64 SH1106-compatible I2C display, six buttons and microSD, but uses an **ESP32-C5**, not a generic original ESP32. An OLED of the same size does not establish controller compatibility. Do not flash POOM onto an original ESP32.

Paths to evaluate once the board and display are identified:

1. Retain the existing ESP32: determine whether an official target matches or a source-level custom target can support the actual board, display, buttons, flash and SD. POOM's small-screen implementation may be useful reference code, but porting is unverified.
2. If an unmodified supported build is preferred, select a documented complete Marauder target and match its MCU, display, controls, pinout, flash layout and peripherals. A supported board may replace some owned parts.
3. A [headless target](https://github.com/justcallmekoko/ESP32Marauder/wiki/Supported-Hardware) may offer an initial serial/CLI experiment on a compatible original ESP32, subject to its exact target requirements and flash layout. It does not automatically support our display.

Use the [official installer](https://github.com/justcallmekoko/ESP32Marauder/wiki/Getting-Started) for a matching supported target or build from source for a custom configuration; verify bootloader, partitions and flashing offsets for that exact target.

## Capability implications

| Goal | Marauder prototype status |
| --- | --- |
| Wi-Fi and Bluetooth | Primary Marauder focus, subject to chip and target support. Original ESP32 and C5 have different radio capabilities. |
| microSD | Supported on many specific Marauder targets; wiring, filesystem and build must match. |
| Sub-GHz / configurable external radio | CC1101 hardware was proposed, but standalone Marauder integration and protocol coverage for our target are not established. External radios on a companion board do not by themselves prove standalone firmware support. |
| NFC, 125 kHz RFID and infrared | Hardware and integrated Marauder applications for our build are not established. POOM explicitly does not use NFC or IR. Custom development or separate firmware may be needed; extra modules do not automatically add functions. |

First achieve a working Marauder target with screen, controls and SD, then investigate each additional interface in source and implement/test support as needed. Preserve the broader checklist as goals; do not mark unimplemented functions complete. Do not assume a practical dual-boot workflow until flash and partition requirements are checked.

## Purchasing impact

The [breadboard list](breadboard-shopping-list.md) contains provisional parts for the larger eventual device, not a verified Marauder-compatible bill of materials. Buy only general prototyping supplies until the exact board, display, firmware target and country are known. Defer the CC1101, NFC, LF RFID and IR-specific purchases until a compatible integration plan is verified. Six buttons are needed if reproducing POOM's documented controls; the previous five-button quantity was a generic proposal.
