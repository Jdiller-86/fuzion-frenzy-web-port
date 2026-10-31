import test from 'node:test';
import assert from 'node:assert/strict';
import {createRound,stepRound,endPractice,courseRate,hazardKind,visibleHazards,makeRules} from '../packages/simulation/twisted-system.mjs';

const commands=state=>Array.from({length:state.players.length},()=>({}));
function perfect(state,id=0){
  const output=commands(state),p=state.players.find(p=>p.id===id);
  if(!p||p.eliminated)return output;
  const ticks=(p.nextHazard-state.coursePhase+p.lag)/courseRate(state.activeTick,state.rules)*state.rules.tickRate;
  output[id]={jumpPressed:hazardKind(state.seed,p.nextHazard)==='low'&&ticks<=12,duckHeld:hazardKind(state.seed,p.nextHazard)==='high'};
  return output;
}
function finish(state,policy=commands,limit=20000){
  for(let i=0;i<limit&&state.phase!=='results';i++)stepRound(state,policy(state));
  assert.equal(state.phase,'results','round must terminate');return state;
}
function boundary(kind='low'){
  const s=createRound({rules:{countdownTicks:0,knockbackPerMiss:0}});
  s.activeTick=100;s.coursePhase=(kind==='low'?1:2)-0.001;s.players[0].nextHazard=kind==='low'?1:2;
  return s;
}

test('countdown ignores actions and activates on the exact configured tick',()=>{
  const s=createRound({rules:{countdownTicks:3}});
  for(let i=0;i<2;i++){stepRound(s,[{jumpPressed:true,duckHeld:true}]);assert.equal(s.phase,'countdown');}
  stepRound(s,[{jumpPressed:true}]);assert.equal(s.phase,'active');assert.equal(s.activeTick,0);assert.equal(s.players[0].jumpStart,null);
  stepRound(s,[{}]);assert.equal(s.activeTick,1);
});

test('jump protection includes both frozen endpoints and rejects adjacent ticks',()=>{
  for(const [age,score]of [[4,0],[5,1],[18,1],[19,0]]){
    const s=boundary();s.players[0].jumpStart=s.activeTick+1-age;stepRound(s,[{}]);
    assert.equal(s.players[0].score,score,`jump age ${age}`);assert.equal(s.players[0].misses,1-score);
  }
});

test('high/low hazards require distinct actions; duck entry and airborne exclusion matter',()=>{
  const high=boundary('high');high.players[0].duckTicks=3;stepRound(high,[{duckHeld:true}]);assert.equal(high.players[0].score,1);
  const early=boundary('high');early.players[0].duckTicks=2;stepRound(early,[{duckHeld:true}]);assert.equal(early.players[0].misses,1);
  const airborne=boundary('high');airborne.players[0].jumpStart=95;stepRound(airborne,[{duckHeld:true}]);assert.equal(airborne.players[0].misses,1);
  const low=boundary();low.players[0].duckTicks=8;stepRound(low,[{duckHeld:true}]);assert.equal(low.players[0].misses,1);
});

test('pulse duck is configurable and cannot be extended by continuously holding',()=>{
  const s=createRound({rules:{countdownTicks:0,duckMode:'pulse'}});
  for(let i=0;i<30;i++)stepRound(s,[{duckHeld:true}]);assert.equal(s.players[0].duckTicks,0);
  stepRound(s,[{duckHeld:false}]);stepRound(s,[{duckHeld:true}]);assert.equal(s.players[0].duckTicks,1);
});

test('a solo no-input run eliminates at seven misses and freezes its score',()=>{
  const s=finish(createRound());assert.equal(s.players[0].misses,7);assert.equal(s.players[0].score,0);
  const snapshot=JSON.stringify(s.result);for(let i=0;i<100;i++)stepRound(s,[{jumpPressed:true}]);assert.equal(JSON.stringify(s.result),snapshot);
});

test('normal two/four-seat rounds finish and simultaneous final misses share the win',()=>{
  for(const seatCount of [2,4]){
    const s=finish(createRound({seatCount}));assert.deepEqual(s.result.winners,Array.from({length:seatCount},(_,id)=>id));
    assert.ok(s.result.standings.every(p=>p.rank===1&&p.score===0&&p.misses===7));
  }
});

