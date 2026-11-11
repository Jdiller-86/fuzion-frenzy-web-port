"""Build the bounded private native BGRA pack from existing inventory/pilot PNGs."""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from PIL import Image
from private_output import private_directory, no_links

p = argparse.ArgumentParser()
p.add_argument('--inventory', required=True)
p.add_argument('--pilot', required=True)
p.add_argument('--out', required=True)
a = p.parse_args()
inventory = no_links(a.inventory)
pilot = no_links(a.pilot)
out = private_directory(a.out, [inventory, pilot])
inv = json.loads(inventory.read_text(encoding='utf-8-sig'))
recipe = json.loads((pilot / 'recipe.json').read_text(encoding='utf-8-sig'))
ids = ['dded0e74ad2bff44135c6a04', 'b4a089816ad14c0d16965def', '0393008381c77d88832749d1']
rows = {r['stableId']: r for r in inv['resources']}
variants = {r['stableId']: r for r in recipe['assets']}
payload = bytearray(b'FUIBGRA1' + struct.pack('<I', 3))
records = []
for slot, asset_id in enumerate(ids):
    r = rows[asset_id]
    if r['headerMipLevels'] != 1 or r['frameCount'] != 1:
        raise ValueError('Only the confirmed static single-mip set is admitted')
    src = no_links(inventory.parent / r['thumbnail'])
    if inventory.parent not in src.parents:
        raise ValueError('Inventory image escaped private root')
    variant = next(v for v in variants[asset_id]['variants'] if v['method'] == 'premultiplied-lanczos-preview')
    enlarged = no_links(pilot / variant['file'])
    if pilot not in enlarged.parents:
        raise ValueError('Pilot image escaped private root')
    if hashlib.sha256(src.read_bytes()).hexdigest() != variants[asset_id]['previewSha256'] or hashlib.sha256(enlarged.read_bytes()).hexdigest() != variant['sha256']:
        raise ValueError('Existing pilot/input identity mismatch')
    original = Image.open(src).convert('RGBA')
    replacement = Image.open(enlarged).convert('RGBA')
    if original.size != (r['width'], r['height']) or replacement.size != (r['width'] * 4, r['height'] * 4):
        raise ValueError('Unexpected source/pilot dimensions')
    # Both decoders use straight alpha BGRA8 at the native upload seam.
    bgra = original.tobytes('raw', 'BGRA')
    if slot == 2:
        # Inventory previews round 5-bit channels to 255/31; the SDK expands
        # them by bit replication. Recover those exact 5 bits before matching.
        canonical = bytearray(bgra)
        for offset in range(0, len(canonical), 4):
            for channel in range(3):
                v = round(canonical[offset + channel] * 31 / 255)
                canonical[offset + channel] = (v << 3) | (v >> 2)
        bgra = bytes(canonical)
    bigger = replacement.tobytes('raw', 'BGRA')
    fmt = 0x0B if slot < 2 else 0x02
    payload += struct.pack('<8I', slot, fmt, *original.size, *replacement.size, len(bgra), len(bigger)) + bgra + bigger
    records.append({'slot': slot, 'stableId': asset_id, 'format': fmt, 'logical': original.size, 'physical': replacement.size,
                    'matchBgraSha256': hashlib.sha256(bgra).hexdigest(), 'replacementBgraSha256': hashlib.sha256(bigger).hexdigest()})
(out / 'ui.bgra').write_bytes(payload)
(out / 'pack-recipe.json').write_text(json.dumps({'schemaVersion': 1, 'format': 'FUIBGRA1', 'inventorySha256': hashlib.sha256(inventory.read_bytes()).hexdigest(),
    'packSha256': hashlib.sha256(payload).hexdigest(), 'assets': records, 'mips': 1,
    'matchCanonicalization': 'ARGB1555 preview channels recovered to 5 bits then expanded by SDK bit replication; P8 preview BGRA exact',
    'alpha': 'straight BGRA8; premultiplied filtering was undone by pilot generator'}, indent=2) + '\n')
print(json.dumps({'pack': str(out / 'ui.bgra'), 'bytes': len(payload), 'assets': len(records)}))
