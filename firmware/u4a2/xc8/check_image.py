#!/usr/bin/env python3
"""Validate an XC8 application HEX and its linked symbols; no third-party modules."""
import argparse
from pathlib import Path


def read_hex(path):
    image, base, eof = {}, 0, False
    for line_number, line in enumerate(Path(path).read_text().splitlines(), 1):
        if not line.strip():
            continue
        if eof or not line.startswith(':'):
            raise ValueError(f'invalid record at line {line_number}')
        record = bytes.fromhex(line[1:])
        if len(record) < 5 or len(record) != record[0] + 5 or sum(record) & 255:
            raise ValueError(f'length/checksum error at line {line_number}')
        count, kind = record[0], record[3]
        address = int.from_bytes(record[1:3], 'big')
        data = record[4:-1]
        if kind == 0:
            for offset, value in enumerate(data):
                absolute = base + address + offset
                if absolute in image:
                    raise ValueError(f'duplicate address {absolute:#x}')
                image[absolute] = value
        elif kind == 1 and count == 0 and address == 0:
            eof = True
        elif kind in (2, 4) and count == 2 and address == 0:
            base = int.from_bytes(data, 'big') << (4 if kind == 2 else 16)
        else:
            raise ValueError(f'unsupported HEX record type {kind}')
    if not eof or not image:
        raise ValueError('empty or incomplete HEX file')
    return image


def read_symbols(path):
    result = {}
    for line in Path(path).read_text().splitlines():
        fields = line.split()
        if len(fields) == 5:
            try:
                result[fields[0]] = int(fields[1], 16)
            except ValueError:
                pass
    return result


def validate(image, symbols):
    def need(condition, message):
        if not condition:
            raise ValueError(message)

    def data(address, count):
        try:
            return bytes(image[address + i] for i in range(count))
        except KeyError as exc:
            raise ValueError(f'missing image bytes at {address:#x}') from exc

    need(all(0x8C0 <= address <= 0x7FFF for address in image),
         'HEX writes outside application flash (boot/config/EEPROM/ID protection)')
    # PIC18 GOTO and CALL are two words; target is a word address.
    # XC8 may use CALL at the high vector to refresh the shadow registers.
    for address in (0x8C0, 0x8C8):
        instruction = data(address, 4)
        allowed = (0xEF,) if address == 0x8C0 else (0xEC, 0xED, 0xEF)
        need(instruction[1] in allowed and instruction[3] & 0xF0 == 0xF0,
             f'expected GOTO/CALL at vector {address:#x}')
        target = ((instruction[3] & 15) << 16 | instruction[2] << 8 | instruction[0]) * 2
        need(target in image and target + 1 in image, f'vector target absent: {target:#x}')
    need(data(0x8D8, 2) == b'\x10\x00', 'low vector must contain RETFIE 0')
    expected = {'_ep0Bo': 0x400, '_ep0Bi': 0x404, '_ep1Bo': 0x408,
                '_ep1Bi': 0x40C, '_SetupPkt': 0x480, '_CtrlTrfData': 0x488,
                '_ep1_out_buffer': 0x4C8, '_ep1_in_buffer': 0x508,
                '_interruption': 0x8C8}
    for name, address in expected.items():
        need(symbols.get(name) == address, f'wrong/missing symbol {name}: expected {address:#x}')
    need('_device_dsc' in symbols and '_cfg01' in symbols, 'USB descriptors missing')
    descriptor = data(symbols['_device_dsc'], 18)
    need(descriptor == bytes.fromhex('12 01 00 02 00 00 00 08 d8 04 0c 00 00 00 01 02 03 01'),
         'USB device descriptor changed')
    config = data(symbols['_cfg01'], 32)
    need(config[:4] == bytes.fromhex('09 02 20 00'), 'bad configuration length/header')
    need(config[18:25] == bytes.fromhex('07 05 01 02 40 00 ff'), 'EP1 OUT descriptor changed')
    need(config[25:32] == bytes.fromhex('07 05 81 02 40 00 ff'), 'EP1 IN descriptor changed')
    stacks = []
    for prefix in ('___stack', '___inthi_stack'):
        lo, hi = symbols.get(prefix + '_lo'), symbols.get(prefix + '_hi')
        need(lo is not None and hi is not None and 0x548 <= lo < hi <= 0x800,
             f'invalid software stack allocation: {prefix}')
        stacks.append((lo, hi))
    need(stacks[0][1] <= stacks[1][0] or stacks[1][1] <= stacks[0][0], 'software stacks overlap')
    return f'OK: {len(image)} flash bytes; vectors, USB RAM/descriptors and stack allocation checked'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('hex_file', type=Path)
    parser.add_argument('symbols_file', type=Path)
    args = parser.parse_args()
    try:
        print(validate(read_hex(args.hex_file), read_symbols(args.symbols_file)))
    except (OSError, ValueError) as exc:
        parser.exit(1, f'FAIL: {exc}\n')


if __name__ == '__main__':
    main()
