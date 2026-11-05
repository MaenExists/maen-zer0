# How Flipper Zero works

## The big picture

Flipper Zero combines a microcontroller, screen, buttons, storage, battery management, and several separate electronic interfaces. Firmware coordinates those interfaces, understands selected protocols, and presents applications to the user. An antenna or sensor handles the physical signal; a transceiver translates it into electrical data; firmware interprets that data. A single antenna cannot receive all kinds of signals, and a supported frequency does not imply support for every protocol on it.

Its documented controller is an STM32WB55, its sub-GHz transceiver is a CC1101, and its 13.56 MHz NFC transceiver is an ST25R3916. It also has a separate 125 kHz RFID subsystem, IR hardware, iButton contacts, a screen, buttons, microSD, battery and USB. [Flipper specifications](https://docs.flipper.net/zero/development/hardware/tech-specs) and [schematics](https://docs.flipper.net/zero/development/hardware/schematic) are the best starting points. Flipper's product firmware is made for that hardware; it is not a drop-in ESP32 program.

## Radio, light, and contacts

| Interface | How it works | Example and limits | maen-zer0 approach |
| --- | --- | --- | --- |
| Infrared | An LED sends pulses of invisible light, often modulated around 38 kHz; an IR receiver demodulates them into digital pulses. Firmware identifies timing patterns. | A TV remote normally needs line of sight. A 38 kHz demodulator is not a general light or all-carrier receiver. | 3.3 V-compatible 38 kHz receiver and 940 nm LED, with a transistor driver. |
| NFC / HF RFID | A reader creates a 13.56 MHz field. A nearby tag couples to it and responds; protocol and possibly cryptographic authentication determine what can be read. | UID detection does not mean all memory is readable, a credential is cloneable, or a payment card can be emulated. | PN532 breakout and known compatible test tags; start by reading a UID. |
| Low-frequency RFID | A separate coil generates a field, commonly 125 kHz. A tag modulates the field with an identifier or other data. | EM4100-family tags and other formats differ. A basic module often only reads a subset. | Optional dedicated 125 kHz reader plus matching tags; PN532 cannot replace it. |
| Sub-GHz | The CC1101 modulates/demodulates radio signals; firmware configures the channel, modulation, bitrate, and framing and handles the protocol. | Fixed codes, rolling codes, encryption and packet formats vary. Tuning a receiver is not universal decoding; a replay is not a universal remote. | Optional matched-band CC1101 breakout with antenna; begin with two of your own modules. |
| iButton / 1-Wire | Contacts carry data over a shared wire referenced to ground; some keys and related formats use distinct electrical protocols. | Reader contact geometry, timing, and electrical interface matter. | Optional future contact and interface; no universal support assumed. |
| Wi-Fi and Bluetooth | 2.4 GHz radio uses standardized physical and link protocols. | Wi-Fi isn't sub-GHz; a 2.4 GHz antenna is not a substitute for the other front ends. | ESP32 already has 2.4 GHz Wi-Fi and Bluetooth Classic/BLE. |
| USB | Flipper's MCU has USB device capability. | A device can present specific USB functions when supported by firmware. | A typical original ESP32 DevKit's USB-to-UART bridge provides programming and serial logs, not native keyboard emulation. |

Flipper's IR specifications list a 38 kHz receive carrier, and its NFC and RFID sections distinguish 13.56 MHz and 125 kHz. Its CC1101 coverage and transmit configuration are region dependent. See [Flipper hardware specs](https://docs.flipper.net/zero/development/hardware/tech-specs) and [sub-GHz documentation](https://docs.flipper.net/zero/sub-ghz). The ESP32 radio details and peripheral features are in the [ESP32 datasheet](https://documentation.espressif.com/esp32_datasheet_en.html).

## Important distinctions

**Carrier frequency** is the oscillation frequency of the radio or light signal; **modulation** changes it to convey information; **encoding** specifies how bits are represented; **protocol** defines conversations and meanings. The same frequency can carry incompatible protocols. A transceiver and antenna must both suit the intended band; for example, TI specifies three non-contiguous CC1101 chip bands, but a particular breakout's matching network and antenna can be optimized for just one of them. [TI CC1101](https://www.ti.com/product/CC1101).

**Reading vs writing vs emulation:** identifying a tag, reading accessible memory, writing a supported writable tag, and impersonating a tag are distinct capabilities. PN532 has chip-level support for selected reader and emulation modes, but actual breakout wiring, firmware and library determine what works; it is not a promise of Flipper feature parity. [NXP PN532 datasheet](https://www.nxp.com/docs/en/nxp/data-sheets/PN532_C1.pdf).

**Security:** A displayed identifier is often public and is not necessarily an authentication secret. Modern authenticated cards, encrypted protocols, and rolling-code remotes have security and synchronization properties that simple capture/replay does not reproduce. Experiment with test devices and tags you own or have permission to use.
