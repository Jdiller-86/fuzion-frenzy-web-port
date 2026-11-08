import assert from 'node:assert/strict';
import test from 'node:test';
import { parseTraceCaptureStart } from '../runtime/browser/trace-capture.mjs';
import { parseLocalConsoleSettings } from '../runtime/browser/local-title-proof.mjs';

test('trace capture selection admits only the two explicit windows',()=>{
  assert.equal(parseTraceCaptureStart(undefined),undefined);
  assert.equal(parseTraceCaptureStart(0),0);
  assert.equal(parseTraceCaptureStart(8192),8192);
  for(const bad of [null,1,512,'8192',-1,8193,0.5]) assert.throws(()=>parseTraceCaptureStart(bad));
});

test('local settings preserve legacy absence and admit numeric trace window selections only',()=>{
  const settings=value=>new TextEncoder().encode(JSON.stringify({kind:'local-console-settings',synthetic:true,miscFlags:0,...value})).buffer;
  assert.equal(Object.hasOwn(parseLocalConsoleSettings(settings({})),'traceCaptureStart'),false);
  assert.equal(parseLocalConsoleSettings(settings({traceCaptureStart:8192})).traceCaptureStart,8192);
  for(const value of ['8192',null,1,16384]) assert.throws(()=>parseLocalConsoleSettings(settings({traceCaptureStart:value})),/trace capture start/);
});
