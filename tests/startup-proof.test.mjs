import test from 'node:test';
import assert from 'node:assert/strict';
import http from 'node:http';
import { inspectProofWasm, PROOF_MEMORY_PAGES, PROOF_WASM_LIMIT } from '../src/runtime/wasm-proof.mjs';
import { createStartupProofServer } from '../tools/serve-startup-proof.mjs';
import { loadLocalTitleProof, parseLocalConsoleSettings, parseLocalVolumeProfile, parseLocalDirectoryProfile, parseLocalFileProfile, proofTraceCapacity, proofKernelCapacity } from '../src/runtime/local-title-proof.mjs';
import { createProofClock } from '../src/runtime/browser-clock.mjs';
import { runOriginalCopyProof } from '../src/runtime/copy-proof.mjs';
import { inspectOpenFileRequest, inspectCreateFileRequest, inspectWriteFileRequest, compareWideMetadataFormat, inspectCountedAnsiString } from '../src/runtime/file-request.mjs';

const header = [0, 97, 115, 109, 1, 0, 0, 0];
const uint = value => {
  const bytes = [];
  do { const low = value % 128; value = Math.floor(value / 128); bytes.push(low | (value ? 128 : 0)); } while (value);
  return bytes;
};
const section = (id, bytes) => [id, ...uint(bytes.length), ...bytes];
const memory = (minimum = 1, maximum = PROOF_MEMORY_PAGES, flags = 1, count = 1) =>
  section(5, [count, flags, ...uint(minimum), ...uint(maximum)]);
const module = (...sections) => Uint8Array.from([...header, ...sections.flat()]).buffer;

function ownedOpenRequest() {
  const ram = new Uint8Array(4096), view = new DataView(ram.buffer);
  const name = '\\Device\\OwnedVolume';
  ram.set(new TextEncoder().encode(name), 192);
  view.setUint32(84, 128, true); view.setUint32(88, 0x40, true);
  view.setUint16(128, name.length, true); view.setUint16(130, name.length + 1, true);
  view.setUint32(132, 192, true);
  return { ram, view, name, args: [64, 0x100001, 80, 96, 3, 0x21] };
}

test('file request diagnostic preserves counted ANSI bytes, six arguments and guest memory without opening anything', () => {
  const { ram, view, name, args } = ownedOpenRequest(), accesses = [];
  const saved = ram.slice();
  const request = inspectOpenFileRequest(ram, args, (address, bytes, mode) => { accesses.push([address, bytes, mode]); return true; });
  assert.equal(request.name, name); assert.equal(request.rootDirectory, 0);
  assert.equal(request.attributes, 0x40); assert.equal(request.desiredAccess, args[1]);
  assert.equal(request.serviceImplemented, false);
  assert.deepEqual(accesses, [[64, 4, 2], [96, 8, 2], [80, 12, 1], [128, 8, 1], [192, name.length, 1]]);
  assert.deepEqual(ram, saved);
  ram[192] = 0xff;
  assert.equal(inspectOpenFileRequest(ram, args, () => true).name.charCodeAt(0), 255);
  view.setUint32(80, 123, true); view.setUint16(128, 0, true); view.setUint32(132, 0, true);
  assert.equal(inspectOpenFileRequest(ram, args, () => true).name, '');
  assert.equal(inspectOpenFileRequest(ram, args, () => true).rootDirectory, 123);
});

test('file request diagnostic rejects overflow, unaligned outputs, missing memory permissions and wrong argument counts', () => {
  const { ram, args } = ownedOpenRequest();
  for (const bad of [args.slice(0, 5), [...args, 0], [null, ...args.slice(1)],
    [-1, ...args.slice(1)], [1.5, ...args.slice(1)], [0x100000000, ...args.slice(1)]]) {
    assert.throws(() => inspectOpenFileRequest(ram, bad, () => true));
  }
  for (const [index, value] of [[0, 0], [0, 65], [0, 4096], [2, 0xfffffffc], [3, 4092]]) {
    const bad = [...args]; bad[index] = value;
    assert.throws(() => inspectOpenFileRequest(ram, bad, () => true), /not admitted/);
  }
  for (const denied of [1, 2]) assert.throws(() => inspectOpenFileRequest(ram, args, (_a, _n, mode) => mode !== denied), /not admitted/);
  assert.throws(() => inspectOpenFileRequest(ram, args));
});

