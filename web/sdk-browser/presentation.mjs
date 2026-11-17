const status = document.querySelector('#status');
const toolbar = document.querySelector('.toolbar');
const toggle = document.querySelector('#toolbar-toggle');
const actions = document.querySelector('#toolbar-actions');
const fullscreen = document.querySelector('#fullscreen');
const reopen = document.querySelector('#reopen-launch');
const stop = document.querySelector('#stop');
const toolbarStop = document.querySelector('#toolbar-stop');
const gameScreen = () => document.querySelector('#screen');
const displayFeedback = document.querySelector('#display-feedback');
const launch = document.querySelector('#launch-card');
const details = document.querySelector('#run-details');
const launchStatus = document.querySelector('#launch-status');
let wasRunning = false;
new MutationObserver(() => {
  launchStatus.textContent = status.textContent;
}).observe(status,{childList:true,characterData:true,subtree:true});
window.addEventListener('fuzion-busy',event => {
  document.body.classList.toggle('is-loading',event.detail?.busy===true);
});

function updateFullscreenLabel() {
  fullscreen.textContent = document.fullscreenElement ? 'Exit fullscreen' : 'Enter fullscreen';
}

fullscreen.addEventListener('click', async () => {
  try {
    if (document.fullscreenElement) await document.exitFullscreen();
    else await document.documentElement.requestFullscreen();
    displayFeedback.textContent = '';
  } catch (error) {
    displayFeedback.textContent = `Fullscreen unavailable: ${error?.message || 'browser denied the request.'} The game still fills this window. You can also use F11.`;
    details.open = true;
  } finally {
    gameScreen().focus();
  }
});
document.addEventListener('fullscreenchange', updateFullscreenLabel);

toggle.addEventListener('click', () => {
  const open = toggle.getAttribute('aria-expanded') !== 'true';
  toggle.setAttribute('aria-expanded', String(open));
  toolbar.classList.toggle('is-open', open);
});
window.addEventListener('keydown', event => {
  if (event.code === 'F1') {
    event.preventDefault();
    toolbar.classList.add('is-open');toggle.setAttribute('aria-expanded','true');fullscreen.focus();
  } else if (event.code === 'Escape' && toolbar.contains(document.activeElement)) {
    event.preventDefault();toolbar.classList.remove('is-open');toggle.setAttribute('aria-expanded','false');gameScreen().focus();
  }
});

reopen.addEventListener('click', () => {
  document.body.classList.toggle('show-launch');
  const visible = document.body.classList.contains('show-launch');
  reopen.textContent = visible ? 'Hide launch panel' : 'Launch panel';
  if (visible) launch.querySelector('#iso').focus();
});

toolbarStop.addEventListener('click', () => stop.click());

window.addEventListener('fuzion-state', event => {
  const running = event.detail?.running === true;
  if (running && !wasRunning) gameScreen().focus();
  document.body.classList.toggle('is-running', running);
  reopen.hidden = !running;
  toolbarStop.hidden = !running;
  if (!running) document.body.classList.remove('show-launch');
  wasRunning = running;
});
updateFullscreenLabel();

// F11 remains the browser's native fullscreen shortcut.
