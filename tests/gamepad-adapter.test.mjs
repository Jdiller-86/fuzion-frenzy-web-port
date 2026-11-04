import assert from 'node:assert/strict';
import test from 'node:test';
import {
  adaptStandardGamepad, applyStickDeadzone, applyTriggerValue,
  GAMEPAD_DEADZONE_PERCENT, GAMEPAD_TRIGGER_THRESHOLD_PERCENT,
} from '../src/runtime/gamepad-adapter.mjs';

const neutral = () => ({
  connected: true,
  buttons: Array.from({ length: 16 }, () => ({ pressed: false, value: 0 })),
  axes: [0, 0, 0, 0],
});

test('the adapter maps a neutral standard pad to neutral Xbox state', () => {
  const state = adaptStandardGamepad(neutral());
  assert.equal(state.available, true);
  assert.equal(state.connected, true);
  assert.deepEqual(state.analog, [0, 0, 0, 0, 0, 0, 0, 0]);
  assert.deepEqual([state.lx, state.ly, state.rx, state.ry], [0, 0, 0, 0]);
  assert.equal(state.buttons, 0);
});

test('face, shoulder, dpad and start buttons map to their Xbox fields', () => {
  const pad = neutral();
  pad.buttons[0] = { pressed: true, value: 1 };
  pad.buttons[4] = { pressed: true, value: 1 };
  pad.buttons[9] = { pressed: true, value: 1 };
  pad.buttons[10] = { pressed: true, value: 1 };
  pad.buttons[12] = { pressed: true, value: 1 };
  pad.buttons[14] = { pressed: true, value: 1 };
  const state = adaptStandardGamepad(pad);
  assert.equal(state.analog[0], 255);
  assert.equal(state.analog[4], 255);
  assert.equal(state.buttons, 0x0001 | 0x0004 | 0x0010 | 0x0040);
});

test('trigger thresholds rescale the remaining travel', () => {
  assert.equal(applyTriggerValue(0, 5), 0);
  assert.equal(applyTriggerValue(12, 5), 0);
  assert.equal(applyTriggerValue(128, 5), 121);
  assert.equal(applyTriggerValue(255, 5), 255);
  const pad = neutral();
  pad.buttons[6] = { pressed: true, value: 0.5 };
  const state = adaptStandardGamepad(pad);
  assert.equal(state.analog[6], 121);
});

test('the stick deadzone is radial and rescales full deflection', () => {
  assert.deepEqual(applyStickDeadzone(0, 0), { x: 0, y: 0 });
  assert.deepEqual(applyStickDeadzone(3277, 0), { x: 0, y: 0 });
  assert.deepEqual(applyStickDeadzone(32767, 0), { x: 32767, y: 0 });
  assert.equal(GAMEPAD_DEADZONE_PERCENT, 15);
  assert.equal(GAMEPAD_TRIGGER_THRESHOLD_PERCENT, 5);
});

test('Y is inverted to the Xbox convention and axes stay bounded', () => {
  const horizontal = neutral();
  horizontal.axes = [1, 0, 0, 0];
  assert.equal(adaptStandardGamepad(horizontal).lx, 32767);
  const vertical = neutral();
  vertical.axes = [0, -1, 0, 0];
  assert.equal(adaptStandardGamepad(vertical).ly, 32767);
});

test('disconnected and malformed snapshots fail closed', () => {
  const disconnected = adaptStandardGamepad({ ...neutral(), connected: false });
  assert.equal(disconnected.available, true);
  assert.equal(disconnected.connected, false);
  assert.deepEqual(adaptStandardGamepad(null), { available: false, reason: 'missing gamepad snapshot' });
  assert.deepEqual(adaptStandardGamepad({ connected: true, buttons: [], axes: [0, 0, 0, 0] }),
    { available: false, reason: 'standard gamepad requires at least 16 buttons' });
  assert.deepEqual(adaptStandardGamepad({ connected: true, buttons: Array.from({ length: 16 }, () => ({ pressed: false })), axes: [0, 0] }),
    { available: false, reason: 'standard gamepad requires at least 4 axes' });
  assert.deepEqual(adaptStandardGamepad({ connected: true, buttons: Array.from({ length: 16 }, () => ({ pressed: false })), axes: [0, 0, 0, 2] }),
    { available: false, reason: 'axes must be finite normalized values' });
  assert.deepEqual(adaptStandardGamepad({ connected: true, buttons: Array.from({ length: 16 }, () => ({ pressed: 1 })), axes: [0, 0, 0, 0] }),
    { available: false, reason: 'buttons must expose checked pressed/value fields' });
});
