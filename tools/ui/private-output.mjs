// Keep derived content inside ignored output paths, separate from all inputs.
import fs from 'node:fs/promises';
import path from 'node:path';
import { execFileSync } from 'node:child_process';
import { pathToFileURL } from 'node:url';

export async function assertNoLinks(value) {
  const absolute = path.resolve(value);
  const root = path.parse(absolute).root;
  let current = root;
  for (const part of absolute.slice(root.length).split(path.sep).filter(Boolean)) {
    current = path.join(current, part);
    try {
      if ((await fs.lstat(current)).isSymbolicLink()) throw new Error(`Links are not allowed in artifact paths: ${current}`);
    } catch (error) {
      if (error.code === 'ENOENT') break;
      throw error;
    }
  }
}

function comparable(value) {
  const absolute = path.resolve(value);
  return process.platform === 'win32' ? absolute.toLowerCase() : absolute;
}
function contains(parent, child) {
  const relative = path.relative(parent, child);
  return relative === '' || (!path.isAbsolute(relative) && relative !== '..' && !relative.startsWith(`..${path.sep}`));
}
export async function preparePrivateOutput(output, { directory = false, inputs = [] } = {}) {
  output = path.resolve(output);
  await assertNoLinks(output);
  const target = comparable(output);
  for (const input of inputs.filter(Boolean)) {
    const absolute = path.resolve(input);
    await assertNoLinks(absolute);
    const stat = await fs.stat(absolute);
    const source = comparable(absolute);
    if (target === source || (stat.isDirectory() && contains(source, target)) || (directory && contains(target, source))) {
      throw new Error(`Output overlaps a supplied input: ${output}`);
    }
  }
  let candidate = directory ? output : path.dirname(output), repo = null;
  for (;;) {
    try { await fs.lstat(path.join(candidate, '.git')); repo = candidate; break; } catch (error) { if (error.code !== 'ENOENT' && error.code !== 'ENOTDIR') throw error; }
    const parent = path.dirname(candidate);
    if (parent === candidate) break;
    candidate = parent;
  }
  if (!repo) throw new Error('Private outputs must be inside an ignored project directory');
  try { execFileSync('git', ['-C', repo, 'check-ignore', '--no-index', '--quiet', '--', output], { stdio: 'ignore' }); }
  catch { throw new Error(`Private output is not Git-ignored: ${output}`); }
  await fs.mkdir(directory ? output : path.dirname(output), { recursive: true });
  await assertNoLinks(output);
  return output;
}

if (process.argv[1] && import.meta.url === pathToFileURL(path.resolve(process.argv[1])).href) {
  let output;
  const inputs = [];
  for (let i = 2; i < process.argv.length; i += 2) {
    if (process.argv[i] === '--directory') output = process.argv[i + 1];
    else if (process.argv[i] === '--input') inputs.push(process.argv[i + 1]);
    else throw new Error('Use --directory <ignored output> --input <source>');
  }
  if (!output) throw new Error('Missing --directory');
  console.log(await preparePrivateOutput(output, { directory: true, inputs }));
}
