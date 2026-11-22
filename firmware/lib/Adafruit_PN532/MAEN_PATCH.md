Vendored from Adafruit PN532 1.3.4, licensed under the included BSD license.
The ISO14443-A reader clears its response buffer and rejects UID lengths other
than 4 or 7 before copying, because it reads only 20 response bytes. Failed
I2C reads clear the caller's buffer instead of copying uninitialized data.
These changes are compile-tested only; physical PN532 tests are outstanding.