test('all contacts in the final tick resolve before selecting winners, regardless of iteration order',()=>{
  const a=boundary(),b=boundary();
  for(const s of [a,b]){
    const second={...s.players[0],id:1};s.players.push(second);s.rules=makeRules({...s.rules,missesToEliminate:1});
    s.players.forEach(p=>{p.jumpStart=95;});
  }
  b.players.reverse();stepRound(a,[{},{}]);stepRound(b,[{},{}]);
  assert.deepEqual(a.players.map(p=>p.score),[1,1]);assert.deepEqual([...b.players].sort((a,b)=>a.id-b.id).map(p=>p.score),[1,1]);
  // Now both miss the same high obstacle: both must receive a shared win.
  for(const s of [a,b]){s.coursePhase=1.999;s.players.forEach(p=>{p.jumpStart=null;p.nextHazard=2;});stepRound(s,[{},{}]);}
  assert.deepEqual(a.result,b.result);assert.deepEqual(a.result.winners,[0,1]);
});

test('practice retains the final survivor and ends without ranked awards',()=>{
  const s=createRound({seatCount:2,practice:true,rules:{countdownTicks:0}});
  for(let i=0;i<3000;i++)stepRound(s,perfect(s));
  assert.equal(s.players[1].eliminated,true);assert.equal(s.players[0].eliminated,false);assert.equal(s.phase,'active');
  assert.equal(endPractice(s),true);finish(s);assert.deepEqual(s.result.winners,[]);assert.equal(s.result.reason,'practice-ended');
});

test('knockback changes the same phase used for contact and does not double-count a rod',()=>{
  const s=boundary();s.rules=makeRules({...s.rules,knockbackPerMiss:0.22});stepRound(s,[{}]);
  assert.equal(s.players[0].nextHazard,2);assert.equal(s.players[0].misses,1);
  while(s.players[0].nextHazard===2)stepRound(s,[{}]);
  assert.ok(s.players[0].lag>=0.22-1e-9);assert.ok(s.coursePhase>=2.22-1e-9);assert.equal(s.players[0].misses,2);
  const visual=visibleHazards(s).find(h=>h.id===2);assert.ok(Math.abs(visual.distance+s.players[0].lag)<0.02);
});

test('recorded semantic commands replay identically; new round has no previous state',()=>{
  const a=createRound({seatCount:4,practice:true,seed:0xabcd}),b=createRound({seatCount:4,practice:true,seed:0xabcd});
  for(let tick=0;tick<6000;tick++){
    const input=Array.from({length:4},(_,id)=>({jumpPressed:(tick+id*13)%71===0,duckHeld:(tick+id*11)%100<30}));
    stepRound(a,input);stepRound(b,input);
  }
  assert.deepEqual(a,b);const next=createRound({seatCount:4,seed:2});assert.ok(next.players.every(p=>p.score===0&&p.misses===0&&p.jumpStart===null));assert.equal(next.coursePhase,0);
});

test('provisional no-miss progression stays within the pre-frozen comparison envelope',()=>{
  const s=createRound({practice:true,rules:{countdownTicks:0}}),expected=[8,16,27,48,70,92];let sample=0;
  for(let tick=1;tick<=3600;tick++){
    stepRound(s,perfect(s));
    if(tick%600===0){assert.equal(s.players[0].misses,0);assert.ok(Math.abs(s.players[0].score-expected[sample])<=(sample<3?2:4));sample++;}
  }
});

test('invalid seat counts, command ownership and rule configurations are rejected',()=>{
  for(const seatCount of [0,5,1.5])assert.throws(()=>createRound({seatCount}));
  for(const rules of [{jumpSafeEnd:24},{duckMode:'automatic'},{finalRate:Infinity},{missesToEliminate:0},{unknown:1}])assert.throws(()=>makeRules(rules));
  assert.throws(()=>stepRound(createRound({seatCount:2}),[{}]));
});
