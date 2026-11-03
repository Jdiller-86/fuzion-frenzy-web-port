// Bounded browser-style gamepad source. It consumes an explicit list shaped
// like the result of navigator.getGamepads(), assigns at most four positions
// to controller ports, and adapts each present standard-mapping pad to the Xbox
// state used by the packet encoder. It never calls a browser API, reads a
// device, or uses a clock; the caller supplies the list. Only standard-mapping
// pads are admitted, port assignment is positional (a pad whose index is not
// its port is rejected), and malformed entries fail closed per port.
import { adaptStandardGamepad } from './gamepad-adapter.mjs';

export const CONTROLLER_MAX_PORTS = 4;

const unavailable = reason => ({ available: false, reason });

const disconnectedPort = () => ({ available: true, connected: false, state: null });

const rejectedPort = reason => ({ available: false, connected: false, state: null, reason });

export function collectControllerPorts(gamepads, { maxPorts = CONTROLLER_MAX_PORTS } = {}) {
  if (!Number.isInteger(maxPorts) || maxPorts < 1 || maxPorts > CONTROLLER_MAX_PORTS)
    return unavailable('maxPorts must be an integer from 1 to 4');
  if (gamepads == null || typeof gamepads !== 'object' || typeof gamepads.length !== 'number')
    return unavailable('missing gamepad list');

  const ports = [];
  for (let slot = 0; slot < maxPorts; ++slot) {
    const pad = gamepads[slot] ?? null;
    if (pad == null || pad.connected === false) {
      ports.push(disconnectedPort());
      continue;
    }
    if (pad.mapping !== 'standard') {
      ports.push(rejectedPort('gamepad mapping is not standard'));
      continue;
    }
    if (!Number.isInteger(pad.index) || pad.index !== slot) {
      ports.push(rejectedPort('gamepad index does not match its port'));
      continue;
    }
    const state = adaptStandardGamepad({
      connected: true,
      buttons: pad.buttons,
      axes: pad.axes,
    });
    if (!state.available) {
      ports.push(rejectedPort(state.reason));
      continue;
    }
    ports.push({ available: true, connected: true, state });
  }
  return { available: true, ports };
}
