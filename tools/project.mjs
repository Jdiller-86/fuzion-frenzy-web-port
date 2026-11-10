#!/usr/bin/env node
// Repository-scoped issue/milestone tracking. It never runs an implementation agent.
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import {execFileSync} from 'node:child_process';
import {fileURLToPath,pathToFileURL} from 'node:url';
import {loadBrowserWork,validateBrowserWork,renderNativeBrowserWork,syncGoalDocuments} from './project-goals.mjs';
import {validateBrief,renderDeliveryPlan,upsertDeliveryPlan,renderWorkPackages} from './project-issues.mjs';

export const ROOT=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
export const REPOSITORY='Jdiller-86/fuzion-frenzy-web-port';
export const STATUSES=new Map([
  ['planned','Planned'],['in-progress','In progress'],['blocked','Blocked'],
  ['awaiting-evidence','Awaiting evidence'],['review','Ready for review'],['accepted','Accepted']
]);
const COLORS={'milestone':'5319e7','work-item':'0052cc','work:implementation':'0052cc','work:evidence':'0e8a16',
  'work:package':'0052cc','work:delivery':'0366d6',
  'size:S':'c2e0c6','size:M':'bfd4f2','size:L':'d4c5f9','size:S-M':'c2e0c6','size:M-L':'d4c5f9',
  'area:runtime':'1d76db','area:content':'0366d6','area:online':'006b75','area:experience':'c5def5','area:release':'f9d0c4'};
const AREAS=['runtime','content','online','experience','release'];
const START='<!-- fuzion:managed:start -->',END='<!-- fuzion:managed:end -->';
const marker=id=>`<!-- fuzion:tracking:${id} -->`;
const issueUrl=number=>`https://github.com/${REPOSITORY}/issues/${number}`;

export function deliveryDefinition(item,task,milestone,index){
  return {id:item.id,title:item.title,type:'delivery',area:AREAS[index],kind:task.kind,parent:task.id,milestone:milestone.id,
    scope:item.doneWhen,size:item.size,criteria:[{id:item.id+'-exit',text:item.doneWhen}],
    dependsOn:[...new Set([...task.dependsOn,...(item.dependsOn??[])])],...(item.issueNumber?{issueNumber:item.issueNumber}:{}),...(item.firstStep?{firstStep:item.firstStep}:{}),parentBrief:task.brief};
}
const issueTitle=def=>def.type==='milestone'?def.id.toUpperCase()+': '+def.title:def.type==='delivery'?'['+def.id+'] '+def.title:def.title;

