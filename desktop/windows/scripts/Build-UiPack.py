"""Create the three-image native UI pack directly from the player's local disc.

Reuses the pinned native XDVDFS reader and the format/identity rules in
host/xbp.mjs, host/texture.mjs and tools/ui/native-pack.py. No inventory pass,
network game content, or original bytes are distributed with the installer.
"""
import argparse
import hashlib
import importlib
import json
import os
from pathlib import Path
import re
import struct
import subprocess
import sys

IDS = ('dded0e74ad2bff44135c6a04', 'b4a089816ad14c0d16965def', '0393008381c77d88832749d1')
SHAPES = ((5, 128, 128), (5, 32, 32), (3, 256, 64))
PACKAGES = re.compile(r'^(?:charactersel|generic|local|scorescreen|gamewinner|minigameintro|demosplash|com_(?:mapscreen|scorescreen|teamscreen|winscreen|zonescorescreen|frenzyintro|ingame))\.xbp$', re.I)
MAX_PACKAGE = 128 * 1024 * 1024


def no_links(path):
    for item in (path, *path.parents):
        if item.is_symlink() or (hasattr(item, 'is_junction') and item.is_junction()):
            raise ValueError('Linked UI input/output paths are not supported')
    return path


def resources(data):
    if len(data) < 64:
        raise ValueError('Truncated XBP header')
    u32 = lambda off: struct.unpack_from('<I', data, off)[0]
    unit, count = u32(4), u32(12)
    table, names, names_size, table_size = u32(16) * unit, u32(40) * unit, u32(44), u32(48)
    if not unit or count > 100000 or table_size != count * 32 or table_size > 8 << 20 or names_size > 8 << 20:
        raise ValueError('Unsupported XBP dimensions')
    if table + table_size > len(data) or names + names_size > len(data):
        raise ValueError('XBP tables outside package')
    digest = hashlib.sha256(data).hexdigest()
    for index in range(count):
        off, key, size, name_off = struct.unpack_from('<4I', data, table + index * 32)
        off *= unit
        if off + size > len(data):
            raise ValueError('XBP resource outside package')
        if name_off == names_size and off == names and size == names_size:
            continue
        if name_off >= names_size:
            raise ValueError('XBP name outside table')
        end = data.find(b'\0', names + name_off, min(names + names_size, names + name_off + 1025))
        if end < 0:
            raise ValueError('Invalid XBP name')
        name = data[names + name_off:end].decode('utf-8').replace('\\', '/')
        if any(part in ('', '.', '..') or ':' in part for part in name.split('/')):
            raise ValueError('Unsafe XBP resource name')
        identity = hashlib.sha256(f'{digest}\0{name}\0{off}\0{size}\0{key}'.encode()).hexdigest()[:24]
        if identity in IDS:
            yield identity, data[off:off + size]


