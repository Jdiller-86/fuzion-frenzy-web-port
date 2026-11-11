"""Package the independently authored concept sources and captured previews."""
from pathlib import Path
import argparse
import zipfile
from PIL import Image

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('output', nargs='?', default='artifacts/alloy-studies', type=Path)
args = parser.parse_args()
root = Path(__file__).resolve().parent
original = root.parent / 'concepts'
out = args.output.resolve()
out.mkdir(parents=True, exist_ok=True)

def standalone(index, styles, script):
    text = index.read_text()
    for href, source in styles:
        text = text.replace(f'<link rel="stylesheet" href="{href}">', '<style>' + source.read_text() + '</style>')
    text = text.replace(f'<script src="{script.name}" defer></script>', '')
    return text.replace('</body>', '<script>' + script.read_text() + '</script></body>')

html = standalone(root/'index.html', [('../concepts/concepts.css', original/'concepts.css'), ('studies.css', root/'studies.css')], root/'studies.js')
html = html.replace('../concepts/index.html', 'FuzionFrenzy-Original-Concepts.html')
(out/'FuzionFrenzy-Alloy-Studies.html').write_text(html)
html = standalone(original/'index.html', [('concepts.css', original/'concepts.css')], original/'concepts.js')
(out/'FuzionFrenzy-Original-Concepts.html').write_text(html)
frames = []
for file in sorted((out/'animation-frames').glob('*.png')):
    with Image.open(file) as opened:
        frames.append(opened.convert('RGB').quantize(colors=96))
if frames:
    frames[0].save(out/'matrix-build-demo.gif', save_all=True, append_images=frames[1:], duration=[500]*(len(frames)-1)+[1800], loop=0, optimize=True)
with zipfile.ZipFile(out/'FuzionFrenzy-Alloy-Studies.zip', 'w', zipfile.ZIP_DEFLATED) as package:
    for source in [root, original]:
        allowed = ['index.html', 'README.md', 'studies.css', 'studies.js'] if source==root else ['index.html', 'README.md', 'concepts.css', 'concepts.js']
        for name in allowed:
            package.write(source/name, f'design/{source.name}/{name}')
    for name in ['FuzionFrenzy-Alloy-Studies.html','FuzionFrenzy-Original-Concepts.html','matrix-build-demo.gif']:
        if (out/name).exists():
            package.write(out/name, name)
    for file in out.glob('*.png'):
        if file.name!='animation-endpoints.png':
            package.write(file, 'previews/'+file.name)
print('Packaged eight selectable concepts, rendered previews and the dot-matrix animation.')
