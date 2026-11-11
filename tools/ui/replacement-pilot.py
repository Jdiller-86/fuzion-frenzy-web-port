"""Deterministic, local-only resampling comparison; never installs replacements."""
import argparse
import hashlib
import html
import json
import re
from pathlib import Path
import PIL
from PIL import Image
from private_output import no_links, private_directory

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--inventory', required=True, type=Path)
parser.add_argument('--joins', required=True, type=Path)
parser.add_argument('--out', required=True, type=Path)
parser.add_argument('--id', required=True, action='append', dest='ids')
args = parser.parse_args()
inventory = json.loads(args.inventory.read_text(encoding='utf-8'))
joins = json.loads(args.joins.read_text(encoding='utf-8'))
if joins['executableSha256'] != inventory['sourceIdentity']['executable']['sha256']:
    raise ValueError('Evidence identities differ')
resources = {row['stableId']: row for row in inventory['resources']}
evidence = {row['stableId']: row for row in joins['resources']}
if len(args.ids) > 8:
    raise ValueError('Pilot is limited to eight representative textures')
args.out = private_directory(args.out, [args.inventory, args.joins, inventory['sourceIdentity']['sourceTree']])
records = []
sections = []
for asset_id in args.ids:
    if not re.fullmatch(r'[0-9a-f]{24}', asset_id):
        raise ValueError('Invalid stable asset ID')
    row, proof = resources[asset_id], evidence[asset_id]
    if proof['confidence'] != 'confirmed' or proof['classification'] != 'ui-texture':
        raise ValueError('Only statically confirmed UI textures are admitted')
    if row['frameCount'] != 1 or row['width'] > 1024 or row['height'] > 1024:
        raise ValueError('Only static small 2D base-mip previews are admitted')
    root = args.inventory.parent.resolve()
    source = no_links(root / row['thumbnail']).resolve()
    if root not in source.parents or source.suffix != '.png':
        raise ValueError('Preview must be a PNG within the private inventory')
    with Image.open(source) as opened:
        if opened.size != (row['width'], row['height']):
            raise ValueError('Preview dimensions differ from inventory')
        image = opened.convert('RGBA')
    if image.size != (row['width'], row['height']):
        raise ValueError('Preview dimensions differ from inventory')
    variants = []
    for scale, method in [(2, 'nearest-control'), (4, 'premultiplied-lanczos-preview')]:
        size = (image.width * scale, image.height * scale)
        if size[0] * size[1] * 4 > 32 * 1024 * 1024:
            raise ValueError('Pilot decoded image exceeds 32 MiB')
        if method == 'nearest-control':
            enlarged = image.resize(size, Image.Resampling.NEAREST)
        else:
            # Interpolate associated color and alpha together to avoid dark fringes.
            enlarged = image.convert('RGBa').resize(size, Image.Resampling.LANCZOS).convert('RGBA')
        filename = f'{asset_id}-{method}.png'
        destination = no_links(args.out / filename)
        enlarged.save(destination, compress_level=9)
        variants.append({'file': filename, 'scale': scale, 'physicalPixels': list(size),
                         'logicalPixels': list(image.size), 'uvRect': [0, 0, 1, 1],
                         'basePixelRect': [0, 0, image.width, image.height],
                         'replacementPixelRect': [0, 0, *size], 'method': method,
                         'sha256': hashlib.sha256(destination.read_bytes()).hexdigest(),
                         'alpha': 'nearest preserves source alpha; Lanczos introduces intermediate alpha even for a cutout source',
                         'mips': 'base mip only; no renderer upload performed'})
    records.append({'stableId': asset_id, 'resourceSha256': row['sha256'],
                    'previewSha256': hashlib.sha256(source.read_bytes()).hexdigest(),
                    'sourceFormat': row['format'], 'headerMipLevels': row['headerMipLevels'],
                    'variants': variants, 'readyForInstallation': False,
                    'manualReviewNeeded': ['edge/halo comparison', 'cutout alpha policy', 'tint and filtering', 'mip chain and logical/physical size API']})
    pictures = ''.join(f'<figure><img src="{html.escape(v["file"])}" alt="{html.escape(v["method"])}"><figcaption>{html.escape(v["method"])} / {v["scale"]}x; full-resolution file</figcaption></figure>' for v in variants)
    sections.append(f'<section><h2>{html.escape(asset_id)}</h2><p>Original logical size {image.width} x {image.height}; UV rectangle unchanged.</p>{pictures}</section>')
recipe = {'schemaVersion': 1, 'kind': 'private-read-only-replacement-pilot',
          'pillowVersion': PIL.__version__, 'originalExecutableSha256': joins['executableSha256'],
          'scope': 'Three representative static base-mip texture comparisons when invoked with the documented IDs; no font, animation or renderer replacement is claimed.',
          'installed': False, 'assets': records}
no_links(args.out / 'recipe.json').write_text(json.dumps(recipe, indent=2) + '\n', encoding='utf-8')
style = 'body{font:16px system-ui;background:#171e2c;color:#eee;padding:24px}section{margin:32px 0}figure{display:inline-block;max-width:95%;vertical-align:top}img{max-width:100%;height:auto;background:repeating-conic-gradient(#555 0 25%,#999 0 50%) 0/16px 16px}figcaption{margin:12px 0}'
no_links(args.out / 'index.html').write_text('<!doctype html><html lang="en"><meta charset="utf-8"><meta name="viewport" content="width=device-width"><title>Private resampling comparison</title><style>' + style + '</style><h1>Read-only replacement pilot</h1><p>Deterministic reference enlargements, not installed game replacements. Originals remain unchanged. No network or scripts are used.</p>' + ''.join(sections) + '</html>', encoding='utf-8')
print(json.dumps({'output': str(args.out.resolve()), 'assets': len(records), 'images': len(records) * 2,
                  'pillow': PIL.__version__, 'installed': False}))
