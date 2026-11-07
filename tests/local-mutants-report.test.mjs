import test from 'node:test';
import assert from 'node:assert/strict';
import { validateProofReport } from '../runtime/browser/proof-report.mjs';

const mp={kind:'synthetic-win32-mutants',synthetic:true,capacity:4};
const ep={kind:'synthetic-win32-named-events',synthetic:true,capacity:4};
const emptyEvents={...ep,objectCount:0,handleCount:0,status:0,objects:[],handles:[]};
const H=n=>0x80000+4*n;
const obj=(n,nameBytes=0,named=false)=>({object:n,nameBytes,named,owned:false});
const handle=(n,object)=>({handle:H(n),object});
const out=(n,object,options={})=>({backend:mp.kind,handle:H(n),object,openedExisting:false,objectCount:object,handleCount:n,
  eaxBefore:0xdeadbeef,espBefore:0x03c00000,eaxAfter:0,espAfter:0x03c00010,
  root:0,attributes:0,nameBytes:0,nameDescriptor:0,nameBuffer:0,...options});
const call=(n,object,options={})=>({ordinal:192,status:0,arguments:[0x11000,0,0],output:out(n,object,options)});
const report=(mutantState,calls,stop={status:10,target:0xfd000100,ordinal:1})=>({kind:'thread-startup-boundary',phase:'original-initializer-bringup',verified:false,
  stop,trace:[{kind:'enter',address:0x1fdc30,stack:0x03c00000}],originalStartup:{totalInitializers:83,completedInitializers:83,
    distinctOriginalFunctions:276,completedKernelOperations:403},consoleSettings:{mutants:mp,events:ep},localMutantState:mutantState,eventState:emptyEvents,calls});

test('validates anonymous then named mutant creation in the selected Win32 namespace',()=>{
  const s={...mp,objectCount:2,handleCount:2,status:0,objects:[obj(1),obj(2,5,true)],handles:[handle(1,1),handle(2,2)]};
  const anon=call(1,1),named=call(2,2,{root:0xfffffffc,attributes:0x80,nameBytes:5,nameDescriptor:0x12000,nameBuffer:0x13000});
  named.arguments=[0x11004,0x12004,0];
  assert.equal(validateProofReport(report(s,[anon,named])).localMutantState.handleCount,2);
  for(const bad of [
    {...named,arguments:[0x11004,0x12004,1]},
    {...named,output:{...named.output,backend:'native-mutant'}},
    {...named,output:{...named.output,root:0xfffffffd}},
    {...named,output:{...named.output,nameBuffer:0x4000000}},
    {...named,output:{...named.output,nameDescriptor:'46088'}},
    {...named,output:{...named.output,nameBuffer:0.5}},
    {...named,output:{...named.output,espAfter:0x03c00014}},
    {...named,output:{...named.output,handleCount:1}},
  ]) assert.throws(()=>validateProofReport(report(s,[anon,bad])),/mutant/i);
});

test('duplicate named open uses informational status and retained canonical object identity',()=>{
  const first=call(1,1,{root:0xfffffffc,attributes:0x80,nameBytes:5,nameDescriptor:0x12000,nameBuffer:0x13000});
  first.arguments=[0x11000,0x12004,0];
  const second=call(2,1,{openedExisting:true,objectCount:1,root:0xfffffffc,attributes:0x80,nameBytes:5,
    nameDescriptor:0x14000,nameBuffer:0x15000,eaxAfter:0x40000000});
  second.arguments=[0x11004,0x14004,0];
  const s={...mp,objectCount:1,handleCount:2,status:0x40000000,objects:[obj(1,5,true)],handles:[handle(1,1),handle(2,1)]};
  assert.equal(validateProofReport(report(s,[first,second])).localMutantState.status,0x40000000);
  assert.throws(()=>validateProofReport(report({...s,status:0},[first,second])),/fresh mutant status/i);
});

test('terminal NTSTATUS refusals bind exact stop and retain only successful state',()=>{
  for(const status of [0xc000000d,0xc00000bb,0xc0000005,0xc000009a]) {
    const count=status===0xc000009a?4:0, objects=Array.from({length:count},(_,i)=>obj(i+1)),handles=Array.from({length:count},(_,i)=>handle(i+1,i+1));
    const successes=Array.from({length:count},(_,i)=>call(i+1,i+1));
    const failed={ordinal:192,status:1,arguments:[0x11000,0x12000,0]};
    const r=report({...mp,objectCount:count,handleCount:count,status,objects,handles},[...successes,failed],
      {status:status===0xc000009a?4:9,target:status,ordinal:192});
    assert.equal(validateProofReport(r).localMutantState.status,status);
    assert.throws(()=>validateProofReport({...r,stop:{...r.stop,target:0xc0000001}}),/Mutant error state/i);
  }
});