export function validatePlan(plan){
  assert.equal(plan.schemaVersion,1,'Unsupported project schema');
  assert.equal(plan.repository,REPOSITORY,'Project repository mismatch');
  assert.equal(plan.milestones?.length,5,'The plan must define five milestones');
  assert.equal(plan.reference?.repository,'Jdiller-86/CTR-2-WUP','Unexpected reference repository');
  assert.match(plan.reference.commit,/^[a-f0-9]{40}$/,'Invalid reference revision');
  assert.equal(plan.reference.path,'plans.md','Unexpected reference plan');
  if(plan.browserWork)validateBrowserWork(plan.browserWork);
  const nodes=new Map();
  function add(item){
    assert.match(item.id,/^m[1-5](?:-[a-z0-9-]+)?$/,'Invalid tracking ID');
    assert.ok(!nodes.has(item.id),'Duplicate tracking ID');nodes.set(item.id,item);
    for(const key of ['title'])assert.ok(typeof item[key]==='string'&&item[key].length>0&&item[key].length<200,'Invalid title');
    assert.ok(Array.isArray(item.criteria)&&item.criteria.length>0&&item.criteria.length<=20,'Invalid acceptance criteria');
    const ids=new Set();
    for(const check of item.criteria){assert.match(check.id,/^[a-z0-9-]+$/);assert.ok(!ids.has(check.id),'Duplicate acceptance ID');ids.add(check.id);assert.ok(typeof check.text==='string'&&check.text.length>0&&check.text.length<=2500,'Invalid acceptance text');}
    assert.ok(Array.isArray(item.dependsOn),'Dependencies must be explicit');
    if(item.issueNumber!==undefined)assert.ok(Number.isSafeInteger(item.issueNumber)&&item.issueNumber>0,'Invalid issue number');
  }
  plan.milestones.forEach((m,index)=>{
    assert.equal(m.id,`m${index+1}`);assert.ok(typeof m.foundation==='string'&&typeof m.goal==='string');add(m);
    assert.ok(Array.isArray(m.tasks)&&m.tasks.length>0&&m.tasks.length<=20,'Invalid work breakdown');
    m.tasks.forEach(t=>{assert.ok(['implementation','evidence'].includes(t.kind));assert.ok(typeof t.scope==='string');assert.ok(t.id.startsWith(m.id+'-'),'Task is outside its milestone');add(t);validateBrief(t.brief,t.id);for(const item of t.brief.deliverables)add(deliveryDefinition(item,t,m,index));});
  });
  for(const item of plan.browserWork?.items??[]){assert.ok(plan.milestones[0].tasks.some(task=>task.id===item.workPackage),'Browser work package is missing from M1');assert.equal(nodes.get(item.deliveryIssue)?.parent,item.workPackage,'Browser connection points to a missing or foreign delivery issue');}
  const usedNumbers=new Set();
  for(const item of nodes.values()){
    assert.equal(new Set(item.dependsOn).size,item.dependsOn.length,'Duplicate dependency');
    for(const id of item.dependsOn)assert.ok(nodes.has(id)&&id!==item.id,'Unknown or self dependency');
    if(item.issueNumber){assert.ok(!usedNumbers.has(item.issueNumber),'An issue is assigned twice');usedNumbers.add(item.issueNumber);}
  }
  const visiting=new Set(),done=new Set();
  function visit(id){assert.ok(!visiting.has(id),'Dependency cycle');if(done.has(id))return;visiting.add(id);for(const dep of nodes.get(id).dependsOn)visit(dep);visiting.delete(id);done.add(id);}
  for(const id of nodes.keys())visit(id);
  return plan;
}
export function loadPlan(){return validatePlan({...JSON.parse(fs.readFileSync(path.join(ROOT,'profiles/project.json'),'utf8')),browserWork:loadBrowserWork()});}
export function definitions(plan){return plan.milestones.flatMap((m,index)=>[{...m,type:'milestone',area:AREAS[index],milestone:m.id},...m.tasks.flatMap(t=>[{...t,type:'task',area:AREAS[index],parent:m.id,milestone:m.id,...(t.id==='m1-build'&&plan.browserWork?{browserWork:plan.browserWork}:{})},...t.brief.deliverables.map(item=>deliveryDefinition(item,t,m,index))])]);}
export function renderMilestones(plan){
  validatePlan(plan);
  const ref=plan.reference;
  const link=m=>m.issueNumber?issueUrl(m.issueNumber):`https://github.com/${REPOSITORY}/milestones`;
  let text='# Project milestones\n\nFive verifiable outcomes cover the project. The structure follows [CTR-2-WUP’s plan](https://github.com/'+ref.repository+'/blob/'+ref.commit+'/'+ref.path+'#milestones): existing foundation, remaining outcome, explicit acceptance, and linked work. [The roadmap](../plans.md) retains the full requirements.\n\n';
  text+='This plan replaces the old M1–M12 runtime ladder. These M1–M5 IDs name overall-project milestones; old startup/policy/repair checkpoints are supporting evidence. None of the five was accepted at the replanning baseline. Live status and review records belong to the linked GitHub issues, not a commit-count percentage.\n\n';
  text+='| Milestone | Existing foundation | Remaining outcome |\n| --- | --- | --- |\n';
  for(const m of plan.milestones)text+=`| [${m.id.toUpperCase()}: ${m.title}](${link(m)}) | ${m.foundation} | ${m.goal} |\n`;
  text+='\n## Acceptance rules\n\nA milestone passes only when every criterion below has evidence tied to the same supported build, source revision and tested environment, followed by an explicit review/acceptance record. Missing, unknown or not-yet-reached requirements remain open. Task closure, compilation and standalone fixtures do not accept a milestone.\n\nRetain the control/experiment reports and independent audit for runtime changes. Evaluate control invariance at its own declared stop; the playable candidate must demonstrate the full scenario. Keep original media, derived title code, RAM, saves and raw/private evidence local. Publish curated outcomes and evidence identities.\n\nWork can overlap, but acceptance follows the dependencies below. The roadmap retains mobile/touch as later scope: desktop comes first, and later device support passes the same applicable input, fidelity and release checks before being advertised. No implementation task starts automatically from this plan.\n';
  for(const m of plan.milestones){
    text+=`\n## ${m.id.toUpperCase()} — ${m.title}\n\n**Pass when:**\n\n`;
    for(const c of m.criteria)text+='- '+c.text+'\n';
    text+='\n**Acceptance depends on:** '+(m.dependsOn.length?m.dependsOn.map(x=>x.toUpperCase()).join(', '):'no earlier milestone')+'.\n';
    text+='\n**Work packages:** '+m.tasks.map(t=>(t.issueNumber?`[${t.title}](${issueUrl(t.issueNumber)})`:t.title)+` ([delivery brief](work-packages.md#${t.id}))`).join('; ')+'.\n';
    text+='\n**Tracked delivery issues:** '+m.tasks.reduce((total,task)=>total+task.brief.deliverables.length,0)+' child issues inside these packages. Native delivery status, package status and milestone acceptance are reported separately; counts are not remaining-effort estimates.\n';
    if(m.id==='m1')text+='\n**Detailed browser work:** [Named connections, structural sizes and evidence](m1-browser-work.md), with the [largest-first implementation order](m1-browser-work.md#largest-first-implementation-order). These scope rows decompose the work packages; they are not extra forecast packages.\n';
  }
  text+='\n[Management workflow](management.md) describes status, native parent/dependency links and the evidence required before acceptance. Scope is maintained in [profiles/project.json](../profiles/project.json); current runtime facts remain in [status](status.md).\n';
  return text;
}

