import assert from 'node:assert/strict';
import test from 'node:test';
import {
  createHostStep,
  HOST_STEP_DEFAULT_HZ,
  HOST_STEP_DEFAULT_MAX_CATCH_UP,
  HOST_STEP_MAX_HZ,
  HOST_STEP_MAX_CATCH_UP,
} from '../src/runtime/host-step.mjs';

const close = (actual, expected) => assert.ok(Math.abs(actual - expected) < 1e-9, `${actual} !== ${expected}`);

test('the defaults expose a 60 Hz five-step catch-up loop', () => {
  const step = createHostStep();
  const description = step.describe();
  assert.equal(HOST_STEP_DEFAULT_HZ, 60);
  assert.equal(HOST_STEP_DEFAULT_MAX_CATCH_UP, 5);
  assert.equal(HOST_STEP_MAX_HZ, 1000);
  assert.equal(HOST_STEP_MAX_CATCH_UP, 240);
  assert.equal(description.kind, 'host-step');
  assert.equal(description.hz, 60);
  close(description.stepMs, 1000 / 60);
  assert.equal(description.maxCatchUp, 5);
  assert.equal(description.totalSteps, 0);
  assert.equal(description.paused, false);
});

test('a partial step carries time into the next frame without drift', () => {
  const step = createHostStep({ hz: 60 });
  let result = step.advance(16);
  assert.equal(result.steps, 0);
  close(result.pendingMs, 16);
  result = step.advance(1);
  assert.equal(result.steps, 1);
  close(result.pendingMs, 1 / 3);
  assert.equal(result.droppedSteps, 0);
});

test('exact step multiples produce exact step counts', () => {
  const step = createHostStep({ hz: 1000 });
  assert.equal(step.advance(1).steps, 1);
  assert.equal(step.advance(0.5).steps, 0);
  close(step.advance(0.5).pendingMs, 0);
  assert.equal(step.advance(2).steps, 2);
});

test('long frames are capped and the excess is reported as dropped', () => {
  const step = createHostStep({ hz: 60, maxCatchUp: 5 });
  const result = step.advance(1000);
  assert.equal(result.steps, 5);
  assert.equal(result.droppedSteps, 55);
  close(result.pendingMs, 0);
  const description = step.describe();
  assert.equal(description.totalSteps, 5);
  assert.equal(description.droppedSteps, 55);
});

test('pause discards partial time and already-elapsed frames', () => {
  const step = createHostStep({ hz: 60 });
  step.advance(8);
  step.pause();
  const paused = step.advance(1000);
  assert.equal(paused.steps, 0);
  close(paused.pendingMs, 0);
  assert.equal(paused.paused, true);
  step.resume();
  assert.equal(step.advance(17).steps, 1);
});

test('reset clears totals, dropped count, partial time and pause', () => {
  const step = createHostStep({ hz: 60 });
  step.advance(1000);
  step.pause();
  step.reset();
  const description = step.describe();
  assert.equal(description.totalSteps, 0);
  assert.equal(description.droppedSteps, 0);
  assert.equal(description.paused, false);
  assert.equal(step.advance(0).pendingMs, 0);
});

test('invalid configuration and deltas fail closed', () => {
  for (const hz of [0, 1001, 60.5, '60', NaN]) assert.throws(() => createHostStep({ hz }), /hz/);
  for (const maxCatchUp of [0, 241, 1.5, '5', NaN]) assert.throws(() => createHostStep({ maxCatchUp }), /maxCatchUp/);
  const step = createHostStep();
  for (const delta of [-1, NaN, Infinity, -Infinity, '16']) assert.throws(() => step.advance(delta), /deltaMs/);
  assert.throws(() => step.advance(1e12), /safe range/);
});
