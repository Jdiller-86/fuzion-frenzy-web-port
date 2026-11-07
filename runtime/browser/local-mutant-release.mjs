import { parseMutantWaitProfile } from './local-mutant-wait.mjs';
import { parseLocalMutantProfile, validateLocalMutantState } from './local-mutants.mjs';

const PROFILE={kind:'synthetic-owned-local-mutant-release',synthetic:true,capacity:4};
const MUTANT_PROFILE={kind:'synthetic-win32-mutants',synthetic:true,capacity:4};
const WAIT_LAST=['handle','threadId','object','waitMode','alertable','timeout','ownerBefore','ownerAfter','eaxBefore','espBefore','eaxAfter','espAfter'];
const RELEASE_LAST=['handle','threadId','object','previousCount','ownerBefore','ownerAfter','eaxBefore','espBefore','eaxAfter','espAfter'];
const MAX_ATTEMPTS=1024;
const STATUSES=new Set([0,0xc0000008,0xc000000d,0xc00000bb]);
const dword=n=>Number.isInteger(n)&&n>=0&&n<=0xffffffff;
const exact=(v,keys)=>!!v&&typeof v==='object'&&!Array.isArray(v)&&Reflect.ownKeys(v).length===keys.length&&keys.every(k=>Object.hasOwn(v,k));
const fail=()=>{throw new Error('Invalid local mutant synchronization evidence');};
const profileKeys=Object.keys(PROFILE);
const sameProfile=(value,selected)=>profileKeys.every(k=>value[k]===selected[k]);
function rawWord(api,name,index,field) {
  const value=api[`ff_probe_local_mutant_${name}_word`](index,field);
  if(!Number.isInteger(value)||value< -0x80000000||value>0xffffffff)fail();
  return value>>>0;
}
function validateState(state,profile,lastFields,mutants,kind) {
  if(!exact(state,[...Object.keys(profile),'attempts','completed','status','owners','last'])||
      Object.keys(profile).some(k=>state[k]!==profile[k])||!dword(state.attempts)||!dword(state.completed)||
      state.attempts>MAX_ATTEMPTS||state.completed>MAX_ATTEMPTS||state.completed>state.attempts||
      state.attempts>state.completed+1||!dword(state.status)||!STATUSES.has(state.status)||
      (state.status===0?state.attempts!==state.completed:state.attempts!==state.completed+1)||
      !Array.isArray(state.owners)||state.owners.length!==mutants.objectCount)fail();
  for(let i=0;i<state.owners.length;i++) {
    const row=state.owners[i];
    if(!exact(row,['object','threadId'])||row.object!==i+1||!dword(row.threadId)||row.threadId>4||
        mutants.objects[i].owned!==(row.threadId!==0))fail();
  }
  if(!state.attempts) { if(state.last!==null)fail(); return state; }
  const last=state.last;
  if(!exact(last,lastFields)||lastFields.some(k=>!dword(last[k]))||last.threadId>4||
      last.object>mutants.objectCount||last.ownerBefore>4||last.ownerAfter>4)fail();
  const handle=mutants.handles.find(h=>h.handle===last.handle);
  if((handle?.object??0)!==last.object)fail();
  if(state.status===0) {
    if(!last.object||!last.threadId||last.ownerBefore===last.ownerAfter)fail();
    if(kind==='wait') {
      if(last.waitMode!==1||last.alertable!==0||last.timeout!==0||last.ownerBefore!==0||
          last.ownerAfter!==last.threadId||last.eaxAfter!==0||last.espBefore>0xffffffeb||
          last.espAfter!==last.espBefore+20)fail();
    } else if(last.previousCount!==0||last.ownerBefore!==last.threadId||last.ownerAfter!==0||
        last.eaxAfter!==0||last.espBefore>0xfffffff3||last.espAfter!==last.espBefore+12)fail();
  } else if(last.ownerBefore!==last.ownerAfter||last.eaxAfter!==last.eaxBefore||last.espAfter!==last.espBefore)fail();
  return state;
}
function readState(api,profile,mutants,name,lastFields) {
  const h=field=>rawWord(api,name,0xffffffff,field);
  if(h(0)!==1||h(4)!==4)fail();
  const attempts=h(1),completed=h(2),status=h(3);
  const owners=Array.from({length:mutants.objectCount},(_,i)=>({object:rawWord(api,name,i,0),threadId:rawWord(api,name,i,1)}));
  const lastWords=lastFields.map((_,i)=>h(i+5));
  if(!attempts&&lastWords.some(v=>v!==0))fail();
  const state={...profile,attempts,completed,status,owners,last:attempts?Object.fromEntries(lastFields.map((k,i)=>[k,lastWords[i]])):null};
  return validateState(state,profile,lastFields,mutants,name==='wait'?'wait':'release');
}
function stable(value) {
  if(Array.isArray(value))return value.map(stable);
  if(value&&typeof value==='object')return Object.fromEntries(Object.keys(value).sort().map(k=>[k,stable(value[k])]));
  return value;
}
function sameValue(a,b) { return JSON.stringify(stable(a))===JSON.stringify(stable(b)); }
function runningThread(report,id) {
  const rows=Array.isArray(report.threads)?report.threads.filter(t=>t?.id===id&&t?.state===3):[];
  if(rows.length!==1)return undefined;
  const t=rows[0];
  if(!dword(t.stackLow)||!dword(t.stackTop)||t.stackLow>=t.stackTop||t.stackTop>0x4000000)return undefined;
  return t;
}
function validCallFrame(out,thread,cleanup) {
  return !!thread&&out.espBefore>=thread.stackLow&&out.espAfter<=thread.stackTop&&
    out.espBefore<=0xffffffff-cleanup&&out.espAfter===out.espBefore+cleanup;
}
function assertLastMatches(state,calls,fields) {
  if(!state.attempts) { if(calls.length||state.last!==null||state.status!==0)fail(); return; }
  if(calls.length!==state.attempts)fail();
  const last=calls.at(-1),output=last?.output;
  if(!output||!state.last||fields.some(k=>state.last[k]!==output[k])||((last.status===0)!==(state.status===0)))fail();
}

