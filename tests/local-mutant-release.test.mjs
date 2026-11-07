import assert from 'node:assert/strict';
import test from 'node:test';
import {parseLocalConsoleSettings} from '../runtime/browser/local-title-proof.mjs';
import {validateProofReport} from '../runtime/browser/proof-report.mjs';
import {parseMutantReleaseProfile,requireMutantReleaseCapability,selectMutantRelease,
  readMutantSynchronizationStates,validateMutantReleaseReport} from '../runtime/browser/local-mutant-release.mjs';

const wp={kind:'synthetic-unowned-local-mutant-acquire',synthetic:true,capacity:4};
const rp={kind:'synthetic-owned-local-mutant-release',synthetic:true,capacity:4};
const mp={kind:'synthetic-win32-mutants',synthetic:true,capacity:4};
const ep={kind:'synthetic-win32-named-events',synthetic:true,capacity:4};
const waitFields=['handle','threadId','object','waitMode','alertable','timeout','ownerBefore','ownerAfter','eaxBefore','espBefore','eaxAfter','espAfter'];
const releaseFields=['handle','threadId','object','previousCount','ownerBefore','ownerAfter','eaxBefore','espBefore','eaxAfter','espAfter'];
const acquire=()=>({handle:0x80004,threadId:1,object:1,waitMode:1,alertable:0,timeout:0,ownerBefore:0,ownerAfter:1,eaxBefore:7,espBefore:0x3c00000,eaxAfter:0,espAfter:0x3c00014});
const release=()=>({handle:0x80004,threadId:1,object:1,previousCount:0,ownerBefore:1,ownerAfter:0,eaxBefore:0,espBefore:0x3c00014,eaxAfter:0,espAfter:0x3c00020});
function report(){
 const a=acquire(),r=release();
 return {kind:'thread-startup-boundary',phase:'original-initializer-bringup',verified:false,kernelCapacity:512,
  stop:{status:10,target:0xfd000100,ordinal:1},trace:[{kind:'enter',address:0x1fdc30,stack:0x3c00000}],
  originalStartup:{totalInitializers:83,completedInitializers:83,distinctOriginalFunctions:276,completedKernelOperations:403},
  consoleSettings:{mutants:mp,events:ep,mutantWait:wp,mutantRelease:rp},experiments:['localMutantAcquire','localMutantRelease'],
  localMutantState:{...mp,objectCount:1,handleCount:1,status:0,objects:[{object:1,nameBytes:0,named:false,owned:false}],handles:[{handle:0x80004,object:1}]},
  eventState:{...ep,objectCount:0,handleCount:0,status:0,objects:[],handles:[]},
  mutantWaitState:{...wp,attempts:1,completed:1,status:0,owners:[{object:1,threadId:0}],last:a},
  mutantReleaseState:{...rp,attempts:1,completed:1,status:0,owners:[{object:1,threadId:0}],last:r},
  threads:[{id:1,state:3,stackLow:0x3b00000,stackTop:0x3c10000}],
  calls:[{ordinal:192,status:0,arguments:[0x1000,0,0],output:{backend:mp.kind,handle:0x80004,object:1,openedExisting:false,objectCount:1,handleCount:1,nameBytes:0,eaxBefore:0,espBefore:0x3c00000,eaxAfter:0,espAfter:0x3c00010,root:0,attributes:0,nameDescriptor:0,nameBuffer:0}},
   {ordinal:234,status:0,arguments:[a.handle,1,0,0],output:{backend:wp.kind,...a}},
   {ordinal:221,status:0,arguments:[r.handle,0],output:{backend:rp.kind,...r}}]};
}
function cycleViaAlias(){
 const x=report(),a=acquire(),r1=release(),a2={...acquire(),handle:0x80008,eaxBefore:0,espBefore:0x3c00020,espAfter:0x3c00034},r2={...release(),handle:0x80004,espBefore:0x3c00034,espAfter:0x3c00040};
 x.localMutantState={...mp,objectCount:1,handleCount:2,status:0x40000000,objects:[{object:1,nameBytes:1,named:true,owned:false}],handles:[{handle:0x80004,object:1},{handle:0x80008,object:1}]};
 x.calls=[{ordinal:192,status:0,arguments:[0x1000,0x1004,0],output:{backend:mp.kind,handle:0x80004,object:1,openedExisting:false,objectCount:1,handleCount:1,nameBytes:1,eaxBefore:0,espBefore:0x3c00000,eaxAfter:0,espAfter:0x3c00010,root:0xfffffffc,attributes:0x80,nameDescriptor:0x1010,nameBuffer:0x1020}},
  {ordinal:192,status:0,arguments:[0x1000,0x1004,0],output:{backend:mp.kind,handle:0x80008,object:1,openedExisting:true,objectCount:1,handleCount:2,nameBytes:1,eaxBefore:0,espBefore:0x3c00010,eaxAfter:0x40000000,espAfter:0x3c00020,root:0xfffffffc,attributes:0x80,nameDescriptor:0x1010,nameBuffer:0x1020}},
  {ordinal:234,status:0,arguments:[a.handle,1,0,0],output:{backend:wp.kind,...a}},
  {ordinal:221,status:0,arguments:[r1.handle,0],output:{backend:rp.kind,...r1}},
  {ordinal:234,status:0,arguments:[a2.handle,1,0,0],output:{backend:wp.kind,...a2}},
  {ordinal:221,status:0,arguments:[r2.handle,0],output:{backend:rp.kind,...r2}}];
 x.mutantWaitState={...wp,attempts:2,completed:2,status:0,owners:[{object:1,threadId:0}],last:a2};
 x.mutantReleaseState={...rp,attempts:2,completed:2,status:0,owners:[{object:1,threadId:0}],last:r2};
 return x;
}

