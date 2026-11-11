#!/usr/bin/env node
import fs from 'node:fs';
import path from 'node:path';
import http from 'node:http';
import { assertNoLinks } from './private-output.mjs';
const argv = process.argv.slice(2); let rootArg = '', port = 4176;
for (let i = 0; i < argv.length; i++) { if (argv[i] === '--root') rootArg = argv[++i] || ''; else if (argv[i] === '--port') port = Number(argv[++i]); else throw new Error('Usage: node server.mjs --root <catalog output> [--port 4176]'); }
if (!rootArg || !Number.isInteger(port) || port < 1 || port > 65535) throw new Error('An absolute --root and valid port are required.');
if (!path.isAbsolute(rootArg)) throw new Error('--root must be absolute.');
const root = path.resolve(rootArg);
function hasNoSymlinkComponents(target) {
  const absolute = path.resolve(target), parsed = path.parse(absolute); let current = parsed.root;
  for (const part of absolute.slice(parsed.root.length).split(path.sep).filter(Boolean)) {
    current = path.join(current, part);
    try { if (fs.lstatSync(current).isSymbolicLink()) return false; } catch { return false; }
  }
  return true;
}
await assertNoLinks(root);
const manifestPath = path.join(root, 'catalog-manifest.json');
await assertNoLinks(manifestPath);
const manifest = JSON.parse(fs.readFileSync(manifestPath, 'utf8'));
if (!Array.isArray(manifest.files)) throw new Error('Invalid catalog allowlist.');
const allow = new Set(manifest.files);
const types = { '.html':'text/html; charset=utf-8', '.mjs':'text/javascript; charset=utf-8', '.css':'text/css; charset=utf-8', '.json':'application/json; charset=utf-8', '.png':'image/png', '.svg':'image/svg+xml' };
function resolveSafe(urlPath) {
  let decoded; try { decoded = decodeURIComponent(urlPath); } catch { return null; }
  if (decoded.includes('\0') || decoded.includes('\\') || decoded.split('/').some(s => s === '..' || s === '.')) return null;
  let rel = decoded.replace(/^\/+/, '') || 'index.html';
  if (!allow.has(rel)) return null;
  const full = path.resolve(root, rel);
  if (!full.startsWith(root + path.sep)) return null;
  if (!hasNoSymlinkComponents(full)) return null;
  return fs.existsSync(full) && fs.statSync(full).isFile() ? full : null;
}
const server = http.createServer((req, res) => {
  const headers = { 'Content-Security-Policy': "default-src 'self'; img-src 'self' data:; style-src 'self'; script-src 'self'; connect-src 'self'; object-src 'none'; base-uri 'none'; frame-ancestors 'none'", 'X-Content-Type-Options':'nosniff', 'Referrer-Policy':'no-referrer', 'Cache-Control':'no-store' };
  const host = req.headers.host;
  const allowedHosts = new Set([`127.0.0.1:${port}`, `localhost:${port}`]);
  if (!allowedHosts.has(host)) { res.writeHead(421, headers); return res.end('Misdirected request'); }
  if (req.headers.origin) {
    let origin;
    try { origin = new URL(req.headers.origin); } catch { res.writeHead(403, headers); return res.end('Forbidden origin'); }
    if (!['http://127.0.0.1:' + port, 'http://localhost:' + port].includes(origin.origin)) { res.writeHead(403, headers); return res.end('Forbidden origin'); }
  }
  if (!['GET','HEAD'].includes(req.method)) { res.writeHead(405, { ...headers, Allow:'GET, HEAD' }); return res.end('Method not allowed'); }
  const file = resolveSafe(new URL(req.url, 'http://127.0.0.1').pathname);
  if (!file) { res.writeHead(404, headers); return res.end('Not found'); }
  const stat = fs.statSync(file), type = types[path.extname(file).toLowerCase()] || 'application/octet-stream';
  res.writeHead(200, { ...headers, 'Content-Type':type, 'Content-Length':stat.size });
  if (req.method === 'HEAD') return res.end();
  fs.createReadStream(file).pipe(res);
});
server.listen(port, '127.0.0.1', () => console.log(`Private UI catalog at http://127.0.0.1:${port}/`));