export function parseMutantReleaseProfile(value) {
  if(value===undefined)return undefined;
  if(!exact(value,profileKeys)||profileKeys.some(k=>value[k]!==PROFILE[k]))
    throw new Error('Invalid explicit owned local mutant release profile');
  return {...PROFILE};
}
export function requireMutantReleaseCapability(api) {
  for(const name of ['local_mutant_wait_profile','local_mutant_wait_word','local_mutant_profile','local_mutant_word',
    'local_mutant_release_profile','local_mutant_release_word'])
    if(typeof api?.[`ff_probe_${name}`]!=='function')throw new Error('Missing explicit local mutant release capability');
}
export function selectMutantRelease(api) {
  requireMutantReleaseCapability(api);
  if(rawWord(api,'wait',0xffffffff,0)!==1)throw new Error('Local mutant acquisition must be selected before release');
  if(api.ff_probe_local_mutant_release_profile(1)!==0)throw new Error('Module refused local mutant release profile');
}
export function readMutantSynchronizationStates(api,waitProfile,releaseProfile,mutants) {
  requireMutantReleaseCapability(api);
  const wait=parseMutantWaitProfile(waitProfile),release=parseMutantReleaseProfile(releaseProfile);
  if(!wait||!release)throw new Error('Both local mutant acquisition and release profiles are required');
  const selected=parseLocalMutantProfile(MUTANT_PROFILE);
  const mutantState=validateLocalMutantState(mutants,selected,true);
  const mutantWaitState=readState(api,wait,mutantState,'wait',WAIT_LAST);
  const mutantReleaseState=readState(api,release,mutantState,'release',RELEASE_LAST);
  if(!sameValue(mutantWaitState.owners,mutantReleaseState.owners))fail();
  return {mutantWaitState,mutantReleaseState};
}

