# maen-zer0: shopping and build guide

This guide assumes a common original ESP32-WROOM-32 DevKit V1, USB-powered, with GPIO labels visible. DevKit V1 is a family of layouts: check your board, cable connector, and module labels before connecting anything. These are proposed connections, not a verified photograph or tested circuit for your particular board.

## What to buy

Buy in stages. No prices are quoted: seller, country, quality, and shipping make them unreliable without knowing where you will buy.

### First order: working interface and learning basics

| Quantity | Part | What to look for / why |
| --- | --- | --- |
| 1 | Solderless breadboard, preferably full-size | A wide ESP32 board may cover all rows on a narrow breadboard; two breadboards or a breakout adapter can make both sides accessible. |
| 1 set | Jumper wires | Male-to-male for breadboard; male-to-female for module headers. Headers may need soldering. |
| 1 | USB **data** cable | Correct connector for your board, commonly Micro-USB; charge-only cables cannot upload firmware. |
| 1 | Digital multimeter | Continuity and DC volts; useful before applying power. |
| 1 | 0.96-inch 128×64 SSD1306 **I2C** OLED breakout | Four pins GND, VCC, SCL, SDA; confirm 3.3 V power and logic. Do not accidentally buy an SPI-only display. |
| 5 | Momentary pushbuttons | Up, down, left/back, right, select; ordinary 4-leg tactile buttons work but check which legs are internally connected. |
| 2–5 | Visible LEDs | For first circuit and debugging. |
| 1 assortment | Resistors | At least several 330 Ω, 1 kΩ, 10 kΩ, and 4.7 kΩ, 1/4 W; 330 Ω for first LED. |

You already have the ESP32. A computer and Arduino IDE are also needed. For an initial useful portable test, a standard USB power bank and cable can be used after the USB prototype works, subject to the bank's minimum-load behavior.

### Second order: IR and NFC

| Quantity | Part | Selection |
| --- | --- | --- |
| 1 | 38 kHz demodulating IR receiver | Choose a part with a documented 3.3 V supply range and pinout, such as a TSOP-compatible module; bare receiver pin order varies. |
| 1–2 | 940 nm IR LEDs | Check forward current and polarity in the actual datasheet. |
| 1 | NPN transistor, e.g. 2N2222 or BC337 | Check the exact package pinout; similar names do not guarantee identical pin order. |
| 1 | PN532 NFC breakout | Explicitly documented 3.3 V supply and 3.3 V-safe I2C logic, I2C mode selector and included antenna. Avoid assuming every blue PN532 board is identical. |
| 2 | Known NFC test tags | NTAG213 or another type explicitly supported by the selected reader library; buy tags you can freely test. |

### Later and optional

| Quantity | Part | Selection and caveats |
| --- | --- | --- |
| 2 | CC1101 breakout + appropriate antennas | Two for a controlled sender/receiver experiment. Choose both for the *same locally permitted band*, with matched antenna and breakout RF network. The chip's frequency range is not necessarily the breakout's useful range. |
| 1 | 125 kHz RFID reader + matching test tags | Choose a reader with a documented supported tag format and 3.3 V-safe UART output; e.g. an EM4100-compatible reader with matching tags. Many modules are 5 V and read-only; a level shifter and compatible supply may be necessary. This is not general RFID emulation. |
| 1 | SPI microSD breakout + card | A breakout with documented 3.3 V operation and logic, card formatted as recommended by the chosen firmware. Some 5 V modules have incompatible level shifting. |
| 1 | Enclosure, perfboard, headers | Only after the circuit and dimensions are stable; soldering tools are needed. |
| 1 | Battery system | Defer until power architecture is selected; see below. |

## 1. Identify and test the ESP32

Find the module marking (commonly ESP32-WROOM-32), number of header pins, USB connector, and the labels 3V3, GND, GPIO21/22/23/18/19/27/26/25/32/33 and GPIO34/35. Some are printed as D21 etc. If absent or ambiguous, stop and identify the board from its vendor schematic. Photograph or sketch both sides for your own reference.

