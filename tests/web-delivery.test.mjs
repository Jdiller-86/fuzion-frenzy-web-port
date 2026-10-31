import test from 'node:test';import assert from 'node:assert/strict';
import {createWebServer} from '../tools/serve-web.mjs';
import {webFiles} from '../tools/web-files.mjs';
import {readFile} from 'node:fs/promises';
import {execFileSync} from 'node:child_process';
test('source server serves every module and denies original paths, uploads and traversal',async()=>{
  const server=createWebServer();await new Promise(resolve=>server.listen(0,'127.0.0.1',resolve));const base=`http://127.0.0.1:${server.address().port}`;
  try{for(const path of ['/',...webFiles.map(p=>'/'+p)]){const response=await fetch(base+path);assert.equal(response.status,200,path);assert.match(response.headers.get('content-security-policy'),/connect-src 'none'/);assert.ok((await response.text()).length>0);}
    for(const path of ['/research/reference/default.xbe','/Fuzion%20Frenzy%20(USA)/default.xbe','/src/../research/file','/package.json','/.env','/?file=default.xbe'])assert.equal((await fetch(base+path)).status,404,path);
    assert.equal((await fetch(base+'/',{method:'POST',body:'source-owned upload probe'})).status,404);
    for(const path of webFiles){const text=await readFile(new URL('../'+path,import.meta.url),'utf8');assert.doesNotMatch(text,/\bfetch\s*\(|XMLHttpRequest|sendBeacon|WebSocket|localStorage|indexedDB/,'browser source must not upload or persist imported content');}
  }finally{await new Promise(resolve=>server.close(resolve));}
});
test('source-only build serves identical reviewed modules without local content',async()=>{
  execFileSync(process.execPath,['tools/build-web.mjs'],{cwd:new URL('../',import.meta.url),stdio:'pipe'});
  const server=createWebServer({built:true});await new Promise(resolve=>server.listen(0,'127.0.0.1',resolve));const base=`http://127.0.0.1:${server.address().port}`;
  try{for(const path of webFiles){const response=await fetch(base+'/'+path);assert.equal(response.status,200);assert.equal(await response.text(),await readFile(new URL('../'+path,import.meta.url),'utf8'));}
    assert.equal((await fetch(base+'/research/reference/default.xbe')).status,404);
  }finally{await new Promise(resolve=>server.close(resolve));}
});
