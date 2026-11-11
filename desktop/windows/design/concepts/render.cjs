// Capture visual design artifacts, without invoking the game or build tools.
const { chromium } = require('playwright');
const http = require('node:http');
const fs = require('node:fs');
const path = require('node:path');
const files = new Map([['/index.html','text/html'],['/concepts.css','text/css'],['/concepts.js','text/javascript']]);
const themes=['graphite','alloy','arena','paper'];
const out=path.resolve(process.argv[2] || 'artifacts/launcher-concepts');
fs.mkdirSync(out,{recursive:true});
const server=http.createServer((req,res)=>{const pathname=new URL(req.url,'http://localhost').pathname;const type=files.get(pathname);if(!type){res.writeHead(404);res.end();return;}res.writeHead(200,{'Content-Type':type,'Cache-Control':'no-store'});res.end(fs.readFileSync(path.join(__dirname,pathname)));});
(async()=>{
 await new Promise(resolve=>server.listen(0,'127.0.0.1',resolve));
 let browser;
 try {
  browser=await chromium.launch({headless:true,executablePath:process.env.CHROMIUM_PATH || '/usr/bin/chromium',args:['--no-sandbox']});
  const page=await browser.newPage({viewport:{width:1200,height:760},deviceScaleFactor:1});
  const root=`http://127.0.0.1:${server.address().port}/index.html`;
  for(const theme of themes)for(const view of ['home','settings','build']){
   await page.goto(`${root}?capture&concept=${theme}&view=${view}&state=${view==='build'?'building':'ready'}`);
   await page.screenshot({path:path.join(out,`${theme}-${view}.png`)});
  }
  await page.goto(root);
  await page.screenshot({path:path.join(out,'interactive-gallery.png'),fullPage:true});
  // The comparison contains the same full-size renders, scaled equally.
  const labels=['A / Graphite','B / Alloy','C / Arena','D / Paper'];
  await page.setViewportSize({width:1600,height:1120});
  await page.setContent(`<html><body style="margin:0;padding:34px;background:#e7e9ec;font-family:Arial;color:#28313b"><h1 style="font-size:27px;margin:0 0 6px;letter-spacing:-.8px">Fuzion Frenzy — launcher concepts</h1><p style="font-size:13px;color:#66717b;margin:0 0 23px">Visual prototypes · simulated installed state · platform and version retained</p><div style="display:grid;grid-template-columns:1fr 1fr;gap:25px 24px">${themes.map((theme,i)=>`<section><h2 style="margin:0 0 10px;font-size:17px">${labels[i]}</h2><img style="display:block;width:100%;border-radius:7px;box-shadow:0 5px 15px #2835411a" src="data:image/png;base64,${fs.readFileSync(path.join(out,theme+'-home.png')).toString('base64')}"></section>`).join('')}</div></body></html>`);
  await page.screenshot({path:path.join(out,'comparison.png'),fullPage:true});
  console.log(`Captured four concepts in ${out}`);
 } finally {if(browser)await browser.close();server.close();}
})().catch(error=>{console.error(error);process.exitCode=1;});
