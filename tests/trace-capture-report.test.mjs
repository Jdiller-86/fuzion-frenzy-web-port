import assert from 'node:assert/strict';
import test from 'node:test';
import { validateProofReport } from '../runtime/browser/proof-report.mjs';
import { readTraceCapture } from '../runtime/browser/trace-capture.mjs';

const row={kind:'enter',address:0x1000,stack:0x3000,cpu:{eax:1,fs:2,exceptionHead:3}};
const fixture=()=>({kind:'thread-startup-boundary',phase:'original-initializer-bringup',verified:false,
  consoleSettings:{traceCaptureStart:0},traceCapacity:8192,trace:[row],originalFunctionsReached:[0x1000],
  traceCapture:{kind:'original-trace-window',version:1,requestedStart:0,capturedStart:0,capturedCount:1,totalCount:1,
    totalEntries:1,prefixEntries:0,capacity:8192,limit:8192,routines:[0x1000],prefixRoutines:0,
    sha256:'a'.repeat(64),prefixSha256:null,complete:true},
  stop:{status:2,target:0x1000,ordinal:0},originalStartup:{totalInitializers:1,completedInitializers:1,
    distinctOriginalFunctions:1,completedKernelOperations:0}});
test('selected complete trace window validates with exact captured routines',()=>assert.equal(validateProofReport(fixture()).traceCapture.complete,true));
test('Wasm capture metadata adapter reads all bounded fields and digest words',()=>{
  const fields=[1,1,0,0,1,1,1,0,1,0,8192,8192];
  const api={ff_probe_trace_cpu:()=>0,ff_probe_trace_capture_profile:()=>0,ff_probe_trace_capture_word:i=>fields[i],ff_probe_trace_capture_routine:()=>0x1000,
    ff_probe_trace_capture_digest_word:()=>0};
  const capture=readTraceCapture(api,0,[row]);
  assert.equal(capture.sha256,'0'.repeat(64)); assert.deepEqual(capture.routines,[0x1000]);
});
test('trace window metadata cannot appear without selection or lie about captured routine sequence',()=>{
  const unselected=fixture(); delete unselected.consoleSettings; assert.throws(()=>validateProofReport(unselected),/match an explicitly selected/);
  const wrong=fixture(); wrong.traceCapture.routines=[0x2000]; assert.throws(()=>validateProofReport(wrong),/routine order/);
});
test('trace digest fields require primitive lowercase hex strings',()=>{
  for(const value of [['a'.repeat(64)],0,new String('a'.repeat(64))]) {
    const report=fixture(); report.traceCapture.sha256=value;
    assert.throws(()=>validateProofReport(report),/Malformed original trace window/);
    const suffix=fixture(); suffix.consoleSettings.traceCaptureStart=8192;
    suffix.traceCapture={...suffix.traceCapture,requestedStart:8192,capturedStart:8192,capturedCount:1,totalCount:8193,
      prefixEntries:0,prefixRoutines:0,prefixSha256:'b'.repeat(64),complete:false,limit:16384};
    suffix.originalStartup.completedInitializers=null; suffix.originalStartup.initializerCoverage='prefix-omitted';
    suffix.traceCapture.prefixSha256=value;
    assert.throws(()=>validateProofReport(suffix),/Malformed original trace window/);
  }
});
test('suffix-only windows cannot claim initializer completion or a next initializer',()=>{
  const report=fixture(); report.consoleSettings.traceCaptureStart=8192;
  report.traceCapture={...report.traceCapture,requestedStart:8192,capturedStart:8192,capturedCount:1,totalCount:8193,
    prefixEntries:2,prefixRoutines:1,routines:[0x2000,0x1000],prefixSha256:'b'.repeat(64),complete:false,totalEntries:3,limit:16384};
  report.originalStartup.completedInitializers=null; report.originalStartup.initializerCoverage='prefix-omitted';
  report.originalStartup.distinctOriginalFunctions=2;
  assert.equal(validateProofReport(report).originalStartup.completedInitializers,null);
  report.originalStartup.completedInitializers=1;
  assert.throws(()=>validateProofReport(report),/startup counters/);
});
test('trace totals cannot claim entries or unique routines absent from either window',()=>{
  const report=fixture(); report.traceCapture.totalEntries=2;
  assert.throws(()=>validateProofReport(report),/Malformed original trace window/);
  const suffix=fixture(); suffix.consoleSettings.traceCaptureStart=8192;
  suffix.traceCapture={...suffix.traceCapture,requestedStart:8192,capturedStart:8192,capturedCount:1,totalCount:8193,
    prefixEntries:2,prefixRoutines:0,routines:[0x1000],prefixSha256:'b'.repeat(64),complete:false,totalEntries:3,limit:16384};
  suffix.originalStartup.completedInitializers=null; suffix.originalStartup.initializerCoverage='prefix-omitted';
  assert.throws(()=>validateProofReport(suffix),/Malformed original trace window/);
});
