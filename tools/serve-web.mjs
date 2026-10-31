import {createServer} from 'node:http';
import {readFile} from 'node:fs/promises';
import {pathToFileURL} from 'node:url';
import {webFiles} from './web-files.mjs';
export function createWebServer({built=false}={}){
  const root=new URL(built?'../dist/':'../',import.meta.url),allowed=new Map(webFiles.map(p=>['/'+p,p]));allowed.set('/','apps/web/index.html');
  return createServer(async(req,res)=>{
    const path=req.method==='GET'?allowed.get(req.url):undefined;
    if(!path){res.writeHead(404);res.end();return;}
    try{const data=await readFile(new URL(path,root)),type=path.endsWith('.html')?'text/html':path.endsWith('.css')?'text/css':'text/javascript';res.writeHead(200,{'Content-Type':type+'; charset=utf-8','Cache-Control':'no-store','Content-Security-Policy':"default-src 'none'; script-src 'self'; style-src 'self'; worker-src 'self'; connect-src 'none'; base-uri 'none'; form-action 'none'; frame-ancestors 'none'",'X-Content-Type-Options':'nosniff','Cross-Origin-Opener-Policy':'same-origin','Cross-Origin-Embedder-Policy':'require-corp'});res.end(data);}
    catch{res.writeHead(500);res.end();}
  });
}
if(process.argv[1]&&import.meta.url===pathToFileURL(process.argv[1]).href)createWebServer({built:process.argv.includes('--built')}).listen(4174,'127.0.0.1',()=>console.log('Offline Twisted System: http://127.0.0.1:4174/'));
