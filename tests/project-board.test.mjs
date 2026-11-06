import test from 'node:test';
import assert from 'node:assert/strict';
import {loadPlan,definitions,renderIssue,REPOSITORY} from '../tools/project.mjs';
import {BoardGitHub,OWNER,TITLE,MARKER,loadConfig,validateConfig,selectBoard,syncBoard} from '../tools/project-board.mjs';
const SHA='1'.repeat(40),URL='https://github.com/users/Jdiller-86/projects/3';
const clone=value=>structuredClone(value);
const config=()=>({...loadConfig(),number:null,id:null,templateId:null});
const field=(name,names)=>({id:'F_'+name,name,dataType:'SINGLE_SELECT',options:names.map((name,i)=>({id:`${name}_${i}`,name}))});
const fields=()=>[field('Status',['Planned','In progress','Awaiting evidence','Blocked','Ready for review','Done','Closed','Accepted']),field('Work type',['Milestone','Task','Feature']),{id:'F_wait',name:'Waiting resource',dataType:'TEXT'}];
const views=()=>[{id:'V_board',name:'Board',layout:'BOARD_LAYOUT',filter:''},{id:'V_gates',name:'Milestones',layout:'TABLE_LAYOUT',filter:'work-type:Milestone'}];
class MemoryBoard extends BoardGitHub{
  constructor(){super({token:'owned-fixture-token'});this.ownerId='U_owner';this.repositoryId='R_repo';this.templateId='PVT_template';this.boards=[];this.writes=[];this.created=0;this.sha=SHA;this.templateFields=fields();this.templateViews=views();
    this.issues=definitions(loadPlan()).map(d=>({number:d.issueNumber,node_id:'I_'+d.issueNumber,html_url:`https://github.com/${REPOSITORY}/issues/${d.issueNumber}`,body:renderIssue(d,null,new Map()),user:{login:'github-actions[bot]'},state:'open',state_reason:null,labels:['status:planned']}));
  }
  async context(){return {ownerId:this.ownerId,sourceCommit:this.sha,boards:clone(this.boards),template:{id:this.templateId}};}
  async list(){return clone(this.issues);}
  async template(){return {fields:clone(this.templateFields),views:clone(this.templateViews)};}
  async copy(){this.created++;this.writes.push({kind:'copy',includeDraftIssues:false});this.target={id:'PVT_target',number:3,url:URL,title:TITLE,owner:{login:OWNER},public:false,closed:false,readme:'Template text',shortDescription:'Template description',fields:fields(),views:views(),items:[],repositories:[]};this.boards=[this.target];this.targetId=this.target.id;return clone(this.target);}
  async board(id){assert.equal(id,this.target.id);return clone(this.target);}
  async connection(id,name){assert.equal(id,this.target.id);return clone(this.target[name]);}
  async tip(){return this.sha;}
  async pause(){this.pauses=(this.pauses??0)+1;}
  async graphql(query,{input}){
    if(this.failWrite)throw new Error('Simulated write failure');this.writes.push({query,input:clone(input)});
    if(query.includes('updateProjectV2(input:')){const {projectId,...metadata}=input;Object.assign(this.target,metadata);}
    else if(query.includes('linkProjectV2ToRepository'))this.target.repositories=[{id:this.repositoryId,nameWithOwner:REPOSITORY,isPrivate:false}];
    else if(query.includes('addProjectV2ItemById')){const issue=this.issues.find(i=>i.node_id===input.contentId);const row={id:'PVTI_'+issue.number,isArchived:false,content:{__typename:'Issue',id:issue.node_id,number:issue.number,url:issue.html_url,repository:{nameWithOwner:REPOSITORY,isPrivate:false}},status:null,workType:null,waiting:'Private reviewer note retained locally'};this.target.items.push(row);return {addProjectV2ItemById:{item:clone(row)}};}
    else if(query.includes('updateProjectV2ItemFieldValue')){const row=this.target.items.find(i=>i.id===input.itemId);const f=this.target.fields.find(f=>f.id===input.fieldId);const option=f.options.find(o=>o.id===input.value.singleSelectOptionId);row[f.name==='Status'?'status':'workType']={optionId:option.id,name:option.name};}
    else assert.fail('Unexpected mutation');return {};
  }
}
async function provision(){const client=new MemoryBoard();const result=await syncBoard(client,loadPlan(),config(),{apply:true,sourceCommit:SHA});return {client,result};}
test('preview is read-only; creation copies views without drafts and populates exactly the 15 existing issues',async()=>{
  const client=new MemoryBoard();const preview=await syncBoard(client,loadPlan(),config(),{sourceCommit:SHA});assert.equal(preview.applied,false);assert.equal(preview.operations.length,48);assert.equal(client.writes.length,0);
  const result=await syncBoard(client,loadPlan(),config(),{apply:true,sourceCommit:SHA});assert.equal(result.project.url,URL);assert.equal(result.readbackVerified,true);assert.equal(client.created,1);assert.equal(client.target.items.length,15);assert.ok(client.target.public);assert.deepEqual(client.target.views,views());
  assert.equal(client.target.items.filter(i=>i.workType.name==='Milestone').length,5);assert.ok(client.target.items.every(i=>i.status.name==='Planned'));assert.ok(!JSON.stringify(result).includes('Private reviewer'));assert.ok(!JSON.stringify(result).includes(client.token));
  client.writes=[];await syncBoard(client,loadPlan(),{...config(),number:3,id:'PVT_target'},{apply:true,sourceCommit:SHA});assert.equal(client.created,1);assert.equal(client.writes.length,0);
});
test('identity conflicts, unmarked same-name projects and stale main prevent mutations',async()=>{
  for(const mutate of [c=>c.token='',c=>c.sha='2'.repeat(40),c=>c.boards=[{id:'PVT_unknown',number:3,url:URL,title:TITLE,owner:{login:OWNER},public:true,closed:false,readme:'Human-owned board'}]]){const client=new MemoryBoard();mutate(client);await assert.rejects(syncBoard(client,loadPlan(),config(),{apply:true,sourceCommit:SHA}));assert.equal(client.writes.length,0);}
  for(const mutate of [c=>c.owner='other',c=>c.number=2,c=>c.number=3,c=>c.id='unvalidated']){const bad=config();mutate(bad);assert.throws(()=>validateConfig(bad));}
  const {client}=await provision();for(const mutate of [b=>b.owner.login='other',b=>b.url='https://github.com/other/project',b=>b.id='PVT_changed',b=>b.public=false,b=>b.closed=true]){const b=clone(client.target);mutate(b);assert.throws(()=>selectBoard({...config(),number:3,id:'PVT_target'},[b]));}
  assert.throws(()=>selectBoard(config(),[client.target,clone(client.target)]),/Multiple/);
});
test('partial creation failure is visible and cannot create a duplicate board on retry',async()=>{
  const client=new MemoryBoard();client.failWrite=true;const announced=[];await assert.rejects(syncBoard(client,loadPlan(),config(),{apply:true,sourceCommit:SHA,onCreated:p=>announced.push(p)}));assert.equal(announced[0].url,URL);assert.equal(client.created,1);
  await assert.rejects(syncBoard(client,loadPlan(),config(),{apply:true,sourceCommit:SHA}),/ownership marker/);assert.equal(client.created,1);
});
test('foreign, private, draft or duplicate cards and issue identity/acceptance drift cannot be synchronized',async()=>{
  for(const mutate of [r=>r.content.repository.nameWithOwner='Jdiller-86/CTR-2-WUP',r=>r.content.repository.isPrivate=true,r=>r.content={__typename:'DraftIssue'},r=>r.content.id='I_changed',r=>r.isArchived=true]){const {client}=await provision();mutate(client.target.items[0]);client.writes=[];await assert.rejects(syncBoard(client,loadPlan(),config(),{apply:true,sourceCommit:SHA}));assert.equal(client.writes.length,0);}
  const {client}=await provision();client.target.items.push(clone(client.target.items[0]));await assert.rejects(syncBoard(client,loadPlan(),config(),{apply:true,sourceCommit:SHA}),/Duplicate/);
  for(const mutate of [i=>i.html_url='https://github.com/foreign/repo/issues/22',i=>i.user.login='foreign',i=>i.labels=['status:accepted']]){const c=new MemoryBoard();mutate(c.issues[0]);await assert.rejects(syncBoard(c,loadPlan(),config(),{apply:true,sourceCommit:SHA}));assert.equal(c.writes.length,0);}
});
test('status follows reviewed issue records, waiting notes and outside project notes survive updates',async()=>{
  const {client}=await provision();const note='\nHuman project note.\n';client.target.readme+=note;client.issues[1].state='closed';client.issues[1].state_reason='completed';client.target.items[0].status={name:'Accepted',optionId:'Accepted_7'};
  const result=await syncBoard(client,loadPlan(),config(),{apply:true,sourceCommit:SHA});assert.equal(client.target.items[0].status.name,'Planned');assert.equal(client.target.items[1].status.name,'Done');assert.ok(client.target.items.every(i=>i.waiting==='Private reviewer note retained locally'));assert.ok(client.target.readme.endsWith(note));assert.ok(!JSON.stringify(result).includes('Private reviewer'));
});
test('public pull requests from this repository remain available to the copied Pull requests view',async()=>{
  const {client}=await provision();const row={id:'PVTI_pr',isArchived:false,content:{__typename:'PullRequest',id:'PR_37',number:37,url:`https://github.com/${REPOSITORY}/pull/37`,repository:{nameWithOwner:REPOSITORY,isPrivate:false}}};client.target.items.push(row);client.writes=[];
  const receipt=await syncBoard(client,loadPlan(),config(),{apply:true,sourceCommit:SHA});assert.equal(receipt.managedIssues.length,15);assert.equal(client.writes.length,0);assert.deepEqual(client.target.items.at(-1),row);
  client.target.items.at(-1).content.repository.isPrivate=true;await assert.rejects(syncBoard(client,loadPlan(),config(),{apply:true,sourceCommit:SHA}),/Private/);
});
test('unsupported template filters/options fail before copying and source drift fails readback',async()=>{
  for(const mutate of [c=>c.templateViews[0].filter='repo:Jdiller-86/CTR-2-WUP',c=>c.templateFields[0].options.pop(),c=>c.templateViews=[]]){const client=new MemoryBoard();mutate(client);await assert.rejects(syncBoard(client,loadPlan(),config(),{apply:true,sourceCommit:SHA}));assert.equal(client.writes.length,0);}
  const client=new MemoryBoard();client.tip=async()=>'2'.repeat(40);await assert.rejects(syncBoard(client,loadPlan(),config(),{apply:true,sourceCommit:SHA}),/Main changed/);
  const other=new MemoryBoard();await assert.rejects(syncBoard(other,loadPlan(),{...config(),templateId:'PVT_changed'},{apply:true,sourceCommit:SHA}),/Template project ID/);assert.equal(other.writes.length,0);
});
test('native add receipts avoid stale item listings and final readback tolerates bounded indexing delay',async()=>{
  const client=new MemoryBoard();let reads=0;const board=client.board.bind(client);client.connection=async()=>assert.fail('New items must be verified from mutation receipts');
  client.board=async id=>{const result=await board(id);if(++reads===2)result.items.pop();return result;};
  const receipt=await syncBoard(client,loadPlan(),config(),{apply:true,sourceCommit:SHA});assert.equal(receipt.readbackVerified,true);assert.equal(client.pauses,1);assert.equal(client.target.items.length,15);assert.equal(client.created,1);
});
test('real transport copies configuration only, rejects foreign mutations, and redacts GraphQL credentials',async()=>{
  const client=new BoardGitHub({token:'fixture-secret-value'});client.templateId='PVT_template';client.ownerId='U_owner';let input;client.request=async(method,endpoint,payload)=>{input=payload.variables.input;return {data:{copyProjectV2:{projectV2:{id:'PVT_target',number:3,url:URL,closed:false,owner:{login:OWNER}}}}};};
  await client.copy();assert.equal(input.includeDraftIssues,false);assert.equal(input.projectId,'PVT_template');assert.equal(input.ownerId,'U_owner');
  await assert.rejects(client.mutate('updateProjectV2','UpdateProjectV2Input',{projectId:'PVT_template'}),/different Project/);
  await assert.rejects(client.mutate('deleteProjectV2','DeleteProjectV2Input',{projectId:'PVT_target'}),/Unsupported/);
  client.request=async()=>({errors:[{message:'Rejected fixture-secret-value'}]});await assert.rejects(client.graphql('query {}'),e=>e.message.includes('[redacted]')&&!e.message.includes(client.token));
});
