// Bounded host gamepad adapter. It consumes an explicit, already-normalized
// standard-mapping snapshot and produces the Xbox controller state used by the
// packet encoder. It never touches the browser Gamepad API, a wall clock or a
// device. Rules follow the pinned compatibility implementation
// (input_map.c): 15% radial stick deadzone with rescale, 5% trigger threshold,
// 50% digital-button threshold, analog face/shoulder buttons at 255, and Y
// positive upwards.
export const GAMEPAD_DEADZONE_PERCENT = 15;
export const GAMEPAD_TRIGGER_THRESHOLD_PERCENT = 5;
export const GAMEPAD_BUTTON_THRESHOLD_PERCENT = 50;

const clamp = (value, low, high) => value < low ? low : value > high ? high : value;
const round = value => value >= 0 ? Math.floor(value + 0.5) : Math.ceil(value - 0.5);

export function applyTriggerValue(value, thresholdPercent = GAMEPAD_TRIGGER_THRESHOLD_PERCENT) {
  const v = clamp(Math.trunc(value), 0, 255);
  const pct = clamp(Math.trunc(thresholdPercent), 0, 95);
  const threshold = Math.trunc(255 * pct / 100);
  if (v <= threshold) return 0;
  return Math.trunc((v - threshold) * 255 / (255 - threshold));
}

export function applyStickDeadzone(x, y, deadzonePercent = GAMEPAD_DEADZONE_PERCENT) {
  const pct = clamp(Math.trunc(deadzonePercent), 0, 95);
  const dz = 32767 * pct / 100;
  const magnitude = Math.sqrt(x * x + y * y);
  if (!magnitude || magnitude <= dz) return { x: 0, y: 0 };
  let scale = (magnitude - dz) / (32767 - dz);
  if (scale > 1) scale = 1;
  return {
    x: round(x / magnitude * scale * 32767),
    y: round(y / magnitude * scale * 32767),
  };
}

const unavailable = reason => ({ available: false, reason });

export function adaptStandardGamepad(snapshot, {
  deadzonePercent = GAMEPAD_DEADZONE_PERCENT,
  triggerThresholdPercent = GAMEPAD_TRIGGER_THRESHOLD_PERCENT,
  buttonThresholdPercent = GAMEPAD_BUTTON_THRESHOLD_PERCENT,
} = {}) {
  if (!snapshot || typeof snapshot !== 'object') return unavailable('missing gamepad snapshot');
  if (!Array.isArray(snapshot.buttons) || snapshot.buttons.length < 16) return unavailable('standard gamepad requires at least 16 buttons');
  if (!Array.isArray(snapshot.axes) || snapshot.axes.length < 4) return unavailable('standard gamepad requires at least 4 axes');
  for (const axis of snapshot.axes.slice(0, 4)) {
    if (typeof axis !== 'number' || !Number.isFinite(axis) || axis < -1 || axis > 1)
      return unavailable('axes must be finite normalized values');
  }
  for (const button of snapshot.buttons) {
    if (button && (typeof button.pressed !== 'boolean' ||
      (button.value !== undefined && (typeof button.value !== 'number' || !Number.isFinite(button.value) || button.value < 0 || button.value > 1))))
      return unavailable('buttons must expose checked pressed/value fields');
  }
  const pressed = index => snapshot.buttons[index]?.pressed === true ||
    (snapshot.buttons[index]?.value ?? 0) >= buttonThresholdPercent / 100;
  const value = index => snapshot.buttons[index]?.value ?? (snapshot.buttons[index]?.pressed ? 1 : 0);
  const axisValue = index => round(clamp(snapshot.axes[index], -1, 1) * 32767);

  const analog = [0, 0, 0, 0, 0, 0, applyTriggerValue(round(value(6) * 255), triggerThresholdPercent),
    applyTriggerValue(round(value(7) * 255), triggerThresholdPercent)];
  for (const index of [0, 1, 2, 3, 4, 5]) if (pressed(index)) analog[index] = 255;

  let buttons = 0;
  if (pressed(12)) buttons |= 0x0001;
  if (pressed(13)) buttons |= 0x0002;
  if (pressed(14)) buttons |= 0x0004;
  if (pressed(15)) buttons |= 0x0008;
  if (pressed(9)) buttons |= 0x0010;
  if (pressed(8)) buttons |= 0x0020;
  if (pressed(10)) buttons |= 0x0040;
  if (pressed(11)) buttons |= 0x0080;

  const left = applyStickDeadzone(axisValue(0), -axisValue(1), deadzonePercent);
  const right = applyStickDeadzone(axisValue(2), -axisValue(3), deadzonePercent);
  return {
    available: true,
    connected: snapshot.connected !== false,
    buttons,
    analog,
    lx: clamp(Math.trunc(left.x), -32768, 32767),
    ly: clamp(Math.trunc(left.y), -32768, 32767),
    rx: clamp(Math.trunc(right.x), -32768, 32767),
    ry: clamp(Math.trunc(right.y), -32768, 32767),
  };
}