export class GitHub{
  constructor({token=process.env.GH_TOKEN??process.env.GITHUB_TOKEN,fetchImpl=fetch,now=()=>Date.now(),pause=ms=>new Promise(resolve=>setTimeout(resolve,ms))}={}){this.token=token;this.fetch=fetchImpl;this.requestNow=now;this.requestPause=pause;this.nextWriteAt=0;this.writeQueue=Promise.resolve();}
  async request(method,endpoint,payload){
    assert.ok(endpoint===`/repos/${REPOSITORY}`||endpoint.startsWith(`/repos/${REPOSITORY}/`)||endpoint==='/graphql','GitHub request is outside this repository');
    const mutation=method!=='GET'&&!(endpoint==='/graphql'&&/^\s*query\b/.test(payload?.query??''));
    if(mutation){const slot=this.writeQueue.then(async()=>{const delay=Math.max(0,this.nextWriteAt-this.requestNow());if(delay)await this.requestPause(delay);this.nextWriteAt=this.requestNow()+1000;});this.writeQueue=slot.catch(()=>{});await slot;}
    const headers={'Accept':'application/vnd.github+json','X-GitHub-Api-Version':'2022-11-28','Content-Type':'application/json'};
    if(this.token)headers.Authorization=`Bearer ${this.token}`;
    const response=await this.fetch('https://api.github.com'+endpoint,{method,headers,body:payload===undefined?undefined:JSON.stringify(payload),signal:AbortSignal.timeout(30000)});
    if(!response.ok)throw new Error(`GitHub ${method} failed (HTTP ${response.status}); check repository tracking permissions for ${endpoint.split('?')[0]}.`);
    return response.status===204?null:await response.json();
  }
  async list(endpoint){
    const rows=[];
    for(let page=1;page<=20;page++){
      const data=await this.request('GET',endpoint+(endpoint.includes('?')?'&':'?')+`per_page=100&page=${page}`);
      assert.ok(Array.isArray(data)&&data.length<=100,'Invalid GitHub page');rows.push(...data);
      if(data.length<100)return rows;
    }
    throw new Error('Tracking exceeds the 2,000-item page budget');
  }
  async graphql(query,variables){const r=await this.request('POST','/graphql',{query,variables});assert.ok(r?.data&&!r.errors,'GitHub issue relationship request failed');return r.data;}
  async relations(issue){
    const data=await this.graphql('query($id:ID!){node(id:$id){... on Issue {parent{id} blockedBy(first:100){nodes{id} pageInfo{hasNextPage}}}}}',{id:issue.node_id});
    assert.ok(data.node&&data.node.blockedBy&&!data.node.blockedBy.pageInfo.hasNextPage,'Invalid or excessive issue dependencies');
    return {parent:data.node.parent?.id??null,blockedBy:data.node.blockedBy.nodes.map(n=>n.id)};
  }
  async addParent(parent,child){await this.graphql('mutation($input:AddSubIssueInput!){addSubIssue(input:$input){clientMutationId}}',{input:{issueId:parent.node_id,subIssueId:child.node_id}});}
  async addBlocker(issue,blocker){await this.graphql('mutation($input:AddBlockedByInput!){addBlockedBy(input:$input){clientMutationId}}',{input:{issueId:issue.node_id,blockingIssueId:blocker.node_id}});}
}
const endpoint=tail=>`/repos/${REPOSITORY}/${tail}`;
const labels=issue=>(issue.labels??[]).map(x=>typeof x==='string'?x:x.name);
const requiredLabels=(def,issue={})=>[...(def.type==='milestone'?['milestone']:['work-item','work:'+def.kind,def.type==='delivery'?'work:delivery':'work:package']),...(def.type==='delivery'?['size:'+def.size.replace('–','-')]:[]),'area:'+def.area,...(labels(issue).some(l=>l.startsWith('status:'))?[]:['status:planned'])];
export function indexIssues(plan,issues){
  const found=new Map(),numbers=new Set(),allowed=new Set(definitions(plan).map(d=>d.id));
  for(const issue of issues){
    if(issue.pull_request)continue;
    const match=(issue.body??'').match(/^<!-- fuzion:tracking:([a-z0-9-]+) -->/);
    if(!match)continue;
    const key=match[1];assert.ok(allowed.has(key),'Unknown managed issue ID');assert.ok(!found.has(key),'Duplicate managed issue ID');
    assert.ok(!numbers.has(issue.number),'A native issue is assigned to multiple tracking IDs');numbers.add(issue.number);
    assert.equal(issue.html_url,issueUrl(issue.number),'Issue belongs to another repository');
    assert.ok(['Jdiller-86','github-actions[bot]'].includes(issue.user?.login),'Managed issue has an unexpected author');
    found.set(key,issue);
  }
  for(const d of definitions(plan))if(d.issueNumber){assert.ok(found.has(d.id),'Registered issue is missing its tracking marker');assert.equal(found.get(d.id).number,d.issueNumber,'Managed issue identity changed');}
  return found;
}
export function renderIssue(def,current,lookup,{sourceRef='main'}={}){
  const checked=new Set();
  for(const c of def.criteria){const line=`- [x] ${c.text} <!-- acceptance:${c.id} -->`;if((current?.body??'').toLowerCase().includes(line.toLowerCase()))checked.add(c.id);}
  let managed=START+'\n## Scope\n\n'+(def.goal??def.scope)+'\n';
  if(def.foundation)managed+='\n## Existing foundation\n\n'+def.foundation+'\n';
  managed+='\n## Full '+(def.type==='milestone'?'milestone ':'')+'acceptance\n\n'+def.criteria.map(c=>`- [${checked.has(c.id)?'x':' '}] ${c.text} <!-- acceptance:${c.id} -->`).join('\n')+'\n';
  if(def.tasks)managed+='\n## Work breakdown\n\n'+def.tasks.map(t=>`- [ ] ${lookup.get(t.id)?'#'+lookup.get(t.id).number:t.title}`).join('\n')+'\n';
  if(def.type==='task')managed+='\n## Tracked delivery issues\n\n'+def.brief.deliverables.map(item=>`- ${lookup.get(item.id)?'#'+lookup.get(item.id).number:item.id} — ${item.title} (${item.size})`).join('\n')+'\n';
  if(def.parent)managed+='\n'+(def.type==='delivery'?'Owning work package: ':'Milestone overview: ')+(lookup.get(def.parent)?'#'+lookup.get(def.parent).number:def.parent.toUpperCase())+'.\n';
  if(def.type==='delivery')managed+='\nMilestone overview: '+(lookup.get(def.milestone)?'#'+lookup.get(def.milestone).number:def.milestone.toUpperCase())+'. Structural size: **'+def.size+'** (breadth, not hours).\n';
  if(def.browserWork)managed+='\n'+renderNativeBrowserWork(def.browserWork);
  managed+='\nAcceptance dependencies: '+(def.dependsOn.length?def.dependsOn.map(id=>lookup.get(id)?'#'+lookup.get(id).number:id.toUpperCase()).join(', '):'none')+'.\n'+END;
  const outside=current?.body?.match(/<!-- fuzion:managed:end -->([\s\S]*)$/)?.[1]??'\n\n## Implementation and evidence\n\nPlanned; no full acceptance is recorded. Keep private original content out of issues.\n\n## Acceptance record\n\nSource commit: Pending\nEvidence: Pending\nReview: Pending\n';
  return upsertDeliveryPlan(marker(def.id)+'\n'+managed+outside,renderDeliveryPlan(def,lookup,{ref:sourceRef}));
}
export function issueStatus(issue,def){
  const statuses=labels(issue).filter(x=>x.startsWith('status:'));
  assert.ok(statuses.length<=1,'Issue has conflicting status labels');
  const key=statuses[0]?.slice(7)??'planned';assert.ok(STATUSES.has(key),'Unknown tracking status');
  if(def.type==='milestone'&&key==='accepted'){
    assert.equal(issue.state,'closed','An accepted milestone must be closed');assert.equal(issue.state_reason,'completed','An accepted milestone must be completed');
    for(const c of def.criteria)assert.ok((issue.body??'').includes(`- [x] ${c.text} <!-- acceptance:${c.id} -->`),'Milestone acceptance criteria are incomplete or changed');
    assert.match(issue.body,/^Source commit: [a-f0-9]{40}$/m,'Acceptance lacks a source identity');
    assert.match(issue.body,/^Evidence: (?!Pending\s*$).+$/m,'Acceptance lacks evidence');
    assert.match(issue.body,new RegExp('^Review: https://github\\.com/'+REPOSITORY+'/(?:issues|pull)/[0-9]+(?:#[^\\s]+)?$','m'),'Acceptance lacks a review reference');
    return 'Accepted';
  }
  if(issue.state==='closed')return def.type!=='milestone'&&issue.state_reason==='completed'?'Done':'Closed';
  assert.ok(key!=='accepted','Only milestones may be accepted');return STATUSES.get(key);
}
export function buildSnapshot(plan,issues,sourceCommit,now=new Date()){
  validatePlan(plan);assert.match(sourceCommit,/^[a-f0-9]{40}$/,'Invalid source commit');
  const lookup=indexIssues(plan,issues),defs=new Map(definitions(plan).map(d=>[d.id,d]));
  const rows=new Map();
  for(const [id,def] of defs){const issue=lookup.get(id);assert.ok(issue,'A managed issue is missing');const status=issueStatus(issue,def);rows.set(id,{id,title:def.title,status,url:issueUrl(issue.number)});}
  for(const m of plan.milestones)if(rows.get(m.id).status==='Accepted')for(const dep of m.dependsOn)assert.equal(rows.get(dep).status,'Accepted','A prerequisite milestone is not accepted');
  return {schemaVersion:1,repository:REPOSITORY,sourceCommit,generatedAt:now.toISOString(),available:true,...(plan.browserWork?{browserWork:validateBrowserWork(plan.browserWork)}:{}),milestones:plan.milestones.map(m=>({...rows.get(m.id),issueUrl:rows.get(m.id).url,foundation:m.foundation,goal:m.goal,accepted:rows.get(m.id).status==='Accepted',dependsOn:m.dependsOn,tasks:m.tasks.map(t=>({...rows.get(t.id),deliverables:t.brief.deliverables.map(item=>({...rows.get(item.id),size:item.size,dependsOn:defs.get(item.id).dependsOn}))}))}))};
}
async function currentRepository(client,sourceCommit,apply){
  const repo=await client.request('GET',`/repos/${REPOSITORY}`);assert.equal(repo.full_name,REPOSITORY);assert.equal(repo.private,false);assert.equal(repo.default_branch,'main');
  if(apply){assert.ok(client.token,'Applying tracking requires a repository credential');const ref=await client.request('GET',endpoint('commits/main'));assert.equal(ref.sha,sourceCommit,'Apply must run from the current trusted main revision');}
}
export async function syncProject(client,plan,{apply=false,sourceCommit}={}){
  validatePlan(plan);await currentRepository(client,sourceCommit,apply);
  const issues=await client.list(endpoint('issues?state=all')),lookup=indexIssues(plan,issues);
  const milestones=await client.list(endpoint('milestones?state=all')),existingLabels=await client.list(endpoint('labels'));
  const operations=[],groups=new Map();
  const wantedLabels={...COLORS,...Object.fromEntries([...STATUSES.keys()].map(k=>['status:'+k,k==='accepted'?'0e8a16':'d4c5f9']))};
  for(const [name,color] of Object.entries(wantedLabels))if(!existingLabels.some(l=>l.name===name)){
    operations.push({kind:'label',name});if(apply)await client.request('POST',endpoint('labels'),{name,color,description:name==='milestone'?'Overall project acceptance gate':'Fuzion Frenzy project tracking'});
  }
  for(const m of plan.milestones){
    const selected=milestones.filter(row=>(row.description??'').startsWith(marker(m.id)));
    assert.ok(selected.length<=1,'Duplicate managed GitHub milestone');let group=selected[0];
    if(!group){operations.push({kind:'milestone',id:m.id});if(apply)group=await client.request('POST',endpoint('milestones'),{title:m.id.toUpperCase()+': '+m.title,description:marker(m.id)+'\n'+m.goal+'\nFull acceptance: https://github.com/'+REPOSITORY+'/blob/main/docs/milestones.md#'+m.id+'--'+m.title.toLowerCase().replaceAll(' ','-')});}
    if(group){
      assert.equal(group.html_url,`https://github.com/${REPOSITORY}/milestone/${group.number}`,'Milestone belongs to another repository');
      if(group.state==='closed'){const owner=lookup.get(m.id);assert.ok(owner&&issueStatus(owner,{...m,type:'milestone'})==='Accepted','Closed native milestone lacks accepted parent evidence');}
    }
    if(group){
      const wanted={title:m.id.toUpperCase()+': '+m.title,description:marker(m.id)+'\n'+m.goal+'\nFull acceptance: https://github.com/'+REPOSITORY+'/blob/main/docs/milestones.md#'+m.id+'--'+m.title.toLowerCase().replaceAll(' ','-')};
      if(group.title!==wanted.title||group.description!==wanted.description){operations.push({kind:'milestone-metadata',id:m.id});if(apply)group=await client.request('PATCH',endpoint('milestones/'+group.number),wanted);}
    }
    groups.set(m.id,group);
  }
  for(const def of definitions(plan)){
    const current=lookup.get(def.id);const group=groups.get(def.milestone);
    const desired={title:issueTitle(def),body:renderIssue(def,current,lookup,{sourceRef:sourceCommit})};
    if(group)desired.milestone=group.number;
    if(!current){
      operations.push({kind:'issue',id:def.id});
      if(apply){const created=await client.request('POST',endpoint('issues'),{...desired,labels:requiredLabels(def)});lookup.set(def.id,created);}
    }
  }
  if(!apply){
    for(const def of definitions(plan)){
      const issue=lookup.get(def.id),group=groups.get(def.milestone);
      if(issue){
        const body=renderIssue(def,issue,lookup,{sourceRef:sourceCommit}),title=issueTitle(def);
        if(issue.body!==body||issue.title!==title||issue.milestone?.number!==group?.number)operations.push({kind:'metadata',id:def.id});
        if(requiredLabels(def,issue).some(l=>!labels(issue).includes(l)))operations.push({kind:'issue-labels',id:def.id});
        const links=await client.relations(issue);
        if(def.parent&&links.parent!==lookup.get(def.parent)?.node_id)operations.push({kind:'parent',id:def.id});
        for(const id of def.dependsOn)if(!links.blockedBy.includes(lookup.get(id)?.node_id))operations.push({kind:'dependency',id:def.id,blockedBy:id});
      }else{
        if(def.parent)operations.push({kind:'parent',id:def.id});
        for(const id of def.dependsOn)operations.push({kind:'dependency',id:def.id,blockedBy:id});
      }
    }
  }
  if(apply){
    // Re-render after all child identities exist; retain human evidence outside the managed block.
    for(const def of definitions(plan)){
      const known=lookup.get(def.id);
      const issue=await client.request('GET',endpoint('issues/'+known.number));
      assert.ok(issue.body?.startsWith(marker(def.id))&&issue.html_url===issueUrl(known.number),'Issue identity changed before update');
      lookup.set(def.id,issue);
      const body=renderIssue(def,issue,lookup,{sourceRef:sourceCommit}),title=issueTitle(def);
      const group=groups.get(def.milestone);
      if(issue.body!==body||issue.title!==title||issue.milestone?.number!==group.number){
        operations.push({kind:'metadata',id:def.id});const updated=await client.request('PATCH',endpoint('issues/'+issue.number),{title,body,milestone:group.number});lookup.set(def.id,updated);
      }
      const current=lookup.get(def.id);
      const required=requiredLabels(def,current);
      const missing=required.filter(l=>!labels(current).includes(l));
      if(missing.length){operations.push({kind:'issue-labels',id:def.id});await client.request('POST',endpoint('issues/'+current.number+'/labels'),{labels:missing});}
      const links=await client.relations(current);
      if(def.parent){const parent=lookup.get(def.parent);assert.ok(!links.parent||links.parent===parent.node_id,'Managed task has a different native parent');if(!links.parent){operations.push({kind:'parent',id:def.id});await client.addParent(parent,current);}}
      for(const id of def.dependsOn){const blocker=lookup.get(id);if(!links.blockedBy.includes(blocker.node_id)){operations.push({kind:'dependency',id:def.id,blockedBy:id});await client.addBlocker(current,blocker);}}
    }
    const latest=await client.list(endpoint('issues?state=all'));const verified=indexIssues(plan,latest);
    for(const def of definitions(plan)){
      const issue=verified.get(def.id);assert.ok(issue,'Created issue could not be read back');for(const label of requiredLabels(def,issue))assert.ok(labels(issue).includes(label),'Managed issue label readback differs');const relations=await client.relations(issue);
      assert.equal(issue.title,issueTitle(def),'Issue title readback differs');assert.equal(issue.milestone?.number,groups.get(def.milestone).number,'Issue milestone readback differs');
      assert.equal(issue.body,renderIssue(def,issue,verified,{sourceRef:sourceCommit}),'Issue scope/delivery readback differs');
      if(def.parent)assert.equal(relations.parent,verified.get(def.parent).node_id,'Native parent verification failed');
      for(const id of def.dependsOn)assert.ok(relations.blockedBy.includes(verified.get(id).node_id),'Native dependency verification failed');
    }
    const tip=await client.request('GET',endpoint('commits/main'));assert.equal(tip.sha,sourceCommit,'Main changed during tracking synchronization');
    return {schemaVersion:1,repository:REPOSITORY,sourceCommit,applied:true,operations,issues:Object.fromEntries([...verified].map(([id,i])=>[id,{number:i.number,url:issueUrl(i.number)}])),snapshot:buildSnapshot(plan,latest,sourceCommit)};
  }
  return {schemaVersion:1,repository:REPOSITORY,sourceCommit,applied:false,operations};
}
function artifactPath(relative){const resolved=path.resolve(ROOT,relative);const base=path.join(ROOT,'artifacts')+path.sep;assert.ok(resolved.startsWith(base),'Generated output must stay in artifacts/');let p=path.dirname(resolved);while(p.startsWith(base)){if(fs.existsSync(p))assert.ok(!fs.lstatSync(p).isSymbolicLink(),'Artifact path contains a symlink');p=path.dirname(p);}if(fs.existsSync(resolved))assert.ok(fs.lstatSync(resolved).isFile()&&!fs.lstatSync(resolved).isSymbolicLink());return resolved;}
function writeJson(relative,data){const p=artifactPath(relative);fs.mkdirSync(path.dirname(p),{recursive:true});fs.writeFileSync(p,JSON.stringify(data,null,2)+'\n');}
export function validateSnapshot(snapshot){
  assert.equal(snapshot.repository,REPOSITORY);assert.equal(snapshot.schemaVersion,1);assert.equal(typeof snapshot.available,'boolean');
  assert.match(snapshot.sourceCommit,/^[a-f0-9]{40}$/);assert.ok(Number.isFinite(Date.parse(snapshot.generatedAt)),'Snapshot time is missing');
  const result={schemaVersion:1,repository:REPOSITORY,sourceCommit:snapshot.sourceCommit,generatedAt:snapshot.generatedAt,available:snapshot.available};
  if(snapshot.browserWork)result.browserWork=validateBrowserWork(snapshot.browserWork);
  if(!snapshot.available)return {...result,reason:'Tracking data unavailable; inspect the workflow result.'};
  assert.equal(snapshot.milestones?.length,5,'Snapshot must contain five milestones');
  const urls=new Set();
  const publicUrl=url=>{assert.match(url,new RegExp('^https://github\\.com/'+REPOSITORY+'/issues/[0-9]+$'));assert.ok(!urls.has(url),'A native issue URL is counted more than once');urls.add(url);return url;};
  const text=value=>{assert.ok(typeof value==='string'&&value.length<=10000,'Invalid snapshot text');return value;};
  const ids=new Set();
  result.milestones=snapshot.milestones.map((m,index)=>{
    assert.equal(m.id,`m${index+1}`);assert.ok([...STATUSES.values(),'Closed'].includes(m.status),'Invalid milestone status');assert.equal(m.accepted,m.status==='Accepted');
    assert.ok(Array.isArray(m.dependsOn)&&m.dependsOn.every(id=>/^m[1-5]$/.test(id)&&Number(id[1])<index+1),'Invalid dependency');
    assert.ok(Array.isArray(m.tasks)&&m.tasks.length<=100,'Invalid tasks');
    return {id:m.id,title:text(m.title),foundation:text(m.foundation),goal:text(m.goal),status:m.status,accepted:m.accepted,issueUrl:publicUrl(m.issueUrl),dependsOn:m.dependsOn,tasks:m.tasks.map(t=>{
      assert.ok(t.id.startsWith(m.id+'-')&&!ids.has(t.id),'Duplicate or misplaced task');ids.add(t.id);assert.ok([...STATUSES.values()].filter(x=>x!=='Accepted').concat(['Done','Closed']).includes(t.status),'Invalid task status');
      assert.ok(Array.isArray(t.deliverables)&&t.deliverables.length>0&&t.deliverables.length<=100,'Delivery issue rows are required');
      return {id:t.id,title:text(t.title),status:t.status,url:publicUrl(t.url),deliverables:t.deliverables.map(item=>{
        assert.ok(typeof item.id==='string'&&item.id.startsWith(t.id+'-')&&!ids.has(item.id),'Duplicate or misplaced delivery issue');ids.add(item.id);
        assert.ok([...STATUSES.values()].filter(value=>value!=='Accepted').concat(['Done','Closed']).includes(item.status),'Invalid delivery status');
        assert.ok(['S','M','L','S–M','M–L'].includes(item.size),'Invalid delivery size');
        assert.ok(Array.isArray(item.dependsOn??[])&&new Set(item.dependsOn??[]).size===(item.dependsOn??[]).length,'Invalid delivery dependencies');
        for(const dependency of item.dependsOn??[])assert.match(dependency,/^m[1-5](?:-[a-z0-9-]+)?$/);
        return {id:item.id,title:text(item.title),size:item.size,status:item.status,url:publicUrl(item.url),dependsOn:item.dependsOn??[]};
      })};
    })};
  });return result;
}
export function renderDashboard(snapshot,template){const safe=validateSnapshot(snapshot);assert.equal(template.split('__PROJECT_JSON__').length,2,'Dashboard must have one data placeholder');return template.replace('__PROJECT_JSON__',JSON.stringify(safe).replaceAll('<','\\u003c'));}
async function main(args){
  const command=args.shift()??'check',options={apply:false};
  while(args.length){const flag=args.shift();if(flag==='--apply')options.apply=true;else if(['--output','--input','--source-commit'].includes(flag)){assert.ok(args.length,'Missing option value');options[flag.slice(2)]=args.shift();}else throw new Error('Unknown project option');}
  const plan=loadPlan();
  if(command==='docs'){fs.writeFileSync(path.join(ROOT,'docs/milestones.md'),renderMilestones(plan));fs.writeFileSync(path.join(ROOT,'docs/work-packages.md'),renderWorkPackages(plan));const changed=syncGoalDocuments(plan);console.log(`Updated milestone/work-package definitions and ${changed.length} goal/map projections.`);return;}
  if(command==='check'){assert.equal(fs.readFileSync(path.join(ROOT,'docs/milestones.md'),'utf8'),renderMilestones(plan),'Milestone docs differ; run npm run project:docs');assert.equal(fs.readFileSync(path.join(ROOT,'docs/work-packages.md'),'utf8'),renderWorkPackages(plan),'Work-package docs differ; run npm run project:docs');syncGoalDocuments(plan,{check:true});console.log('Milestone/work-package scope, browser work, priorities and generated documentation passed.');return;}
  if(command==='render'){
    const input=artifactPath(options.input??'artifacts/project.json');assert.ok(fs.statSync(input).size<=1024*1024,'Snapshot is too large');
    const snapshot=JSON.parse(fs.readFileSync(input,'utf8'));const target=artifactPath(options.output??'artifacts/progress/index.html');fs.mkdirSync(path.dirname(target),{recursive:true});
    fs.writeFileSync(target,renderDashboard(snapshot,fs.readFileSync(path.join(ROOT,'tools/project-dashboard.html'),'utf8')));console.log('Built read-only offline progress view.');return;
  }
  assert.ok(['sync','snapshot'].includes(command),'Unknown project command');
  const sourceCommit=options['source-commit']??execFileSync('git',['rev-parse','HEAD'],{cwd:ROOT,encoding:'utf8'}).trim();assert.match(sourceCommit,/^[a-f0-9]{40}$/);
  const client=new GitHub();
  if(command==='sync'){const result=await syncProject(client,plan,{apply:options.apply,sourceCommit});writeJson(options.output??'artifacts/project-sync.json',result);console.log(`Tracking ${options.apply?'applied and verified':'previewed'}: ${result.operations.length} operations.`);}
  else{try{await currentRepository(client,sourceCommit,false);const result=buildSnapshot(plan,await client.list(endpoint('issues?state=all')),sourceCommit);writeJson(options.output??'artifacts/project.json',result);console.log(`Exported ${result.milestones.filter(m=>m.accepted).length}/5 accepted milestones.`);}catch(error){writeJson(options.output??'artifacts/project.json',{schemaVersion:1,repository:REPOSITORY,sourceCommit,generatedAt:new Date().toISOString(),available:false,browserWork:plan.browserWork,reason:'Tracking data unavailable; inspect the workflow result.'});throw error;}}
}
if(process.argv[1]&&import.meta.url===pathToFileURL(path.resolve(process.argv[1])).href)main(process.argv.slice(2)).catch(error=>{console.error(error instanceof assert.AssertionError?error.message.split('\n')[0]:error.message);process.exitCode=1;});
