import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import {ROOT,loadPlan,definitions,renderIssue,validatePlan} from '../tools/project.mjs';
import {validateBrief,renderDeliveryPlan,upsertDeliveryPlan,renderWorkPackages} from '../tools/project-issues.mjs';
import {README_PROGRESS_CHART,syncGoalDocuments} from '../tools/project-goals.mjs';

test('every existing child issue has a bounded, source-backed brief without increasing native task counts',()=>{
  const plan=loadPlan(),tasks=plan.milestones.flatMap(m=>m.tasks);
  assert.equal(tasks.length,10);assert.equal(tasks.flatMap(t=>t.brief.deliverables).length,51);assert.equal(definitions(plan).length,5+10+51);
  const ids=new Set();
  for(const task of tasks){validateBrief(task.brief,task.id);assert.ok(task.brief.deliverables.length>=4);for(const slice of task.brief.deliverables){assert.ok(!ids.has(slice.id));ids.add(slice.id);}}
  const defs=definitions(plan),deliveryDefs=defs.filter(def=>def.type==='delivery');assert.equal(deliveryDefs.length,51);
  for(const milestone of plan.milestones)for(const task of milestone.tasks)for(const slice of task.brief.deliverables){const def=deliveryDefs.find(row=>row.id===slice.id);assert.ok(def);assert.equal(def.parent,task.id);assert.equal(def.milestone,milestone.id);assert.equal(def.kind,task.kind);assert.equal(def.scope,slice.doneWhen);assert.equal(def.size,slice.size);assert.deepEqual(def.criteria,[{id:slice.id+'-exit',text:slice.doneWhen}]);}
  assert.equal(fs.readFileSync(path.join(ROOT,'docs/work-packages.md'),'utf8'),renderWorkPackages(plan));
});

test('missing, ambiguous and unsafe delivery instructions fail before issue synchronization',()=>{
  const task=loadPlan().milestones[0].tasks[0];
  for(const change of [b=>b.deliverables.push(b.deliverables[0]),b=>b.deliverables[0].id='m2-build-other',b=>b.deliverables[0].size='2 days',b=>b.deliverables[0].firstStep='',b=>b.deliverables[0].firstStep='<script>',b=>b.firstStep='',b=>b.references[0].path='docs/../../private',b=>b.references[0].path='docs/missing-file.md',b=>b.privateNote='exclude',b=>b.validation=[]]){
    const brief=structuredClone(task.brief);change(brief);assert.throws(()=>validateBrief(brief,task.id));
  }
  const plan=structuredClone(loadPlan());delete plan.milestones[0].tasks[0].brief;assert.throws(()=>validatePlan(plan));
});

test('delivery enrichment leaves native scope, evidence and acceptance bytes intact and is idempotent',()=>{
  const task=definitions(loadPlan()).find(def=>def.id==='m2-build');
  const scope='<!-- fuzion:tracking:m2-build -->\n<!-- fuzion:managed:start -->\n- [x] Retained criterion\n<!-- fuzion:managed:end -->';
  const evidence='\n\n## Human implementation note\nRetained collaborator evidence.\n\n## Acceptance record\nSource commit: Pending\nEvidence: Pending\nReview: Pending\n';
  const original=scope+evidence,content=renderDeliveryPlan(task,new Map(),{ref:'a'.repeat(40)});
  const enriched=upsertDeliveryPlan(original,content);
  assert.ok(enriched.startsWith(scope));assert.ok(enriched.endsWith(evidence));assert.equal(upsertDeliveryPlan(enriched,content),enriched);
  assert.ok(enriched.includes('/blob/'+'a'.repeat(40)+'/docs/work-packages.md'));
  const changed=structuredClone(task);changed.brief.firstStep='A revised bounded assignment.';
  const revised=upsertDeliveryPlan(enriched,renderDeliveryPlan(changed,new Map()));
  assert.ok(revised.startsWith(scope));assert.ok(revised.endsWith(evidence));assert.ok(!revised.includes(task.brief.firstStep));
  for(const corrupt of [original+'<!-- fuzion:delivery:start -->',enriched+'<!-- fuzion:delivery:end -->',enriched.replace('<!-- fuzion:managed:end -->','')])assert.throws(()=>upsertDeliveryPlan(corrupt,content));
});

