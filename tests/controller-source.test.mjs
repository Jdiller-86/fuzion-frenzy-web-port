import assert from 'node:assert/strict';
import test from 'node:test';
import { collectControllerPorts, CONTROLLER_MAX_PORTS } from '../runtime/browser/controller-source.mjs';

const button = () => ({ pressed: false, value: 0 });

const pad = (index, overrides = {}) => ({
  index,
  connected: true,
  mapping: 'standard',
  buttons: Array.from({ length: 16 }, button),
  axes: [0, 0, 0, 0],
  ...overrides,
});

test('the source exposes four controller ports and treats absent pads as disconnected', () => {
  assert.equal(CONTROLLER_MAX_PORTS, 4);
  const result = collectControllerPorts([null, null, null, null]);
  assert.equal(result.available, true);
  assert.equal(result.ports.length, 4);
  for (const port of result.ports) {
    assert.deepEqual(port, { available: true, connected: false, state: null });
  }
});

test('a present standard pad is adapted into its positional port', () => {
  const result = collectControllerPorts([pad(0), null, null, null]);
  assert.equal(result.ports[0].connected, true);
  assert.equal(result.ports[0].state.buttons, 0);
  assert.deepEqual(result.ports[0].state.analog, [0, 0, 0, 0, 0, 0, 0, 0]);
  for (const slot of [1, 2, 3]) assert.equal(result.ports[slot].connected, false);
});

test('a pad in another slot keeps the earlier seats disconnected', () => {
  const result = collectControllerPorts([null, null, pad(2), null]);
  assert.equal(result.ports[0].connected, false);
  assert.equal(result.ports[1].connected, false);
  assert.equal(result.ports[2].connected, true);
  assert.equal(result.ports[3].connected, false);
});

test('button state stays isolated per port', () => {
  const pressed = pad(1, { buttons: Array.from({ length: 16 }, (_, i) => i === 0 ? { pressed: true, value: 1 } : button()) });
  const result = collectControllerPorts([pad(0), pressed, null, null]);
  assert.equal(result.ports[0].state.buttons, 0);
  assert.equal(result.ports[1].state.buttons, 0);
  assert.equal(result.ports[1].state.analog[0], 255);
});

test('a non-standard mapping is rejected without affecting other ports', () => {
  const result = collectControllerPorts([pad(0), pad(1, { mapping: '' }), pad(2), null]);
  assert.equal(result.ports[0].connected, true);
  assert.deepEqual(result.ports[1],
    { available: false, connected: false, state: null, reason: 'gamepad mapping is not standard' });
  assert.equal(result.ports[2].connected, true);
});

test('a pad whose index is not its port is rejected', () => {
  const result = collectControllerPorts([null, pad(0), null, null]);
  assert.deepEqual(result.ports[1],
    { available: false, connected: false, state: null, reason: 'gamepad index does not match its port' });
});

test('malformed pad data fails closed per port through the adapter', () => {
  const result = collectControllerPorts([pad(0, { axes: [0, 0] }), pad(1), null, null]);
  assert.equal(result.ports[0].available, false);
  assert.equal(result.ports[0].reason, 'standard gamepad requires at least 4 axes');
  assert.equal(result.ports[1].connected, true);
});

test('a connected flag of false is treated as an absent pad', () => {
  const result = collectControllerPorts([pad(0, { connected: false }), null, null, null]);
  assert.deepEqual(result.ports[0], { available: true, connected: false, state: null });
});

test('pads beyond the requested port count are ignored', () => {
  const result = collectControllerPorts([pad(0), pad(1), pad(2), pad(3), pad(4)], { maxPorts: 2 });
  assert.equal(result.ports.length, 2);
  assert.equal(result.ports[0].connected, true);
  assert.equal(result.ports[1].connected, true);
});

test('invalid lists and port counts fail closed', () => {
  assert.deepEqual(collectControllerPorts(null), { available: false, reason: 'missing gamepad list' });
  assert.deepEqual(collectControllerPorts({}), { available: false, reason: 'missing gamepad list' });
  assert.deepEqual(collectControllerPorts([], { maxPorts: 0 }), { available: false, reason: 'maxPorts must be an integer from 1 to 4' });
  assert.deepEqual(collectControllerPorts([], { maxPorts: 5 }), { available: false, reason: 'maxPorts must be an integer from 1 to 4' });
  assert.deepEqual(collectControllerPorts([], { maxPorts: 1.5 }), { available: false, reason: 'maxPorts must be an integer from 1 to 4' });
});