export function validateMutantReleaseReport(report,releaseProfile,mutants) {
  if(!report||typeof report!=='object'||Array.isArray(report))fail();
  const calls=report.calls??[];
  if(!Array.isArray(calls))fail();
  const selected=parseMutantReleaseProfile(releaseProfile);
  const experiments=report.experiments??[];
  if(!Array.isArray(experiments))fail();
  const releaseMarkers=experiments.filter(x=>x==='localMutantRelease');
  const acquireMarkers=experiments.filter(x=>x==='localMutantAcquire');
  const releaseCalls=calls.filter(c=>c?.ordinal===221);
  if(!selected) {
    if(report.mutantReleaseState!==undefined||releaseMarkers.length||releaseCalls.some(c=>c.status===0||c.output!==undefined))fail();
    return;
  }
  const wait=parseMutantWaitProfile(report.consoleSettings?.mutantWait);
  const consoleRelease=parseMutantReleaseProfile(report.consoleSettings?.mutantRelease);
  if(!wait||!consoleRelease||!sameProfile(consoleRelease,selected)||
      acquireMarkers.length!==1||releaseMarkers.length!==1||report.phase!=='original-initializer-bringup')fail();
  if(!Number.isInteger(report.kernelCapacity)||report.kernelCapacity<=0||report.kernelCapacity>MAX_ATTEMPTS)fail();
  const local=validateLocalMutantState(report.localMutantState,MUTANT_PROFILE,true);
  const supplied=validateLocalMutantState(mutants,MUTANT_PROFILE,true);
  if(!sameValue(local,supplied))fail();
  const waitState=validateState(report.mutantWaitState,wait,WAIT_LAST,local,'wait');
  const releaseState=validateState(report.mutantReleaseState,selected,RELEASE_LAST,local,'release');
  const serviceCalls=calls.filter(c=>c.ordinal===234||c.ordinal===221);
  const waitCalls=calls.filter(c=>c.ordinal===234);
  const relCalls=calls.filter(c=>c.ordinal===221);
  if(serviceCalls.length>MAX_ATTEMPTS||serviceCalls.length>report.kernelCapacity||
      waitCalls.length!==waitState.attempts||relCalls.length!==releaseState.attempts||
      waitCalls.filter(c=>c.status===0).length!==waitState.completed||
      relCalls.filter(c=>c.status===0).length!==releaseState.completed)fail();
  assertLastMatches(waitState,waitCalls,WAIT_LAST);
  assertLastMatches(releaseState,relCalls,RELEASE_LAST);
  // The report may begin after earlier creations, so only enforce ordering for
  // successful handle creations present in this captured prefix.
  const creationIndex=new Map();
  calls.forEach((call,index)=>{
    if(call?.ordinal!==192||call.status!==0||!call.output||call.output.backend!==MUTANT_PROFILE.kind)return;
    const handle=call.output.handle;
    if(!dword(handle)||creationIndex.has(handle))fail();
    creationIndex.set(handle,index);
  });
  const owners=new Map(Array.from({length:local.objectCount},(_,i)=>[i+1,0]));
  for(const [index,call] of serviceCalls.entries()) {
    const isWait=call.ordinal===234, fields=isWait?WAIT_LAST:RELEASE_LAST, state=isWait?waitState:releaseState;
    const expectedBackend=isWait?'synthetic-unowned-local-mutant-acquire':PROFILE.kind;
    const expectedArgs=isWait?['handle','waitMode','alertable','timeout']:['handle','previousCount'];
    const out=call.output;
    if(!exact(out,['backend',...fields])||out.backend!==expectedBackend||fields.some(k=>!dword(out[k]))||
        !Array.isArray(call.arguments)||call.arguments.length!==expectedArgs.length||
        call.arguments.some((v,i)=>v!==out[expectedArgs[i]])||!dword(call.status)||![0,1].includes(call.status))fail();
    if(out.object>local.objectCount||out.threadId>4||out.ownerBefore>4||out.ownerAfter>4)fail();
    const handle=local.handles.find(h=>h.handle===out.handle);
    if((handle?.object??0)!==out.object)fail();
    const createdAt=creationIndex.get(out.handle);
    if(createdAt!==undefined&&createdAt>=calls.indexOf(call))fail();
    const before=owners.get(out.object)??0;
    if(out.ownerBefore!==before)fail();
    if(call.status===0) {
      const thread=runningThread(report,out.threadId);
      const cleanup=isWait?20:12;
      if(!thread||!validCallFrame(out,thread,cleanup)||out.eaxAfter!==0)fail();
      if(isWait) {
        if(!out.object||!out.threadId||before!==0||out.ownerAfter!==out.threadId||
            out.waitMode!==1||out.alertable!==0||out.timeout!==0)fail();
        owners.set(out.object,out.threadId);
      } else {
        if(!out.object||!out.threadId||before!==out.threadId||out.previousCount!==0||out.ownerAfter!==0)fail();
        owners.set(out.object,0);
      }
    } else {
      if(index!==serviceCalls.length-1||call!==calls.at(-1)||report.stop?.status!==9||
          report.stop.ordinal!==call.ordinal||report.stop.target!==state.status||state.status===0||
          out.ownerAfter!==out.ownerBefore||out.eaxAfter!==out.eaxBefore||out.espAfter!==out.espBefore)fail();
      if(out.object===0&&out.ownerBefore!==0)fail();
    }
  }
  for(const row of waitState.owners)if(row.threadId!==(owners.get(row.object)??0))fail();
  for(const row of releaseState.owners)if(row.threadId!==(owners.get(row.object)??0))fail();
  for(const row of local.objects)if(row.owned!==((owners.get(row.object)??0)!==0))fail();
}