test('release selection requires the explicit profile and acquisition capability first',()=>{
 assert.equal(parseMutantReleaseProfile(undefined),undefined);assert.deepEqual(parseMutantReleaseProfile(rp),rp);
 for(const bad of [null,{}, {...rp,capacity:5},{...rp,synthetic:false},{...rp,extra:true}])assert.throws(()=>parseMutantReleaseProfile(bad));
 const api=Object.fromEntries(['local_mutant_wait_profile','local_mutant_wait_word','local_mutant_profile','local_mutant_word','local_mutant_release_profile','local_mutant_release_word'].map(n=>[`ff_probe_${n}`,()=>0]));
 api.ff_probe_local_mutant_wait_word=(_i,f)=>f===0?1:0;
 requireMutantReleaseCapability(api);assert.throws(()=>selectMutantRelease({...api,ff_probe_local_mutant_wait_word:()=>0}),/acquisition/);
 assert.throws(()=>selectMutantRelease({...api,ff_probe_local_mutant_release_profile:()=>-1}),/refused/);
 for(const key of Object.keys(api)){const partial={...api};delete partial[key];assert.throws(()=>requireMutantReleaseCapability(partial),/Missing/);}
 const enc=v=>new TextEncoder().encode(JSON.stringify({kind:'local-console-settings',synthetic:true,miscFlags:0,...v})).buffer;
 assert.throws(()=>parseLocalConsoleSettings(enc({mutants:mp,events:ep,mutantRelease:rp})),/requires/);
 assert.deepEqual(parseLocalConsoleSettings(enc({mutants:mp,events:ep,mutantWait:wp,mutantRelease:rp})).mutantRelease,rp);
});

