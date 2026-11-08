import assert from 'node:assert/strict';
import test from 'node:test';
import { readFileSync } from 'node:fs';
import { fileURLToPath } from 'node:url';
import vm from 'node:vm';
import { readTraceCpuWord, selectTraceCapture, traceCaptureExportsAvailable } from '../runtime/browser/trace-capture.mjs';

const api=()=>({ff_probe_trace_cpu:()=>0,...Object.fromEntries(['profile','word','routine','digest_word'].map(name=>[`ff_probe_trace_capture_${name}`,
  name==='profile'?start=>start===8192?0:0:()=>0]))});
test('selected trace capture requires every capability and checks selection refusal',()=>{
  assert.equal(traceCaptureExportsAvailable(api()),true);
  selectTraceCapture(api(),8192);
  const missing=api(); delete missing.ff_probe_trace_capture_routine;
  assert.equal(traceCaptureExportsAvailable(missing),false);
  assert.throws(()=>selectTraceCapture(missing,0),/Missing selected/);
  assert.throws(()=>selectTraceCapture({...api(),ff_probe_trace_capture_profile:()=>1},0),/refused/);
  const noCpu=api(); delete noCpu.ff_probe_trace_cpu;
  assert.throws(()=>selectTraceCapture(noCpu,0),/Missing selected/);
  assert.equal(readTraceCpuWord({ff_probe_trace_cpu:()=>-1},0,0),0xffffffff);
  assert.throws(()=>readTraceCpuWord({ff_probe_trace_cpu:()=>0x100000000},0,0),/non-DWORD/);
});

test('thread worker rejects old or incomplete selected ABI before settings setters',async()=>{
  const source=readFileSync(fileURLToPath(new URL('../runtime/browser/thread-proof-worker.mjs',import.meta.url)),'utf8').replace(/^import .*?;\s*$/gm,'');
  for(const [abi,missing] of [[24,[]],[25,['digest_word']]]) {
    const calls=[],messages=[];
    const api={ff_probe_abi:()=>abi,ff_probe_trace_capacity:()=>8192,ff_probe_trace_cpu:()=>0,
      ff_probe_trace_capture_profile:start=>(calls.push(['select',start]),0),
      ff_probe_trace_capture_word:()=>0,ff_probe_trace_capture_routine:()=>0,
      ff_probe_trace_capture_digest_word:()=>0};
    for(const name of missing) delete api[`ff_probe_trace_capture_${name}`];
    const context=vm.createContext({self:{postMessage:m=>messages.push(m)},console,ArrayBuffer,Uint8Array,Uint32Array,
      DataView,TextDecoder,TextEncoder,WebAssembly,performance:{now:()=>0},BUILD_REFERENCE:{},
      parseLocalConsoleSettings:()=>({traceCaptureStart:8192}),createProofClock:()=>({}),
      proofTraceCapacity:()=>8192,traceCaptureExportsAvailable:api=>typeof api.ff_probe_trace_cpu==='function' && ['profile','word','routine','digest_word'].every(name=>typeof api[`ff_probe_trace_capture_${name}`]==='function'),
      selectTraceCapture:(api,start)=>calls.push(['select',start]),
      loadLocalTitleProof:async()=>({api,memory:new ArrayBuffer(64*1024*1024)})});
    vm.runInContext(source,context); await context.self.onmessage({data:{settingsBytes:new ArrayBuffer(1)}});
    assert.equal(messages[0].kind,'error'); assert.match(messages[0].message,/ABI 25/); assert.deepEqual(calls,[]);
  }
});
