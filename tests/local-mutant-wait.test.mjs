import assert from 'node:assert/strict';
import test from 'node:test';
import {readFileSync} from 'node:fs';
import vm from 'node:vm';
import {parseLocalConsoleSettings} from '../runtime/browser/local-title-proof.mjs';
import {validateProofReport} from '../runtime/browser/proof-report.mjs';
import {validateLocalMutantState} from '../runtime/browser/local-mutants.mjs';
import {parseMutantWaitProfile,requireMutantWaitCapability,selectMutantWait,readMutantWaitState,validateMutantWaitReport} from '../runtime/browser/local-mutant-wait.mjs';
import {describeKernelService} from '../runtime/browser/kernel-service.mjs';

const wp=()=>({kind:'synthetic-unowned-local-mutant-acquire',synthetic:true,capacity:4});
const mp={kind:'synthetic-win32-mutants',synthetic:true,capacity:4};
const ep={kind:'synthetic-win32-named-events',synthetic:true,capacity:4};
const fields=['handle','threadId','object','waitMode','alertable','timeout','ownerBefore','ownerAfter','eaxBefore','espBefore','eaxAfter','espAfter'];
const last=()=>({handle:0x80004,threadId:1,object:1,waitMode:1,alertable:0,timeout:0,
  ownerBefore:0,ownerAfter:1,eaxBefore:7,espBefore:0x3c00000,eaxAfter:0,espAfter:0x3c00014});
function report() {
  const l=last();
  return {kind:'thread-startup-boundary',phase:'original-initializer-bringup',verified:false,
    stop:{status:10,target:0xfd000100,ordinal:1},trace:[{kind:'enter',address:0x1fdc30,stack:0x3c00000}],
    originalStartup:{totalInitializers:83,completedInitializers:83,distinctOriginalFunctions:276,completedKernelOperations:403},
    consoleSettings:{mutants:mp,events:ep,mutantWait:wp()},experiments:['localMutantAcquire'],
    localMutantState:{...mp,objectCount:1,handleCount:1,status:0,
      objects:[{object:1,nameBytes:0,named:false,owned:true}],handles:[{handle:0x80004,object:1}]},
    eventState:{...ep,objectCount:0,handleCount:0,status:0,objects:[],handles:[]},
    mutantWaitState:{...wp(),attempts:1,completed:1,status:0,owners:[{object:1,threadId:1}],last:l},
    threads:[{id:1,state:3,stackLow:0x3b00000,stackTop:0x3c10000}],
    calls:[{ordinal:192,status:0,arguments:[0x11000,0,0],output:{backend:mp.kind,handle:0x80004,object:1,
      openedExisting:false,objectCount:1,handleCount:1,eaxBefore:0,espBefore:0x3c00000,eaxAfter:0,espAfter:0x3c00010,
      root:0,attributes:0,nameBytes:0,nameDescriptor:0,nameBuffer:0}},
    {ordinal:234,status:0,arguments:[0x80004,1,0,0],output:{backend:wp().kind,...l}}]};
}

test('unowned acquisition is an explicit strict profile with complete pre-run capabilities',()=>{
  assert.equal(parseMutantWaitProfile(undefined),undefined);assert.deepEqual(parseMutantWaitProfile(wp()),wp());
  for(const bad of [null,{}, {...wp(),capacity:5},{...wp(),synthetic:false},{...wp(),recursive:true}])assert.throws(()=>parseMutantWaitProfile(bad));
  const enc=v=>new TextEncoder().encode(JSON.stringify({kind:'local-console-settings',synthetic:true,miscFlags:0,...v})).buffer;
  assert.throws(()=>parseLocalConsoleSettings(enc({mutantWait:wp()})),/requires/);
  assert.deepEqual(parseLocalConsoleSettings(enc({mutants:mp,events:ep,mutantWait:wp()})).mutantWait,wp());
  assert.equal(Object.hasOwn(parseLocalConsoleSettings(enc({})),'mutantWait'),false);
  const api=Object.fromEntries(['local_mutant_wait_profile','local_mutant_wait_word','local_mutant_profile','local_mutant_word'].map(n=>[`ff_probe_${n}`,()=>0]));
  selectMutantWait(api);assert.throws(()=>selectMutantWait({...api,ff_probe_local_mutant_wait_profile:()=>1}),/refused/);
  for(const name of Object.keys(api)){const partial={...api};delete partial[name];assert.throws(()=>requireMutantWaitCapability(partial),/Missing/);}
});