test('file request diagnostic bounds string descriptors and lengths and rejects embedded NUL', () => {
  for (const mutate of [
    ({ view }) => view.setUint32(84, 4092, true),
    ({ view }) => view.setUint32(84, 129, true),
    ({ view }) => view.setUint16(130, 1, true),
    ({ view }) => { view.setUint16(128, 1025, true); view.setUint16(130, 1025, true); },
    ({ view }) => view.setUint32(132, 0xfffffff0, true),
    ({ view }) => view.setUint32(132, 4090, true),
    ({ ram }) => { ram[193] = 0; }
  ]) {
    const fixture = ownedOpenRequest(); mutate(fixture);
    assert.throws(() => inspectOpenFileRequest(fixture.ram, fixture.args, () => true));
  }
});

test('standalone counted namespace strings reject invalid descriptors and read permissions', () => {
  const { ram, name } = ownedOpenRequest();
  assert.equal(inspectCountedAnsiString(ram,128,() => true).name,name);
  for (const address of [0,-1,NaN,1.5,0x100000000,4092]) assert.throws(() => inspectCountedAnsiString(ram,address,() => true));
  assert.throws(() => inspectCountedAnsiString(ram,128,() => false), /not admitted/);
  assert.throws(() => inspectCountedAnsiString(null,128,() => true));
});

test('create-file diagnostic uses nine Xbox arguments and reads optional signed allocation without writes', () => {
  const {ram,view,name,args} = ownedOpenRequest();
  const create = [...args.slice(0,4),256,0x80,args[4],3,args[5]];
  view.setBigInt64(256,-0x100000001n,true);
  const saved=ram.slice();
  const result=inspectCreateFileRequest(ram,create,()=>true);
  assert.equal(result.name,name); assert.equal(result.allocationBytes,'-4294967297');
  assert.equal(result.fileAttributes,0x80); assert.equal(result.shareAccess,3);
  assert.equal(result.createDisposition,3); assert.equal(result.openOptions,args[5]);
  assert.equal(result.serviceImplemented,false); assert.deepEqual(ram,saved);
  create[4]=0;
  assert.equal(inspectCreateFileRequest(ram,create,()=>true).allocationBytes,undefined);
  for(const bad of [create.slice(0,8),[...create,0],[...create.slice(0,8),0x100000000]]) {
    assert.throws(()=>inspectCreateFileRequest(ram,bad,()=>true));
  }
  for(const pointer of [257,4092,0xfffffffc]) {
    create[4]=pointer;
    assert.throws(()=>inspectCreateFileRequest(ram,create,()=>true),/not admitted/);
  }
  create[4]=256;
  assert.throws(()=>inspectCreateFileRequest(ram,create,(address)=>address!==256),/not admitted/);
});

test('write-file diagnostic preserves the eight Xbox arguments, binary payload and signed offset without mutation', () => {
  const ram=new Uint8Array(new ArrayBuffer(544),32,512), view=new DataView(ram.buffer,ram.byteOffset,ram.byteLength);
  const payload=[0,0x80,0xff,65,10], args=[123,456,789,1000,64,192,payload.length,128], accesses=[];
  ram.set(payload,192); view.setBigInt64(128,-0x100000001n,true);
  const saved=ram.slice();
  const result=inspectWriteFileRequest(ram,args,(...access)=>{accesses.push(access);return true;});
  assert.deepEqual([result.fileHandle,result.eventHandle,result.apcRoutine,result.apcContext,result.ioStatusBlockPointer,result.buffer,result.bytes,result.byteOffsetPointer],args);
  assert.equal(result.byteOffset,'-4294967297'); assert.equal(result.serviceImplemented,false);
  assert.deepEqual(result.payload,payload); assert.deepEqual(ram,saved);
  assert.deepEqual(accesses,[[64,8,2],[192,5,1],[128,8,1]]);
  ram[192]=9; assert.deepEqual(result.payload,payload);
  args[7]=0; assert.equal(inspectWriteFileRequest(ram,args,()=>true).byteOffset,undefined);
});