test('validates a suffix window whose captured named mutant follows an omitted anonymous creation',()=>{
  const first=call(2,2,{root:0xfffffffc,attributes:0x80,nameBytes:5,nameDescriptor:0x12000,nameBuffer:0x13000});
  first.arguments=[0x11004,0x12004,0];
  const state={...mp,objectCount:2,handleCount:2,status:0,objects:[obj(1),obj(2,5,true)],handles:[handle(1,1),handle(2,2)]};
  const r=report(state,[first]);r.consoleSettings.kernelCaptureStart=512;r.kernelCapacity=512;
  r.kernelCapture={kind:'kernel-capture-window',version:1,requestedStart:512,capturedStart:512,capturedCount:1,totalCount:513,
    completedOperations:511,prefixCompletedOperations:510,complete:false,sha256:'a'.repeat(64),prefixSha256:'b'.repeat(64)};
  r.originalStartup.completedKernelOperations=511;
  assert.equal(validateProofReport(r),r);
  assert.throws(()=>validateProofReport({...r,calls:[{...first,output:{...first.output,objectCount:1}}]}),/counter|order|state/i);
});

test('records an event-name type collision as a terminal mutant refusal without allocating a mutant',()=>{
  const mutantState={...mp,objectCount:0,handleCount:0,status:0xc0000024,objects:[],handles:[]};
  const eventProfile=ep,eventState={...ep,objectCount:1,handleCount:1,status:0,
    objects:[{object:1,nameBytes:5,type:1,signaled:false}],handles:[{handle:0x90004,object:1}]};
  const failed={ordinal:192,status:1,arguments:[0x11000,0x12000,0]};
  const r=report(mutantState,[failed],{status:9,target:0xc0000024,ordinal:192});
  r.eventState=eventState;r.consoleSettings.events=eventProfile;
  r.consoleSettings.kernelCaptureStart=512;r.kernelCapacity=512;
  r.kernelCapture={kind:'kernel-capture-window',version:1,requestedStart:512,capturedStart:512,capturedCount:1,totalCount:513,
    completedOperations:510,prefixCompletedOperations:510,complete:false,sha256:'a'.repeat(64),prefixSha256:'b'.repeat(64)};
  r.originalStartup.completedKernelOperations=510;
  assert.equal(validateProofReport(r).localMutantState.status,0xc0000024);
  for(const args of [[0x11000,0x12000],[0x11000,0x12000,0,0],[0x11000,0x12000,'0']])
    assert.throws(()=>validateProofReport({...r,calls:[{...failed,arguments:args}]}),/mutant/i);
  assert.throws(()=>validateProofReport({...r,stop:{...r.stop,target:0xc000000d}}),/Mutant error state/i);
});

test('records a mutant-name collision as an event refusal while preserving named local state',()=>{
  const mutantState={...mp,objectCount:1,handleCount:1,status:0,objects:[obj(1,5,true)],handles:[handle(1,1)]};
  const eventState={...emptyEvents,status:0xc0000024};
  const failed={ordinal:189,status:1,arguments:[0x11000,0x12000,1,0]};
  const r=report(mutantState,[failed],{status:9,target:0xc0000024,ordinal:189});
  r.eventState=eventState;
  r.consoleSettings.kernelCaptureStart=512;r.kernelCapacity=512;
  r.kernelCapture={kind:'kernel-capture-window',version:1,requestedStart:512,capturedStart:512,capturedCount:1,totalCount:513,
    completedOperations:510,prefixCompletedOperations:510,complete:false,sha256:'a'.repeat(64),prefixSha256:'b'.repeat(64)};
  r.originalStartup.completedKernelOperations=510;
  assert.equal(validateProofReport(r).eventState.status,0xc0000024);
  const infoState={...mutantState,handleCount:2,status:0x40000000,handles:[handle(1,1),handle(2,1)]};
  assert.equal(validateProofReport({...r,localMutantState:infoState}).localMutantState.status,0x40000000);
  const noNamedMutant={...mutantState,objects:[obj(1,0,false)]};
  assert.throws(()=>validateProofReport({...r,localMutantState:noNamedMutant}),/unchanged named local mutant namespace/i);
  const standalone={...r,consoleSettings:{events:ep},eventState:{...emptyEvents,status:0xc0000024},calls:[failed]};
  delete standalone.localMutantState;
  assert.throws(()=>validateProofReport(standalone),/Named event type mismatch requires/i);
});

test('rejects new mutant kind without namespace profile and keeps legacy serialization path',()=>{
  const empty={...mp,objectCount:0,handleCount:0,status:0,objects:[],handles:[]};
  const r=report(empty,[]); delete r.consoleSettings.events; delete r.eventState;
  assert.throws(()=>validateProofReport(r),/named-event namespace/i);
});

test('new and legacy mutant state fields are mutually exclusive and profile-bound',()=>{
  const empty={...mp,objectCount:0,handleCount:0,status:0,objects:[],handles:[]};
  const both=report(empty,[]);both.mutantState={...empty};
  assert.throws(()=>validateProofReport(both),/only its explicit profile and localMutantState/i);
  const legacyField=report(empty,[]);delete legacyField.localMutantState;legacyField.mutantState=empty;
  assert.throws(()=>validateProofReport(legacyField),/only its explicit profile and localMutantState/i);
  const unselected=report(empty,[]);delete unselected.consoleSettings.mutants;delete unselected.localMutantState;unselected.mutantState=empty;
  assert.throws(()=>validateProofReport(unselected),/explicit synthetic profile and state/i);
});
