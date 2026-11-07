import test from 'node:test';
import assert from 'node:assert/strict';
import { parseLocalMutantProfile, readLocalMutantState, readLocalMutantStateFromWasm, validateLocalMutantState } from '../runtime/browser/local-mutants.mjs';

const p=()=>({kind:'synthetic-win32-mutants',synthetic:true,capacity:4}), H=0xffffffff;
const table=(objects,handles,status=0)=> (index,field)=>index===H?[1,objects.length,handles.length,4,status][field]:
  field<4&&index>=0&&index<objects.length?[objects[index].object,objects[index].nameBytes,objects[index].named?1:0,0][field]:
  field>=4&&index>=0&&index<handles.length?[handles[index].handle,handles[index].object][field-4]:undefined;
const anon={object:1,nameBytes:0,named:false},named={object:2,nameBytes:5,named:true};
const hs=[{handle:0x80004,object:1},{handle:0x80008,object:2},{handle:0x8000c,object:2}];

test('local mutant profile is strict and legacy profile remains a separate kind',()=>{
  assert.equal(parseLocalMutantProfile(undefined),undefined);assert.deepEqual(parseLocalMutantProfile(p()),p());
  for(const bad of [null,[],{}, {...p(),extra:true},{...p(),capacity:5},{...p(),synthetic:false},{...p(),kind:'synthetic-unnamed-unowned-mutants'}])
    assert.throws(()=>parseLocalMutantProfile(bad),/Win32 mutant/);
});

test('reads bounded anonymous and named objects with canonical first-reference handles',()=>{
  const s=readLocalMutantState(table([anon,named],hs,0x40000000),p());
  assert.deepEqual(s,{...p(),objectCount:2,handleCount:3,status:0x40000000,
    objects:[{...anon,owned:false},{...named,owned:false}],handles:hs});
  assert.deepEqual(validateLocalMutantState(JSON.parse(JSON.stringify(s)),p()),s);
});

test('rejects malformed rows, status/counter bounds and noncanonical state',()=>{
  for(const [o,h,status] of [
    [[{...anon,object:2}],hs.slice(0,1),0],
    [[{...named,nameBytes:0}],[hs[1]],0],
    [[{...anon,named:true}],[hs[0]],0],
    [[anon,named],[hs[1]],0],
    [[anon,named],hs,0xc000009a],
  ]) assert.throws(()=>readLocalMutantState(table(o,h,status),p()));
  assert.equal(readLocalMutantState(table([],[],0xc0000024),p()).status,0xc0000024);
  for(const value of [-1,NaN,0x1_0000_0000,'1'])
    assert.throws(()=>readLocalMutantState((index,field)=>index===H?[1,value,0,4,0][field]:value,p()),/local mutant/);
  const s=readLocalMutantState(table([anon,named],hs,0x40000000),p());
  for(const bad of [{...s,extra:1},{...s,handleCount:5},{...s,objects:[...s.objects,{...anon}]},
    {...s,objects:[s.objects[0],{...s.objects[1],owned:true}]},
    {...s,handles:[s.handles[0],{...s.handles[1],object:1},s.handles[2]]}]) assert.throws(()=>validateLocalMutantState(bad,p()));
  assert.equal(validateLocalMutantState({...s,status:0xc000000d},p()).status,0xc000000d);
});

test('WASM getter accepts signed i32 NTSTATUS values but rejects malformed words',()=>{
  const signedStatus=0xc0000024-0x100000000;
  const getter=table([],[],signedStatus);
  assert.equal(readLocalMutantStateFromWasm(getter,p()).status,0xc0000024);
  for(const value of [NaN,0.5,-0x80000001,0x1_0000_0000,'-1073741788'])
    assert.throws(()=>readLocalMutantStateFromWasm((index,field)=>index===H&&field===0?value:table([],[],0)(index,field),p()),/WASM word/);
});