test('write-file diagnostic rejects invalid arguments, excessive lengths, ranges and denied permissions', () => {
  const ram=new Uint8Array(512), args=[1,0,0,0,64,192,5,128];
  for(const bad of [args.slice(1),[...args,0],[...args.slice(0,7),-1],[...args.slice(0,7),1.5],[...args.slice(0,7),0x100000000]]) {
    assert.throws(()=>inspectWriteFileRequest(ram,bad,()=>true),/Invalid write-file/);
  }
  for(const [index,value] of [[4,0],[4,65],[4,508],[5,510],[5,0xfffffffe],[6,65537],[7,129],[7,508],[7,0xfffffffc]]) {
    const invalid=args.slice(); invalid[index]=value;
    assert.throws(()=>inspectWriteFileRequest(ram,invalid,()=>true));
  }
  for(const denied of [64,192,128]) assert.throws(()=>inspectWriteFileRequest(ram,args,address=>address!==denied),/not admitted/);
  assert.throws(()=>inspectWriteFileRequest(null,args,()=>true));
  assert.throws(()=>inspectWriteFileRequest(ram,args,null));
});

test('zero-length write diagnostics do not read the unused buffer or invent an explicit offset', () => {
  const ram=new Uint8Array(256), accesses=[];
  const result=inspectWriteFileRequest(ram,[1,0,0,0,64,0xffffffff,0,0],(...access)=>{accesses.push(access);return true;});
  assert.deepEqual(result.payload,[]); assert.equal(result.byteOffset,undefined);
  assert.deepEqual(accesses,[[64,8,2]]);
});

test('wide metadata comparison preserves BOM, surrogate code units and little-endian payload bytes', () => {
  const output='\ufeffOwned=\ud83d\ude80\r\n%', observation={format:'%lc%ls%lc%ls%ls%%',arguments:[{kind:'character',value:0xfeff},
    {kind:'string',value:'Owned'},{kind:'character',value:61},{kind:'string',value:'\ud83d\ude80'},{kind:'string',value:'\r\n'}],output,returnedUnits:output.length,maximumUnits:64};
  const result=compareWideMetadataFormat(observation);
  assert.equal(result.matches,true); assert.equal(result.expectedUnits,output.length);
  assert.equal(result.expectedBytes,output.length*2);
  assert.deepEqual(result.expectedPayload,[255,254,79,0,119,0,110,0,101,0,100,0,61,0,61,216,128,222,13,0,10,0,37,0]);
});

test('wide metadata comparison rejects mismatched output, byte counts, normalization and truncation', () => {
  const observation={format:'%lc:%ls',arguments:[{kind:'character',value:65},{kind:'string',value:'e\u0301'}],output:'A:e\u0301',returnedUnits:4,maximumUnits:8};
  assert.equal(compareWideMetadataFormat(observation).matches,true);
  for(const changed of [{output:'A:\u00e9'},{returnedUnits:8},{maximumUnits:4},{output:'A:x'}]) {
    assert.equal(compareWideMetadataFormat({...observation,...changed}).matches,false);
  }
});

