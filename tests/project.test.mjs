import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
import {GitHub,REPOSITORY,ROOT,loadPlan,validatePlan,definitions,renderMilestones,indexIssues,renderIssue,issueStatus,buildSnapshot,syncProject,renderDashboard} from '../tools/project.mjs';
const SHA='1'.repeat(40);
const fresh=()=>{const plan=structuredClone(loadPlan());for(const m of plan.milestones){delete m.issueNumber;for(const t of m.tasks)delete t.issueNumber;}return plan;};
class MemoryGitHub{
  constructor(){this.token='source-owned-test-only';this.labels=[];this.milestones=[];this.issues=[];this.links=new Map();this.writes=[];this.sha=SHA;}
  async list(endpoint){return structuredClone(endpoint.includes('/issues?')?this.issues:endpoint.includes('/milestones?')?this.milestones:this.labels);}
  async request(method,endpoint,data){
    if(method==='GET'){if(/\/issues\/[0-9]+$/.test(endpoint))return structuredClone(this.issues.find(i=>endpoint.endsWith('/'+i.number)));return endpoint.endsWith('/commits/main')?{sha:this.sha}:{full_name:REPOSITORY,private:false,default_branch:'main'};}
    this.writes.push({method,endpoint,data:structuredClone(data)});
    if(/\/issues\/[0-9]+\/labels$/.test(endpoint)){const number=Number(endpoint.split('/').at(-2));const i=this.issues.find(i=>i.number===number);i.labels=[...new Set([...i.labels,...data.labels])];return i.labels;}
    if(endpoint.endsWith('/labels')){this.labels.push(data);return structuredClone(data);}
    if(method==='PATCH'&&/\/milestones\/[0-9]+$/.test(endpoint)){const m=this.milestones.find(m=>endpoint.endsWith('/'+m.number));Object.assign(m,data);return structuredClone(m);}
    if(endpoint.endsWith('/milestones')){const m={...data,number:this.milestones.length+1,html_url:`https://github.com/${REPOSITORY}/milestone/${this.milestones.length+1}`,state:'open'};this.milestones.push(m);return structuredClone(m);}
    const group=this.milestones.find(m=>m.number===data.milestone);
    if(method==='POST'){
      const number=this.issues.length+100;
      const issue={...data,id:number+1000,node_id:'I_test_'+number,number,user:{login:'github-actions[bot]'},state:'open',state_reason:null,html_url:`https://github.com/${REPOSITORY}/issues/${number}`,milestone:group};
      this.issues.push(issue);this.links.set(issue.node_id,{parent:null,blockedBy:[]});return structuredClone(issue);
    }
    const issue=this.issues.find(i=>endpoint.endsWith('/'+i.number));assert.ok(issue);Object.assign(issue,data,{milestone:group});return structuredClone(issue);
  }
  async relations(issue){return structuredClone(this.links.get(issue.node_id));}
  async addParent(parent,child){this.writes.push({kind:'parent'});this.links.get(child.node_id).parent=parent.node_id;}
  async addBlocker(issue,blocker){this.writes.push({kind:'dependency'});this.links.get(issue.node_id).blockedBy.push(blocker.node_id);}
}
async function provision(){const client=new MemoryGitHub();const plan=fresh();const result=await syncProject(client,plan,{apply:true,sourceCommit:SHA});return {client,plan,result};}
function accept(issue,definition){
  issue.state='closed';issue.state_reason='completed';issue.labels=['milestone','status:accepted'];
  issue.body=renderIssue({...definition,type:'milestone'},issue,new Map()).replaceAll('- [ ]','- [x]').replace('Source commit: Pending','Source commit: '+SHA).replace('Evidence: Pending','Evidence: retained-source-owned-report').replace('Review: Pending',`Review: https://github.com/${REPOSITORY}/issues/${issue.number}`);
}