test('joint reader normalizes signed DWORDs and reads both selected raw states',()=>{
 const r=report(),a=acquire(),z=release();
 const wait=[1,1,1,0,4,...waitFields.map(k=>a[k])],rel=[1,1,1,0,4,...releaseFields.map(k=>z[k])];
 const api={ff_probe_local_mutant_wait_profile:()=>0,ff_probe_local_mutant_wait_word:(i,f)=>i===0xffffffff?wait[f]:[1,0][f],
  ff_probe_local_mutant_profile:()=>0,ff_probe_local_mutant_word:(i,f)=>i===0xffffffff?0:[1,0][f],
  ff_probe_local_mutant_release_profile:()=>0,ff_probe_local_mutant_release_word:(i,f)=>i===0xffffffff?rel[f]:[1,0][f]};
 const got=readMutantSynchronizationStates(api,wp,rp,{...r.localMutantState,objects:[{object:1,nameBytes:0,named:false,owned:false}]});
 assert.equal(got.mutantWaitState.last.espBefore,0x3c00000);assert.equal(got.mutantReleaseState.last.espAfter,0x3c00020);
 assert.throws(()=>readMutantSynchronizationStates({...api,ff_probe_local_mutant_release_word:()=>0x100000000},wp,rp,r.localMutantState));
});

test('report validator accepts acquire-release and alias reacquisition histories',()=>{
 const x=report();assert.equal(validateMutantReleaseReport(x,rp,x.localMutantState),undefined);assert.equal(validateProofReport(x),x);
 const alias=cycleViaAlias();assert.equal(validateMutantReleaseReport(alias,rp,alias.localMutantState),undefined);
});

test('history rejects invalid ownership, call frames, status and creation order',()=>{
 const changes=[
  x=>x.calls.splice(3,0,{...x.calls[2]}),
  x=>x.calls.at(-1).output.ownerBefore=0,
  x=>x.calls.at(-1).output.threadId=2,
  x=>x.calls.at(-1).output.espAfter+=4,
  x=>x.calls.at(-1).arguments=[0x80004,1],
  x=>x.mutantReleaseState.status=0xc0000001,
  x=>x.kernelCapacity=0,
  x=>{const c=x.calls.shift();x.calls.push(c);},
  x=>{const c=x.calls.shift();x.calls.splice(1,0,c);}
 ];
 for(const [i,change] of changes.entries()){const x=report();change(x);assert.throws(()=>validateMutantReleaseReport(x,rp,x.localMutantState),`mutation ${i}`);}
 const future=cycleViaAlias();const created=future.calls.splice(1,1)[0];future.calls.splice(4,0,created);
 assert.throws(()=>validateMutantReleaseReport(future,rp,future.localMutantState));
});

test('final refusal is correlated without assigning an undocumented status meaning',()=>{
 const x=report(),out={...acquire(),waitMode:0,ownerAfter:0,eaxAfter:7,espAfter:0x3c00000};
 x.localMutantState.objects[0].owned=false;x.calls[1].status=1;x.calls[1].arguments=[out.handle,0,0,0];x.calls[1].output={backend:wp.kind,...out};
 x.calls.pop();
 x.mutantWaitState={...wp,attempts:1,completed:0,status:0xc000000d,owners:[{object:1,threadId:0}],last:out};
 x.stop={status:9,target:0xc000000d,ordinal:234};x.mutantReleaseState={...rp,attempts:0,completed:0,status:0,owners:[{object:1,threadId:0}],last:null};
 x.experiments=['localMutantAcquire','localMutantRelease'];
 assert.equal(validateMutantReleaseReport(x,rp,x.localMutantState),undefined);
 x.stop.target=0xc0000008;assert.throws(()=>validateMutantReleaseReport(x,rp,x.localMutantState));
});

test('unselected release preserves legacy reports but rejects release evidence',()=>{
 const legacy={phase:'original-initializer-bringup',experiments:[]};
 assert.equal(validateMutantReleaseReport(legacy,undefined,{}),undefined);
 for(const mutate of [x=>x.mutantReleaseState={},x=>x.experiments=['localMutantRelease'],x=>x.calls=[{ordinal:221,status:0}],x=>x.calls=[{ordinal:221,status:1,output:{}}]]){
  const x={...legacy,experiments:[],calls:[]};mutate(x);assert.throws(()=>validateMutantReleaseReport(x,undefined,{}));
 }
});
