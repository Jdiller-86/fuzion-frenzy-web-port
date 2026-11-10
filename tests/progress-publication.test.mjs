import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import {ROOT,REPOSITORY,loadPlan} from '../tools/project.mjs';
import {SITE_URL,SITE_MARKER,buildProgressFiles,writeProgressFiles,verifyProgressSite} from '../tools/publish-progress.mjs';
import {checkProgressPages} from '../tools/progress-pages.mjs';
const SHA='a'.repeat(40),plan=loadPlan(),template=fs.readFileSync(path.join(ROOT,'tools/project-graph.html'),'utf8');
function snapshot(){let next=400;const number=item=>item.issueNumber??next++;return {schemaVersion:1,repository:REPOSITORY,sourceCommit:SHA,generatedAt:'2026-11-06T00:00:00Z',available:true,privateNote:'never-publish-this',milestones:plan.milestones.map(m=>({id:m.id,title:m.title,foundation:m.foundation,goal:m.goal,status:'Planned',accepted:false,issueUrl:`https://github.com/${REPOSITORY}/issues/${number(m)}`,dependsOn:m.dependsOn,tasks:m.tasks.map(t=>({id:t.id,title:t.title,status:'Planned',url:`https://github.com/${REPOSITORY}/issues/${number(t)}`,deliverables:t.brief.deliverables.map(d=>({id:d.id,title:d.title,size:d.size,status:'Planned',url:`https://github.com/${REPOSITORY}/issues/${number(d)}`}))}))}))};}
test('publication contains only five checked report files and requires the exact source identity',()=>{
  const files=buildProgressFiles(snapshot(),plan,template,SHA);assert.deepEqual(Object.keys(files).sort(),['.nojekyll','index.html','progress/gates.svg','progress/index.html','progress/project.json']);assert.ok(files['index.html'].includes(SITE_MARKER));assert.ok(files['index.html'].includes('href="progress/"'));assert.ok(fs.readFileSync(path.join(ROOT,'README.md'),'utf8').includes('progress/gates.svg'));assert.ok(!Object.values(files).join('').includes('never-publish-this'));assert.match(files['progress/gates.svg'],/0\/5 milestones accepted/);assert.match(files['progress/gates.svg'],/0\/51 delivery issues done/);
  assert.throws(()=>buildProgressFiles(snapshot(),plan,template,'b'.repeat(40)),/source revision/);
  const unavailable={schemaVersion:1,repository:REPOSITORY,sourceCommit:SHA,generatedAt:'2026-11-06T00:00:00Z',available:false};assert.match(buildProgressFiles(unavailable,plan,template,SHA)['progress/gates.svg'],/unavailable/);
});
test('publication refuses extra files and symlinks rather than exposing a repository or stale artifacts',()=>{
  fs.mkdirSync(path.join(ROOT,'artifacts'),{recursive:true});const output=fs.mkdtempSync(path.join(ROOT,'artifacts/graph-test-'));
  try{const files=buildProgressFiles(snapshot(),plan,template,SHA);const receipt=writeProgressFiles(files,output);assert.equal(Object.keys(receipt.files).length,5);fs.writeFileSync(path.join(output,'unreviewed.txt'),'do not publish');assert.throws(()=>writeProgressFiles(files,output),/Unexpected publication file/);fs.unlinkSync(path.join(output,'unreviewed.txt'));
    if(process.platform!=='win32'){fs.unlinkSync(path.join(output,'progress/index.html'));fs.symlinkSync(path.join(ROOT,'README.md'),path.join(output,'progress/index.html'));assert.throws(()=>writeProgressFiles(files,output),/symlink/);}
    assert.throws(()=>writeProgressFiles({...files,'research/private.json':'private'},output),/Unexpected publication paths/);
  }finally{fs.rmSync(output,{recursive:true,force:true});}
});
test('HTTP verification checks the exact published bytes and tolerates a bounded CDN delay',async()=>{
  fs.mkdirSync(path.join(ROOT,'artifacts'),{recursive:true});const output=fs.mkdtempSync(path.join(ROOT,'artifacts/graph-http-test-'));try{
    const files=buildProgressFiles(snapshot(),plan,template,SHA),receipt=writeProgressFiles(files,output);let reads=0,pauses=0;const fetchImpl=async url=>({url,ok:++reads>4,text:async()=>files[url.slice(SITE_URL.length)]});
    const result=await verifyProgressSite(receipt,{fetchImpl,pause:async()=>{pauses++;},attempts:2});assert.equal(result.files,4);assert.equal(pauses,1);
    await assert.rejects(verifyProgressSite(receipt,{attempts:1,fetchImpl:async url=>({url,ok:true,text:async()=>'wrong version'})}),/did not match/);
  }finally{fs.rmSync(output,{recursive:true,force:true});}
});
test('Pages preflight allows a new or owned report and preserves unrelated or custom-domain sites',async()=>{
  const token='owned-fixture-token';const fresh=await checkProgressPages({token,fetchImpl:async(url,options)=>{assert.equal(url,`https://api.github.com/repos/${REPOSITORY}/pages`);assert.equal(options.headers.Authorization,'Bearer '+token);return {status:404};}});assert.equal(fresh.existing,false);
  const state={cname:null,html_url:SITE_URL,build_type:'workflow'};const mock=(config,body)=>async url=>url.includes('api.github.com')?{status:200,ok:true,json:async()=>config}:{status:200,ok:true,url:SITE_URL,text:async()=>body};
  assert.equal((await checkProgressPages({token,fetchImpl:mock(state,SITE_MARKER)})).published,true);
  await assert.rejects(checkProgressPages({token,fetchImpl:mock(state,'An unrelated public site')}),/unrelated/);
  await assert.rejects(checkProgressPages({token,fetchImpl:mock({...state,cname:'other.example'},SITE_MARKER)}),/custom-domain/);
  await assert.rejects(checkProgressPages({token,fetchImpl:mock({...state,build_type:'legacy'},SITE_MARKER)}),/branch-based/);
});
