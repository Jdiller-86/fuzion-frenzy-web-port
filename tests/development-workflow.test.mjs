import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import {createHash} from 'node:crypto';
import {loadState, validateState, stateBlock, replaceStateBlock, syncDocuments, DOCUMENTS} from '../tools/development-state.mjs';
import {REQUIRED, baselineTemplate, validateBaseline, inspectBaseline} from '../tools/development-preflight.mjs';
import {appendRecord, summarize, runScripts, validateRecord} from '../tools/development-task.mjs';

function temporary(t) {
  const root = fs.mkdtempSync(path.join(os.tmpdir(), 'ff-workflow-'));
  t.after(() => fs.rmSync(root, {recursive:true, force:true}));
  return root;
}
const digest = bytes => createHash('sha256').update(bytes).digest('hex');
function baseline(t) {
  const root = temporary(t), state = structuredClone(loadState());
  const files = REQUIRED.map((id,n) => {
    const bytes = Buffer.from(`owned fixture ${id} ${n}`);
    fs.writeFileSync(path.join(root,id),bytes);
    return {id,path:id,bytes:bytes.length,sha256:digest(bytes)};
  });
  for (const [id,field] of [['xbe','inputSha256'],['module','moduleSha256'],['audit','auditSha256']]) state.checkpoint[field]=files.find(f=>f.id===id).sha256;
  state.checkpoint.inputBytes=files.find(f=>f.id==='xbe').bytes;
  const manifest = {schemaVersion:1,profile:state.checkpoint.profile,checkpointSourceCommit:state.checkpoint.sourceCommit,files};
  const file=path.join(root,'baseline.json'); fs.writeFileSync(file,JSON.stringify(manifest));
  return {root,state,manifest,file};
}

test('current pointers are synchronized; stale or duplicated pointers fail closed', t => {
  const root=temporary(t), state=loadState();
  fs.mkdirSync(path.join(root,'profiles')); fs.mkdirSync(path.join(root,'docs'));
  fs.writeFileSync(path.join(root,'profiles/development-state.json'),JSON.stringify(state));
  for (const name of DOCUMENTS) fs.writeFileSync(path.join(root,name),'# Owned document\n\nExisting evidence stays intact.\n');
  assert.equal(syncDocuments(root).length,DOCUMENTS.length);
  assert.deepEqual(syncDocuments(root,{check:true}),[]);
  assert.match(fs.readFileSync(path.join(root,'docs/status.md'),'utf8'),/Existing evidence stays intact/);
  const file=path.join(root,'AGENTS.md');fs.writeFileSync(file,fs.readFileSync(file,'utf8').replace('SCOPE-37','SCOPE-999'));
  assert.throws(()=>syncDocuments(root,{check:true}),/Development state drift/);
  assert.throws(()=>replaceStateBlock('# T\n<!-- development-state:start -->\n',stateBlock(state,'AGENTS.md')),/Malformed/);
  assert.throws(()=>replaceStateBlock('# T\n<!-- development-state:end --><!-- development-state:start -->',stateBlock(state,'AGENTS.md')),/Reversed/);
  syncDocuments(root);
  fs.appendFileSync(file,'\nImplementation is paused pending another task.\n');
  assert.throws(()=>syncDocuments(root,{check:true}),/Stale entry guidance/);
});

test('state rejects over-budget counts and invalid evidence identities', () => {
  for (const mutate of [s=>s.admission.functions.used=s.admission.functions.limit+1,s=>s.checkpoint.moduleSha256='unknown',s=>s.nextTask.id='../private']) {
    const state=structuredClone(loadState());mutate(state);assert.throws(()=>validateState(state));
  }
});

