import test from 'node:test';
import assert from 'node:assert/strict';
import {validateProofReport} from '../runtime/browser/proof-report.mjs';
const p={kind:'synthetic-win32-named-events',synthetic:true,capacity:4};
const state=(objects=1,handles=1,status=0)=>({...p,objectCount:objects,handleCount:handles,status,
 objects:Array.from({length:objects},(_,i)=>({object:i+1,nameBytes:i?7:11,type:1,signaled:false})),
 handles:Array.from({length:handles},(_,i)=>({handle:0x90004+i*4,object:i?1:1}))});
const out=(handle=0x90004,object=1,openedExisting=false,objectCount=1,handleCount=1,nameBytes=11)=>({backend:p.kind,handle,object,openedExisting,objectCount,handleCount,
 eaxBefore:0x123,espBefore:0x03c00000,eaxAfter:openedExisting?0x40000000:0,espAfter:0x03c00014,root:0xfffffffc,attributes:0x80,nameBytes,nameDescriptor:0x1000,nameBuffer:0x2000});
const call=(output,status=0)=>({ordinal:189,status,arguments:[0x3000,0x4000,1,0],...(output?{output}:{})});
const suffixCapture=(capturedCount,totalCount,completedOperations,prefixCompletedOperations)=>({kind:'kernel-capture-window',version:1,requestedStart:512,capturedStart:512,capturedCount,totalCount,
 completedOperations,prefixCompletedOperations,complete:false,sha256:'a'.repeat(64),prefixSha256:'b'.repeat(64)});
function report(calls=[call(out())],s=state(),settings=p,stop={status:10,target:0xfd000100,ordinal:1}) {return {kind:'thread-startup-boundary',phase:'original-initializer-bringup',verified:false,
 stop,trace:[{kind:'enter',address:1,stack:0x03c00000}],originalStartup:{totalInitializers:2,completedInitializers:2,distinctOriginalFunctions:2,completedKernelOperations:1},
 consoleSettings:settings===null?{}:{events:settings},eventState:settings?s:undefined,calls};}
test('validates fresh and existing named event success, including informational status',()=>{
 const fresh=report();assert.equal(validateProofReport(fresh),fresh);
 const s=state(1,2,0x40000000);const existing=call(out(0x90008,1,true,1,2));const r=report([call(out()),existing],s);assert.equal(validateProofReport(r),r);
});
test('validates sequential fresh objects and a suffix window with its omitted prefix',()=>{
 const stateTwo=state(2,2,0);stateTwo.objects[1].nameBytes=7;stateTwo.handles[1].object=2;
 const freshTwo=call(out(0x90008,2,false,2,2,7));const full=report([call(out()),freshTwo],stateTwo);assert.equal(validateProofReport(full),full);
 const suffix=report([freshTwo],stateTwo);suffix.kernelCapture={kind:'kernel-capture-window',version:1,requestedStart:512,capturedStart:512,capturedCount:1,totalCount:513,
   completedOperations:511,prefixCompletedOperations:510,complete:false,sha256:'a'.repeat(64),prefixSha256:'b'.repeat(64)};
 suffix.consoleSettings.kernelCaptureStart=512;
 suffix.originalStartup.completedKernelOperations=511;
 suffix.kernelCapacity=512;
 assert.equal(validateProofReport(suffix),suffix);
 assert.throws(()=>validateProofReport({...suffix,calls:[call(out(0x90008,2,false,1,2,7))]}),/order|counters|state/i);
 assert.throws(()=>validateProofReport({...suffix,eventState:{...stateTwo,status:0x40000000}}),/existing-object|final status/);
 const futureOpen=report([call(out(0x90008,2,true,1,1,7))],stateTwo);futureOpen.kernelCapture=suffix.kernelCapture;
 assert.throws(()=>validateProofReport(futureOpen),/order|counters|state/i);
});
test('suffix windows may retain state whose earlier creations are outside the capture',()=>{
 const partial=report([{ordinal:999,status:1,arguments:[]}],state(1,1),p,{status:10,target:0xfd000100,ordinal:999});
 partial.consoleSettings.kernelCaptureStart=512;partial.kernelCapture=suffixCapture(1,513,510,510);partial.kernelCapacity=512;partial.originalStartup.completedKernelOperations=510;
 assert.equal(validateProofReport(partial),partial);
 const four=state(4,4,0xc000009a);four.objects.forEach(o=>o.nameBytes=11);four.handles.forEach((h,i)=>h.object=i+1);
 const refusal=report([call(undefined,1)],four,p,{status:4,target:0xc000009a,ordinal:189});
 refusal.consoleSettings.kernelCaptureStart=512;refusal.kernelCapture=suffixCapture(1,513,510,510);refusal.kernelCapacity=512;refusal.originalStartup.completedKernelOperations=510;
 assert.equal(validateProofReport(refusal),refusal);
 const complete=report([{ordinal:999,status:1,arguments:[]}],state(1,1),p,{status:10,target:0xfd000100,ordinal:999});
 assert.throws(()=>validateProofReport(complete),/successful captured calls/);
});
test('rejects success and serialized state tampering, and binds refusal stop',()=>{
 for(const bad of [out(0x90008),{...out(),root:0xfffffffd},{...out(),attributes:0},{...out(),espAfter:1},{...out(),nameBuffer:0x4000000},
   {...out(),nameDescriptor:'4096'},{...out(),nameBuffer:0.5},{...out(),nameBytes:'11'}, {...out(),openedExisting:true}])
   assert.throws(()=>validateProofReport(report([call(bad)])));
 const reuse=report([call(out()),call(out(0x90008,1,true,1,2))],state(1,2,0x40000000));
 assert.throws(()=>validateProofReport({...reuse,calls:[reuse.calls[0],call(out(0x90004,1,true,1,2))]}));
 const valid=report();for(const s of [{...state(),objectCount:2},{...state(),objects:[{...state().objects[0],type:2}]},{...state(),status:0xc000009a}])
   assert.throws(()=>validateProofReport({...valid,eventState:s}));
 const refusal=report([call(out()),call(undefined,1)],state(1,1,0xc000000d),p,{status:9,target:0xc000000d,ordinal:189});assert.equal(validateProofReport(refusal),refusal);
 assert.throws(()=>validateProofReport({...refusal,stop:{...refusal.stop,status:4}}));
 assert.throws(()=>validateProofReport(report([call(out())],{...state(),objectCount:2},p)));
});
test('unselected original ordinal-189 refusal remains compatible; success is rejected',()=>{
 const legacy=report([{ordinal:189,status:1,arguments:[0x1000]}],undefined,null,{status:1,target:0xfe000014,ordinal:189,service:{ordinal:189,name:null,modeled:false}});
 assert.equal(validateProofReport(legacy),legacy);
 assert.throws(()=>validateProofReport({...legacy,calls:[call(out())]}),/explicit synthetic model/);
});
