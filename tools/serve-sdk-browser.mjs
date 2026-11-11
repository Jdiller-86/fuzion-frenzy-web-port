import http from 'node:http';
import fs from 'node:fs/promises';
import path from 'node:path';
import { fileURLToPath, pathToFileURL } from 'node:url';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const routes = new Map([
  ['/sdk-browser/', ['web/sdk-browser/index.html', 'text/html; charset=utf-8']],
  ['/sdk-browser/index.html', ['web/sdk-browser/index.html', 'text/html; charset=utf-8']],
  ['/sdk-browser/app.mjs', ['web/sdk-browser/app.mjs', 'text/javascript; charset=utf-8']],
  ['/sdk-browser/worker.mjs', ['web/sdk-browser/worker.mjs', 'text/javascript; charset=utf-8']],
  ['/host/xdvdfs.mjs', ['host/xdvdfs.mjs', 'text/javascript; charset=utf-8']],
  ['/artifacts/sdk-browser-full/fuzion-sdk-full.mjs', ['.tools/artifacts/sdk-browser-full/fuzion-sdk-full.mjs', 'text/javascript; charset=utf-8']],
  ['/artifacts/sdk-browser-full/fuzion-sdk-full.wasm', ['.tools/artifacts/sdk-browser-full/fuzion-sdk-full.wasm', 'application/wasm']],
  ['/artifacts/sdk-browser-full/fuzion-sdk-full.worker.js', ['.tools/artifacts/sdk-browser-full/fuzion-sdk-full.worker.js', 'text/javascript; charset=utf-8']],
]);

export function createSdkBrowserServer() {
  return http.createServer(async (request, response) => {
    response.setHeader('Cache-Control', 'no-store');
    response.setHeader('Cross-Origin-Opener-Policy', 'same-origin');
    response.setHeader('Cross-Origin-Embedder-Policy', 'require-corp');
    response.setHeader('Content-Security-Policy', "default-src 'none'; script-src 'self' 'wasm-unsafe-eval'; style-src 'unsafe-inline'; worker-src 'self'; connect-src 'self'; base-uri 'none'; form-action 'none'");
    let pathname;
    try { pathname = new URL(request.url, 'http://localhost').pathname; }
    catch { response.writeHead(400).end(); return; }
    const route = routes.get(pathname);
    if (!['GET', 'HEAD'].includes(request.method) || !route) {
      response.writeHead(404, { 'Content-Type': 'text/plain; charset=utf-8' }).end('Not found');
      return;
    }
    try {
      const filename = path.join(root, route[0]);
      const stat = await fs.lstat(filename);
      if (!stat.isFile() || stat.isSymbolicLink()) throw new Error('Not a regular file');
      const bytes = await fs.readFile(filename);
      response.writeHead(200, {
        'Content-Type': route[1],
        'Content-Length': bytes.length,
        'X-Content-Type-Options': 'nosniff',
        'Referrer-Policy': 'no-referrer',
      });
      response.end(request.method === 'HEAD' ? undefined : bytes);
    } catch {
      response.writeHead(503, { 'Content-Type': 'text/plain; charset=utf-8' }).end('Local SDK browser artifact is unavailable. Build it with tools/build-sdk-browser-full.py first.');
    }
  });
}

if (process.argv[1] && import.meta.url === pathToFileURL(path.resolve(process.argv[1])).href) {
  const [portArg = '4182', ...extra] = process.argv.slice(2);
  const port = Number(portArg);
  if (extra.length || !Number.isInteger(port) || port < 1024 || port > 65535) throw new Error('Supply one valid loopback port');
  createSdkBrowserServer().listen(port, '127.0.0.1', () => {
    console.log(`Local SDK browser host: http://127.0.0.1:${port}/sdk-browser/ (PID ${process.pid})`);
  });
}
