import assert from 'node:assert/strict';
import test from 'node:test';
import { readFileSync } from 'node:fs';
import vm from 'node:vm';
import { parseLocalConsoleSettings } from '../runtime/browser/local-title-proof.mjs';
import { parseDebugServiceStart, selectDebugServiceWindow, readDebugServiceWindow } from '../runtime/browser/debug-service-window.mjs';
import { validateProofReport } from '../runtime/browser/proof-report.mjs';

const window = (completed=9) => ({kind:'bounded-debug-service-window',version:1,
  requestedStart:8,capacity:8,limit:16,prefixCompleted:Math.min(8,completed),
  capturedCompleted:Math.max(0,completed-8),totalCompleted:completed});
const report = () => ({kind:'thread-startup-boundary',phase:'original-initializer-bringup',verified:false,
  stop:{status:10,target:0xfd000100,ordinal:1},trace:[{kind:'enter',address:0x1fdc30,stack:0x3c10c40}],
  originalStartup:{totalInitializers:83,completedInitializers:83,distinctOriginalFunctions:276,completedKernelOperations:403},
  consoleSettings:{debugServiceStart:8},experiments:['debugServiceWindow:8'],debugServiceWindow:window(),
  debugServiceState:{completed:9,service:1,bytes:24,status:0}});

test('debug windows require an explicit finite profile and complete module capability',()=>{
  const settings=value=>new TextEncoder().encode(JSON.stringify({kind:'local-console-settings',synthetic:true,miscFlags:0,...value})).buffer;
  assert.equal(Object.hasOwn(parseLocalConsoleSettings(settings({})),'debugServiceStart'),false);
  for(const start of [0,8]) assert.equal(parseLocalConsoleSettings(settings({debugServiceStart:start})).debugServiceStart,start);
  for(const bad of [null,'8',1,16,-1,0.5])assert.throws(()=>parseDebugServiceStart(bad),/debug service start/);
  const api={ff_probe_debug_service_profile:start=>start===8?0:1,ff_probe_debug_service_window_word:()=>0,
    ff_probe_debug_service_word:()=>0};
  selectDebugServiceWindow(api,8);
  assert.throws(()=>selectDebugServiceWindow(api,0),/refused/);
  assert.throws(()=>selectDebugServiceWindow({ff_probe_debug_service_profile:()=>0},8),/Missing/);
  const words=[8,8,16,8,1,9];
  assert.deepEqual(readDebugServiceWindow({ff_probe_debug_service_window_word:i=>words[i]},8,9),window());
  for(const bad of [-1,0x100000000,NaN])assert.throws(()=>readDebugServiceWindow({ff_probe_debug_service_window_word:()=>bad},8,9),/Malformed/);
});

test('reports bind debug budget, counts, selection, experiment and guarded refusal',()=>{
  const valid=report(); assert.equal(validateProofReport(valid),valid);
  const early=report();early.debugServiceState={completed:0,service:0,bytes:0,status:0};early.debugServiceWindow=window(0);
  assert.equal(validateProofReport(early),early);
  const full=report();full.stop={status:9,target:0xc00000bb,ordinal:0};
  full.debugServiceState={completed:16,service:1,bytes:0,status:0xc00000bb};full.debugServiceWindow=window(16);
  assert.equal(validateProofReport(full),full);
  for(const mutate of [
    r=>delete r.consoleSettings.debugServiceStart, r=>delete r.debugServiceWindow,
    r=>delete r.debugServiceState, r=>r.experiments=[], r=>r.experiments.push('debugServiceWindow:8'),
    r=>r.experiments=['debugServiceWindow:0'],r=>r.consoleSettings.debugServiceStart=0,
    r=>r.debugServiceWindow.limit=24,r=>r.debugServiceWindow.capacity=16,
    r=>r.debugServiceWindow.capturedCompleted=0,r=>r.debugServiceWindow.prefixCompleted=0,
    r=>r.debugServiceWindow.totalCompleted=8,r=>r.debugServiceWindow.payload='private',
    r=>r.debugServiceState.completed=17,r=>r.debugServiceState.bytes=1025,
    r=>r.debugServiceState.status=0xc00000bb,
  ]) {const bad=report();mutate(bad);assert.throws(()=>validateProofReport(bad));}
  const legacy=report();delete legacy.consoleSettings;delete legacy.debugServiceWindow;legacy.experiments=[];
  assert.throws(()=>validateProofReport(legacy),/Malformed bounded debug-service/);
  legacy.debugServiceState.completed=8;assert.equal(validateProofReport(legacy),legacy);
});

test('worker refuses missing debug window exports before executing or applying settings',async()=>{
  const source=readFileSync(new URL('../runtime/browser/thread-proof-worker.mjs',import.meta.url),'utf8').replace(/^import .*?;\s*$/gm,'');
  for(const api of [{}, {ff_probe_debug_service_profile:()=>0},
    {ff_probe_debug_service_profile:()=>0,ff_probe_debug_service_window_word:()=>0}]) {
    const messages=[];
    const context=vm.createContext({self:{postMessage:m=>messages.push(m)},ArrayBuffer,
      parseLocalConsoleSettings:()=>({debugServiceStart:8}),createProofClock:()=>({}),
      loadLocalTitleProof:async()=>({api:{ff_probe_abi:()=>25,...api}}),selectDebugServiceWindow});
    vm.runInContext(source,context);await context.self.onmessage({data:{settingsBytes:new ArrayBuffer(1)}});
    assert.equal(messages[0].kind,'error');assert.match(messages[0].message,/Missing selected debug service window/);
  }
});
