# ESP32 and electronics primer

## Start here

- **Voltage (V)** is electrical potential; **current (A)** is flow; **resistance (Ω)** limits current. A resistor in series with an LED prevents excessive current.
- **Ground (GND)** is the shared voltage reference. Modules communicating with the ESP32 need a common ground unless an isolated interface explicitly says otherwise.
- A **breadboard** connects groups of holes internally. Center rows are commonly groups of five on each side; power rails run lengthwise and may be split. Check its markings or continuity with a multimeter.
- A **GPIO** is a programmable input/output pin. Names such as GPIO21 refer to the chip signal, not the physical header position. DevKit V1 layouts and labels vary.
- **I2C** uses SDA and SCL shared by devices at distinct addresses. **SPI** shares clock and data lines, with a separate chip-select (CS) per device. **UART** sends serial data on TX and RX; connect TX to RX and RX to TX.

## Electrical rules for this board

ESP32 GPIO is 3.3 V logic; do not apply a 5 V signal to a GPIO. A breakout powered at 5 V may output 5 V even if it has a 3.3 V regulator. Check the actual schematic or measure its outputs; use a suitable level shifter if needed. Never connect a module or LED without checking voltage and pin order. Power off and unplug before changing wiring. Check 3V3-to-GND for a short before powering up. A multimeter is strongly recommended.

The 3V3 pin is a regulated supply on common boards, not a universal high-current supply. Regulator capacity, USB supply, and module current vary; do not promise that a complete set of modules can run from it. Check regulator ratings and rail voltage under load. GPIO is not a power supply for motors, radios, or high-current LEDs; use an appropriate transistor driver and supply.

On the original ESP32, GPIO34–39 are input-only and lack internal pull resistors. GPIO6–11 are generally used for flash; 16–17 may also be reserved depending on module. GPIO0, 2, 5, 12 and 15 are boot strapping pins; external circuitry can prevent boot. GPIO1/3 normally handle serial programming/logs. ADC2 conflicts with Wi-Fi; use ADC1 when both are needed. Not every GPIO is exposed on every DevKit. See [Espressif GPIO guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html) and [board pin restrictions](https://docs.espressif.com/projects/arduino-esp32/en/latest/boards/ESP32-DevKitC-1.html).

## How the board is powered

A typical DevKit V1 has USB, an onboard USB-to-serial converter, and a 3.3 V regulator; layouts and labels vary by vendor. USB is the simplest first power source. The original ESP32 does not have native USB device hardware; the board's USB connection is ordinarily for serial/programming. Do not connect a bare lithium cell to 3V3, 5V/VIN, or USB. A cell needs an appropriate protected power system, charging and regulation; see the later battery milestone in the build guide.

## A small electronics vocabulary

An LED has polarity: its anode is positive and its cathode negative. A transistor is an electrically controlled switch; a base resistor limits the control current. I2C lines need pull-up resistors to **3.3 V**; many breakouts already include them. Multiple parallel pull-ups change their effective resistance. A multimeter in voltage mode goes across two points, whereas resistance/continuity measurements are taken with power disconnected.

## Learn next

First light an LED using a resistor, then read a button, then drive a screen. Check measurements against the circuit before adding another module. The [ESP32 datasheet](https://documentation.espressif.com/esp32_datasheet_en.html) is the reference for the chip; your particular DevKit seller's schematic is needed for board-level power and pinout details.
