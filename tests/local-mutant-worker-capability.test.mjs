import assert from 'node:assert/strict';
import { readFileSync } from 'node:fs';
import { fileURLToPath } from 'node:url';
import test from 'node:test';
import vm from 'node:vm';
import { createProofClock } from '../runtime/browser/browser-clock.mjs';
import { parseLocalConsoleSettings } from '../runtime/browser/local-title-proof.mjs';

const source=readFileSync(fileURLToPath(new URL('../runtime/browser/thread-proof-worker.mjs',import.meta.url)),'utf8').replace(/^import .*?;\s*$/gm,'');
const settingsBytes=new TextEncoder().encode(JSON.stringify({kind:'local-console-settings',synthetic:true,miscFlags:0,
  events:{kind:'synthetic-win32-named-events',synthetic:true,capacity:4},
  mutants:{kind:'synthetic-win32-mutants',synthetic:true,capacity:4}})).buffer;

async function attempt(abi, missing=[]) {
  const calls=[],messages=[];
  const api={ff_probe_abi:()=>abi,ff_probe_run:()=>calls.push('run'),
    ff_probe_event_profile:()=>calls.push('event-profile')??0,ff_probe_event_word:()=>0,
    ff_probe_local_mutant_profile:()=>calls.push('local-mutant-profile')??0,ff_probe_local_mutant_word:()=>0};
  for(const name of missing)delete api[name];
  const self={postMessage:m=>messages.push(m)};
  const ctx=vm.createContext({self,console,ArrayBuffer,Uint8Array,Uint32Array,DataView,TextDecoder,TextEncoder,WebAssembly,
    performance:{now:()=>0},BUILD_REFERENCE:{},parseLocalConsoleSettings,createProofClock,
    loadLocalTitleProof:async()=>({api,memory:new ArrayBuffer(64*1024*1024),range:()=>0})});
  vm.runInContext(source,ctx,{filename:'thread-proof-worker.mjs'});
  await ctx.self.onmessage({data:{settingsBytes}});
  return {calls,messages};
}

test('new mutant kind requires ABI 5+ and rejects ABI 2 before running despite fake exports',async()=>{
  const r=await attempt(2);
  assert.deepEqual(r.calls,[]);assert.equal(r.messages.length,1);assert.equal(r.messages[0].kind,'error');
  assert.match(r.messages[0].message,/Win32 mutant and namespace capability/);
});

test('new mutant kind requires both namespace and local mutant exports before startup',async()=>{
  for(const missing of [['ff_probe_local_mutant_profile'],['ff_probe_local_mutant_word'],['ff_probe_event_profile'],['ff_probe_event_word']]) {
    const r=await attempt(25,missing);
    assert.deepEqual(r.calls,[]);assert.equal(r.messages.length,1);assert.equal(r.messages[0].kind,'error');
    assert.match(r.messages[0].message,/Win32 mutant and namespace capability/);
  }
});
