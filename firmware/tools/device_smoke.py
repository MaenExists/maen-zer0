#!/usr/bin/env python3
"""Exercise a flashed board over serial without printing nearby scan results."""
import argparse
import re
import time
import serial


def wait_for(port, marker, timeout):
    deadline = time.monotonic() + timeout
    data = bytearray()
    while time.monotonic() < deadline:
        data.extend(port.read(1024))
        if marker in data:
            return bytes(data)
        if len(data) > 65536:
            raise RuntimeError("response exceeded limit")
    raise RuntimeError(f"timed out waiting for {marker.decode()}")


def command(port, text, marker, timeout=15):
    port.reset_input_buffer()
    port.write(text.encode('ascii') + b'\n')
    port.flush()
    return wait_for(port, marker, timeout)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--port', required=True)
    parser.add_argument('--cycles', type=int, default=3)
    args = parser.parse_args()
    if not 1 <= args.cycles <= 20:
        parser.error('cycles must be between 1 and 20')
    port = serial.Serial(port=None, baudrate=115200, timeout=0.2, write_timeout=2)
    # CP210x DTR/RTS can drive BOOT/EN. Set inactive before opening so test
    # reconnects don't intentionally place the board in ROM download mode.
    port.dtr = False
    port.rts = False
    port.port = args.port
    with port:
        time.sleep(2)
        command(port, 'info', b' s\n')
        command(port, 'help', b'Commands:')
        command(port, 'x' * 65, b'Command too long; discarded.')
        command(port, 'sd info', b'SD disabled:')
        command(port, 'lf read', b'LF reader disabled:')
        command(port, 'ir read', b'IR receive disabled:')
        command(port, 'ir send', b'IR transmit disabled:')
        command(port, 'nfc read', b'NFC disabled:')
        command(port, 'rf info', b'RF disabled:')
        print('PASS: serial info, help, oversized input, disabled SD, LF, IR, NFC and RF')
        heaps = []
        for cycle in range(args.cycles):
            result = command(port, 'wifi scan', b'networks\n', timeout=30)
            if b'Wi-Fi scan failed' in result or b'Wi-Fi initialization failed' in result:
                raise RuntimeError('Wi-Fi scan reported failure')
            wifi_count = re.search(rb'Found (\d+) networks', result)
            if not wifi_count:
                raise RuntimeError('Wi-Fi scan count missing')
            command(port, 'help', b'Commands:', timeout=30)
            result = command(port, 'ble scan', b'duplicates possible)\n', timeout=20)
            if b'BLE initialization failed' in result:
                raise RuntimeError('BLE initialization failed')
            ble_count = re.search(rb'Displayed (\d+) advertisements', result)
            if not ble_count:
                raise RuntimeError('BLE scan count missing')
            command(port, 'help', b'Commands:')
            status = command(port, 'info', b' s\n')
            heap = re.search(rb'free heap: (\d+) bytes', status)
            if not heap:
                raise RuntimeError('free heap reading missing')
            heaps.append(int(heap.group(1)))
            print(f'PASS: scan cycle {cycle + 1}: Wi-Fi networks={int(wifi_count.group(1))}, '
                  f'BLE advertisements={int(ble_count.group(1))}, free heap={heaps[-1]}')
        if heaps[0] - heaps[-1] > 16384:
            raise RuntimeError('free heap fell by more than 16 KiB after the first cycle')
        print('PASS: serial responsive and heap loss within 16 KiB of first cycle')


if __name__ == '__main__':
    try:
        main()
    except (OSError, RuntimeError, serial.SerialException) as exc:
        print(f'FAIL: {exc}')
        raise SystemExit(1)
