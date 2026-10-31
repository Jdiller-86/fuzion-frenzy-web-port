import test from 'node:test';
import assert from 'node:assert/strict';
import {LocalInput,KEYBOARD_LAYOUTS} from '../packages/input/local-input.mjs';
const pad=(index=0,{jump=false,duck=false,id='Test controller',mapping='standard'}={})=>({index,id,mapping,connected:true,buttons:[{pressed:jump},{pressed:duck}]});

test('four disjoint keyboard seats latch short taps and ignore auto-repeat',()=>{
  const input=new LocalInput(4);
  for(const keys of KEYBOARD_LAYOUTS){input.keyDown(keys.jump);input.keyUp(keys.jump);input.keyDown(keys.duck);}
  assert.deepEqual(input.consume(),Array.from({length:4},()=>({jumpPressed:true,duckHeld:true})));
  assert.ok(input.consume().every(c=>!c.jumpPressed&&c.duckHeld));
  input.keyDown('Space',{repeat:true});assert.equal(input.consume()[0].jumpPressed,false);
});

test('focus clearing drops held/pending input and requires key release before rearming',()=>{
  const input=new LocalInput();input.keyDown('Space');input.keyDown('ArrowDown');input.clear();
  assert.deepEqual(input.consume(),[{jumpPressed:false,duckHeld:false}]);
  input.keyDown('Space');input.keyDown('ArrowDown');assert.deepEqual(input.consume(),[{jumpPressed:false,duckHeld:false}]);
  input.keyUp('Space');input.keyUp('ArrowDown');input.keyDown('Space');assert.equal(input.consume()[0].jumpPressed,true);
});

test('editable focus and unowned keys do not trigger gameplay',()=>{
  const input=new LocalInput();assert.equal(input.keyDown('Space',{editable:true}),false);assert.equal(input.keyDown('KeyZ'),false);
  assert.deepEqual(input.consume(),[{jumpPressed:false,duckHeld:false}]);
});

test('keyboard ownership rejects overlapping keys without changing the original binding',()=>{
  const input=new LocalInput(2);assert.throws(()=>input.assignKeyboard(1,'Space','KeyS'),/one seat/);
  assert.deepEqual(input.bindings[1],{type:'keyboard',jump:'KeyW',duck:'KeyS'});
  assert.throws(()=>input.assignKeyboard(1,'KeyW','KeyW'));assert.throws(()=>input.assignKeyboard(1,'Escape','KeyS'));
});

test('mixed seats receive only their owned standard gamepad',()=>{
  const input=new LocalInput(2);input.pollGamepads([pad()]);input.assignGamepad(1,0,'Test controller');input.pollGamepads([pad()]);
  input.keyDown('Space');input.pollGamepads([pad(0,{duck:true})]);assert.deepEqual(input.consume(),[{jumpPressed:true,duckHeld:false},{jumpPressed:false,duckHeld:true}]);
  input.pollGamepads([pad(0,{jump:true})]);assert.equal(input.consume()[1].jumpPressed,true);
  input.pollGamepads([pad(0,{jump:true})]);assert.equal(input.consume()[1].jumpPressed,false);
  assert.throws(()=>input.assignGamepad(0,0,'Test controller'),/one seat/);
});

test('disconnect clears actions once and reconnect requires released controls',()=>{
  const input=new LocalInput();input.pollGamepads([pad()]);input.assignGamepad(0,0,'Test controller');input.pollGamepads([pad()]);input.pollGamepads([pad(0,{jump:true,duck:true})]);
  assert.deepEqual(input.pollGamepads([]),[{seat:0,index:0}]);assert.equal(input.ready(),false);assert.deepEqual(input.consume(),[{jumpPressed:false,duckHeld:false}]);assert.deepEqual(input.pollGamepads([]),[]);
  input.pollGamepads([pad(0,{jump:true,duck:true})]);assert.deepEqual(input.consume(),[{jumpPressed:false,duckHeld:false}]);
  input.pollGamepads([pad()]);input.pollGamepads([pad(0,{jump:true})]);assert.equal(input.consume()[0].jumpPressed,true);assert.equal(input.ready(),true);
});

test('reused controller index with a different device identity cannot take over a seat',()=>{
  const input=new LocalInput();input.pollGamepads([pad()]);input.assignGamepad(0,0,'Test controller');input.pollGamepads([pad()]);
  assert.deepEqual(input.pollGamepads([pad(0,{id:'Different controller',jump:true})]),[{seat:0,index:0}]);assert.equal(input.ready(),false);
  assert.equal(input.consume()[0].jumpPressed,false);
  input.pollGamepads([pad(0,{mapping:'unknown'})]);assert.throws(()=>input.assignGamepad(0,0,'Test controller'),/standard/);
});
