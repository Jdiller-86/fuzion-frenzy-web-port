const PROFILE={kind:'synthetic-unowned-local-mutant-acquire',synthetic:true,capacity:4};
const LAST=['handle','threadId','object','waitMode','alertable','timeout','ownerBefore','ownerAfter','eaxBefore','espBefore','eaxAfter','espAfter'];
const dword=n=>Number.isInteger(n)&&n>=0&&n<=0xffffffff;
const exact=(v,keys)=>!!v&&typeof v==='object'&&!Array.isArray(v)&&Reflect.ownKeys(v).length===keys.length&&keys.every(k=>Object.hasOwn(v,k));
const fail=()=>{throw new Error('Invalid local mutant acquisition evidence');};
export function parseMutantWaitProfile(value) {
  if(value===undefined)return undefined;
  if(!exact(value,Object.keys(PROFILE))||Object.keys(PROFILE).some(k=>value[k]!==PROFILE[k]))
    throw new Error('Invalid explicit unowned local mutant acquisition profile');
  return {...PROFILE};
}
export function requireMutantWaitCapability(api) {
  for(const name of ['local_mutant_wait_profile','local_mutant_wait_word','local_mutant_profile','local_mutant_word'])
    if(typeof api[`ff_probe_${name}`]!=='function')throw new Error('Missing explicit local mutant acquisition capability');
}
export function selectMutantWait(api) {
  requireMutantWaitCapability(api);
  if(api.ff_probe_local_mutant_wait_profile(1)!==0)throw new Error('Module refused local mutant acquisition profile');
}
export function validateMutantWaitState(state,profile,mutants) {
  const selected=parseMutantWaitProfile(profile);
  if(!selected||!mutants||!exact(state,[...Object.keys(PROFILE),'attempts','completed','status','owners','last'])||
      Object.keys(PROFILE).some(k=>state[k]!==selected[k])||
      !dword(state.attempts)||!dword(state.completed)||state.completed>4||state.attempts>state.completed+1||
      state.attempts<state.completed||![0,0xc0000008,0xc000000d,0xc00000bb].includes(state.status)||
      (state.status===0?state.attempts!==state.completed:state.attempts!==state.completed+1)||
      !Array.isArray(state.owners)||state.owners.length!==mutants.objectCount)fail();
  for(const [i,row]of state.owners.entries()) {
    if(!exact(row,['object','threadId'])||row.object!==i+1||!dword(row.threadId)||row.threadId>4||
        mutants.objects[i].owned!==(row.threadId!==0))fail();
  }
  if(state.completed!==state.owners.filter(o=>o.threadId!==0).length)fail();
  if(!state.attempts){if(state.last!==null)fail();return state;}
  const last=state.last;
  if(!exact(last,LAST)||LAST.some(k=>!dword(last[k]))||last.threadId>4||last.object>mutants.objectCount||
      last.ownerBefore>4||last.ownerAfter>4)fail();
  const handle=mutants.handles.find(h=>h.handle===last.handle);
  if(last.object!==(handle?.object??0)||last.ownerAfter!==(state.owners[last.object-1]?.threadId??0))fail();
  if(state.status===0) {
    if(!last.object||!last.threadId||last.waitMode!==1||last.alertable!==0||last.timeout!==0||
        last.ownerBefore!==0||last.ownerAfter!==last.threadId||last.eaxAfter!==0||
        last.espBefore>0xffffffeb||last.espAfter!==last.espBefore+20)fail();
  } else if(last.ownerBefore!==last.ownerAfter||last.eaxAfter!==last.eaxBefore||last.espAfter!==last.espBefore)fail();
  return state;
}
export function readMutantWaitState(api,profile,mutants) {
  requireMutantWaitCapability(api);
  const word=(index,field)=>{
    const v=api.ff_probe_local_mutant_wait_word(index,field);
    if(!Number.isInteger(v)||v< -0x80000000||v>0xffffffff)fail();
    return v>>>0;
  };
  const h=field=>word(0xffffffff,field);
  if(h(0)!==1||h(4)!==4)fail();
  const state={...parseMutantWaitProfile(profile),attempts:h(1),completed:h(2),status:h(3),
    owners:Array.from({length:mutants.objectCount},(_,i)=>({object:word(i,0),threadId:word(i,1)})),
    last:h(1)?Object.fromEntries(LAST.map((k,i)=>[k,h(i+5)])):null};
  if(!state.attempts&&LAST.some((_,i)=>h(i+5)!==0))fail();
  return validateMutantWaitState(state,profile,mutants);
}
export function validateMutantWaitReport(report,profile,mutants) {
  const selected=parseMutantWaitProfile(profile),marker=(report.experiments??[]).filter(x=>x==='localMutantAcquire');
  const captures=(report.calls??[]).filter(c=>c.ordinal===234);
  if(!selected) {
    if(report.mutantWaitState!==undefined||marker.length||captures.some(c=>c.status===0||c.output!==undefined))fail();
    return;
  }
  if(marker.length!==1||report.phase!=='original-initializer-bringup')fail();
  const state=validateMutantWaitState(report.mutantWaitState,selected,mutants);
  // This first-acquisition proof requires every admitted attempt in its capture.
  if(captures.length!==state.attempts||captures.filter(c=>c.status===0).length!==state.completed)fail();
  for(const row of state.owners)if(row.threadId&&!report.threads?.some(t=>t.id===row.threadId&&t.state===3))fail();
  const acquired=new Map();
  for(const [i,call]of captures.entries()) {
    const out=call.output;
    if(!exact(out,['backend',...LAST])||out.backend!==PROFILE.kind||LAST.some(k=>!dword(out[k]))||
        call.arguments.length!==4||call.arguments.some((v,n)=>v!==out[['handle','waitMode','alertable','timeout'][n]]))fail();
    const thread=report.threads?.find(t=>t.id===out.threadId&&t.state===3);
    if(call.status===0) {
      if(!thread||!dword(thread.stackLow)||!dword(thread.stackTop)||thread.stackLow>=thread.stackTop||thread.stackTop>0x4000000||
          out.ownerBefore!==0||out.ownerAfter!==out.threadId||out.waitMode!==1||out.alertable!==0||out.timeout!==0||
          out.eaxAfter!==0||out.espBefore>0xffffffeb||out.espAfter!==out.espBefore+20||
          out.espBefore<thread.stackLow||out.espAfter>thread.stackTop||
          !mutants.handles.some(h=>h.handle===out.handle&&h.object===out.object)||
          state.owners[out.object-1]?.threadId!==out.threadId)fail();
      if(acquired.has(out.object))fail();
      acquired.set(out.object,out.threadId);
    } else if(i!==captures.length-1||call!==report.calls.at(-1)||call.status!==1||
        report.stop.status!==9||report.stop.ordinal!==234||report.stop.target!==state.status||
        out.ownerAfter!==out.ownerBefore||out.eaxAfter!==out.eaxBefore||out.espAfter!==out.espBefore)fail();
  }
  for(const row of state.owners)if(row.threadId!==(acquired.get(row.object)??0))fail();
  if(state.last&&LAST.some(k=>state.last[k]!==captures.at(-1).output[k]))fail();
}
