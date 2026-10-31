// Browser event wiring kept injectable so focus/device behavior is exercised in CI.
export function bindBrowserSafety({window,document,getInput,getGamepads,pause}){
  const blur=()=>{getInput().clear();pause('Window lost focus.');};
  const visibility=()=>{if(document.hidden){getInput().clear();pause('Tab was hidden.');}};
  const disconnect=()=>{const input=getInput(),lost=input.pollGamepads(getGamepads());if(lost.length){input.clear();pause('An owned gamepad disconnected.');}};
  window.addEventListener('blur',blur);document.addEventListener('visibilitychange',visibility);window.addEventListener('gamepaddisconnected',disconnect);
  return ()=>{window.removeEventListener('blur',blur);document.removeEventListener('visibilitychange',visibility);window.removeEventListener('gamepaddisconnected',disconnect);};
}