test('wide metadata comparison bounds snapshots and rejects unsupported argument/conversion shapes', () => {
  const observation={format:'%lc%ls',arguments:[{kind:'character',value:65},{kind:'string',value:'owned'}],output:'Aowned',returnedUnits:6,maximumUnits:8};
  for(const changed of [{format:'%ls%lc'},{format:'%lc%d'},{format:'%lc%ls%'},{format:'%lc'},{format:'%lc%ls%ls'},
    {arguments:[{kind:'character',value:-1}]},{arguments:[{kind:'character',value:65536}]},{arguments:[{kind:'character',value:1.5}]},
    {returnedUnits:260},{maximumUnits:261},{maximumUnits:0},{arguments:[{kind:'string',value:'x'.repeat(128)}]},
    {output:'x'.repeat(260)},{format:'x'.repeat(128)},{arguments:[{kind:'string',value:'a\0b'}]},{arguments:[]},
    {arguments:Array(9).fill({kind:'string',value:'owned'})},{arguments:[null]},{arguments:[{kind:'unknown',value:65}]}]) {
    assert.throws(()=>compareWideMetadataFormat({...observation,...changed}));
  }
  assert.throws(()=>compareWideMetadataFormat());
});

test('title execution loader rejects non-browser-worker execution before compiling or instantiating a module', async () => {
  await assert.rejects(loadLocalTitleProof({}, 2), /requires a browser worker/);
});

test('trace admission retains legacy capacity and rejects arbitrary declared sizes', () => {
  assert.equal(proofTraceCapacity({}),64);
  for (const capacity of [64,128,1024]) assert.equal(proofTraceCapacity({ff_probe_trace_capacity:()=>capacity}),capacity);
  for (const capacity of [0,32,65,129,256,1023,1025,0x80000000,-1,NaN,Infinity,'128','1024',true]) {
    assert.throws(()=>proofTraceCapacity({ff_probe_trace_capacity:()=>capacity}),/bounded trace capacity/);
  }
});

test('kernel-record admission retains legacy capacity and rejects arbitrary sizes', () => {
  assert.equal(proofKernelCapacity({}),32);
  for(const capacity of [32,128]) assert.equal(proofKernelCapacity({ff_probe_kernel_capacity:()=>capacity}),capacity);
  for(const capacity of [0,31,33,64,129,-1,NaN,Infinity,'128',true]) {
    assert.throws(()=>proofKernelCapacity({ff_probe_kernel_capacity:()=>capacity}),/bounded kernel-record capacity/);
  }
});

test('copy diagnostic runner covers bounded overlap/edge cases and reports failures without hiding them', () => {
  let recovered = 0, calls = 0;
  const api = { ff_probe_copy_case(entry, destination, source, bytes) {
    ++calls;
    if (!entry || destination > 8192 || source > 8192 || bytes > 4096 || bytes > 8192 - destination || bytes > 8192 - source) return 0x80000000;
    return bytes === 48 && destination > source && destination - source < bytes ? 1 : 0;
  }, ff_probe_copy_recover() { ++recovered; } };
  const result = runOriginalCopyProof(api, 123);
  assert.equal(result.passed, false);
  assert.ok(result.failures.some(row => row.category === 'overlap-backward' && row.result === 1));
  assert.equal(recovered, calls);
  assert.equal(result.cases, calls);
  assert.equal(result.categories['invalid-range'], 6);
  assert.ok(result.categories['buffer-edge'] > 0 && result.categories['large-disjoint'] > 0 && result.categories['same-pointer'] > 0);
  api.ff_probe_copy_case = () => { throw new WebAssembly.RuntimeError('owned fixture trap'); };
  assert.throws(() => runOriginalCopyProof(api, 123), /owned fixture trap/);
  assert.equal(recovered, calls + 1);
  assert.throws(() => runOriginalCopyProof({}, 123), /Missing original-copy/);
});

