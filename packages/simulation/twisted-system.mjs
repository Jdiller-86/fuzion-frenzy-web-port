// TS-R1: independently written offline rules. No DOM, clock, rendering or transport.
// Reference evidence and unverified reconstruction parameters: docs/TWISTED_SYSTEM_REFERENCE.md.
export const DEFAULT_RULES = Object.freeze({
  tickRate:60, countdownTicks:180, resolveTicks:90, missesToEliminate:7,
  jumpTicks:24, jumpSafeStart:5, jumpSafeEnd:18,
  duckEntryTicks:4, duckPulseTicks:18, duckMode:'hold', hitRecoveryTicks:6,
  initialRate:0.8, finalRate:2.2, rampStartSeconds:25, rampSeconds:5,
  knockbackPerMiss:0.22, knockbackStep:0.04
});

export function makeRules(overrides={}) {
  for(const key of Object.keys(overrides))if(!(key in DEFAULT_RULES))throw new Error(`Unknown rule: ${key}`);
  const r={...DEFAULT_RULES,...overrides};
  for(const key of ['tickRate','countdownTicks','resolveTicks','missesToEliminate','jumpTicks','jumpSafeStart','jumpSafeEnd','duckEntryTicks','duckPulseTicks','hitRecoveryTicks'])
    if(!Number.isSafeInteger(r[key])||r[key]<0)throw new Error(`Invalid integer rule: ${key}`);
  if(r.tickRate<30||r.tickRate>120||r.missesToEliminate<1||r.missesToEliminate>20||r.jumpTicks<2||r.jumpSafeStart>r.jumpSafeEnd||r.jumpSafeEnd>=r.jumpTicks||r.duckEntryTicks<1||r.duckPulseTicks<r.duckEntryTicks)throw new Error('Invalid action rules');
  for(const key of ['initialRate','finalRate','rampStartSeconds','rampSeconds','knockbackPerMiss','knockbackStep'])if(!Number.isFinite(r[key]))throw new Error(`Invalid numeric rule: ${key}`);
  if(r.initialRate<=0||r.finalRate<r.initialRate||r.finalRate>r.tickRate/4||r.rampStartSeconds<0||r.rampSeconds<=0||r.knockbackPerMiss<0||r.knockbackPerMiss>0.5||r.knockbackStep<=0||r.knockbackStep>1||!['hold','pulse'].includes(r.duckMode))throw new Error('Invalid progression rules');
  return Object.freeze(r);
}

export function hazardKind(seed,id) {
  if(!Number.isSafeInteger(id)||id<1)throw new Error('Invalid hazard ID');
  if(id===1)return 'low';if(id===2)return 'high';
  let x=((seed>>>0)^Math.imul(id,0x9e3779b9))>>>0;
  x^=x>>>16;x=Math.imul(x,0x7feb352d);x^=x>>>15;x=Math.imul(x,0x846ca68b);x^=x>>>16;
  return (x&1)?'low':'high';
}

export function courseRate(activeTick,rules=DEFAULT_RULES) {
  const time=activeTick/rules.tickRate;
  const fraction=Math.max(0,Math.min(1,(time-rules.rampStartSeconds)/rules.rampSeconds));
  return rules.initialRate+(rules.finalRate-rules.initialRate)*fraction;
}

export function createRound({seatCount=1,practice=false,seed=1,rules={}}={}) {
  if(!Number.isSafeInteger(seatCount)||seatCount<1||seatCount>4||!Number.isInteger(seed))throw new Error('Invalid local round');
  const configuration=makeRules(rules);
  return {version:1,rules:configuration,seed:seed>>>0,practice:!!practice,
    phase:configuration.countdownTicks?'countdown':'active',tick:0,activeTick:0,phaseTick:0,coursePhase:0,
    players:Array.from({length:seatCount},(_,id)=>({id,score:0,misses:0,eliminated:false,eliminatedTick:null,
      nextHazard:1,lag:0,jumpStart:null,duckTicks:0,duckPulseStart:null,previousDuck:false,recoveryUntil:0,lastEvent:null})),
    events:[],result:null};
}

export function seatAction(state,player) {
  const age=player.jumpStart===null?null:state.activeTick-player.jumpStart;
  return {jumpAge:age,jumping:age!==null&&age<state.rules.jumpTicks,
    jumpingSafely:age!==null&&age>=state.rules.jumpSafeStart&&age<=state.rules.jumpSafeEnd,
    ducking:age===null&&player.duckTicks>=state.rules.duckEntryTicks,
    recovering:state.activeTick<player.recoveryUntil};
}