test('private baseline verification distinguishes missing, same-size corrupt and matching files', async t => {
  const f=baseline(t);let result=await inspectBaseline(f.file,f.state);
  assert.ok(result.every(row=>row.status==='verified'));
  assert.ok(result.every(row=>Object.keys(row).sort().join(',')==='id,status'),'Never include local paths or contents');
  fs.unlinkSync(path.join(f.root,'seedIndex'));
  const module=path.join(f.root,'module');fs.writeFileSync(module,Buffer.alloc(fs.statSync(module).size,42));
  result=await inspectBaseline(f.file,f.state);
  assert.equal(result.find(r=>r.id==='seedIndex').status,'missing');
  assert.equal(result.find(r=>r.id==='module').status,'digest-mismatch');
  assert.equal(result.find(r=>r.id==='xbe').status,'verified');
});

test('a self-consistent but wrong module or stale profile cannot pass preflight', t => {
  const f=baseline(t), wrong=structuredClone(f.manifest);
  wrong.files.find(r=>r.id==='module').sha256='0'.repeat(64);
  assert.throws(()=>validateBaseline(wrong,f.state),/identity differs/);
  wrong.files=f.manifest.files;wrong.profile++;
  assert.throws(()=>validateBaseline(wrong,f.state),/profile differs/);
  const duplicate=structuredClone(f.manifest);duplicate.files[1]=duplicate.files[0];
  assert.throws(()=>validateBaseline(duplicate,f.state),/duplicate/);
  assert.throws(()=>validateBaseline(baselineTemplate(f.state),f.state),/SHA-256|byte count/,'An unfilled template is never a passing baseline');
});

function sample(overrides={}) {
  return {schemaVersion:1,kind:'reported-local-work',scope:'SCOPE-37',phase:'browser',outcome:'passed',failureClass:'none',durationMs:1000,
    sourceCommit:'1'.repeat(40),workingTreeDirty:true,timestamp:'2026-11-09T12:00:00.000Z',acceptance:false,...overrides};
}

test('local timing projection excludes arbitrary private data and separates reported work from checks', t => {
  const root=temporary(t), saved=appendRecord(root,sample({privateReport:'MUST_NOT_BE_LOGGED',path:'/private/input'}));
  assert.ok(!JSON.stringify(saved).includes('MUST_NOT_BE_LOGGED'));
  assert.ok(!fs.readFileSync(path.join(root,'artifacts/development/iterations.jsonl'),'utf8').includes('/private'));
  const summary=summarize([saved,sample({kind:'automated-source-checks',phase:'source-checks',outcome:'failed',failureClass:'source-check',durationMs:2000})]);
  assert.equal(summary.groups.length,2);assert.equal(summary.samples,2);
  for (const bad of [{scope:'../../input'},{durationMs:-1},{acceptance:true},{outcome:'passed',failureClass:'runtime'}]) assert.throws(()=>validateRecord(sample(bad)));
});

test('task runner uses active Node and npm, rejects arbitrary scripts, and stops on first failure', t => {
  const root=temporary(t), npm=path.join(root,'npm-cli.js'), previous=process.env.npm_execpath;
  fs.writeFileSync(npm,'// Owned fake entry point; never executed.\n');
  fs.writeFileSync(path.join(root,'package.json'),JSON.stringify({scripts:{'test:runtime':'owned','test:formats':'owned'}}));
  process.env.npm_execpath=npm;
  t.after(()=>{if(previous===undefined)delete process.env.npm_execpath;else process.env.npm_execpath=previous;});
  const calls=[];
  const code=runScripts(root,['test:runtime','test:formats'],(command,args,options)=>{calls.push({command,args,options});return {status:7};});
  assert.equal(code,7);assert.equal(calls.length,1);
  assert.equal(calls[0].command,process.execPath);assert.deepEqual(calls[0].args,[npm,'run','test:runtime']);assert.equal(calls[0].options.shell,false);
  assert.throws(()=>runScripts(root,['arbitrary'],()=>assert.fail('must not execute')),/Unknown check script/);
});

test('timing writer refuses a symlinked artifact directory', t => {
  if(process.platform==='win32')return t.skip('Windows symlink creation requires account privileges');
  const root=temporary(t), elsewhere=temporary(t);fs.symlinkSync(elsewhere,path.join(root,'artifacts'));
  assert.throws(()=>appendRecord(root,sample()),/symlink/);
  assert.deepEqual(fs.readdirSync(elsewhere),[]);
});
