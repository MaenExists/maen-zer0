# Research notes and open decisions

Research date: 2026-11-05. Sources below are manufacturer and project documentation. These references establish component capabilities, not compatibility of an unidentified marketplace breakout or compliance in a particular jurisdiction.

## Primary references

- [Flipper Zero technical specifications](https://docs.flipper.net/zero/development/hardware/tech-specs): controller, NFC, low-frequency RFID, IR, GPIO, storage and power specifications. Some figures and regional details can change; verify the specific product revision.
- [Flipper Zero schematics](https://docs.flipper.net/zero/development/hardware/schematic): subsystem-level hardware references; useful for understanding why antennas, power and interfaces are separate.
- [Flipper sub-GHz documentation](https://docs.flipper.net/zero/sub-ghz): identifies the CC1101 radio and sub-GHz functions.
- [Espressif ESP32 datasheet](https://documentation.espressif.com/esp32_datasheet_en.html): original ESP32 CPU, 2.4 GHz Wi-Fi and Bluetooth, electrical characteristics, boot behavior, and peripherals including RMT for IR timing.
- [Espressif GPIO reference](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html): input-only pins, flash pin restrictions, boot strapping and ADC2/Wi-Fi limitation.
- [Espressif Arduino installation](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html) and [board selection](https://docs.espressif.com/projects/arduino-esp32/en/latest/guides/tools_menu.html): supported setup and generic board selection.
- [TI CC1101 product and datasheet](https://www.ti.com/product/CC1101): 300–348, 387–464 and 779–928 MHz chip bands; modes, supply and SPI details. Actual breakout RF matching, antenna and jurisdiction restrict practical use further.
- [NXP PN532 datasheet](https://www.nxp.com/docs/en/nxp/data-sheets/PN532_C1.pdf): 13.56 MHz reader and selected card-emulation modes. Breakout and software support vary.

## Decisions made for the initial design

- Original ESP32 DevKit V1 is the controller, rather than replacing the board the user already owns.
- USB power first; small I2C OLED and buttons create a useful interface early.
- PN532 is chosen for an accessible NFC learning path, not for full Flipper parity. Dedicated LF hardware is needed for 125 kHz.
- IR uses a demodulating receiver and a transistor-driven LED; GPIO does not directly supply a high-current emitter.
- CC1101 and microSD are optional shared-SPI additions after independent testing; two CC1101 boards make an owned-endpoint experiment possible.
- Arduino IDE/Arduino-ESP32 is the approachable initial software environment. Library versions and precise API choices remain to be tested once parts are selected.

## Unknowns to resolve as the project progresses

1. Exact DevKit V1 vendor, module, pinout, USB connector, 3.3 V regulator and available pins.
2. Country and legal band/conditions, before choosing a radio module or transmitting. Time zone alone does not establish jurisdiction.
3. Budget and preferred stores; no specific prices or purchasing links have been verified.
4. Specific breakout schematics, pull-up voltage, pin order, library compatibility, and test-tag format.
5. Prioritization and specific protocols for the target capabilities in the project checklist. Flipper-like breadth is the goal, but feature equivalence depends on hardware and protocol support and has not been established.
6. Firmware implementation, physical measurements, battery/power-path design, enclosure, and all integrated tests remain future work.

## Verification status

This is a researched design and beginner roadmap, not a tested construction manual for an identified board. No code was compiled, no module purchased or wired, and no legal determination or cost estimate was made. The LED example and pin plan should be checked against the actual parts before use.
