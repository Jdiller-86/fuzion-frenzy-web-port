// Supply the existing loose-file readers from a local XISO for one inventory run.
import { createWriteStream, openAsBlob } from 'node:fs';
import { mkdir, readdir } from 'node:fs/promises';
import path from 'node:path';
import { pipeline } from 'node:stream/promises';
import { openXiso } from '../../host/xdvdfs.mjs';
import { preparePrivateOutput } from './private-output.mjs';

const [image, output, extra] = process.argv.slice(2);
if (!image || !output || extra) throw new Error('Pass the local XISO and an empty ignored scratch directory');
const destination = await preparePrivateOutput(output, { directory: true, inputs: [image] });
if ((await readdir(destination)).length) throw new Error('UI input scratch must be empty');
const volume = await openXiso(await openAsBlob(image));
const executable = await volume.find('default.xbe');
if (executable.directory) throw new Error('Expected default.xbe file');

const inputs = [{ entry: executable, target: path.join(destination, 'default.xbe') }];
const packageRoot = await volume.find('Packages');
const pending = [{ directory: packageRoot, parent: path.join(destination, packageRoot.name) }];
const visited = new Set();
while (pending.length) {
  const { directory, parent } = pending.pop();
  if (visited.has(directory.offset) || visited.size >= 128) throw new Error('Unexpected XISO package directory layout');
  visited.add(directory.offset);
  for (const entry of await volume.list(directory)) {
    if (/[<>"|?*]|[ .]$|^(?:aux|con|nul|prn|com[0-9]|lpt[0-9])(?:\.|$)/i.test(entry.name)) {
      throw new Error('Invalid loose package path');
    }
    const target = path.join(parent, entry.name);
    if (entry.directory) pending.push({ directory: entry, parent: target });
    else if (/\.xbp$/i.test(entry.name)) inputs.push({ entry, target });
  }
}
const bytes = inputs.reduce((sum, { entry }) => sum + entry.size, 0);
if (inputs.length < 2 || inputs.length > 2048 || bytes > 1024 ** 3) throw new Error('Unexpected XISO UI input size');
for (const { entry, target } of inputs) {
  await mkdir(path.dirname(target), { recursive: true });
  await pipeline(volume.file(entry).stream(), createWriteStream(target, { flags: 'wx' }));
}
console.log(`Staged ${inputs.length - 1} private XBP files and the XBE (${bytes} bytes)`);