test('normal synchronization owns delivery detail while preserving checked acceptance and outside notes',()=>{
  const task=definitions(loadPlan()).find(def=>def.id==='m3-build');
  let body=renderIssue(task,null,new Map()).replace('- [ ]','- [x]')+'\nHuman handoff survives.\n';
  const current={body};task.brief=structuredClone(task.brief);task.brief.firstStep='Revised authority experiment.';
  body=renderIssue(task,current,new Map());
  assert.ok(body.includes('- [x]'));assert.ok(body.endsWith('Human handoff survives.\n'));assert.ok(body.includes('Revised authority experiment.'));
  assert.equal(body.split('<!-- fuzion:delivery:start -->').length,2);
});

test('each delivery is rendered as its own native issue with exact exit criterion and retained evidence',()=>{
  const plan=loadPlan(),def=definitions(plan).find(item=>item.type==='delivery');assert.ok(def);const item=plan.milestones.flatMap(m=>m.tasks).flatMap(t=>t.brief.deliverables).find(d=>d.id===def.id);
  let body=renderIssue(def,null,new Map());assert.ok(body.includes(`- [ ] ${item.doneWhen}`));assert.ok(body.includes('<!-- fuzion:tracking:'+item.id+' -->'));
  body=body.replace(`- [ ] ${item.doneWhen}`,`- [x] ${item.doneWhen}`)+'\nHuman delivery evidence remains.\n';
  const refreshed=renderIssue(def,{body},new Map());assert.ok(refreshed.includes(`- [x] ${item.doneWhen}`));assert.ok(refreshed.endsWith('Human delivery evidence remains.\n'));
  assert.equal(def.firstStep,item.firstStep);assert.ok(refreshed.includes(item.firstStep));
  const changed=renderIssue({...def,firstStep:'A revised executable integration assignment.'},{body},new Map());
  assert.ok(changed.includes('A revised executable integration assignment.'));assert.ok(changed.includes(`- [x] ${item.doneWhen}`));assert.ok(changed.endsWith('Human delivery evidence remains.\n'));
});

test('milestone issues expose child deliverables and first assignments without accepting them',()=>{
  const plan=loadPlan(),lookup=new Map(definitions(plan).map(def=>[def.id,{number:def.issueNumber}]));
  for(const milestone of plan.milestones){const body=renderDeliveryPlan({...milestone,type:'milestone'},lookup);for(const task of milestone.tasks){assert.ok(body.includes(task.brief.firstStep));for(const slice of task.brief.deliverables)assert.ok(body.includes(slice.title));}assert.match(body,/do not accept this gate/);}
});

test('goal regeneration preserves the README chart and refuses its removal',()=>{
  const root=fs.mkdtempSync(path.join(os.tmpdir(),'fuzion-chart-'));
  try{
    for(const name of ['README.md','docs/README.md','docs/m1-browser-work.md','profiles/development-state.json']){fs.mkdirSync(path.dirname(path.join(root,name)),{recursive:true});fs.copyFileSync(path.join(ROOT,name),path.join(root,name));}
    const plan=loadPlan(),file=path.join(root,'README.md');assert.ok(fs.readFileSync(file,'utf8').includes(README_PROGRESS_CHART));syncGoalDocuments(plan,{root});assert.ok(fs.readFileSync(file,'utf8').includes(README_PROGRESS_CHART));
    fs.writeFileSync(file,fs.readFileSync(file,'utf8').replace(README_PROGRESS_CHART,''));assert.throws(()=>syncGoalDocuments(plan,{root,check:true}),/Keep the README progress chart/);
  }finally{fs.rmSync(root,{recursive:true,force:true});}
});
