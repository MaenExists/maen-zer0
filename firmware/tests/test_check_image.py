import importlib.util
from pathlib import Path
import struct
import hashlib
import unittest

spec = importlib.util.spec_from_file_location('check_image', Path(__file__).parents[1] / 'tools/check_image.py')
check = importlib.util.module_from_spec(spec)
spec.loader.exec_module(check)


def table(*entries):
    result = bytearray()
    for typ, subtype, offset, size in entries:
        result.extend(struct.pack('<HBBII16sI', 0x50aa, typ, subtype, offset, size, b'test', 0))
    return bytes(result).ljust(0xC00, b'\xff')


class CheckImageTest(unittest.TestCase):
    def setUp(self):
        self.parts = table((1, 2, 0x9000, 0x6000), (0, 0, 0x10000, 0x300000))
        self.boot = bytes([0xe9]) + bytes(23)
        self.app = bytes([0xe9]) + bytes(23)

    def test_valid(self):
        self.assertEqual(check.validate(self.parts, self.boot, self.app), [])
        self.assertEqual(check.validate(table((0, 0x10, 0x10000, 0x300000)), self.boot, self.app), [])

    def test_wrong_chip_and_corrupt_image(self):
        app = bytearray(self.app)
        app[12] = 9
        self.assertTrue(check.validate(self.parts, self.boot, app))
        self.assertTrue(check.validate(self.parts, self.boot, b'bad'))
        boot = bytearray(self.boot)
        boot[12] = 9
        self.assertTrue(check.validate(self.parts, boot, self.app))

    def test_overlap_and_out_of_flash(self):
        self.assertTrue(check.validate(table((1, 2, 0x9000, 0x8000), (0, 0, 0x10000, 0x300000)), self.boot, self.app))
        self.assertTrue(check.validate(table((0, 0, 0x10000, 0x400000)), self.boot, self.app))

    def test_truncated_or_invalid_table(self):
        self.assertTrue(check.validate(b'bad', self.boot, self.app))
        self.assertTrue(check.validate(bytes(0x1000), self.boot, self.app))

    def test_bootloader_overlap_and_missing_factory(self):
        self.assertTrue(check.validate(self.parts, self.boot + bytes(0x7000), self.app))
        self.assertTrue(check.validate(table((1, 2, 0x9000, 0x6000)), self.boot, self.app))

    def test_partition_checksum(self):
        entries = self.parts[:64]
        marker = b'\xeb\xeb' + b'\xff' * 14 + hashlib.md5(entries).digest()
        parts = (entries + marker).ljust(0xC00, b'\xff')
        self.assertEqual(check.validate(parts, self.boot, self.app), [])
        broken = bytearray(parts)
        broken[64 + 16] ^= 1
        self.assertIn('partition table MD5 does not match', check.validate(broken, self.boot, self.app))


if __name__ == '__main__':
    unittest.main()
