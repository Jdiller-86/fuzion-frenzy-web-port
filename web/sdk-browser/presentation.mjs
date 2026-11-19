const STORAGE_KEY = 'fuzion.browser.settings.v1';
const DEFAULTS = {frameCap:60, aspect:'original', startFullscreen:false, volume:100, showPerformance:false};
const fields = Object.keys(DEFAULTS);

function validated(value) {
  if (!value || typeof value !== 'object' || ![30,60].includes(value.frameCap) ||
      !['original','widescreen','fill'].includes(value.aspect) ||
      typeof value.startFullscreen !== 'boolean' || typeof value.showPerformance !== 'boolean' ||
      !Number.isFinite(value.volume) || value.volume < 0 || value.volume > 100) return null;
  return {frameCap:value.frameCap, aspect:value.aspect, startFullscreen:value.startFullscreen,
    volume:Math.round(value.volume), showPerformance:value.showPerformance};
}

function loadSettings() {
  try {
    const stored = JSON.parse(localStorage.getItem(STORAGE_KEY));
    if (stored?.version === 1) return validated(stored.settings) || {...DEFAULTS};
  } catch { /* Storage is optional; live settings still work. */ }
  return {...DEFAULTS};
}

const settings = loadSettings();
export function getSettings() { return settings; }

const status = document.querySelector('#status');
const launchStatus = document.querySelector('#launch-status');
const stage = document.querySelector('.stage');
const toggle = document.querySelector('#toolbar-toggle');
const fullscreen = document.querySelector('#fullscreen');
const reopen = document.querySelector('#reopen-launch');
const stop = document.querySelector('#stop');
const toolbarStop = document.querySelector('#toolbar-stop');
const resume = document.querySelector('#return-game');
const run = document.querySelector('#run');
const displayFeedback = document.querySelector('#display-feedback');
const performanceReadout = document.querySelector('#performance');
const dialog = document.querySelector('#settings-dialog');
const cap = document.querySelector('#settings-frame-cap');
const aspect = document.querySelector('#settings-aspect');
const startFullscreen = document.querySelector('#settings-start-fullscreen');
const volume = document.querySelector('#settings-volume');
const volumeValue = document.querySelector('#settings-volume-value');
const showPerformance = document.querySelector('#settings-show-performance');
const apply = document.querySelector('#settings-apply');
const save = document.querySelector('#settings-save');
const cancel = document.querySelector('#settings-cancel');
const close = document.querySelector('#settings-close');
const dialogFullscreen = document.querySelector('#settings-fullscreen');
const dialogStop = document.querySelector('#settings-stop');
const feedback = document.querySelector('#settings-feedback');
let running = false, busy = false, stopping = false, previousFocus;
const gameScreen = () => document.querySelector('#screen');

function applyDisplay() {
  document.documentElement.dataset.aspect = settings.aspect;
  performanceReadout.hidden = !settings.showPerformance;
}

function applySettings(next) {
  const value = validated(next);
  if (!value) return false;
  const changed = fields.some(key => settings[key] !== value[key]);
  Object.assign(settings, value);
  applyDisplay();
  if (changed) window.dispatchEvent(new CustomEvent('fuzion-settings', {detail:{...settings}}));
  return changed;
}

function fillDialog() {
  cap.value = String(settings.frameCap);
  aspect.value = settings.aspect;
  startFullscreen.checked = settings.startFullscreen;
  volume.value = String(settings.volume);
  showPerformance.checked = settings.showPerformance;
  volumeValue.textContent = `${settings.volume}%`;
}

function dialogSettings() {
  return {frameCap:Number(cap.value), aspect:aspect.value, startFullscreen:startFullscreen.checked,
    volume:Number(volume.value), showPerformance:showPerformance.checked};
}

