import assert from 'node:assert/strict';
import {readFileSync} from 'node:fs';
import {fileURLToPath} from 'node:url';
import test from 'node:test';
import vm from 'node:vm';
import {parseLocalConsoleSettings} from '../runtime/browser/local-title-proof.mjs';
import {createProofClock} from '../runtime/browser/browser-clock.mjs';
const source=readFileSync(fileURLToPath(new URL('../runtime/browser/thread-proof-worker.mjs',import.meta.url)),'utf8').replace(/^import .*?;\s*$/gm,'');
const settingsBytes=new TextEncoder().encode(JSON.stringify({kind:'local-console-settings',synthetic:true,miscFlags:0,events:{kind:'synthetic-win32-named-events',synthetic:true,capacity:4}})).buffer;
async function run(abi,missing=[],bytes=settingsBytes) {const calls=[],messages=[];const api={ff_probe_abi:()=>abi,ff_probe_event_profile:x=>(calls.push(['profile',x]),0),ff_probe_event_word:()=>0,ff_probe_run:()=>calls.push(['run'])};for(const n of missing)delete api[`ff_probe_event_${n}`];let loads=0;
 const context=vm.createContext({self:{postMessage:m=>messages.push(m)},console,ArrayBuffer,Uint8Array,Uint32Array,DataView,TextDecoder,TextEncoder,WebAssembly,performance:{now:()=>0},BUILD_REFERENCE:{},parseLocalConsoleSettings,createProofClock,loadLocalTitleProof:async()=>{loads++;return {api,memory:new ArrayBuffer(64*1024*1024),range:()=>0};}});
 vm.runInContext(source,context);await context.self.onmessage({data:{settingsBytes:bytes}});return {calls,messages,loads};}
test('selected model fails closed for old ABI and missing ABI-25 exports before probes',async()=>{
 for(const [abi,missing] of [[2,[]],[2,['profile','word']],[25,['profile']],[25,['word']],[25,['profile','word']]]) {const r=await run(abi,missing);assert.deepEqual(r.calls,[]);assert.equal(r.messages[0].kind,'error');assert.match(r.messages[0].message,/named event capability/);}
});
test('event settings are parsed before title load',async()=>{const bad=new TextEncoder().encode(JSON.stringify({kind:'local-console-settings',synthetic:true,miscFlags:0,events:{kind:'bad'}})).buffer;assert.throws(()=>parseLocalConsoleSettings(bad),/event profile/);const r=await run(25,[],bad);assert.equal(r.loads,0);assert.deepEqual(r.calls,[]);assert.equal(r.messages[0].kind,'error');});
