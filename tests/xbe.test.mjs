import test from 'node:test';import assert from 'node:assert/strict';
import {mkdir,mkdtemp,writeFile,readFile,stat} from 'node:fs/promises';
import {spawnSync} from 'node:child_process';import {fileURLToPath} from 'node:url';import path from 'node:path';
import {readXbeProfile} from '../src/xbe.mjs';
// Entirely source-owned executable metadata. No original instructions/resources.
function fixture(variant='retail'){
  const b=new ArrayBuffer(0x600),v=new DataView(b),u=(o,n)=>v.setUint32(o,n,true);
  u(0,0x48454258);u(0x104,0x10000);u(0x108,0x200);u(0x10c,0x4000);u(0x110,0x178);u(0x11c,2);u(0x120,0x10180);u(0x130,0x10000);
  const keys={retail:[0xa8fc57ab,0x5b6d40b6],debug:[0x94859d4b,0xefb1f152],beta:[0xe682f45b,0x46437dcd]}[variant];u(0x128,0x11000^keys[0]);u(0x158,0x12000^keys[1]);
  u(0x180,4);u(0x184,0x11000);u(0x188,0x100);u(0x18c,0x200);u(0x190,0x100);
  u(0x1b8,1);u(0x1bc,0x12000);u(0x1c0,0x200);u(0x1c4,0x300);u(0x1c8,0x100);
  u(0x300,0x8000000e);u(0x304,0x8000000e);u(0x308,0x8000014f);u(0x30c,0);
  return b;
}
test('retail/debug/beta must select consistent executable-entry and import keys',()=>{
  for(const variant of ['retail','debug','beta']){const p=readXbeProfile(fixture(variant));assert.equal(p.variant,variant);assert.equal(p.entry,0x11000);assert.equal(p.kernelThunk,0x12000);assert.deepEqual(p.imports.map(x=>x.ordinal),[14,14,335]);}
  const b=fixture();new DataView(b).setUint32(0x158,0x12000^0xefb1f152,true);assert.throws(()=>readXbeProfile(b),/profile/);
});
test('malformed headers, excessive tables, section overlaps and truncated payloads are rejected',()=>{
  assert.throws(()=>readXbeProfile(new ArrayBuffer(12)));assert.throws(()=>readXbeProfile(new ArrayBuffer(0x600)));
  for(const [offset,value]of [[0x11c,129],[0x120,0xfffffff0],[0x108,0x601],[0x1bc,0x11080],[0x190,0x500],[0x188,0xffffffff]]){const b=fixture();new DataView(b).setUint32(offset,value,true);assert.throws(()=>readXbeProfile(b));}
});
test('import ordinals require tags, file-backed ranges and an in-range terminator',()=>{
  for(const [offset,value]of [[0x300,14],[0x300,0x80000000],[0x158,0x12100^0x5b6d40b6]]){const b=fixture();new DataView(b).setUint32(offset,value,true);assert.throws(()=>readXbeProfile(b),/profile/);}
  const b=fixture(),v=new DataView(b);for(let o=0x300;o<0x400;o+=4)v.setUint32(o,0x8000000e,true);assert.throws(()=>readXbeProfile(b),/profile/);
});
test('entry must be file backed executable code; zero-fill cannot supply metadata',()=>{
  for(const [offset,value]of [[0x180,0],[0x128,0x12000^0xa8fc57ab],[0x128,0x11100^0xa8fc57ab]]){const b=fixture();new DataView(b).setUint32(offset,value,true);assert.throws(()=>readXbeProfile(b),/profile/);}
});
test('TLS zero-fill and kernel index may live in mapped BSS; malformed sizes do not',()=>{
  const b=fixture(),v=new DataView(b),u=(o,n)=>v.setUint32(o,n,true);u(0x12c,0x12020);u(0x320,0x12080);u(0x324,0x12084);u(0x328,0x12100);u(0x330,32);
  assert.equal(readXbeProfile(b).tls.zeroFillBytes,32);u(0x324,0x1207f);assert.throws(()=>readXbeProfile(b),/TLS/);
});
test('kernel-free executables and bounded library declarations are profiled without executing bytes',()=>{
  const b=fixture(),v=new DataView(b),u=(o,n)=>v.setUint32(o,n,true);u(0x158,0x5b6d40b6);u(0x160,1);u(0x164,0x12040);
  new Uint8Array(b,0x340,8).set(new TextEncoder().encode('XAPILIB'));v.setUint16(0x348,1,true);v.setUint16(0x34c,3911,true);
  const p=readXbeProfile(b);assert.deepEqual(p.imports,[]);assert.equal(p.nonKernelImportsAddress,0);assert.equal(p.libraries[0].name,'XAPILIB');assert.equal(p.libraries[0].build,3911);
  u(0x160,129);assert.throws(()=>readXbeProfile(b),/library count/);u(0x160,1);u(0x164,0x12100);assert.throws(()=>readXbeProfile(b),/file backed/);
});
test('profile CLI keeps reports in ignored research and preserves existing files',async()=>{
  const root=fileURLToPath(new URL('../',import.meta.url)),research=path.join(root,'research'),tools=path.join(root,'.tools');
  await mkdir(research,{recursive:true});await mkdir(tools,{recursive:true});
  const work=await mkdtemp(path.join(research,'source-owned-xbe-')),outside=await mkdtemp(path.join(tools,'source-owned-xbe-'));
  const input=path.join(work,'fixture.xbe'),output=path.join(work,'profile.json');await writeFile(input,new Uint8Array(fixture()),{flag:'wx'});
  const run=target=>spawnSync(process.execPath,[path.join(root,'tools/profile-execution.mjs'),input,target],{encoding:'utf8'});
  const rejected=path.join(outside,'must-not-exist.json');assert.notEqual(run(rejected).status,0);await assert.rejects(stat(rejected),{code:'ENOENT'});
  assert.equal(run(output).status,0);const first=await readFile(output,'utf8');assert.equal(JSON.parse(first).kind,'static-execution-dependencies');
  assert.notEqual(run(output).status,0);assert.equal(await readFile(output,'utf8'),first);assert.deepEqual(await readFile(input),Buffer.from(fixture()));
  // Retain these tiny, source-owned fixtures in ignored directories. Never clean
  // a supplied-content directory or delete any pre-existing local file.
});