function updateControls() {
  const pending = busy || stopping;
  document.body.classList.toggle('is-loading', busy && !running && !stopping);
  document.body.classList.toggle('is-stopping', stopping);
  document.body.classList.toggle('is-running', running);
  document.body.classList.toggle('dialog-open', dialog.open);
  toggle.disabled = pending;
  toggle.setAttribute('aria-expanded', String(dialog.open));
  run.hidden = running;
  const showingLaunch = running && document.body.classList.contains('show-launch');
  stage.inert = !running || showingLaunch || dialog.open;
  stop.hidden = dialog.open || !(showingLaunch || (busy && !running));
  stop.textContent = showingLaunch ? 'Stop & return' : 'Cancel startup';
  toolbarStop.hidden = !running || dialog.open || showingLaunch;
  toolbarStop.disabled = pending || stop.disabled;
  dialogStop.hidden = !running;
  dialogStop.disabled = pending || stop.disabled;
  resume.hidden = !showingLaunch;
  resume.disabled = pending;
  reopen.hidden = !running || showingLaunch || dialog.open;
  for (const button of [apply,save,cancel,close]) button.disabled = pending;
}

function focusableControls() {
  return [...dialog.querySelectorAll('button,select,input')].filter(element =>
    !element.disabled && !element.closest('[hidden]') && element.getClientRects().length);
}

function openSettings() {
  if (dialog.open || busy || stopping) return;
  previousFocus = document.activeElement;
  fillDialog();
  feedback.textContent = '';
  dialog.showModal();
  updateControls();
  [cap,aspect,...focusableControls()].find(element => !element.disabled)?.focus({preventScroll:true});
}

function closeSettings(restore = true) {
  if (!dialog.open || busy || stopping) return;
  dialog.close();
  fillDialog();
  updateControls();
  if (!restore) return;
  const target = running ? gameScreen() : previousFocus?.isConnected && !previousFocus.hidden ? previousFocus : toggle;
  target?.focus({preventScroll:true});
}

function deniedFullscreen(error) {
  const reason = error?.message ? ` (${error.message})` : '';
  const message = `Fullscreen was unavailable${reason}. The game can use this window or browser fullscreen (F11).`;
  displayFeedback.textContent = message;
  if (dialog.open) feedback.textContent = message;
  return false;
}

function requestFullscreen() {
  try {
    // Invoke immediately, while the click still carries browser activation.
    return Promise.resolve(document.documentElement.requestFullscreen()).then(() => {
      displayFeedback.textContent = '';
      return true;
    }, deniedFullscreen);
  } catch (error) { return Promise.resolve(deniedFullscreen(error)); }
}

export function applyLaunchDisplay() {
  applyDisplay();
  return settings.startFullscreen && !document.fullscreenElement ? requestFullscreen() : Promise.resolve(true);
}

function updateFullscreenLabel() {
  fullscreen.textContent = dialogFullscreen.textContent = document.fullscreenElement ? 'Exit fullscreen' : 'Enter fullscreen';
}

async function toggleFullscreen() {
  if (document.fullscreenElement) {
    try { await document.exitFullscreen(); displayFeedback.textContent = ''; }
    catch (error) { deniedFullscreen(error); }
  } else await requestFullscreen();
  if (!dialog.open && running) gameScreen()?.focus({preventScroll:true});
}

function stopAndReturn() {
  if (busy || stopping || stop.disabled) return;
  closeSettings(false);
  document.body.classList.remove('show-launch');
  stop.click();
}

function moveFocus(direction) {
  const controls = focusableControls();
  if (!controls.length) return;
  const index = controls.indexOf(document.activeElement);
  controls[(index + direction + controls.length) % controls.length].focus({preventScroll:true});
}

function menuCommand(command) {
  if (!dialog.open || busy || stopping) return;
  const active = document.activeElement;
  if (command === 'back') { closeSettings(); return; }
  if (command === 'up' || command === 'down') { moveFocus(command === 'up' ? -1 : 1); return; }
  if (command === 'left' || command === 'right') {
    const direction = command === 'left' ? -1 : 1;
    if (active instanceof HTMLSelectElement && !active.disabled) {
      active.selectedIndex = (active.selectedIndex + direction + active.options.length) % active.options.length;
      active.dispatchEvent(new Event('change', {bubbles:true}));
    } else if (active instanceof HTMLInputElement && !active.disabled) {
      if (active.type === 'range') active.value = String(Math.max(0, Math.min(100, Number(active.value) + direction * 5)));
      else if (active.type === 'checkbox') active.checked = direction > 0;
      active.dispatchEvent(new Event('input', {bubbles:true}));
    } else moveFocus(direction);
    return;
  }
  if (command === 'confirm') {
    if (active instanceof HTMLButtonElement || (active instanceof HTMLInputElement && active.type === 'checkbox')) active.click();
    else moveFocus(1);
  }
}

