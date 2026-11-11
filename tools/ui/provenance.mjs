// Hash supplied local inputs and retained source without copying their contents.
import fs from 'node:fs/promises';
import { createReadStream } from 'node:fs';
import crypto from 'node:crypto';
import path from 'node:path';
import { execFileSync } from 'node:child_process';
import { preparePrivateOutput } from './private-output.mjs';

const options = { files: [], trees: [] };
for (let i = 2; i < process.argv.length; i++) {
  const key = process.argv[i], value = process.argv[++i];
  if (!value) throw new Error(`Missing value for ${key}`);
  if (key === '--file' || key === '--tree') {
    const at = value.indexOf('=');
    if (at < 1) throw new Error('Use label=absolute-path');
    options[key === '--file' ? 'files' : 'trees'].push({ label: value.slice(0, at), path: path.resolve(value.slice(at + 1)) });
  } else if (key === '--out') options.out = path.resolve(value);
  else throw new Error(`Unknown argument ${key}`);
}
if (!options.out || !options.files.length) throw new Error('Use --out <private JSON> --file label=path [--tree label=path]');
async function identity(file) {
  const stat = await fs.lstat(file);
  if (!stat.isFile() || stat.isSymbolicLink()) throw new Error(`Regular files only: ${file}`);
  const hash = crypto.createHash('sha256');
  for await (const bytes of createReadStream(file)) hash.update(bytes);
  const after = await fs.stat(file);
  if (after.size !== stat.size || after.mtimeMs !== stat.mtimeMs) throw new Error(`Input changed while hashing: ${file}`);
  return { size: stat.size, sha256: hash.digest('hex') };
}
const excluded = new Set(['.git', '.tools', 'out', 'build', 'assets', 'node_modules', '__pycache__']);
async function sourceFiles(root, dir = root) {
  const result = [];
  for (const item of await fs.readdir(dir, { withFileTypes: true })) {
    if (item.isSymbolicLink() || excluded.has(item.name)) continue;
    const file = path.join(dir, item.name);
    if (item.isDirectory()) result.push(...await sourceFiles(root, file));
    else if (item.isFile() && /\.(?:cpp|h|hpp|c|mjs|py|toml|cmake|txt|json|patch|ps1|css|html)$/.test(item.name)) result.push(file);
  }
  return result.sort();
}
const files = [];
for (const item of options.files) files.push({ ...item, ...await identity(item.path) });
const trees = [];
for (const item of options.trees) {
  let gitHead = null;
  try {
    // Never attribute the enclosing research repository's HEAD to a source copy.
    await fs.lstat(path.join(item.path, '.git'));
    gitHead = execFileSync('git', ['-C', item.path, 'rev-parse', 'HEAD'], { encoding: 'utf8', stdio: ['ignore', 'pipe', 'ignore'] }).trim();
  } catch { /* A retained source copy may have no Git metadata. */ }
  const members = [];
  for (const file of await sourceFiles(item.path)) members.push({ path: path.relative(item.path, file).replaceAll('\\', '/'), ...await identity(file) });
  trees.push({ ...item, gitHead, fileCount: members.length, manifestSha256: crypto.createHash('sha256').update(JSON.stringify(members)).digest('hex'), files: members });
}
await preparePrivateOutput(options.out, { inputs: [...options.files, ...options.trees].map(item => item.path) });
await fs.writeFile(options.out, JSON.stringify({ schemaVersion: 1, recordedAt: new Date().toISOString(), node: process.version, treeScope: { extensions: 'cpp,h,hpp,c,mjs,py,toml,cmake,txt,json,patch,ps1,css,html', excludedDirectories: [...excluded], links: 'not followed', identities: 'current raw bytes; no historical provenance inferred' }, files, trees }, null, 2) + '\n');
console.log(JSON.stringify({ output: options.out, files: files.map(({ label, size, sha256 }) => ({ label, size, sha256 })), trees: trees.map(({ label, gitHead, fileCount, manifestSha256 }) => ({ label, gitHead, fileCount, manifestSha256 })) }, null, 2));
