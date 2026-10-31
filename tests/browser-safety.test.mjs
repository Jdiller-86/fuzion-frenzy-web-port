import test from 'node:test';import assert from 'node:assert/strict';
import {LocalInput} from '../packages/input/local-input.mjs';import {bindBrowserSafety} from '../packages/input/browser-safety.mjs';
function harness(){const window=new EventTarget(),document=new EventTarget(),input=new LocalInput(2),reasons=[];let pads=[];bindBrowserSafety({window,document,getInput:()=>input,getGamepads:()=>pads,pause:reason=>reasons.push(reason)});return {window,document,input,reasons,setPads:value=>{pads=value;}};}
test('bound blur and visibility events clear pending jumps and held ducks for every seat',()=>{
  const h=harness();for(const event of ['blur','visibilitychange']){h.input.keyDown('Space');h.input.keyDown('KeyS');if(event==='blur')h.window.dispatchEvent(new Event(event));else{h.document.hidden=true;h.document.dispatchEvent(new Event(event));}assert.deepEqual(h.input.consume(),[{jumpPressed:false,duckHeld:false},{jumpPressed:false,duckHeld:false}]);h.input.keyUp('Space');h.input.keyUp('KeyS');}
  assert.deepEqual(h.reasons,['Window lost focus.','Tab was hidden.']);
  h.document.hidden=false;h.document.dispatchEvent(new Event('visibilitychange'));assert.equal(h.reasons.length,2,'regaining visibility must not auto-resume');
});
test('bound disconnect pauses owned device only and clears all seats before resuming',()=>{
  const h=harness(),pad={connected:true,index:0,id:'source-owned standard pad',mapping:'standard',buttons:[{},{}]};h.setPads([pad]);h.input.pollGamepads([pad]);h.input.assignGamepad(1,0,pad.id);h.input.keyDown('Space');h.window.dispatchEvent(new Event('gamepaddisconnected'));assert.equal(h.reasons.length,0,'unrelated device notification');
  h.setPads([]);h.window.dispatchEvent(new Event('gamepaddisconnected'));assert.deepEqual(h.reasons,['An owned gamepad disconnected.']);assert.equal(h.input.ready(),false);assert.equal(h.input.consume()[0].jumpPressed,false);
});
