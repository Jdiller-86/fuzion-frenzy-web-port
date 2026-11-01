import http from 'node:http';
import fs from 'node:fs/promises';
import path from 'node:path';
import { fileURLToPath, pathToFileURL } from 'node:url';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '../.tools/emulation');
const files = new Map([
  ['/', ['index.html', 'text/html; charset=utf-8']],
  ['/check.js', ['check.js', 'text/javascript']],
  ['/check.worker.js', ['check.worker.js', 'text/javascript']],
  ['/check.wasm', ['check.wasm', 'application/wasm']]
]);

/** Serves only the four generated, synthetic SDL fixture outputs. No uploads. */
export function createSdlCheckServer(attempt) {
  if (!/^[a-zA-Z0-9_-]{1,64}$/.test(attempt ?? '')) throw new Error('Invalid fixture attempt');
  const directory = path.join(root, `sdl-browser-${attempt}`);
  return http.createServer(async (request, response) => {
    response.setHeader('Cross-Origin-Opener-Policy', 'same-origin');
    response.setHeader('Cross-Origin-Embedder-Policy', 'require-corp');
    response.setHeader('Cache-Control', 'no-store');
    const file = files.get(request.url);
    if (!['GET', 'HEAD'].includes(request.method) || !file) return response.writeHead(404).end();
    try {
      if (!(await fs.lstat(directory)).isDirectory()) throw new Error('Not a fixture directory');
      const filename = path.join(directory, file[0]);
      const stat = await fs.lstat(filename);
      if (!stat.isFile() || stat.size > 16 * 1024 * 1024) throw new Error('Invalid fixture output');
      const bytes = await fs.readFile(filename);
      response.writeHead(200, { 'Content-Type': file[1], 'Content-Length': bytes.length });
      response.end(request.method === 'HEAD' ? undefined : bytes);
    } catch {
      response.writeHead(404).end();
    }
  });
}

if (process.argv[1] && import.meta.url === pathToFileURL(path.resolve(process.argv[1])).href) {
  const args = process.argv.slice(2);
  if (args.length !== 4 || args[0] !== '--attempt' || args[2] !== '--port') {
    throw new Error('Usage: node tools/serve-sdl-check.mjs --attempt <label> --port <port>');
  }
  const port = Number(args[3]);
  if (!Number.isInteger(port) || port < 1024 || port > 65535) throw new Error('Invalid loopback port');
  createSdlCheckServer(args[1]).listen(port, '127.0.0.1', () => {
    console.log(`Synthetic SDL dependency check: http://127.0.0.1:${port}/ (PID ${process.pid})`);
  });
}