test('local settings require explicit synthetic provenance and a bounded DWORD value', () => {
  const bytes = value => new TextEncoder().encode(JSON.stringify(value)).buffer;
  for (const miscFlags of [0, 1, 0xffffffff]) {
    assert.equal(parseLocalConsoleSettings(bytes({ kind: 'local-console-settings', synthetic: true, miscFlags })).miscFlags, miscFlags);
  }
  for (const value of [{ kind: 'EEPROM', synthetic: true, miscFlags: 0 },
    { kind: 'local-console-settings', miscFlags: 0 }, { kind: 'local-console-settings', synthetic: false, miscFlags: 0 },
    ...[-1, 0.5, 0x100000000, null].map(miscFlags => ({ kind: 'local-console-settings', synthetic: true, miscFlags }))]) {
    assert.throws(() => parseLocalConsoleSettings(bytes(value)));
  }
  assert.throws(() => parseLocalConsoleSettings(new ArrayBuffer(4097)));
});

test('volume settings require explicit synthetic capacity and geometry without unsafe integer conversion', () => {
  const valid = { kind:'local-volume-profile', synthetic:true, device:'harddisk0-partition1',
    totalAllocationUnits:'4294967299', availableAllocationUnits:'4294967297', sectorsPerAllocationUnit:32, bytesPerSector:512 };
  const bytes = volume => new TextEncoder().encode(JSON.stringify({kind:'local-console-settings',synthetic:true,miscFlags:0,volume})).buffer;
  assert.deepEqual(parseLocalVolumeProfile(bytes(valid)), valid);
  for (const edit of [{synthetic:false},{device:'host-path'},{kind:'disk-image'},{totalAllocationUnits:123},
    {totalAllocationUnits:'0'},{totalAllocationUnits:'01'},{totalAllocationUnits:'9223372036854775808'},
    {availableAllocationUnits:'4294967300'},{availableAllocationUnits:'-1'},{sectorsPerAllocationUnit:3},
    {sectorsPerAllocationUnit:256},{bytesPerSector:513},{totalAllocationUnits:'9223372036854775807'}]) {
    assert.throws(() => parseLocalVolumeProfile(bytes({...valid,...edit})), /explicit synthetic volume/);
  }
  assert.throws(() => parseLocalVolumeProfile(bytes(undefined)), /explicit synthetic volume/);
  assert.throws(() => parseLocalVolumeProfile(new ArrayBuffer(4097)));
});

test('directory state must be explicitly fresh and synthetic before original startup', () => {
  const volume={kind:'local-volume-profile',synthetic:true,device:'harddisk0-partition1',
    totalAllocationUnits:'65536',availableAllocationUnits:'65536',sectorsPerAllocationUnit:32,bytesPerSector:512};
  const bytes=edit=>new TextEncoder().encode(JSON.stringify({kind:'local-console-settings',synthetic:true,miscFlags:0,volume:{...volume,...edit}})).buffer;
  assert.deepEqual(parseLocalDirectoryProfile(bytes({directoryState:'fresh'})),{synthetic:true,state:'fresh',persistent:false});
  for(const state of [undefined,null,false,'original','existing','',0]) {
    assert.throws(()=>parseLocalDirectoryProfile(bytes({directoryState:state})),/explicit fresh synthetic directory state/);
  }
  assert.throws(()=>parseLocalDirectoryProfile(bytes({directoryState:'fresh',synthetic:false})),/explicit synthetic volume/);
});

test('regular-file state must be explicitly fresh after directory/volume admission', () => {
  const volume={kind:'local-volume-profile',synthetic:true,device:'harddisk0-partition1',totalAllocationUnits:'65536',
    availableAllocationUnits:'65536',sectorsPerAllocationUnit:32,bytesPerSector:512,directoryState:'fresh'};
  const bytes=edit=>new TextEncoder().encode(JSON.stringify({kind:'local-console-settings',synthetic:true,miscFlags:0,volume:{...volume,...edit}})).buffer;
  assert.deepEqual(parseLocalFileProfile(bytes({fileState:'fresh'})),{synthetic:true,state:'fresh',persistent:false,maxFiles:16,maxFileBytes:65536});
  for(const state of [undefined,null,false,'original','existing','',0]) assert.throws(()=>parseLocalFileProfile(bytes({fileState:state})),/explicit fresh synthetic file state/);
  assert.throws(()=>parseLocalFileProfile(bytes({fileState:'fresh',directoryState:'existing'})),/explicit fresh synthetic directory state/);
  assert.throws(()=>parseLocalFileProfile(bytes({fileState:'fresh',synthetic:false})),/explicit synthetic volume/);
});