test('ownership evidence requires exact thread, handle, frame and model agreement',()=>{
  const good=report();assert.equal(validateProofReport(good),good);
  assert.throws(()=>validateLocalMutantState(good.localMutantState,mp),/object row/);
  const mutations=[r=>delete r.consoleSettings.mutantWait,r=>delete r.mutantWaitState,
    r=>r.experiments=[],r=>r.experiments.push('localMutantAcquire'),r=>r.mutantWaitState.last.ownerBefore=1,
    r=>r.mutantWaitState.last.ownerAfter=0,r=>r.mutantWaitState.owners[0].threadId=2,
    r=>r.localMutantState.objects[0].owned=false,r=>r.mutantWaitState.completed=0,
    r=>r.mutantWaitState.attempts=2,r=>r.calls.at(-1).arguments=[0x80004,1,0],
    r=>r.calls.at(-1).output.timeout=4,r=>r.calls.at(-1).output.espAfter+=4,
    r=>r.calls.at(-1).output.backend='native',r=>r.mutantWaitState.last.payload='none',
    r=>r.threads[0].state=1,r=>r.threads[0].stackTop=r.calls.at(-1).output.espBefore,
    r=>delete r.threads[0].stackLow,r=>r.calls.at(-1).output.object=2];
  for(const change of mutations){const bad=report();change(bad);assert.throws(()=>validateProofReport(bad));}
});

test('unreached and refused acquisitions retain canonical zero or unchanged state',()=>{
  const early=report();early.localMutantState.objects[0].owned=false;early.calls.pop();
  early.mutantWaitState={...wp(),attempts:0,completed:0,status:0,owners:[{object:1,threadId:0}],last:null};
  assert.equal(validateProofReport(early),early);
  const denied=report(),l={...last(),waitMode:0,ownerAfter:0,eaxAfter:7,espAfter:0x3c00000};
  denied.localMutantState.objects[0].owned=false;
  denied.stop={status:9,target:0xc00000bb,ordinal:234};
  denied.calls.at(-1).status=1;denied.calls.at(-1).arguments=[0x80004,0,0,0];denied.calls.at(-1).output={backend:wp().kind,...l};
  denied.mutantWaitState={...wp(),attempts:1,completed:0,status:0xc00000bb,owners:[{object:1,threadId:0}],last:l};
  assert.equal(validateProofReport(denied),denied);
  denied.calls.at(-1).output.espAfter+=20;assert.throws(()=>validateProofReport(denied));
});

test('alias acquisitions cannot fabricate an unwitnessed owned object',()=>{
  const r=report(),alias={...last(),handle:0x80008};
  r.localMutantState.objectCount=2;r.localMutantState.handleCount=3;
  r.localMutantState.objects=[{object:1,nameBytes:4,named:true,owned:true},{object:2,nameBytes:0,named:false,owned:true}];
  r.localMutantState.handles=[{handle:0x80004,object:1},{handle:0x80008,object:1},{handle:0x8000c,object:2}];
  r.calls.push({ordinal:234,status:0,arguments:[0x80008,1,0,0],output:{backend:wp().kind,...alias}});
  r.mutantWaitState={...wp(),attempts:2,completed:2,status:0,owners:[{object:1,threadId:1},{object:2,threadId:1}],last:alias};
  assert.throws(()=>validateMutantWaitReport(r,wp(),r.localMutantState),/acquisition evidence/);
});

test('Wasm wait reader validates raw DWORDs, unused fields and owner rows',()=>{
  const r=report(),l=last(),header=[1,1,1,0,4,...fields.map(k=>l[k])];
  const api={ff_probe_local_mutant_profile:()=>0,ff_probe_local_mutant_word:()=>0,ff_probe_local_mutant_wait_profile:()=>0,
    ff_probe_local_mutant_wait_word:(i,f)=>i===0xffffffff?header[f]:[1,1][f]};
  assert.deepEqual(readMutantWaitState(api,wp(),r.localMutantState),r.mutantWaitState);
  assert.throws(()=>readMutantWaitState({...api,ff_probe_local_mutant_wait_word:()=>0x100000000},wp(),r.localMutantState));
});

test('known extended-wait ABI and legacy unidentified stops both retain their scope',()=>{
  const r=report();delete r.consoleSettings.mutantWait;delete r.mutantWaitState;r.experiments=[];
  r.localMutantState.objects[0].owned=false;r.calls.at(-1).status=1;delete r.calls.at(-1).output;
  r.stop={status:1,target:0xfe000020,ordinal:234,service:describeKernelService(234)};
  assert.equal(validateProofReport(r),r);
  r.stop.service={ordinal:234,name:null,modeled:false};r.calls.at(-1).arguments=[0x80004];assert.equal(validateProofReport(r),r);
  r.stop.service={...describeKernelService(234),argumentBytes:12};assert.throws(()=>validateProofReport(r),/four-argument/);
});

test('worker rejects a selected model with incomplete module exports before settings or original execution',async()=>{
  const source=readFileSync(new URL('../runtime/browser/thread-proof-worker.mjs',import.meta.url),'utf8').replace(/^import .*?;\s*$/gm,'');
  const messages=[];
  const context=vm.createContext({self:{postMessage:m=>messages.push(m)},ArrayBuffer,
    parseLocalConsoleSettings:()=>({mutants:mp,mutantWait:wp()}),createProofClock:()=>({}),
    loadLocalTitleProof:async()=>({api:{ff_probe_abi:()=>25}}),requireMutantWaitCapability});
  vm.runInContext(source,context);await context.self.onmessage({data:{settingsBytes:new ArrayBuffer(1)}});
  assert.equal(messages[0].kind,'error');assert.match(messages[0].message,/Missing explicit local mutant acquisition/);
});
