#pragma once

// Disabled until a particular 3.3 V-safe SD breakout and its wiring are known.
// Set all four pins for a tested hardware profile; never use GPIO6-11 (flash).
#ifndef MAEN_SD_SCK
#define MAEN_SD_SCK -1
#endif
#ifndef MAEN_SD_MISO
#define MAEN_SD_MISO -1
#endif
#ifndef MAEN_SD_MOSI
#define MAEN_SD_MOSI -1
#endif
#ifndef MAEN_SD_CS
#define MAEN_SD_CS -1
#endif

// ID-12LA ASCII (9600 8N1) data input; disabled until wiring is verified.
#ifndef MAEN_LF_RX
#define MAEN_LF_RX -1
#endif

// Remote-control demodulator and transistor-driven IR LED, respectively.
#ifndef MAEN_IR_RX
#define MAEN_IR_RX -1
#endif
#ifndef MAEN_IR_TX
#define MAEN_IR_TX -1
#endif

// PN532 in I2C mode; IRQ and reset are required by this driver.
#ifndef MAEN_NFC_SDA
#define MAEN_NFC_SDA -1
#endif
#ifndef MAEN_NFC_SCL
#define MAEN_NFC_SCL -1
#endif
#ifndef MAEN_NFC_IRQ
#define MAEN_NFC_IRQ -1
#endif
#ifndef MAEN_NFC_RESET
#define MAEN_NFC_RESET -1
#endif

// CC1101 SPI identification only; RF receive/transmit is not implemented.
#ifndef MAEN_RF_SCK
#define MAEN_RF_SCK -1
#endif
#ifndef MAEN_RF_MISO
#define MAEN_RF_MISO -1
#endif
#ifndef MAEN_RF_MOSI
#define MAEN_RF_MOSI -1
#endif
#ifndef MAEN_RF_CS
#define MAEN_RF_CS -1
#endif