toggle.addEventListener('click', openSettings);
for (const button of [cancel,close]) button.addEventListener('click', () => closeSettings());
dialog.addEventListener('cancel', event => { event.preventDefault(); closeSettings(); });
dialog.addEventListener('close', () => { document.body.classList.remove('dialog-open'); updateControls(); });
volume.addEventListener('input', () => { volumeValue.textContent = `${volume.value}%`; });
apply.addEventListener('click', () => {
  if (busy || stopping) return;
  feedback.textContent = applySettings(dialogSettings()) ? 'Settings applied.' : 'Settings are up to date.';
});
save.addEventListener('click', () => {
  if (busy || stopping) return;
  applySettings(dialogSettings());
  try {
    localStorage.setItem(STORAGE_KEY, JSON.stringify({version:1, settings:{...settings}}));
    closeSettings();
  } catch { feedback.textContent = 'Settings applied for this session. Browser storage is unavailable.'; }
});
fullscreen.addEventListener('click', toggleFullscreen);
dialogFullscreen.addEventListener('click', toggleFullscreen);
toolbarStop.addEventListener('click', stopAndReturn);
dialogStop.addEventListener('click', stopAndReturn);
document.addEventListener('fullscreenchange', updateFullscreenLabel);
resume.addEventListener('click', () => { document.body.classList.remove('show-launch'); updateControls(); gameScreen()?.focus(); });
reopen.addEventListener('click', () => {
  document.body.classList.toggle('show-launch');
  updateControls();
  if (document.body.classList.contains('show-launch')) resume.focus();
  else gameScreen()?.focus();
});

window.addEventListener('keydown', event => {
  if (event.code === 'F1' || event.code === 'F2') {
    event.preventDefault(); event.stopImmediatePropagation();
    if (event.repeat) return;
    if (event.code === 'F1') { if (dialog.open) closeSettings(); else openSettings(); }
    else {
      applySettings({...settings, showPerformance:!settings.showPerformance});
      showPerformance.checked = settings.showPerformance;
    }
    return;
  }
  if (!dialog.open) return;
  event.stopImmediatePropagation();
  const commands = {ArrowUp:'up', ArrowDown:'down', ArrowLeft:'left', ArrowRight:'right', Enter:'confirm', Escape:'back'};
  if (commands[event.code]) { event.preventDefault(); menuCommand(commands[event.code]); }
}, true);
window.addEventListener('keyup', event => { if (dialog.open || event.code === 'F1' || event.code === 'F2') event.stopImmediatePropagation(); }, true);
window.addEventListener('fuzion-menu-command', event => { if (dialog.open) menuCommand(event.detail); });
window.addEventListener('fuzion-capabilities', event => {
  if (typeof event.detail?.frameCap === 'boolean') {
    cap.disabled = !event.detail.frameCap;
    document.querySelector('#frame-cap-note').hidden = event.detail.frameCap;
  }
  if (typeof event.detail?.multiPad === 'boolean') document.querySelector('#controller-status').textContent = event.detail.multiPad
    ? 'Up to four local gamepad ports are available.' : 'Extra controller ports are unavailable in this runtime.';
});
window.addEventListener('fuzion-busy', event => {
  busy = event.detail?.busy === true;
  stopping = event.detail?.stopping === true;
  updateControls();
});
window.addEventListener('fuzion-state', event => {
  const previous = running;
  running = event.detail?.running === true;
  if (typeof event.detail?.stopping === 'boolean') stopping = event.detail.stopping;
  if (!running) {
    document.body.classList.remove('show-launch');
    if (dialog.open) dialog.close();
  }
  updateControls();
  if (running && !previous && !dialog.open) gameScreen()?.focus({preventScroll:true});
});
new MutationObserver(() => { launchStatus.textContent = status.textContent; }).observe(status, {childList:true,characterData:true,subtree:true});
new MutationObserver(updateControls).observe(stop, {attributes:true,attributeFilter:['disabled']});
fillDialog();
applyDisplay();
updateFullscreenLabel();
updateControls();