def decode(data, slot):
    if len(data) < 256:
        raise ValueError('Truncated UI texture')
    fmt, width, height = struct.unpack_from('<3H', data, 0x2c)
    if (fmt, width, height) != SHAPES[slot] or data[0x36:0x38] != b'\x01\x01':
        raise ValueError('UI texture descriptor differs from confirmed single-mip resource')
    start = 256 + (1024 if fmt == 5 else 0)
    bpp = 1 if fmt == 5 else 2
    if start + width * height * bpp > len(data):
        raise ValueError('Truncated UI texture pixels')
    rgba = bytearray(width * height * 4)
    for y in range(height):
        for x in range(width):
            morton = bit = dest = 0
            while (1 << bit) < width or (1 << bit) < height:
                if (1 << bit) < width:
                    morton |= ((x >> bit) & 1) << dest
                    dest += 1
                if (1 << bit) < height:
                    morton |= ((y >> bit) & 1) << dest
                    dest += 1
                bit += 1
            if fmt == 5:
                color = struct.unpack_from('<I', data, 256 + data[start + morton] * 4)[0]
                pixel = ((color >> 16) & 255, (color >> 8) & 255, color & 255, color >> 24)
            else:
                color = struct.unpack_from('<H', data, start + morton * 2)[0]
                # Match the preview decoder used by the existing Lanczos pilot.
                pixel = tuple(int(((color >> shift) & 31) * 255 / 31 + 0.5) for shift in (10, 5, 0)) + (255 if color & 0x8000 else 0,)
            pos = (y * width + x) * 4
            rgba[pos:pos + 4] = bytes(pixel)
    return width, height, bytes(rgba)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', required=True, type=Path)
    parser.add_argument('--native', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    source, output = no_links(args.source.absolute()), no_links(args.output.absolute())
    local = Path(os.environ['LOCALAPPDATA']).resolve()
    if local not in output.parents:
        raise ValueError('UI pack output must remain in LocalAppData')
    output.parent.mkdir(parents=True, exist_ok=True)
    # Keep Pillow isolated from the user's Python installation. Fetch software
    # only; all artwork comes from the selected local game input.
    packages = no_links(output.parent / 'python-packages')
    sys.path.insert(0, str(packages))
    try:
        import PIL
        if PIL.__version__ != '12.2.0':
            raise ImportError('Pillow version differs')
    except ImportError:
        print('Installing Pillow 12.2.0 into the private UI build cache.', flush=True)
        subprocess.check_call([sys.executable, '-m', 'pip', 'install', '--disable-pip-version-check', '--only-binary=:all:', '--no-deps', '--upgrade', '--target', str(packages), 'Pillow==12.2.0'])
        for name in list(sys.modules):
            if name == 'PIL' or name.startswith('PIL.'):
                del sys.modules[name]
        importlib.invalidate_caches()
    from PIL import Image
    found = {}
    def consume(data):
        for identity, resource in resources(data):
            found.setdefault(identity, resource)

    if source.is_dir():
        for parent, dirs, files in os.walk(source, followlinks=False):
            dirs[:] = [name for name in dirs if not Path(parent, name).is_symlink() and not Path(parent, name).is_junction()]
            for name in sorted(files):
                if not PACKAGES.fullmatch(name):
                    continue
                path = no_links(Path(parent, name))
                if path.stat().st_size > MAX_PACKAGE:
                    raise ValueError('UI package exceeds size limit')
                consume(path.read_bytes())
                if len(found) == 3:
                    break
            if len(found) == 3:
                break
    else:
        sys.path.insert(0, str(no_links(args.native.absolute()) / 'tools'))
        from xiso_extract import Xiso
        with Xiso(str(source)) as disc:
            for entry in disc.walk():
                if not entry.is_dir and PACKAGES.fullmatch(entry.path.replace('\\', '/').split('/')[-1]):
                    if entry.size > MAX_PACKAGE:
                        raise ValueError('UI package exceeds size limit')
                    consume(b''.join(disc.stream(entry)))
                    if len(found) == 3:
                        break
    if len(found) != 3:
        raise ValueError('The selected game files do not contain all three matching UI resources. Use the complete compatible ISO or extracted folder.')
    payload = bytearray(b'FUIBGRA1' + struct.pack('<I', 3))
    records = []
    for slot, identity in enumerate(IDS):
        width, height, rgba = decode(found[identity], slot)
        image = Image.frombytes('RGBA', (width, height), rgba)
        original = bytearray(image.tobytes('raw', 'BGRA'))
        if slot == 2:
            for pos in range(0, len(original), 4):
                for channel in range(3):
                    value = round(original[pos + channel] * 31 / 255)
                    original[pos + channel] = (value << 3) | (value >> 2)
        replacement = image.convert('RGBa').resize((width * 4, height * 4), Image.Resampling.LANCZOS).convert('RGBA').tobytes('raw', 'BGRA')
        payload += struct.pack('<8I', slot, 0x0b if slot < 2 else 0x02, width, height, width * 4, height * 4, len(original), len(replacement)) + original + replacement
        records.append({'slot': slot, 'stableId': identity, 'resourceSha256': hashlib.sha256(found[identity]).hexdigest(), 'matchBgraSha256': hashlib.sha256(original).hexdigest(), 'replacementBgraSha256': hashlib.sha256(replacement).hexdigest()})
    temp = output.with_suffix('.tmp')
    no_links(temp).write_bytes(payload)
    os.replace(temp, output)
    output.with_suffix(output.suffix + '.json').write_text(json.dumps({'schemaVersion': 1, 'format': 'FUIBGRA1', 'pillowVersion': '12.2.0', 'method': '4x premultiplied-alpha Lanczos; single mip', 'packSha256': hashlib.sha256(payload).hexdigest(), 'assets': records}, indent=2) + '\n')
    print(f'Prepared three local UI components: {len(payload)} bytes.', flush=True)


if __name__ == '__main__':
    main()