Install [Arduino IDE](https://www.arduino.cc/en/software/) and follow [Espressif's Arduino-ESP32 installation](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html), using its **stable** Boards Manager URL. Select the matching DevKit V1 entry if present, otherwise the generic **ESP32 Dev Module** for an original ESP32, then the serial port. Flash the IDE's basic Blink example if your board has a documented onboard LED; otherwise use the external LED test below. If upload stalls, check the data cable and port, and try holding BOOT as upload starts, releasing it when writing begins. Serial driver and permissions can vary by operating system.

### First circuit: LED and button

Disconnect USB. Connect GPIO25 → 330 Ω resistor → LED anode (usually long leg), LED cathode → GND. Connect a momentary button between GPIO32 and GND. On a four-leg button, the two legs on each *same* side are often already connected: use opposite sides and check with a multimeter. Plug USB back in. Create a new Arduino sketch and upload:

```cpp
constexpr int LED_PIN = 25;
constexpr int BUTTON_PIN = 32;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  bool pressed = digitalRead(BUTTON_PIN) == LOW;
  digitalWrite(LED_PIN, pressed ? HIGH : LOW);
  delay(20);
}
```

Expected: the external LED lights only while the button is pressed. If not, unplug and check the GPIO number, button orientation, LED polarity, resistor and common ground. The resistor limits LED current. This simple program has not been run on your board; it is a starting test, not the completed maen-zer0 firmware.

## 2. Add screen and navigation

With power removed, connect the verified 3.3 V I2C OLED: VCC → 3V3, GND → GND, SDA → GPIO21, SCL → GPIO22. Confirm breakout labels, as the physical order varies. Install a compatible SSD1306 display library using Arduino Library Manager, then use its I2C example with 128×64 dimensions. Many modules use address 0x3C, but use an I2C scanner or the vendor datasheet rather than assuming. The I2C pull-ups must connect to 3.3 V, not 5 V.

For five buttons, use GPIO32, GPIO33, GPIO25, GPIO26 and GPIO27, each connected to GND when pressed, with `INPUT_PULLUP` in firmware. Remove the test LED from GPIO25 before reusing it. Debounce buttons in software and show a highlighted menu item. A sensible first menu is IR, NFC, Radio, Settings; display “not connected” for unfinished modules. At this point a basic handheld UI works with just USB power, screen and buttons.

## 3. Add infrared

Add the 3.3 V-compatible demodulating receiver: VCC → 3V3, GND → GND, OUT → GPIO34. GPIO34 is input-only and has no internal pull-up, so follow the receiver datasheet for its output circuit and any required external pull-up; do not assume module pin order. Use an ESP32-compatible IR library's receive example and observe the owner's TV remote in serial output. A 38 kHz module may not correctly detect other carriers. Verify library support for your installed Arduino-ESP32 version before relying on example code.

For transmission, reserve GPIO13. One starting circuit for a **single** 940 nm LED on a 3.3 V rail is: 3V3 → 68 Ω resistor → LED anode; LED cathode → NPN collector; NPN emitter → GND; GPIO13 → 1 kΩ → NPN base; 10 kΩ from base to GND. The LED, transistor and resistor must be rated for the resulting current and duty cycle; verify the LED's actual forward voltage, its polarity, and the transistor pinout. At a typical LED forward voltage of 1.2–1.5 V this is roughly 25–30 mA before transistor drop; it is a conservative starting estimate, not a measured value. Use the IR library's modulated send example for a known protocol on your own equipment. Digital cameras sometimes show a transmitting IR LED, though their filters vary. Test close range first. This circuit has not been physically tested.

## 4. Add NFC

Set the chosen PN532 breakout to I2C mode **before power-up**, following its own documentation; selector positions vary. After checking that its supply and SDA/SCL pull-ups are 3.3 V-safe, connect VCC → 3V3, GND → GND, SDA → GPIO21, SCL → GPIO22. Confirm the OLED and PN532 have distinct I2C addresses; libraries and boards can have initialization and clock-stretching differences. Some libraries require IRQ and/or reset connections, so select the library and confirm its example and breakout pinout first; GPIO14 and GPIO4 are provisional choices for these optional signals, provided your board exposes them and their reset states are compatible. GPIO4 is an ADC2 pin, but works as a digital GPIO. Use a compatible library's reader example to display the UID of a purchased test tag. Then integrate the result into the menu. UID detection is the first milestone; tag writing and emulation are separate projects.

## 5. Add optional sub-GHz and storage

Determine country of operation and check current local allocation, permitted use, power and duty-cycle conditions before selecting a band or transmitting; a chip's tuning range and an online example are not authorization. Do not assume 433 MHz is allowed for your use merely because a module is sold with that antenna. Start with two owned modules, supported frequency, low-power settings and short test packets; receive-only testing can establish wiring first. Never transmit without the correct antenna fitted.

A proposed shared SPI bus: GPIO18 SCK, GPIO23 MOSI, GPIO19 MISO; CC1101 CS → GPIO16 and GDO0 → GPIO17; optional microSD CS → GPIO4. Both devices share 3V3 and GND. GPIO16/17 are usable on many WROOM-based DevKit V1 boards but can be reserved by flash/PSRAM on other modules—verify first. If GPIO4 was used for PN532 IRQ/reset, reassign one function; only one device may own a GPIO. Set both CS lines as outputs held HIGH before initializing the bus; only the addressed device should drive MISO. Check actual breakout labels, electrical compatibility, antenna and SPI behavior before wiring. Some microSD breakouts do not release MISO properly and cannot share this bus; test each module independently and then together. No radio or SD code is supplied or validated yet.

## 6. Optional 125 kHz RFID and portable power

A dedicated reader may use UART or another interface. Pick a reader/tag pair with a real datasheet, verify supply voltage and measure TX output before connecting to ESP32 RX. If output is 5 V, use a suitable level shifter. Read an owned test tag first; supported reading, writing, and emulation depend on the reader and protocol.

USB power bank is the recommended first portable supply. For a built-in rechargeable cell, choose a **complete, documented** single-cell Li-ion/LiPo power system with cell protection, a charger matched to the cell, a regulated output compatible with the specific DevKit's input, sufficient peak current, and proper power-path/load-sharing behavior if operation while charging is required. A bare TP4056 charger board alone is not a complete power-path solution. Verify cell connector polarity, charge current, regulator limits, and USB/backfeed behavior against the actual board schematics; do not connect multiple supplies simultaneously until the design explicitly supports it. Measure current and voltage under Wi-Fi/radio load before estimating runtime. Battery and enclosure design remain unvalidated.

## Suggested firmware milestones

1. LED and button; serial diagnostics.
2. Screen and debounced navigation; a menu with placeholders.
3. IR receive, then transmit for one known owner-controlled remote.
4. PN532 tag detection, explicit error messages and timeout handling.
5. Settings and optional persistent storage; optional modules independently initialized and absent modules handled gracefully.
6. Two-radio packet demo after local rules and hardware are verified; then optional RFID and enclosure.

Build one module at a time, record the actual part numbers and wiring, and retest the earlier functions after every addition. No complete firmware has been implemented yet.