test('clock samples separate monotonic duration from adjustable system time and write only the bounded output', () => {
  let now = 100, date = 0;
  const clock = createProofClock({ monotonic: () => now, wall: () => date });
  now = 100.125;
  assert.deepEqual(clock.sample(), { interrupt: 1250n, system: 116444736000000000n });
  now = 101; date = -1000;
  const buffer = new ArrayBuffer(32), bytes = new Uint8Array(buffer); bytes.fill(0xa5);
  clock.write(buffer, 4);
  assert.equal(new DataView(buffer).getBigUint64(4, true), 10000n);
  assert.equal(new DataView(buffer).getBigUint64(12, true), 116444735990000000n);
  assert.ok(bytes.slice(0, 4).every(n => n === 0xa5) && bytes.slice(20).every(n => n === 0xa5));
  const saved = bytes.slice();
  for (const address of [1, 20, -1]) assert.throws(() => clock.write(buffer, address), /outside admitted memory/);
  assert.deepEqual(bytes, saved);
  assert.equal(clock.describe().samples, 2);
  assert.equal(clock.describe().minimumObservedPositiveTicks, '1250');
});

test('clock rejects backward/nonfinite/unsafe samples and timestamps outside its epoch/range', () => {
  for (const origin of [-1, NaN, Infinity]) assert.throws(() => createProofClock({ monotonic: () => origin }));
  for (const [now, date] of [[9, 0], [NaN, 0], [Infinity, 0], [1e20, 0], [11, 0.5],
    [11, -11644473600001], [11, Number.MAX_SAFE_INTEGER]]) {
    let first = true;
    const clock = createProofClock({ monotonic: () => { if (first) { first = false; return 10; } return now; }, wall: () => date });
    assert.throws(() => clock.sample());
  }
});

test('clock import admission is opt-in and accepts only env.ff_host_clock with the exact pointer-to-void signature', () => {
  const name = text => { const bytes = [...new TextEncoder().encode(text)]; return [...uint(bytes.length), ...bytes]; };
  const type = section(1, [1, 0x60, 1, 0x7f, 0]);
  const imported = (namespace = 'env', field = 'ff_host_clock', kind = 0, count = 1) =>
    section(2, [count, ...name(namespace), ...name(field), kind, 0]);
  const bytes = module(type, imported(), memory());
  assert.equal(WebAssembly.validate(bytes), true);
  assert.throws(() => inspectProofWasm(bytes), /no host imports/);
  assert.equal(inspectProofWasm(bytes, { allowClockImport: true }).clockImport, true);
  for (const bad of [module(type, imported('other'), memory()), module(type, imported('env', 'fetch'), memory()),
    module(type, imported('env', 'ff_host_clock', 2), memory()), module(type, imported('env', 'ff_host_clock', 0, 2), memory()),
    module(section(1, [1, 0x60, 1, 0x7c, 0]), imported(), memory()),
    module(section(1, [1, 0x60, 1, 0x7f, 1, 0x7f]), imported(), memory()),
    module(type, type, imported(), memory())]) assert.throws(() => inspectProofWasm(bad, { allowClockImport: true }));
});

test('proof admission accepts bounded memory and leaves full Wasm validation to the engine', () => {
  const bytes = module(memory());
  assert.equal(WebAssembly.validate(bytes), true);
  assert.deepEqual(inspectProofWasm(bytes), { minimumPages: 1, maximumPages: PROOF_MEMORY_PAGES });
  assert.equal(WebAssembly.validate(module(section(5, [1, 1, 1, 1]), section(7, [1]))), false);
});

