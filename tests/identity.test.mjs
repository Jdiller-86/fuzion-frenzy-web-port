import assert from 'node:assert/strict';import {sha256Blob,requireBlobIdentity} from '../src/identity.mjs';
let blob=new Blob(['abc']),hash='ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad';assert.equal(await sha256Blob(blob,3),hash);await requireBlobIdentity(blob,3,hash);let held=0,release,started;const entered=new Promise(r=>{started=r});const gate=new Promise(r=>{release=r});let finished=false;const pending=requireBlobIdentity(blob,3,hash,async n=>{held=n;started();await gate;}).then(()=>{finished=true;});await entered;assert.equal(held,3);assert.equal(finished,false);release();await pending;assert.equal(finished,true);await assert.rejects(requireBlobIdentity(new Blob(['abd']),3,hash));await assert.rejects(requireBlobIdentity(blob,4,hash));await assert.rejects(requireBlobIdentity(blob,3,'broken'));await assert.rejects(sha256Blob(blob,17*1024*1024));console.log('Content identity checks passed');
const large=new Blob([new Uint8Array(17*1024*1024)]);
await assert.rejects(sha256Blob(large,large.size));
assert.match(await sha256Blob(large,large.size,undefined,18*1024*1024),/^[a-f0-9]{64}$/);
await assert.rejects(sha256Blob(blob,3,undefined,33*1024*1024));
