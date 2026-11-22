# Breadboard prototype: complete shopping list

Research snapshot: 2026-11-06. This list targets a USB-powered, modular prototype with Wi-Fi/Bluetooth, IR receive/send, NFC, microSD, sub-GHz and basic 125 kHz tag reading. It is a parts selection, not a tested integrated circuit or a guarantee of Bruce compatibility. No purchases are required for components already owned and verified suitable.

**Firmware decision update:** Custom maen-zer0 firmware is the active direction; Bruce remains a reference. This list is provisional, not a verified compatible order: defer feature modules and any replacement display until the exact board, display, firmware and integration are established. The five buttons below are a generic estimate; the selected interface may use a different arrangement.

## Already owned: identify before ordering

| Qty | Component | Check |
| --- | --- | --- |
| 1 | ESP32 development board | Connected chip identified as ESP32-D0WD-V3 with 4 MB flash; exact module/development board, header pin labels and USB connector still need confirmation. See [Bruce board research](bruce-board-research.md). |
| 1 | 1.3-inch OLED | Controller (e.g. SH1106 or SSD1306), resolution, I2C or SPI, supply/logic voltage and pin labels. Screen size alone is insufficient. |
| 0–1 | IR sensor module | Identify its part and supply. A 38 kHz remote receiver can replace the receiver below; an obstacle-detection module is not necessarily suitable. |

## Order now: prototype essentials

| Qty | Part / exact selection | Notes |
| --- | --- | --- |
| 2 | Full-size 830-point solderless breadboards | Two give room around a wide ESP32 and for modules. |
| 1 pack each | Male-to-male and male-to-female jumper wires, 2.54 mm | At least 40 of each. Female-to-female wires may help depending on headers. |
| 1 | USB data cable matching ESP32 connector | Confirm Micro-USB vs USB-C; data-capable, not charge-only. |
| 5 | Breadboard-compatible momentary tactile pushbuttons, 6×6 mm | Navigation; obtain extra if sold in a pack. |
| 1 assortment | 1/4 W resistors: 68 Ω, 330 Ω, 1 kΩ, 1.5 kΩ, 4.7 kΩ and 10 kΩ | At least 5 of each; 1.5 kΩ values are for the selected PN532 if its I2C bus requires them. Verify combined pull-ups. |
| 1 pack | 100 nF ceramic capacitors | Local supply decoupling as required by module datasheets. |
| 2 | Ordinary visible LEDs, e.g. 5 mm red | Initial circuit and diagnostic tests. |
| 1 | Digital multimeter | Continuity and DC voltage. |
| 1 set | 2.54 mm male header strips | Only if breakouts do not come with installed headers; soldering iron, solder and suitable ventilation are needed to install them. |

Computer with USB port and a suitable development environment are also required. Power from the computer or a known 5 V USB supply initially. The board's regulator rating and loaded 3.3 V rail must be verified before powering all modules together; an additional appropriately rated regulated 3.3 V supply may be necessary once measured. Do not connect two supplies in parallel without a designed power arrangement.

## Order for full prototype capabilities

| Qty | Part / exact selection | Purpose and required notes |
| --- | --- | --- |
| 1 | [Adafruit PN532 breakout, product 364](https://www.adafruit.com/product/364) | 13.56 MHz NFC, integrated antenna, I2C/SPI/UART. Its [I2C wiring guide](https://learn.adafruit.com/adafruit-pn532-rfid-nfc/breakout-wiring) calls for two 1.5 kΩ pull-ups when none are present. Use a verified 3.3 V bus; check parallel pull-ups and firmware interface. |
| 2 | NTAG213 13.56 MHz NFC tags | Known test tags; verify selected firmware/library supports the intended read/write operations. |
| 1 | Vishay TSOP38438 38 kHz IR receiver | Omit if the existing module is verified suitable. Check the [datasheet](https://www.vishay.com/doc?82491=) for pinout, supply and filtering. |
| 2 | 940 nm through-hole IR LEDs, rated at least 50 mA continuous | One for the prototype, one spare. Verify actual forward voltage/current and polarity. |
| 2 | BC337 NPN transistors, through-hole | One driver plus spare; verify package pinout. Use one 68 Ω LED resistor, one 1 kΩ base resistor and one 10 kΩ base pulldown as the starting circuit in the [build guide](build-guide.md). Validate current against actual parts. |
| 1 | SPI microSD breakout, explicitly 3.3 V supply and 3.3 V logic, plus 2.54 mm headers | Select one with a schematic and MISO that releases when deselected; test shared SPI. Avoid assuming generic 5 V modules are safe. |
| 1 | microSD card, 8–32 GB | Format FAT32 if required by the selected firmware; check its current instructions. |
| 2 | CC1101 3.3 V SPI breakout modules, same selected band, with 2 matching antennas | One for the handheld and one for a test peer. Pick the RF-matched band only after country and operating conditions are known. Ensure exposed SPI, CS and GDO pins and compatible antenna connector. [TI's chip bands](https://www.ti.com/product/CC1101) do not establish the breakout's usable band or local permission. |
| 1 | [ID-12LA 125 kHz reader](https://www.sparkfun.com/rfid-reader-id-12la-125-khz.html) | Concrete low-frequency option, with internal antenna, 2.8–5 V supply and EM4001-compatible read capability. This is a read-only starting point, not a general LF emulator/writer. |
| 1 | [SparkFun RFID Reader Breakout](https://www.sparkfun.com/sparkfun-rfid-reader-breakout.html) for ID-12LA | Adapts reader to breadboard wiring; check whether headers are included and soldering is needed. |
| 2 | 125 kHz EM4001-compatible test tags | Must be explicitly compatible with ID-12LA; NFC tags above are different. |

The second CC1101 needs its own controller and power to act as a test peer: add **one compatible ESP32 development board and one matching USB data cable**, or use a suitable controller and cable already owned. Ensure both modules have an antenna fitted before transmitting.

## Before placing the final order

1. Confirm the exact ESP32 and OLED. An unsupported OLED or insufficient flash may require a different screen, a custom firmware adaptation, or a different controller. Do not purchase a replacement display until the firmware target is chosen.
2. Confirm the country and intended frequency before selecting the CC1101 breakout and antenna. Check current local frequency, power and duty-cycle requirements before transmission.
3. Confirm the exact IR module and what supplies, wires, test gear, headers and soldering tools you already have.
4. The ID-12LA provides basic reading; its integration with Bruce is unverified and may require custom firmware. If broader LF writing/emulation is a must, select and research a different front end before buying rather than assuming this reader can be upgraded in software.

The proposed build sequence and firmware comparison are in [hardware-firmware-plan.md](hardware-firmware-plan.md). The provisional wiring examples in [build-guide.md](build-guide.md) must be checked against the exact board, modules, supply and chosen firmware. No integrated wiring, firmware or power budget has been verified yet. Battery, charger, enclosure, iButton and USB HID are outside this breadboard list.
