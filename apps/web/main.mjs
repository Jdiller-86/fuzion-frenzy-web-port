import {createRound,stepRound,endPractice,seatAction,hazardKind,courseRate} from '../../packages/simulation/twisted-system.mjs';
import {LocalInput} from '../../packages/input/local-input.mjs';
import {bindBrowserSafety} from '../../packages/input/browser-safety.mjs';
import {createRenderer} from './renderer.mjs';
const $=id=>document.getElementById(id),show=(id,visible)=>$(id).hidden=!visible;
let worker,renderer,assets,input=new LocalInput(1),state,paused=false,running=false,accumulator=0,lastTime=0,posePending=false,lastPose=0,lastHud='',importStart=0,roundSeed=1;
const seatColors=['#4ce678','#ff6560','#659dff','#ff8dce'];
function error(message=''){$('error').textContent=message;}
function pads(){return navigator.getGamepads?.()??[];}
function releaseContent(){
  running=false;state=undefined;worker?.terminate();worker=undefined;renderer?.dispose();renderer=undefined;assets=undefined;input.clear();posePending=false;
  for(const id of ['setup','arena','results','cancel','clear'])show(id,false);$('image').value='';$('image').disabled=false;$('import-status').textContent='Local content cleared. Select a compatible XISO to play.';error();
}
$('clear').onclick=releaseContent;$('cancel').onclick=releaseContent;
$('image').onchange=()=>{
  const file=$('image').files?.[0];if(!file)return;
  releaseContent();$('image').disabled=true;show('cancel',true);$('import-status').textContent='Starting local conversion…';importStart=performance.now();
  worker=new Worker(new URL('./asset-worker.mjs',import.meta.url),{type:'module'});
  worker.onerror=()=>importError('The local conversion worker failed. Clear and reselect your image.');
  worker.onmessage=({data})=>{
    if(data.type==='progress'){$('import-status').textContent=data.text;return;}
    if(data.type==='error'){importError(data.message);return;}
    if(data.type==='poses'){posePending=false;renderer?.updatePoses(data.poses);return;}
    if(data.type==='assets'){
      try{assets=data;renderer=createRenderer($('game'),data);state=createRound();renderer.render(state);show('cancel',false);show('clear',true);show('setup',true);$('image').disabled=false;$('import-status').textContent=`Compatible content ready in ${((performance.now()-importStart)/1000).toFixed(2)} s. Original reactor, rods and Dub loaded locally; ${(data.textureBytes/1048576).toFixed(1)} MiB of decoded textures. No persistent cache.`;renderBindings();}
      catch(e){importError(e.message);}
    }
  };worker.postMessage({type:'import',file});
};
function importError(message){releaseContent();$('import-status').textContent='Content was not loaded. Select the supported uncompressed USA XISO.';error(message);}
function renderBindings(){
  input.pollGamepads(pads());$('bindings').replaceChildren();
  input.bindings.forEach((b,seat)=>{
    const field=document.createElement('fieldset'),legend=document.createElement('legend');legend.textContent=`Player ${seat+1}`;field.append(legend);
    const label=document.createElement('label');label.textContent='Owned device ';const select=document.createElement('select');select.setAttribute('aria-label',`Player ${seat+1} device`);
    const keyboard=document.createElement('option');keyboard.value='keyboard';keyboard.textContent='Keyboard';select.append(keyboard);
    for(const d of input.devices.filter(d=>d.standard)){const option=document.createElement('option');option.value=String(d.index);option.textContent=`Gamepad ${d.index+1}: ${d.id}`;option.selected=b.type==='gamepad'&&b.index===d.index;select.append(option);}
    if(b.type==='gamepad'&&!input.devices.some(d=>d.index===b.index&&d.id===b.id)){const o=document.createElement('option');o.value=String(b.index);o.textContent='Disconnected gamepad';o.selected=true;select.append(o);}
    label.append(select);field.append(label);
    select.onchange=()=>{try{if(select.value==='keyboard'){const keys=[['Space','ArrowDown'],['KeyW','KeyS'],['KeyI','KeyK'],['Numpad8','Numpad5']][seat];input.assignKeyboard(seat,...keys);}else{const d=input.devices.find(d=>d.index===Number(select.value));input.assignGamepad(seat,d.index,d.id);}error();}catch(e){error(e.message);}renderBindings();};
    if(b.type==='keyboard')for(const [action,code]of [['jump',b.jump],['duck',b.duck]]){const l=document.createElement('label');l.textContent=action==='jump'?'Jump key ':'Duck key ';const key=document.createElement('input');key.value=code;key.readOnly=true;key.setAttribute('aria-label',`Player ${seat+1} ${action} key`);key.title='Focus here and press a new key';key.onkeydown=e=>{if(e.code==='Tab')return;e.preventDefault();e.stopPropagation();try{input.assignKeyboard(seat,action==='jump'?e.code:b.jump,action==='duck'?e.code:b.duck);error();renderBindings();}catch(ex){error(ex.message);}};l.append(key);field.append(l);}
    else{const p=document.createElement('span');p.textContent='A / bottom: Jump · B / right: Duck';field.append(p);}
    $('bindings').append(field);
  });
  $('device-status').textContent=input.ready()?'All seats have an owned device.':'Reconnect or reassign the missing controller before starting.';
}
$('seat-count').onchange=()=>{input.clear();input=new LocalInput(Number($('seat-count').value));renderBindings();};$('refresh-devices').onclick=renderBindings;
function start(practice=false){
  input.pollGamepads(pads());if(!assets||!input.ready()){error('Local content and connected owned devices are required.');return;}
  input.clear();state=createRound({seatCount:input.bindings.length,practice,seed:roundSeed++});running=true;paused=false;accumulator=0;lastTime=performance.now();lastPose=0;lastHud='';error();
  show('setup',false);show('results',false);show('arena',true);show('overlay',false);show('end-practice',practice);$('pause').disabled=false;$('pause').focus();updateHud();
}
$('play').onclick=()=>start();$('practice').onclick=()=>start(true);$('rematch').onclick=()=>start(state?.practice??false);
function instructions(){running=false;paused=false;input.clear();show('arena',false);show('results',false);show('setup',!!assets);renderBindings();$('play').focus();}
$('instructions').onclick=instructions;$('back').onclick=instructions;
function pause(reason='Round paused'){
  if(!running||paused||state.phase==='results')return;paused=true;input.clear();accumulator=0;show('overlay',true);$('overlay-title').textContent='Paused';$('overlay-text').textContent=reason+' Release held controls before resuming.';$('resume').focus();
}
$('pause').onclick=()=>pause();$('resume').onclick=()=>{input.pollGamepads(pads());if(!input.ready()){error('Reconnect the owned controller, or return to instructions to reassign it.');return;}input.clear();paused=false;accumulator=0;lastTime=performance.now();show('overlay',false);$('pause').focus();error();};
$('end-practice').onclick=()=>{if(state?.practice){endPractice(state);paused=false;show('overlay',false);input.clear();}};
bindBrowserSafety({window,document,getInput:()=>input,getGamepads:pads,pause});
window.addEventListener('keydown',e=>{
  if(e.code==='Escape'&&running){e.preventDefault();pause();return;}
  const editable=e.target instanceof HTMLElement&&(e.target.matches('input,select,textarea')||e.target.isContentEditable);
  if(running&&!paused&&input.keyDown(e.code,{repeat:e.repeat,editable}))e.preventDefault();
});window.addEventListener('keyup',e=>input.keyUp(e.code));
function updateHud(){
  if(!state)return;
  const actions=state.players.map(p=>{const a=seatAction(state,p);return p.eliminated?'OUT':a.jumping?'Jumping':a.ducking?'Ducking':'Running';});
  const key=JSON.stringify([state.phase,state.players.map((p,i)=>[p.score,p.misses,p.eliminated,p.lastEvent?.type,actions[i]])]);
  if(key!==lastHud){lastHud=key;$('hud').replaceChildren(...state.players.map((p,i)=>{const div=document.createElement('div');div.className='score';div.style.setProperty('--seat',seatColors[p.id]);const title=document.createElement('span');title.textContent=`P${p.id+1} · `;const score=document.createElement('strong');score.textContent=String(p.score);const line=document.createElement('small');line.textContent=`${p.misses} / ${state.rules.missesToEliminate} misses · ${actions[i]}${p.lastEvent?' · '+p.lastEvent.type:''}`;div.append(title,score,line);return div;}));}
  $('phase').textContent=`${state.practice?'Practice':'Round'} · ${state.phase} · ${(state.activeTick/state.rules.tickRate).toFixed(1)} s`;
  $('countdown').textContent=state.phase==='countdown'?String(Math.ceil((state.rules.countdownTicks-state.phaseTick)/state.rules.tickRate)):state.phase==='resolving'?'Round over':'';
  $('cue').textContent=state.phase==='active'?state.players.filter(p=>!p.eliminated).map(p=>{const kind=hazardKind(state.seed,p.nextHazard),seconds=(p.nextHazard-state.coursePhase+p.lag)/courseRate(state.activeTick,state.rules);return `P${p.id+1}: ${kind==='low'?'JUMP · blue':'DUCK · orange'} in ${Math.max(0,seconds).toFixed(2)} s`;}).join('   /   '):'Watch the rods. Jump low blue; duck high orange.';
}
function results(){
  running=false;input.clear();show('arena',false);show('results',true);
  const r=state.result;$('result-title').textContent=r.practice?'Practice complete':state.players.length===1?'Solo run complete':r.winners.length===1?`Player ${r.winners[0]+1} wins`:`Shared win · ${r.winners.map(id=>'P'+(id+1)).join(' + ')}`;
  $('result-reason').textContent=(r.practice?'Unranked practice. ':r.winners.length>1?'Equal best clearance scores after simultaneous elimination. ':'')+`${r.reason==='last-standing'?'Last player standing.':r.reason==='all-eliminated'?'All players eliminated.':'Practice ended.'} Scores are now final.`;
  $('standings').replaceChildren(...r.standings.map(p=>{const tr=document.createElement('tr');for(const value of [p.rank,`Player ${p.id+1}`,p.score,p.misses]){const td=document.createElement('td');td.textContent=String(value);tr.append(td);}return tr;}));$('rematch').focus();
}
function clipTime(p){
  const action=seatAction(state,p);let id=631728079,time=state.activeTick;
  if(p.eliminated){id=2161363645;time=state.tick-p.eliminatedTick;}
  else if(state.phase==='resolving'||state.phase==='results'){id=state.result?.winners.includes(p.id)?1546342527:1223468090;time=state.phaseTick;}
  else if(action.jumping){id=3022054678;time=action.jumpAge/state.rules.jumpTicks*20;}
  else if(state.phase==='countdown'){id=1342060810;time=state.tick;}
  const clip=assets.clips.find(c=>c.id===id);if(!clip)return 123;
  return clip.start+time%Math.max(1,clip.end-clip.start);
}
function frame(now){
  try{
    const lost=input.pollGamepads(pads());if(lost.length)pause('An owned gamepad disconnected.');
    if(running&&!paused){const elapsed=(now-lastTime)/1000;if(elapsed>.25)pause('Rendering was interrupted.');else accumulator+=Math.max(0,elapsed);while(!paused&&accumulator>=1/state.rules.tickRate){stepRound(state,input.consume());accumulator-=1/state.rules.tickRate;if(state.phase==='results'){results();break;}}updateHud();}
    lastTime=now;
    if(renderer&&state){renderer.render(state);if(!posePending&&now-lastPose>33){posePending=true;lastPose=now;worker.postMessage({type:'pose',times:state.players.map(clipTime)});}}
  }catch(e){pause('Rendering or input failed.');error(e.message);}
  requestAnimationFrame(frame);
}
requestAnimationFrame(frame);
