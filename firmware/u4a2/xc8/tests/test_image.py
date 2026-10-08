"""Positive and negative checks on the actual linked firmware."""
import sys
import tempfile
import unittest
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from check_image import read_hex, read_symbols, validate


class ImageTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.path = Path(sys.argv[1])
        cls.image = read_hex(cls.path.with_suffix('.hex'))
        cls.symbols = read_symbols(cls.path.with_suffix('.sym'))

    def test_valid(self):
        validate(self.image, self.symbols)

    def test_protected_regions(self):
        for address in (0, 0x8BF, 0x8000, 0x200000, 0x300000, 0xF00000):
            with self.subTest(address=address), self.assertRaises(ValueError):
                validate(self.image | {address: 0}, self.symbols)

    def test_bad_vector(self):
        for address in (0x8C0, 0x8C8, 0x8D8):
            altered = self.image.copy()
            altered[address + 1] = 0xFF
            with self.assertRaises(ValueError):
                validate(altered, self.symbols)

    def test_bad_usb_ram(self):
        symbols = self.symbols | {'_ep1Bi': 0x410}
        with self.assertRaises(ValueError):
            validate(self.image, symbols)

    def test_bad_usb_descriptor(self):
        image = self.image | {self.symbols['_device_dsc'] + 10: 0x0B}
        with self.assertRaises(ValueError):
            validate(image, self.symbols)

    def test_stack_overlap(self):
        symbols = self.symbols | {'___inthi_stack_lo': self.symbols['___stack_lo']}
        with self.assertRaises(ValueError):
            validate(self.image, symbols)

    def test_bad_checksum(self):
        lines = self.path.with_suffix('.hex').read_text().splitlines()
        lines[0] = lines[0][:-2] + ('00' if lines[0][-2:] != '00' else '01')
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'bad.hex'
            path.write_text('\n'.join(lines))
            with self.assertRaises(ValueError):
                read_hex(path)


if __name__ == '__main__':
    unittest.main(argv=[sys.argv[0]], verbosity=2)