test('proof admission rejects uncapped, shared, multiple and excessive memories', () => {
  for (const bytes of [module(memory(1, 1, 0)), module(memory(1, 1, 3)), module(memory(1, 1, 5)),
    module(memory(1, 1, 1, 2)), module(memory(1, PROOF_MEMORY_PAGES + 1)), module(memory(2, 1)),
    module(memory(), memory())]) {
    assert.throws(() => inspectProofWasm(bytes));
  }
});

test('proof admission rejects valid host-import and automatic-start modules before instantiation', () => {
  const type = section(1, [1, 0x60, 0, 0]);
  const imported = module(type, section(2, [1, 3, 101, 110, 118, 1, 102, 0, 0]), memory());
  const started = module(type, section(3, [1, 0]), memory(), section(8, [0]), section(10, [1, 2, 0, 11]));
  assert.equal(WebAssembly.validate(imported), true);
  assert.equal(WebAssembly.validate(started), true);
  assert.throws(() => inspectProofWasm(imported), /no host imports/);
  assert.throws(() => inspectProofWasm(started), /Implicit module startup/);
});

test('proof admission bounds file sizes, section spans and unsigned integer decoding', () => {
  const badMagic = new Uint8Array(module(memory())); badMagic[0] = 1;
  for (const bytes of [badMagic.buffer, module(), new ArrayBuffer(PROOF_WASM_LIMIT + 1),
    Uint8Array.from([...header, 5, 127, 1]).buffer,
    Uint8Array.from([...header, 5, 0x80]).buffer,
    Uint8Array.from([...header, 5, 0xff, 0xff, 0xff, 0xff, 0x10]).buffer]) {
    assert.throws(() => inspectProofWasm(bytes));
  }
  assert.throws(() => inspectProofWasm(new Uint8Array(module(memory()))));
});

test('startup host serves only owned source and rejects original modules, files, queries and uploads', async t => {
  const server = createStartupProofServer();
  await new Promise((resolve, reject) => { server.once('error', reject); server.listen(0, '127.0.0.1', resolve); });
  t.after(() => new Promise(resolve => server.close(resolve)));
  const request = (url, method = 'GET', body) => new Promise((resolve, reject) => {
    const req = http.request({ host: '127.0.0.1', port: server.address().port, path: url, method, headers: { Connection: 'close' } }, res => {
      const chunks = []; res.on('data', bytes => chunks.push(bytes));
      res.on('end', () => resolve({ status: res.statusCode, headers: res.headers, body: Buffer.concat(chunks).toString('utf8') }));
    });
    req.on('error', reject); req.end(body);
  });
  for (const url of ['/', '/thread', '/src/xbe.mjs', '/src/runtime/wasm-proof.mjs', '/src/runtime/startup-proof-worker.mjs',
    '/src/runtime/local-title-proof.mjs', '/src/runtime/browser-clock.mjs', '/src/runtime/copy-proof.mjs',
    '/src/runtime/file-request.mjs', '/src/runtime/thread-proof-worker.mjs']) {
    const response = await request(url);
    assert.equal(response.status, 200);
    assert.ok(response.body.length > 100);
    assert.match(response.headers['content-security-policy'], /connect-src 'none'/);
    assert.match(response.headers['content-security-policy'], /form-action 'none'/);
    assert.equal(response.headers['cross-origin-embedder-policy'], 'require-corp');
    const head = await request(url, 'HEAD');
    assert.equal(head.status, 200); assert.equal(head.body, '');
  }
  for (const url of ['/startup.wasm', '/default.xbe', '/Fuzion%20Frenzy%20(USA)/default.xbe',
    '/research/verification/recomp-title-build-02/startup.wasm', '/.tools/emulation/emsdk/.emscripten',
    '/src/xbe.mjs?content=game', '/src/../../research/reference/default.xbe', '/%2e%2e/default.xbe']) {
    assert.equal((await request(url)).status, 404);
  }
  assert.equal((await request('/', 'POST', 'source-owned payload')).status, 404);
  assert.equal((await request('/src/xbe.mjs', 'PUT', 'source-owned payload')).status, 404);
});
