// Development-only static server: only these source files can be requested.
import { createServer } from 'node:http';
import { readFile } from 'node:fs/promises';
import { fileURLToPath } from 'node:url';
const root = new URL('../', import.meta.url);
const allowed = new Map([['/src/transforms.mjs',['src/transforms.mjs','text/javascript; charset=utf-8']],['/src/clips.mjs',['src/clips.mjs','text/javascript; charset=utf-8']],['/', ['proof/index.html','text/html; charset=utf-8']], ['/proof/main.mjs',['proof/main.mjs','text/javascript; charset=utf-8']], ['/proof/worker.mjs',['proof/worker.mjs','text/javascript; charset=utf-8']], ['/src/xdvdfs.mjs',['src/xdvdfs.mjs','text/javascript; charset=utf-8']], ['/src/xbp.mjs',['src/xbp.mjs','text/javascript; charset=utf-8']], ['/src/audio.mjs',['src/audio.mjs','text/javascript; charset=utf-8']], ['/src/obe.mjs',['src/obe.mjs','text/javascript; charset=utf-8']], ['/src/character.mjs',['src/character.mjs','text/javascript; charset=utf-8']], ['/src/animation.mjs',['src/animation.mjs','text/javascript; charset=utf-8']], ['/src/texture.mjs',['src/texture.mjs','text/javascript; charset=utf-8']], ['/src/identity.mjs',['src/identity.mjs','text/javascript; charset=utf-8']], ['/proof/preview.mjs',['proof/preview.mjs','text/javascript; charset=utf-8']]]);
createServer(async (req,res) => {
  const item = req.method === 'GET' ? allowed.get(req.url) : undefined;
  if (!item) { res.writeHead(404); res.end(); return; }
  try {
    const data = await readFile(fileURLToPath(new URL(item[0],root)));
    res.writeHead(200, {'Content-Type': item[1], 'Cache-Control':'no-store', 'Content-Security-Policy': "default-src 'none'; script-src 'self'; worker-src 'self'; connect-src 'none'; base-uri 'none'; form-action 'none'", 'X-Content-Type-Options':'nosniff', 'Cross-Origin-Opener-Policy':'same-origin', 'Cross-Origin-Embedder-Policy':'require-corp'}); res.end(data);
  } catch { res.writeHead(500); res.end(); }
}).listen(4173, '127.0.0.1', () => console.log('Local research server at http://127.0.0.1:4173'));