test('scope is five milestones with unique acyclic work identities and exact maintained docs',()=>{
  const plan=fresh();assert.equal(plan.milestones.length,5);assert.equal(definitions(plan).length,15);
  assert.equal(fs.readFileSync(ROOT+'/docs/milestones.md','utf8'),renderMilestones(loadPlan()));
  for(const mutate of [p=>p.repository='other/repo',p=>p.milestones[1].dependsOn=['missing'],p=>p.milestones[0].dependsOn=['m5'],p=>p.milestones[0].tasks[0].id='m1',p=>{p.milestones[0].issueNumber=1;p.milestones[1].issueNumber=1;}]){const broken=fresh();mutate(broken);assert.throws(()=>validatePlan(broken));}
});
test('preview makes no writes; apply verifies native parents/dependencies and is idempotent',async()=>{
  const client=new MemoryGitHub(),plan=fresh();const preview=await syncProject(client,plan,{sourceCommit:SHA});assert.equal(preview.applied,false);assert.equal(client.writes.length,0);
  const result=await syncProject(client,plan,{apply:true,sourceCommit:SHA});assert.equal(client.issues.length,15);assert.equal(client.milestones.length,5);assert.equal(result.snapshot.milestones.filter(m=>m.accepted).length,0);
  assert.equal(client.issues.filter(i=>i.labels.includes('work-item')).length,10,'CTR Implementation work view must include all ten tasks');assert.ok(client.issues.filter(i=>i.labels.includes('milestone')).every(i=>!i.labels.includes('work-item')));
  for(const def of definitions(plan)){const issue=client.issues.find(i=>i.body.startsWith(`<!-- fuzion:tracking:${def.id} -->`));const rel=await client.relations(issue);assert.equal(Boolean(rel.parent),def.type==='task');assert.equal(rel.blockedBy.length,def.dependsOn.length);}
  client.writes=[];const again=await syncProject(client,plan,{apply:true,sourceCommit:SHA});assert.equal(again.operations.length,0);assert.equal(client.writes.length,0);
});
test('stale main and missing credentials prevent all mutations',async()=>{
  for(const client of [Object.assign(new MemoryGitHub(),{sha:'2'.repeat(40)}),Object.assign(new MemoryGitHub(),{token:''})]){await assert.rejects(syncProject(client,fresh(),{apply:true,sourceCommit:SHA}));assert.equal(client.writes.length,0);}
});
test('sync preserves checked criteria and outside evidence; changed criteria reopen the checkbox',async()=>{
  const {client,plan}=await provision();const issue=client.issues[0];const criterion=plan.milestones[0].criteria[0];
  issue.body=issue.body.replace(`- [ ] ${criterion.text}`,`- [x] ${criterion.text}`)+'\nRetained reviewer note.\n';
  await syncProject(client,plan,{apply:true,sourceCommit:SHA});assert.match(client.issues[0].body,/Retained reviewer note/);assert.ok(client.issues[0].body.includes(`- [x] ${criterion.text}`));
  plan.milestones[0].criteria[0].text+=' Additional explicit requirement.';
  await syncProject(client,plan,{apply:true,sourceCommit:SHA});assert.ok(client.issues[0].body.includes(`- [ ] ${plan.milestones[0].criteria[0].text}`));
});
test('closed work does not accept a milestone; acceptance requires source/evidence/review and prerequisites',async()=>{
  const {client,plan}=await provision();for(const i of client.issues){if(!i.labels.includes('milestone')){i.state='closed';i.state_reason='completed';}}
  let snapshot=buildSnapshot(plan,client.issues,SHA);assert.equal(snapshot.milestones.filter(m=>m.accepted).length,0);assert.equal(snapshot.milestones[0].tasks[0].status,'Done');
  const first=client.issues[0];first.labels.push('status:accepted');assert.throws(()=>buildSnapshot(plan,client.issues,SHA),/conflicting/);first.labels=['milestone','status:accepted'];assert.throws(()=>buildSnapshot(plan,client.issues,SHA));
  accept(first,plan.milestones[0]);snapshot=buildSnapshot(plan,client.issues,SHA);assert.equal(snapshot.milestones[0].accepted,true);
  const m3=client.issues.find(i=>i.body.startsWith('<!-- fuzion:tracking:m3 -->'));accept(m3,plan.milestones[2]);assert.throws(()=>buildSnapshot(plan,client.issues,SHA),/prerequisite/);
});
test('foreign, duplicate, missing and changed-author tracking cannot be reported as complete',async()=>{
  const {client,plan}=await provision();
  for(const mutate of [rows=>rows.push(structuredClone(rows[0])),rows=>rows.pop(),rows=>rows[0].html_url='https://github.com/other/repo/issues/1',rows=>rows[0].user.login='unrelated-user']){const rows=structuredClone(client.issues);mutate(rows);assert.throws(()=>buildSnapshot(plan,rows,SHA));}
  const numbered=fresh();numbered.milestones[0].issueNumber=1;assert.throws(()=>indexIssues(numbered,[]),/Registered/);
});
test('closed-not-planned work is Closed and snapshots exclude raw body/private notes',async()=>{
  const {client,plan}=await provision();const task=client.issues.find(i=>!i.labels.includes('milestone'));task.state='closed';task.state_reason='not_planned';task.body+='\nprivate-original-bytes-should-not-export';
  const snap=buildSnapshot(plan,client.issues,SHA);assert.equal(snap.milestones[0].tasks[0].status,'Closed');assert.ok(!JSON.stringify(snap).includes('private-original-bytes'));
});
test('GitHub transport is repository-scoped and errors do not reveal credentials',async()=>{
  const token='owned-fixture-credential';const client=new GitHub({token,fetchImpl:async()=>({ok:false,status:403})});await assert.rejects(client.request('GET','/repos/other/repo'),/outside/);
  await assert.rejects(client.request('GET',`/repos/${REPOSITORY}`),error=>error.message.includes('403')&&!error.message.includes(token));
});
test('offline dashboard embeds safe JSON once and handles unavailable data without inventing progress',()=>{
  const template=fs.readFileSync(ROOT+'/tools/project-dashboard.html','utf8');
  const html=renderDashboard({schemaVersion:1,repository:REPOSITORY,sourceCommit:SHA,generatedAt:'2026-11-06T00:00:00Z',available:false,reason:'</script><script>bad</script>'},template);
  assert.ok(!html.includes('</script><script>bad'));assert.ok(html.includes('Tracking data unavailable'));assert.ok(!html.includes('bad</script>'));assert.ok(!html.includes('__PROJECT_JSON__'));assert.ok(template.includes('textContent'));assert.ok(!template.includes('.innerHTML'));
  assert.throws(()=>renderDashboard({schemaVersion:1,repository:'foreign/repo'},template));
});

