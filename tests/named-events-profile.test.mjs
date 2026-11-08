import test from 'node:test';
import assert from 'node:assert/strict';
import {parseEventProfile,readEventState,validateEventState} from '../runtime/browser/named-events.mjs';
import {parseLocalConsoleSettings} from '../runtime/browser/local-title-proof.mjs';
const profile={kind:'synthetic-win32-named-events',synthetic:true,capacity:4};
function getter({objects=[[1,11,1,0]],handles=[[0x90004,1]],status=0,objectCount=objects.length,handleCount=handles.length}={}) {
 return (i,f)=>i===0xffffffff?[1,objectCount,handleCount,4,status][f]:f<4?objects[i]?.[f]:handles[i]?.[f-4];
}
test('strict profile and optional local settings parsing',()=>{
 assert.deepEqual(parseEventProfile(profile),profile);assert.equal(parseEventProfile(undefined),undefined);
 for(const bad of [null,[],{}, {...profile,capacity:5},{...profile,extra:1},{...profile,synthetic:false}])assert.throws(()=>parseEventProfile(bad),/profile/);
 const settings=x=>new TextEncoder().encode(JSON.stringify({kind:'local-console-settings',synthetic:true,miscFlags:0,...x})).buffer;
 assert.equal(parseLocalConsoleSettings(settings({})).events,undefined);
 assert.deepEqual(parseLocalConsoleSettings(settings({events:profile})).events,profile);
 assert.throws(()=>parseLocalConsoleSettings(settings({events:{...profile,typo:true}})),/event profile/);
});
test('bounded event getter shape and strict metadata',()=>{
 const state=readEventState(getter(),profile);assert.deepEqual(state.objects,[{object:1,nameBytes:11,type:1,signaled:false}]);
 assert.deepEqual(validateEventState(state,profile),state);
 for(const opts of [{objectCount:0xffffffff},{handleCount:0xffffffff},{objects:[[1,0,1,0]]},{objects:[[1,64,1,0]]},
   {objects:[[1,11,2,0]]},{objects:[[1,11,1,1]]},{handles:[[0x90004,2]]},{objects:[[1,11,1,0],[2,7,1,0]],handles:[[0x90004,2],[0x90008,1]]},
   {objects:[[1,11,1,0],[2,7,1,0],[3,8,1,0]],handles:[[0x90004,1],[0x90008,1],[0x9000c,3],[0x90010,2]]},
   {objects:[[1,11,1,0],[2,7,1,0]],handles:[[0x90004,1]]}, {status:0xc0000001}])
   assert.throws(()=>readEventState(getter(opts),profile));
});
test('last-operation status agrees with the retained handle table',()=>{
 assert.throws(()=>readEventState(getter({objects:[],handles:[],objectCount:0,handleCount:0,status:0x40000000}),profile),/existing-object/);
 assert.throws(()=>readEventState(getter({objects:[[1,11,1,0]],handles:[[0x90004,1]],objectCount:1,handleCount:1,status:0x40000000}),profile),/existing-object/);
 assert.throws(()=>readEventState(getter({objects:[[1,11,1,0]],handles:[[0x90004,1],[0x90008,1]],objectCount:1,handleCount:2,status:0}),profile),/fresh-object/);
 const duplicate=readEventState(getter({objects:[[1,11,1,0]],handles:[[0x90004,1],[0x90008,1]],objectCount:1,handleCount:2,status:0x40000000}),profile);
 assert.equal(validateEventState(duplicate,profile).status,0x40000000);
});
