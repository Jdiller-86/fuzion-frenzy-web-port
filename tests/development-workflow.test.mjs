import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import {createHash} from 'node:crypto';
import {spawnSync} from 'node:child_process';
import {ROOT, loadState, sourceCommit, validateState, stateBlock, replaceStateBlock, syncDocuments, DOCUMENTS} from '../tools/development-state.mjs';
import {REQUIRED, REPLAY_REQUIRED, baselineTemplate, validateBaseline, inspectBaseline} from '../tools/development-preflight.mjs';
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

function replayBaseline(t) {
  const f=baseline(t), run=`browser-${f.state.checkpoint.profile}-run-02`;
  // Owned fixture provenance must exist even in CI's depth-one checkout.
  // Real checkpoint state and its historical provenance requirement stay intact.
  f.state.checkpoint.sourceCommit=sourceCommit();
  f.manifest.checkpointSourceCommit=f.state.checkpoint.sourceCommit;
  const toolchain=fs.readFileSync(path.join(ROOT,'tools/original-execution-toolchain.json'));
  fs.writeFileSync(path.join(f.root,'toolchainManifest'),toolchain);
  Object.assign(f.manifest.files.find(x=>x.id==='toolchainManifest'),{bytes:toolchain.length,sha256:digest(toolchain)});
  const roles={controlReport:'control',inertReport:'inert',previousReport:'default',experimentReport:'selected'};
  const audit={moduleSha256:f.state.checkpoint.moduleSha256,harness:{run},artifacts:Object.fromEntries(
    Object.entries(roles).map(([id,role])=>[`${run}/browser-${role}.json`,f.manifest.files.find(x=>x.id===id).sha256]))};
  const bytes=Buffer.from(JSON.stringify(audit));fs.writeFileSync(path.join(f.root,'audit'),bytes);
  Object.assign(f.manifest.files.find(x=>x.id==='audit'),{bytes:bytes.length,sha256:digest(bytes)});
  f.state.checkpoint.auditSha256=digest(bytes);
  const manifest={schemaVersion:2,purpose:'replay',checkpointSourceCommit:f.state.checkpoint.sourceCommit,
    profile:f.state.checkpoint.profile,files:REPLAY_REQUIRED.map(id=>({...f.manifest.files.find(x=>x.id===id)}))};
  fs.writeFileSync(f.file,JSON.stringify(manifest));
  return {...f,analysisManifest:f.manifest,manifest,audit};
}