test('progress view renders real issue/commit links, dependency navigation and completed work',async()=>{
  const {client,plan}=await provision();const task=client.issues.find(i=>!i.labels.includes('milestone'));task.state='closed';task.state_reason='completed';
  const snapshot=buildSnapshot(plan,client.issues,SHA);const html=renderDashboard(snapshot,fs.readFileSync(ROOT+'/tools/project-dashboard.html','utf8'));
  class Element{
    constructor(tag){this.tag=tag;this.children=[];this.handlers={};this.value='all';this.ownText='';}
    set textContent(text){this.ownText=String(text);this.children=[];}get textContent(){return this.ownText+this.children.map(c=>c.textContent).join('');}
    append(...children){this.children.push(...children.map(c=>typeof c==='string'?Object.assign(new Element('#text'),{ownText:c}):c));}
    replaceChildren(...children){this.children=[];this.append(...children);}addEventListener(name,fn){this.handlers[name]=fn;}
    get childElementCount(){return this.children.filter(c=>c.tag!=='#text').length;}focus(){this.focused=true;}
  }
  const staticNodes=new Map(['project-data','message','content','meta','count','filter','milestones','task-count'].map(id=>[id,Object.assign(new Element('div'),{id})]));
  staticNodes.get('project-data').textContent=JSON.stringify(snapshot);
  const walk=node=>[node,...node.children.flatMap(walk)];
  const document={createElement:tag=>new Element(tag),createTextNode:text=>Object.assign(new Element('#text'),{ownText:String(text)}),getElementById:id=>staticNodes.get(id)??[...staticNodes.values()].flatMap(walk).find(e=>e.id===id)};
  const script=[...html.matchAll(/<script>([\s\S]*?)<\/script>/g)].at(-1)[1];vm.runInNewContext(script,{document,URL},{timeout:1000});
  assert.match(document.getElementById('count').textContent,/0 of 5 milestones accepted/);
  assert.equal(document.getElementById('task-count').textContent,'1 of 10 work packages completed');
  const nodes=[...staticNodes.values()].flatMap(walk);assert.equal(nodes.filter(n=>n.href?.startsWith(`https://github.com/${REPOSITORY}/issues/`)).length,15);
  assert.ok(nodes.some(n=>n.href===`https://github.com/${REPOSITORY}/commit/${SHA}`));
  assert.ok(nodes.some(n=>n.href==='#m1'));assert.equal(document.getElementById('milestones').childElementCount,5);
});
test('snapshot rendering strips unapproved fields and rejects forged status counts and foreign links',async()=>{
  const {client,plan}=await provision();const template=fs.readFileSync(ROOT+'/tools/project-dashboard.html','utf8');
  const snapshot=buildSnapshot(plan,client.issues,SHA);snapshot.privateNote='do-not-publish';snapshot.milestones[0].foundation='</script><script>untrusted</script>';
  const html=renderDashboard(snapshot,template);assert.ok(!html.includes('do-not-publish'));assert.ok(!html.includes('</script><script>untrusted'));assert.ok(html.includes('\\u003c/script>'));
  for(const mutate of [s=>s.milestones.pop(),s=>s.milestones[0].accepted=true,s=>s.milestones[0].issueUrl='https://github.com/other/repo/issues/1',s=>s.milestones[0].tasks[0].status='Accepted']){const invalid=structuredClone(snapshot);mutate(invalid);assert.throws(()=>renderDashboard(invalid,template));}
});

test('preview exposes existing metadata/link drift; sync repairs owned fields and retains unrelated labels',async()=>{
  const {client,plan}=await provision();client.writes=[];client.issues[0].title='Edited title';client.issues[0].labels=['status:planned','custom-label'];client.milestones[0].title='Edited native title';client.links.get(client.issues[1].node_id).parent=null;
  const preview=await syncProject(client,plan,{sourceCommit:SHA});assert.ok(preview.operations.some(o=>o.kind==='metadata'));assert.ok(preview.operations.some(o=>o.kind==='issue-labels'));assert.ok(preview.operations.some(o=>o.kind==='parent'));assert.ok(preview.operations.some(o=>o.kind==='milestone-metadata'));assert.equal(client.writes.length,0);
  await syncProject(client,plan,{apply:true,sourceCommit:SHA});assert.equal(client.issues[0].title,'M1: '+plan.milestones[0].title);assert.ok(client.issues[0].labels.includes('custom-label'));assert.ok(client.issues[0].labels.includes('milestone'));assert.equal(client.milestones[0].title,client.issues[0].title);
});
