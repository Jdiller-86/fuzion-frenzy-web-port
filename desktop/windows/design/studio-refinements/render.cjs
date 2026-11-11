// Capture visual design artifacts, without invoking the game or build tools.
const { chromium } = require('playwright');
const http = require('node:http');
const fs = require('node:fs');
const path = require('node:path');
const files = new Map([['/studio-refinements/index.html',['index.html','text/html']],['/studio-refinements/studio.css',['studio.css','text/css']],['/studio-refinements/studio.js',['studio.js','text/javascript']],['/alloy-studies/index.html',['../alloy-studies/index.html','text/html']],['/alloy-studies/studies.css',['../alloy-studies/studies.css','text/css']],['/alloy-studies/studies.js',['../alloy-studies/studies.js','text/javascript']],['/concepts/concepts.css',['../concepts/concepts.css','text/css']],['/concepts/index.html',['../concepts/index.html','text/html']],['/concepts/concepts.js',['../concepts/concepts.js','text/javascript']]]);
const themes=['studioamber','controlroom','relay','broadcast','obsidian'];
const directions=themes.slice(1);
const out=path.resolve(process.argv[2] || 'artifacts/studio-refinements');
fs.mkdirSync(out,{recursive:true});
const server=http.createServer((req,res)=>{const pathname=new URL(req.url,'http://localhost').pathname;const entry=files.get(pathname);if(!entry){res.writeHead(404);res.end();return;}res.writeHead(200,{'Content-Type':entry[1],'Cache-Control':'no-store'});res.end(fs.readFileSync(path.join(__dirname,entry[0])));});
(async()=>{
 await new Promise(resolve=>server.listen(0,'127.0.0.1',resolve));
 let browser;
 try {
  browser=await chromium.launch({headless:true,executablePath:process.env.CHROMIUM_PATH || '/usr/bin/chromium',args:['--no-sandbox']});
  const page=await browser.newPage({viewport:{width:1200,height:760},deviceScaleFactor:1});
  const root=`http://127.0.0.1:${server.address().port}/studio-refinements/index.html`;
  for(const theme of themes)for(const view of ['home','settings','build']){
   await page.goto(`${root}?capture&freeze&concept=${theme}&view=${view}&state=${view==='build'?'building':'ready'}`);
   await page.evaluate(()=>document.fonts.ready);
   await page.evaluate(()=>new Promise(resolve=>requestAnimationFrame(()=>requestAnimationFrame(resolve))));
   await page.screenshot({path:path.join(out,`${theme}-${view}.png`)});
  }
  await page.goto(root);
  await page.screenshot({path:path.join(out,'interactive-gallery.png'),fullPage:true});
  // The comparison contains the same full-size renders, scaled equally.
  const labels=['B3-A / Control Room','B3-B / Relay','B3-C / Broadcast','B3-D / Obsidian'];
  await page.setViewportSize({width:1600,height:1120});
  await page.setContent(`<html><body style="margin:0;padding:34px;background:#e7e9ec;font-family:Arial;color:#28313b"><h1 style="font-size:27px;margin:0 0 6px;letter-spacing:-.8px">B3 — four further directions</h1><p style="font-size:13px;color:#66717b;margin:0 0 23px">Amber matrix displays · distinct equipment layouts · all earlier concepts preserved</p><div style="display:grid;grid-template-columns:1fr 1fr;gap:25px 24px">${directions.map((theme,i)=>`<section><h2 style="margin:0 0 10px;font-size:17px">${labels[i]}</h2><img style="display:block;width:100%;border-radius:7px;box-shadow:0 5px 15px #2835411a" src="data:image/png;base64,${fs.readFileSync(path.join(out,theme+'-home.png')).toString('base64')}"></section>`).join('')}</div></body></html>`);
  await page.screenshot({path:path.join(out,'comparison.png'),fullPage:true});
  await page.setViewportSize({width:1200,height:760});
  await page.goto(`${root}?capture&concept=studioamber&state=building&view=home`);
  const frameDir=path.join(out,'animation-frames');fs.mkdirSync(frameDir,{recursive:true});
  const matrix=page.locator('.amber-rack-face .readout');
  for(let i=0;i<42;i++){await matrix.screenshot({path:path.join(frameDir,String(i).padStart(3,'0')+'.png')});await page.waitForTimeout(440);}
  console.log(`Captured refined Studio Amber and four further directions in ${out}`);
 } finally {if(browser)await browser.close();server.close();}
})().catch(error=>{console.error(error);process.exitCode=1;});
