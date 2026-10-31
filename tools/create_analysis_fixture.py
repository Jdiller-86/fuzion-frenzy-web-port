"""Create a source-owned PE fixture returning 42; used only for static REA setup verification."""
import pathlib
import struct

output = pathlib.Path(__file__).resolve().parents[1] / '.tools' / 'verification' / 'return42.exe'
output.parent.mkdir(parents=True, exist_ok=True)
data = bytearray(1024)
data[:2] = b'MZ'
struct.pack_into('<I', data, 0x3C, 0x80)
data[0x80:0x84] = b'PE\0\0'
struct.pack_into('<HHIIIHH', data, 0x84, 0x14C, 1, 0, 0, 0, 224, 0x102)
opt = 0x98
struct.pack_into('<H', data, opt, 0x10B)
struct.pack_into('<I', data, opt + 4, 512)
struct.pack_into('<III', data, opt + 16, 0x1000, 0x1000, 0)
struct.pack_into('<III', data, opt + 28, 0x400000, 0x1000, 0x200)
struct.pack_into('<HH', data, opt + 40, 4, 0)
struct.pack_into('<HH', data, opt + 48, 4, 0)
struct.pack_into('<II', data, opt + 56, 0x2000, 0x200)
struct.pack_into('<H', data, opt + 68, 3)
struct.pack_into('<IIIII', data, opt + 72, 0x100000, 0x1000, 0x100000, 0x1000, 0)
struct.pack_into('<I', data, opt + 92, 16)
section = opt + 224
data[section:section + 8] = b'.text\0\0\0'
struct.pack_into('<IIII', data, section + 8, 6, 0x1000, 512, 512)
struct.pack_into('<I', data, section + 36, 0x60000020)
data[512:518] = bytes.fromhex('b8 2a 00 00 00 c3')
output.write_bytes(data)
print(output)
