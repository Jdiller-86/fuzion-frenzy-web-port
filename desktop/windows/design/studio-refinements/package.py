"""Package the B3 refinement, four variants and preserved earlier designs."""
from pathlib import Path
import argparse
import zipfile
from PIL import Image

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('output', nargs='?', default='artifacts/studio-refinements', type=Path)
a = parser.parse_args()
root = Path(__file__).resolve().parent
base = root.parent/'concepts'
alloy = root.parent/'alloy-studies'
out = a.output.resolve()
out.mkdir(parents=True, exist_ok=True)

def standalone(index, styles, script):
    text = index.read_text()
    for href, source in styles:
        text = text.replace(f'<link rel="stylesheet" href="{href}">', '<style>'+source.read_text()+'</style>')
    text = text.replace(f'<script src="{script.name}" defer></script>', '')
    return text.replace('</body>', '<script>'+script.read_text()+'</script></body>')

html = standalone(root/'index.html', [('../concepts/concepts.css',base/'concepts.css'),('../alloy-studies/studies.css',alloy/'studies.css'),('studio.css',root/'studio.css')],root/'studio.js')
html = html.replace('../alloy-studies/index.html','FuzionFrenzy-Alloy-Studies.html')
(out/'FuzionFrenzy-Studio-Refinements.html').write_text(html)
html = standalone(alloy/'index.html',[('../concepts/concepts.css',base/'concepts.css'),('studies.css',alloy/'studies.css')],alloy/'studies.js').replace('../concepts/index.html','FuzionFrenzy-Original-Concepts.html')
(out/'FuzionFrenzy-Alloy-Studies.html').write_text(html)
html = standalone(base/'index.html',[('concepts.css',base/'concepts.css')],base/'concepts.js')
(out/'FuzionFrenzy-Original-Concepts.html').write_text(html)
frames=[]
for file in sorted((out/'animation-frames').glob('*.png')):
    with Image.open(file) as im:
        frames.append(im.convert('RGB').quantize(colors=96))
if frames:
    frames[0].save(out/'amber-matrix-demo.gif',save_all=True,append_images=frames[1:],duration=[500]*(len(frames)-1)+[1800],loop=0,optimize=True)
with zipfile.ZipFile(out/'FuzionFrenzy-Studio-Refinements.zip','w',zipfile.ZIP_DEFLATED) as z:
    for directory, names in [(root,['index.html','studio.css','studio.js','README.md']),(alloy,['index.html','studies.css','studies.js','README.md']),(base,['index.html','concepts.css','concepts.js','README.md'])]:
        for name in names:
            z.write(directory/name,f'design/{directory.name}/{name}')
    for file in list(out.glob('*.html'))+list(out.glob('*.png'))+list(out.glob('*.gif')):
        z.write(file,file.name if file.suffix=='.html' else 'previews/'+file.name)
print('Packaged refined B3, four further directions, and all 13 selectable concepts.')