function resolveRound(state,reason) {
  const sorted=[...state.players].sort((a,b)=>b.score-a.score||a.id-b.id);
  let rank=0,previous;
  const standings=sorted.map((p,i)=>{
    if(p.score!==previous)rank=i+1;previous=p.score;
    return Object.freeze({id:p.id,score:p.score,misses:p.misses,eliminated:p.eliminated,rank});
  });
  const alive=state.players.filter(p=>!p.eliminated);
  const winners=state.practice?[]:alive.length===1?[alive[0].id]:standings.filter(p=>p.rank===1).map(p=>p.id).sort((a,b)=>a-b);
  state.result=Object.freeze({practice:state.practice,reason,winners:Object.freeze(winners),standings:Object.freeze(standings)});
  state.phase=state.rules.resolveTicks?'resolving':'results';state.phaseTick=0;
  state.events.push({type:'resolved',reason});
}

export function endPractice(state) {
  if(!state.practice||!['countdown','active'].includes(state.phase))return false;
  state.events=[];resolveRound(state,'practice-ended');return true;
}

export function stepRound(state,commands=state.players.map(()=>({}))) {
  if(!Array.isArray(commands)||commands.length!==state.players.length)throw new Error('One command per owned seat required');
  state.events=[];
  if(state.phase==='results')return state;
  state.tick++;state.phaseTick++;
  if(state.phase==='countdown'){
    if(state.phaseTick>=state.rules.countdownTicks){state.phase='active';state.phaseTick=0;state.events.push({type:'started'});}
    return state;
  }
  if(state.phase==='resolving'){
    if(state.phaseTick>=state.rules.resolveTicks){state.phase='results';state.events.push({type:'results'});}
    return state;
  }
  if(state.phase!=='active')throw new Error('Invalid round phase');
  state.activeTick++;state.coursePhase+=courseRate(state.activeTick,state.rules)/state.rules.tickRate;
  for(const p of state.players){
    if(p.eliminated)continue;
    const command=commands[p.id]??{},duck=command.duckHeld===true;
    if(p.jumpStart!==null&&state.activeTick-p.jumpStart>=state.rules.jumpTicks)p.jumpStart=null;
    if(command.jumpPressed===true&&p.jumpStart===null&&state.activeTick>=p.recoveryUntil){p.jumpStart=state.activeTick;state.events.push({type:'jump',seat:p.id});}
    if(p.jumpStart!==null||state.activeTick<p.recoveryUntil){p.duckTicks=0;p.duckPulseStart=null;}
    else if(state.rules.duckMode==='hold')p.duckTicks=duck?p.duckTicks+1:0;
    else {
      if(duck&&!p.previousDuck)p.duckPulseStart=state.activeTick;
      const age=p.duckPulseStart===null?null:state.activeTick-p.duckPulseStart;
      p.duckTicks=age!==null&&age<state.rules.duckPulseTicks?age+1:0;
    }
    p.previousDuck=duck;
    const target=p.misses*state.rules.knockbackPerMiss;
    p.lag=Math.min(target,p.lag+state.rules.knockbackStep);
    const progress=state.coursePhase-p.lag;
    while(progress+1e-9>=p.nextHazard&&!p.eliminated){
      const id=p.nextHazard++,kind=hazardKind(state.seed,id),action=seatAction(state,p);
      const clear=kind==='low'?action.jumpingSafely:action.ducking;
      if(clear)p.score++;
      else {
        p.misses++;p.recoveryUntil=state.activeTick+state.rules.hitRecoveryTicks;
        p.jumpStart=null;p.duckTicks=0;p.duckPulseStart=null;
        if(p.misses>=state.rules.missesToEliminate){p.eliminated=true;p.eliminatedTick=state.tick;}
      }
      p.lastEvent={type:clear?'clear':'miss',hazard:id,kind,tick:state.tick};
      state.events.push({...p.lastEvent,seat:p.id});
      if(p.eliminated)state.events.push({type:'eliminated',seat:p.id,tick:state.tick});
    }
  }
  const alive=state.players.filter(p=>!p.eliminated).length;
  if(!alive)resolveRound(state,'all-eliminated');
  else if(!state.practice&&state.players.length>1&&alive===1)resolveRound(state,'last-standing');
  return state;
}

export function visibleHazards(state,ahead=7) {
  const first=Math.max(1,Math.floor(state.coursePhase-state.rules.missesToEliminate*state.rules.knockbackPerMiss)-1);
  const last=Math.ceil(state.coursePhase)+ahead;
  return Array.from({length:last-first+1},(_,i)=>({id:first+i,kind:hazardKind(state.seed,first+i),distance:first+i-state.coursePhase}));
}
