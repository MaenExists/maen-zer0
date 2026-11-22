# Connected ESP32 recovery record

On 2026-11-06 the connected device identified as ESP32-D0WD-V3 rev 3.1 with 4 MB flash. Before the first custom-firmware upload, its entire flash was read to `/home/maen/.local/share/maen-zer0/backups/2026-11-06-initial/flash.bin`; the file is private to the user and was compared against the board using esptool's flash digest verification. It may contain private data; keep it out of Git and do not share it. This backup belongs only to that specific device.

At the time, eFuse inspection reported UART download enabled, no active secure boot and no flash encryption. No eFuses were changed. Hardware identity and security state must be checked again before a later restore; do not reuse these conclusions for another chip. The backup was verified against the original flash but has **not** been restored or boot-tested.

For the same board, if its application no longer boots but the ROM serial downloader remains accessible, use the official [esptool flashing and troubleshooting guidance](https://docs.espressif.com/projects/esptool/en/latest/esp32/esptool/flashing-firmware.html). A full restore writes the entire 4 MB image at offset `0x0`, overwriting subsequent firmware and data. After positively identifying the device and port, an example using esptool 5 is:

```sh
esptool --chip esp32 --port /dev/ttyUSB0 --baud 460800 write-flash 0x0 /home/maen/.local/share/maen-zer0/backups/2026-11-06-initial/flash.bin
```

Do not run this on a different device. A serial port can change after reconnection. If automatic download mode fails, board-specific BOOT/EN controls may be needed. Do not use `--force`, erase flash preemptively, burn eFuses, or bypass secure-boot/flash-encryption checks. A bad cable, disconnected power, a damaged flash chip or altered security configuration can still prevent recovery.