test('current pointers are synchronized; stale or duplicated pointers fail closed', t => {
  const root=temporary(t), state={...loadState(),status:'in-progress'};
  fs.mkdirSync(path.join(root,'profiles')); fs.mkdirSync(path.join(root,'docs'));
  fs.writeFileSync(path.join(root,'profiles/development-state.json'),JSON.stringify(state));
  for (const name of DOCUMENTS) fs.writeFileSync(path.join(root,name),'# Owned document\n\nExisting evidence stays intact.\n');
  assert.equal(syncDocuments(root).length,DOCUMENTS.length);
  assert.deepEqual(syncDocuments(root,{check:true}),[]);
  assert.match(fs.readFileSync(path.join(root,'docs/status.md'),'utf8'),/Existing evidence stays intact/);
  const file=path.join(root,'AGENTS.md');fs.writeFileSync(file,fs.readFileSync(file,'utf8').replace(state.nextTask.id,'SCOPE-999'));
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

test('explicit replay checks retained inputs without claiming a Ghidra project or new code facts', async t => {
  const f=replayBaseline(t);
  for(const id of ['analysisProject','seedIndex','preparedProfile'])fs.unlinkSync(path.join(f.root,id));
  assert.ok((await inspectBaseline(f.file,f.state,{purpose:'replay'})).every(x=>x.status==='verified'));
  const legacy=path.join(f.root,'analysis.json');fs.writeFileSync(legacy,JSON.stringify(f.analysisManifest));
  assert.equal((await inspectBaseline(legacy,f.state)).find(x=>x.id==='analysisProject').status,'missing');
  assert.throws(()=>validateBaseline(f.manifest,f.state),/schema/,'Replay purpose is never selected implicitly');
  assert.throws(()=>validateBaseline(f.analysisManifest,f.state,{purpose:'replay'}),/schema/);
  const template=baselineTemplate(f.state,{purpose:'replay'});
  assert.equal(template.schemaVersion,2);assert.equal(template.purpose,'replay');assert.equal(template.files.length,8);
  assert.throws(()=>validateBaseline(template,f.state,{purpose:'replay'}),/SHA-256|byte count/);
});

test('replay provenance refuses freshly chosen report and toolchain hashes', async t => {
  const f=replayBaseline(t);
  for(const id of ['controlReport','toolchainManifest']) {
    const bytes=Buffer.from('owned replacement with a newly calculated hash');
    fs.writeFileSync(path.join(f.root,id),bytes);
    Object.assign(f.manifest.files.find(x=>x.id===id),{bytes:bytes.length,sha256:digest(bytes)});
  }
  fs.writeFileSync(f.file,JSON.stringify(f.manifest));
  const result=await inspectBaseline(f.file,f.state,{purpose:'replay'});
  assert.equal(result.find(x=>x.id==='controlReport').status,'provenance-mismatch');
  assert.equal(result.find(x=>x.id==='toolchainManifest').status,'provenance-mismatch');
});

test('replay refuses unavailable checkpoint Git provenance', async t => {
  const f=replayBaseline(t);
  f.state.checkpoint.sourceCommit='f'.repeat(40);
  f.manifest.checkpointSourceCommit=f.state.checkpoint.sourceCommit;
  fs.writeFileSync(f.file,JSON.stringify(f.manifest));
  const result=await inspectBaseline(f.file,f.state,{purpose:'replay'});
  assert.equal(result.find(x=>x.id==='toolchainManifest').status,'provenance-unavailable');
  assert.ok(!result.every(x=>x.status==='verified'));
});

test('replay roles derive from the pinned audit; swapped roles and unverified audit fail closed', async t => {
  const f=replayBaseline(t), control=f.manifest.files.find(x=>x.id==='controlReport');
  Object.assign(control,{...f.manifest.files.find(x=>x.id==='experimentReport'),id:'controlReport'});
  fs.writeFileSync(f.file,JSON.stringify(f.manifest));
  assert.equal((await inspectBaseline(f.file,f.state,{purpose:'replay'})).find(x=>x.id==='controlReport').status,'provenance-mismatch');
  f.audit.harness.run='../../untrusted';const bytes=Buffer.from(JSON.stringify(f.audit));
  fs.writeFileSync(path.join(f.root,'audit'),bytes);
  const changed=await inspectBaseline(f.file,f.state,{purpose:'replay'});
  assert.notEqual(changed.find(x=>x.id==='audit').status,'verified');
  Object.assign(f.manifest.files.find(x=>x.id==='audit'),{bytes:bytes.length,sha256:digest(bytes)});
  assert.throws(()=>validateBaseline(f.manifest,f.state,{purpose:'replay'}),/audit identity/);
});

test('replay template and invalid purpose never execute private artifacts', () => {
  const command=path.join(ROOT,'tools/development-preflight.mjs');
  const template=spawnSync(process.execPath,[command,'--purpose','replay','--template'],{encoding:'utf8'});
  assert.equal(template.status,0);const parsed=JSON.parse(template.stdout);
  assert.equal(parsed.purpose,'replay');assert.equal(parsed.files.length,8);
  const invalid=spawnSync(process.execPath,[command,'--purpose','skip-checks'],{encoding:'utf8'});
  assert.equal(invalid.status,1);assert.match(invalid.stderr,/Unknown private preflight purpose/);
});

test('invalid private manifest diagnostics never print JSON fragments or local paths', t => {
  const root=temporary(t), file=path.join(root,'PRIVATE_PATH.json');
  fs.writeFileSync(file,'NOT_JSON_WITH_PRIVATE_CONTENT');
  const result=spawnSync(process.execPath,[path.join(ROOT,'tools/development-preflight.mjs'),'--manifest',file],{encoding:'utf8'});
  assert.equal(result.status,1);assert.equal(result.stderr.trim(),'Invalid private baseline JSON.');
  assert.ok(!result.stderr.includes(root) && !result.stderr.includes('PRIVATE_CONTENT'));
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

test('timing writer refuses a dangling log symlink instead of creating its target', t => {
  if(process.platform==='win32')return t.skip('Windows symlink creation requires account privileges');
  const root=temporary(t), elsewhere=temporary(t), target=path.join(elsewhere,'must-not-create');
  fs.mkdirSync(path.join(root,'artifacts/development'),{recursive:true});
  fs.symlinkSync(target,path.join(root,'artifacts/development/iterations.jsonl'));
  assert.throws(()=>appendRecord(root,sample()),/regular file/);assert.ok(!fs.existsSync(target));
});
