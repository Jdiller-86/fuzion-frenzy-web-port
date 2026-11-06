#!/usr/bin/env node
// Publishes only a derived project report. The app, original content and private notes are excluded.
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import {createHash} from 'node:crypto';
import {execFileSync} from 'node:child_process';
import {pathToFileURL} from 'node:url';
import {ROOT,REPOSITORY,loadPlan,validateSnapshot} from './project.mjs';
import {renderGatesSvg,renderProgressPage} from './project-graph.mjs';

export const SITE_URL='https://jdiller-86.github.io/fuzion-frenzy-web-port/';
export const SITE_MARKER='<meta name="fuzion-progress-report" content="v1">';
const ALLOWED=['.nojekyll','index.html','progress/index.html','progress/gates.svg','progress/project.json'];
const hash=contents=>createHash('sha256').update(contents).digest('hex');
export function buildProgressFiles(snapshot,plan,template,sourceCommit){
  const safe=validateSnapshot(snapshot);assert.match(sourceCommit,/^[a-f0-9]{40}$/);assert.equal(safe.sourceCommit,sourceCommit,'Tracking snapshot must match the published source revision');
  const index='<!doctype html><html lang="en"><head><meta charset="utf-8">'+SITE_MARKER+'<meta name="viewport" content="width=device-width,initial-scale=1"><meta http-equiv="refresh" content="0;url=progress/"><title>Fuzion Frenzy project progress</title></head><body><p><a href="progress/">Open the project tracking graph</a></p></body></html>\n';
  const page=renderProgressPage(safe,plan,template);assert.ok(page.includes(SITE_MARKER),'Graph page must identify its publication');
  return {'.nojekyll':'\n','index.html':index,'progress/index.html':page,'progress/gates.svg':renderGatesSvg(safe),'progress/project.json':JSON.stringify(safe,null,2)+'\n'};
}
function safeDirectory(directory){
  const base=path.join(ROOT,'artifacts');const resolved=path.resolve(directory);assert.ok(resolved.startsWith(base+path.sep),'Publication must stay in artifacts/');
  let current=base;for(const part of ['',...path.relative(base,resolved).split(path.sep)]){if(part)current=path.join(current,part);if(fs.existsSync(current)){const info=fs.lstatSync(current);assert.ok(info.isDirectory()&&!info.isSymbolicLink(),'Publication directory is not a real directory');}else fs.mkdirSync(current);}
  return resolved;
}
export function writeProgressFiles(files,directory=path.join(ROOT,'artifacts/project-site')){
  assert.deepEqual(Object.keys(files).sort(),[...ALLOWED].sort(),'Unexpected publication paths');const output=safeDirectory(directory);
  function inspect(folder,relative=''){for(const entry of fs.readdirSync(folder,{withFileTypes:true})){const name=relative+entry.name;assert.ok(!entry.isSymbolicLink(),'Publication contains a symlink');if(entry.isDirectory()){assert.equal(name,'progress','Unexpected publication directory');inspect(path.join(folder,entry.name),name+'/');}else assert.ok(entry.isFile()&&ALLOWED.includes(name),'Unexpected publication file');}}
  inspect(output);safeDirectory(path.join(output,'progress'));
  for(const [name,contents] of Object.entries(files)){assert.equal(typeof contents,'string');const target=path.join(output,name);if(fs.existsSync(target))assert.ok(fs.lstatSync(target).isFile()&&!fs.lstatSync(target).isSymbolicLink());fs.writeFileSync(target,contents);}
  inspect(output);
  return {schemaVersion:1,repository:REPOSITORY,siteUrl:SITE_URL,files:Object.fromEntries(Object.entries(files).map(([name,body])=>[name,{bytes:Buffer.byteLength(body),sha256:hash(body)}]))};
}
export async function verifyProgressSite(receipt,{fetchImpl=fetch,pause=ms=>new Promise(resolve=>setTimeout(resolve,ms)),attempts=12}={}){
  assert.equal(receipt.repository,REPOSITORY);assert.equal(receipt.siteUrl,SITE_URL);assert.deepEqual(Object.keys(receipt.files).sort(),[...ALLOWED].sort());
  for(let attempt=0;attempt<attempts;attempt++){
    const checks=await Promise.all(ALLOWED.filter(name=>!name.startsWith('.')).map(async name=>{try{const response=await fetchImpl(SITE_URL+name,{headers:{'Cache-Control':'no-cache'},signal:AbortSignal.timeout(20000)});if(!response.ok||!response.url.startsWith(SITE_URL))return false;return hash(await response.text())===receipt.files[name].sha256;}catch{return false;}}));
    if(checks.every(Boolean))return {verified:true,url:SITE_URL+'progress/',files:checks.length};if(attempt+1<attempts)await pause(10000);
  }
  throw new Error('Published graph files did not match the checked report before the verification deadline');
}
async function main(args){
  assert.ok(args.length<=1&&(!args.length||args[0]==='--verify'),'Only --verify is supported');const receiptPath=path.join(ROOT,'artifacts/project-publication.json');
  if(args[0]==='--verify'){const result=await verifyProgressSite(JSON.parse(fs.readFileSync(receiptPath,'utf8')));console.log(`Verified ${result.files} published tracking files: ${result.url}`);return;}
  const sourceCommit=process.env.TRACKING_SOURCE_SHA??execFileSync('git',['rev-parse','HEAD'],{cwd:ROOT,encoding:'utf8'}).trim();
  const input=path.join(ROOT,'artifacts/project.json'),info=fs.lstatSync(input);assert.ok(info.isFile()&&!info.isSymbolicLink(),'Snapshot must be a regular report file');assert.ok(info.size<1024*1024,'Project snapshot is too large');
  const snapshot=JSON.parse(fs.readFileSync(input,'utf8'));const files=buildProgressFiles(snapshot,loadPlan(),fs.readFileSync(path.join(ROOT,'tools/project-graph.html'),'utf8'),sourceCommit);
  const receipt={...writeProgressFiles(files),sourceCommit,available:snapshot.available};if(fs.existsSync(receiptPath))assert.ok(fs.lstatSync(receiptPath).isFile()&&!fs.lstatSync(receiptPath).isSymbolicLink());fs.writeFileSync(receiptPath,JSON.stringify(receipt,null,2)+'\n');console.log('Built a source-only public tracking graph (five allowlisted files).');
}
if(process.argv[1]&&import.meta.url===pathToFileURL(path.resolve(process.argv[1])).href)main(process.argv.slice(2)).catch(error=>{console.error(error.message.split('\n')[0]);process.exitCode=1;});
