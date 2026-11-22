#!/usr/bin/env python3
"""Check ESP32 PlatformIO image placement without connecting or flashing a board."""
import argparse
import hashlib
import struct
from pathlib import Path

FLASH_SIZE = 4 * 1024 * 1024
ENTRY_SIZE = 32


def validate(partitions: bytes, bootloader: bytes, app: bytes) -> list[str]:
    errors = []
    if len(partitions) != 0xC00:
        errors.append("partition table image must occupy exactly 0xc00 bytes")
    if len(bootloader) < 24 or bootloader[0] != 0xE9:
        errors.append("bootloader is not an ESP image")
    elif struct.unpack_from('<H', bootloader, 12)[0] != 0:
        errors.append("bootloader targets a chip other than original ESP32")
    if len(bootloader) > 0x7000:
        errors.append("bootloader overlaps partition table")
    if len(app) < 24 or app[0] != 0xE9:
        errors.append("application is not an ESP image")
    elif struct.unpack_from('<H', app, 12)[0] != 0:
        errors.append("application targets a chip other than original ESP32")

    found_end = False
    entries = []
    for pos in range(0, len(partitions) - ENTRY_SIZE + 1, ENTRY_SIZE):
        entry = partitions[pos:pos + ENTRY_SIZE]
        magic = entry[:2]
        if magic in (b'\xff\xff', b'\xeb\xeb'):
            if magic == b'\xeb\xeb' and hashlib.md5(partitions[:pos]).digest() != entry[16:32]:
                errors.append("partition table MD5 does not match")
            found_end = True
            break
        if magic != b'\xaa\x50':
            errors.append(f"invalid partition entry at {pos:#x}")
            break
        _, part_type, subtype, offset, size = struct.unpack_from('<HBBII', entry)
        if not size or offset < 0x9000 or offset + size > FLASH_SIZE:
            errors.append(f"partition at {offset:#x} is outside usable flash")
        if offset % 0x1000 or size % 0x1000:
            errors.append(f"partition at {offset:#x} is not sector aligned")
        if part_type == 0 and offset % 0x10000:
            errors.append(f"application partition at {offset:#x} is not 64 KiB aligned")
        entries.append((offset, offset + size, part_type, subtype))
    if not found_end:
        errors.append("partition table has no end marker")
    ordered = sorted(entries)
    for previous, current in zip(ordered, ordered[1:]):
        if previous[1] > current[0]:
            errors.append("partition entries overlap")
    apps = [entry for entry in entries if entry[2] == 0 and entry[3] in (0, 0x10)]
    if len(apps) != 1:
        errors.append("expected exactly one factory or OTA_0 application partition")
    else:
        start, end, _, _ = apps[0]
        if start != 0x10000:
            errors.append("application must start at 0x10000")
        if len(app) > end - start:
            errors.append("application exceeds its partition")
    return errors


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('build_dir', type=Path)
    args = parser.parse_args()
    try:
        errors = validate(*(args.build_dir.joinpath(name).read_bytes() for name in
                            ('partitions.bin', 'bootloader.bin', 'firmware.bin')))
    except OSError as exc:
        print(f"FAIL: {exc}")
        return 1
    for error in errors:
        print(f"FAIL: {error}")
    if not errors:
        print("PASS: image headers and partition placement fit original ESP32 4 MB flash")
        print("This is a static check; it does not verify device security state or boot.")
    return int(bool(errors))


if __name__ == '__main__':
    raise SystemExit(main())
