#!/usr/bin/env node
// Copies reviewed template configuration without its content; only this project's board is writable.
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import {execFileSync} from 'node:child_process';
import {pathToFileURL} from 'node:url';
import {GitHub,ROOT,REPOSITORY,STATUSES,loadPlan,definitions,indexIssues,buildSnapshot} from './project.mjs';

export const OWNER='Jdiller-86';
export const TITLE='Fuzion Frenzy Web Port';
export const MARKER='<!-- fuzion:project-board:v1 -->';
const END='<!-- fuzion:project-board:end -->';
const BASE=`https://github.com/${REPOSITORY}`;
const projectUrl=number=>`https://github.com/users/${OWNER}/projects/${number}`;
const META='id number url title public closed readme shortDescription owner { ... on User { login } }';
const FIELDS='nodes { __typename ... on ProjectV2Field { id name dataType } ... on ProjectV2SingleSelectField { id name dataType options { id name } } ... on ProjectV2IterationField { id name dataType } } pageInfo { hasNextPage endCursor }';
const VIEWS='nodes { id name layout filter } pageInfo { hasNextPage endCursor }';
const ITEM='id isArchived content { __typename ... on Issue { id number url repository { nameWithOwner isPrivate } } ... on PullRequest { id number url repository { nameWithOwner isPrivate } } } status:fieldValueByName(name:"Status") { ... on ProjectV2ItemFieldSingleSelectValue { optionId name } } workType:fieldValueByName(name:"Work type") { ... on ProjectV2ItemFieldSingleSelectValue { optionId name } }';
const ITEMS='nodes { '+ITEM+' } pageInfo { hasNextPage endCursor }';
class ProjectAPIError extends Error {}

export function validateConfig(config){
  assert.equal(config.schemaVersion,1);assert.equal(config.owner,OWNER);assert.equal(config.title,TITLE);assert.equal(config.templateNumber,2);
  assert.ok(config.templateId===null||(typeof config.templateId==='string'&&/^PVT_[A-Za-z0-9_-]+$/.test(config.templateId)),'Invalid template project ID');
  assert.ok(config.number===null||(Number.isSafeInteger(config.number)&&config.number>0&&config.number!==2),'Invalid target project number');
  assert.ok(config.id===null||(typeof config.id==='string'&&/^PVT_[A-Za-z0-9_-]+$/.test(config.id)),'Invalid target project ID');
  assert.equal(config.id===null,config.number===null,'Project identity must be registered together');return config;
}
export const loadConfig=()=>validateConfig(JSON.parse(fs.readFileSync(path.join(ROOT,'profiles/project-board.json'),'utf8')));
function identity(board,number){
  assert.ok(board&&typeof board.id==='string'&&/^PVT_[A-Za-z0-9_-]+$/.test(board.id),'Project identity is unavailable');
  assert.ok(Number.isSafeInteger(board.number)&&board.number>0);if(number!==undefined)assert.equal(board.number,number);
  assert.equal(board.owner?.login?.toLowerCase(),OWNER.toLowerCase(),'Project has another owner');
  assert.equal(board.url,projectUrl(board.number),'Project URL does not match its owner');assert.equal(board.closed,false,'Project is closed');return board;
}
function managedReadme(current,created){
  const body=MARKER+'\n# Fuzion Frenzy Web Port\n\nFive project milestones, implementation work and acceptance evidence for the original Xbox game running entirely in a browser.\n\n'+
    `[Development plan](${BASE}/blob/main/plans.md) · [Milestone acceptance](${BASE}/blob/main/docs/milestones.md) · [Management](${BASE}/blob/main/docs/management.md)\n\n`+
    'Issue status labels and reviewed acceptance records own status. This board mirrors those records. All five milestones are unaccepted at the planning baseline; implementation remains paused until the next user-directed task. Keep original content and private evidence local.\n'+END;
  if(created)return body;
  assert.ok(current?.startsWith(MARKER)&&current.includes(END),'Project lacks the managed ownership markers');
  return body+current.slice(current.indexOf(END)+END.length);
}
export function selectBoard(config,boards){
  const matches=boards.filter(p=>p.number===config.number||p.readme?.startsWith(MARKER)||p.title===TITLE);
  assert.ok(matches.length<=1,'Multiple candidate projects exist; inspect before retrying');
  const board=matches[0];if(config.number!==null)assert.ok(board,'Registered project is missing');
  if(board){identity(board,config.number??undefined);assert.notEqual(board.number,2,'The template is read-only');if(config.id)assert.equal(board.id,config.id,'Registered project ID changed');assert.ok(board.readme?.startsWith(MARKER),'A same-named project has no ownership marker; inspect before retrying');assert.equal(board.public,true,'Existing managed board is not public');}
  return board??null;
}
export function trackingFields(rows){
  const selected={};for(const name of ['Status','Work type','Waiting resource']){
    const matches=rows.filter(f=>f.name===name);assert.equal(matches.length,1,`Project field ${name} is missing or ambiguous`);const field=matches[0];assert.ok(typeof field.id==='string');selected[name]=field;
  }
  for(const [name,wanted] of [['Status',[...STATUSES.values(),'Done','Closed']],['Work type',['Milestone','Task']]]){
    const field=selected[name];assert.equal(field.dataType,'SINGLE_SELECT');assert.ok(Array.isArray(field.options));
    assert.equal(new Set(field.options.map(o=>o.id)).size,field.options.length);assert.equal(new Set(field.options.map(o=>o.name.toLowerCase())).size,field.options.length);
    for(const value of wanted)assert.ok(field.options.some(o=>o.name.toLowerCase()===value.toLowerCase()),`Project ${name} option ${value} is missing`);
  }
  assert.equal(selected['Waiting resource'].dataType,'TEXT');return selected;
}
function members(rows){
  const result=new Map();for(const row of rows){const issue=row.content;assert.ok(['Issue','PullRequest'].includes(issue?.__typename),'Unexpected draft or inaccessible board item');assert.equal(issue.repository?.nameWithOwner,REPOSITORY,'Foreign repository item on managed board');assert.equal(issue.repository?.isPrivate,false,'Private board content is outside scope');assert.equal(issue.url,`${BASE}/${issue.__typename==='Issue'?'issues':'pull'}/${issue.number}`);assert.ok(typeof row.id==='string'&&typeof issue.id==='string');if(issue.__typename==='PullRequest')continue;assert.ok(!result.has(issue.number),'Duplicate issue on board');result.set(issue.number,row);}return result;
}

export class BoardGitHub extends GitHub{
  async graphql(query,variables={}){
    const response=await this.request('POST','/graphql',{query,variables});
    if(response?.errors){const messages=response.errors.map(e=>String(e.message??'GraphQL error'));throw new ProjectAPIError(('GitHub Projects: '+messages.join('; ')).replaceAll(this.token||'__missing_credential__','[redacted]').slice(0,700));}
    assert.ok(response?.data&&!response.errors,'GitHub Projects request failed; check project/public_repo token access and the workflow step');return response.data;
  }
  async context(){
    const data=await this.graphql(`query { viewer { login } user(login:"${OWNER}") { id login projectV2(number:2) { ${META} repositories(first:100) { nodes { nameWithOwner isPrivate } pageInfo { hasNextPage } } } projectsV2(first:100) { nodes { ${META} } pageInfo { hasNextPage } } } repository(owner:"${OWNER}",name:"fuzion-frenzy-web-port") { id nameWithOwner isPrivate defaultBranchRef { name target { oid } } } }`);
    assert.equal(data.viewer?.login?.toLowerCase(),OWNER.toLowerCase(),'Token must belong to the project owner');
    assert.equal(data.user?.login?.toLowerCase(),OWNER.toLowerCase());assert.ok(data.user.id);assert.equal(data.user.projectsV2.pageInfo.hasNextPage,false,'Project inventory exceeds the review budget');
    assert.equal(data.repository?.nameWithOwner,REPOSITORY);assert.equal(data.repository.isPrivate,false);assert.equal(data.repository.defaultBranchRef?.name,'main');
    identity(data.user.projectV2,2);assert.equal(typeof data.user.projectV2.public,'boolean','Template visibility is unavailable');
    assert.ok(data.user.projectV2.repositories?.nodes?.some(r=>r.nameWithOwner==='Jdiller-86/CTR-2-WUP'&&!r.isPrivate),'Template is not associated with CTR-2-WUP');
    this.ownerId=data.user.id;this.templateId=data.user.projectV2.id;this.repositoryId=data.repository.id;
    return {ownerId:this.ownerId,template:data.user.projectV2,boards:data.user.projectsV2.nodes,sourceCommit:data.repository.defaultBranchRef.target.oid};
  }
  async connection(id,name,selection){
    const rows=[],seen=new Set();let after=null;
    for(let page=0;page<20;page++){
      const extra=name==='items'?',archivedStates:[ARCHIVED,NOT_ARCHIVED]':'';
      const data=await this.graphql(`query($id:ID!,$after:String) { node(id:$id) { ... on ProjectV2 { ${name}(first:100,after:$after${extra}) { ${selection} } } } }`,{id,after});
      const current=data.node?.[name];assert.ok(Array.isArray(current?.nodes)&&current.nodes.length<=100,'Invalid Project page');
      for(const row of current.nodes){assert.ok(row?.id&&!seen.has(row.id),'Repeated Project item');seen.add(row.id);rows.push(row);}
      assert.equal(typeof current.pageInfo?.hasNextPage,'boolean');if(!current.pageInfo.hasNextPage)return rows;
      assert.ok(current.pageInfo.endCursor&&current.pageInfo.endCursor!==after,'Project pagination stalled');after=current.pageInfo.endCursor;
    }throw new Error('Project exceeds the 2,000-item budget');
  }
  async board(id){
    const data=await this.graphql(`query($id:ID!) { node(id:$id) { ... on ProjectV2 { ${META} } } }`,{id});
    const board=identity(data.node);board.fields=await this.connection(id,'fields',FIELDS);board.views=await this.connection(id,'views',VIEWS);board.items=await this.connection(id,'items',ITEMS);
    board.repositories=await this.connection(id,'repositories','nodes { id nameWithOwner isPrivate } pageInfo { hasNextPage endCursor }');return board;
  }
  async template(){return {fields:await this.connection(this.templateId,'fields',FIELDS),views:await this.connection(this.templateId,'views',VIEWS)};}
  async copy(){
    const data=await this.graphql(`mutation($input:CopyProjectV2Input!) { copyProjectV2(input:$input) { projectV2 { ${META} } } }`,{input:{ownerId:this.ownerId,projectId:this.templateId,title:TITLE,includeDraftIssues:false}});
    const board=identity(data.copyProjectV2?.projectV2);assert.notEqual(board.id,this.templateId);assert.notEqual(board.number,2);this.targetId=board.id;return board;
  }
  async mutate(name,type,input){
    assert.ok(this.targetId&&this.targetId!==this.templateId,'No verified target board');assert.equal(input.projectId,this.targetId,'Mutation targets a different Project');
    assert.ok(['updateProjectV2','linkProjectV2ToRepository','addProjectV2ItemById','updateProjectV2ItemFieldValue'].includes(name),'Unsupported Project mutation');
    if(name==='linkProjectV2ToRepository')assert.equal(input.repositoryId,this.repositoryId);
    if(name==='addProjectV2ItemById')assert.ok(this.issueIds?.has(input.contentId),'Issue is outside managed scope');
    if(name==='updateProjectV2ItemFieldValue'){assert.ok(this.itemIds?.has(input.itemId));assert.ok(this.fieldIds?.has(input.fieldId));}
    const selection=name==='addProjectV2ItemById'?`item { ${ITEM} }`:'clientMutationId';
    return this.graphql(`mutation($input:${type}!) { ${name}(input:$input) { ${selection} } }`,{input});
  }
  async tip(){return (await this.request('GET',`/repos/${REPOSITORY}/commits/main`)).sha;}
  async pause(milliseconds){await new Promise(resolve=>setTimeout(resolve,milliseconds));}
}

export async function syncBoard(client,plan,config,{apply=false,sourceCommit,onCreated=()=>{}}={}){
  validateConfig(config);assert.match(sourceCommit,/^[a-f0-9]{40}$/);if(apply)assert.ok(client.token,'FUZION_GITHUB_MANAGEMENT_TOKEN is required');
  const context=await client.context();if(apply)assert.equal(context.sourceCommit,sourceCommit,'Apply must use current trusted main');
  if(config.templateId!==null)assert.equal(context.template.id,config.templateId,'Template project ID changed');
  const issues=await client.list(`/repos/${REPOSITORY}/issues?state=all`);const lookup=indexIssues(plan,issues);const snapshot=buildSnapshot(plan,issues,sourceCommit);
  const statuses=new Map(snapshot.milestones.flatMap(m=>[[m.id,m.status],...m.tasks.map(t=>[t.id,t.status])]));
  const wanted=definitions(plan).map(d=>({number:lookup.get(d.id).number,contentId:lookup.get(d.id).node_id,status:statuses.get(d.id),kind:d.type==='milestone'?'Milestone':'Task'}));client.issueIds=new Set(wanted.map(i=>i.contentId));
  let board=selectBoard(config,context.boards);const operations=[];let created=false,template=null;
  if(!board){
    // Field definitions are the requested configuration; item values and drafts are never imported.
    template=await client.template();trackingFields(template.fields);assert.ok(template.views.length>0,'Template has no views');
    assert.ok(template.views.every(v=>!v.filter||!/CTR-2-WUP|repo:|milestone:/i.test(v.filter)),'Template view has repository-specific filters; review before copying');
    operations.push({kind:'copy-template',templateNumber:2,includeDraftIssues:false});
    if(!apply){operations.push({kind:'project-description'},{kind:'link-repository'});for(const issue of wanted)operations.push({kind:'add-issue',issue:issue.number},{kind:'field',issue:issue.number,field:'Status',value:issue.status},{kind:'field',issue:issue.number,field:'Work type',value:issue.kind});return {schemaVersion:1,repository:REPOSITORY,sourceCommit,applied:false,operations};}
    board=await client.copy();created=true;onCreated({id:board.id,number:board.number,url:board.url});
  }
  client.targetId=board.id;
  const selectedId=board.id,selectedNumber=board.number;board=await client.board(selectedId);identity(board,selectedNumber);assert.equal(board.id,selectedId);if(!created)assert.ok(board.readme.startsWith(MARKER));
  if(created)assert.equal(board.items.length,0,'Template copy unexpectedly imported issue content');
  if(created){const viewShape=views=>views.map(({name,layout,filter})=>({name,layout,filter})).sort((a,b)=>a.name.localeCompare(b.name));assert.deepEqual(viewShape(board.views),viewShape(template.views),'Copied views differ from the template');}
  assert.ok(board.repositories.every(r=>r.nameWithOwner===REPOSITORY&&!r.isPrivate),'Project links another repository');
  const fields=trackingFields(board.fields),rows=members(board.items);for(const issue of wanted){const row=rows.get(issue.number);if(row){assert.equal(row.content.id,issue.contentId,'Project issue identity changed');assert.equal(row.isArchived,false,'Managed issue is archived; review before synchronizing');}}
  const readme=managedReadme(board.readme,created);const shortDescription='Five milestones, implementation work and verifiable acceptance for the original Fuzion Frenzy in a browser.';
  if(board.title!==TITLE||board.readme!==readme||board.shortDescription!==shortDescription||!board.public){
    operations.push({kind:'project-description'});if(apply)await client.mutate('updateProjectV2','UpdateProjectV2Input',{projectId:board.id,title:TITLE,readme,shortDescription,public:true});
  }
  if(!board.repositories.some(r=>r.nameWithOwner===REPOSITORY)){
    operations.push({kind:'link-repository'});if(apply)await client.mutate('linkProjectV2ToRepository','LinkProjectV2ToRepositoryInput',{projectId:board.id,repositoryId:client.repositoryId});
  }
  client.fieldIds=new Set([fields.Status.id,fields['Work type'].id]);client.itemIds=new Set(board.items.map(i=>i.id));
  for(const issue of wanted){
    let row=rows.get(issue.number);if(row)assert.equal(row.content.id,issue.contentId,'Project issue identity changed');
    if(!row){operations.push({kind:'add-issue',issue:issue.number});if(apply){const added=await client.mutate('addProjectV2ItemById','AddProjectV2ItemByIdInput',{projectId:board.id,contentId:issue.contentId});row=members([added.addProjectV2ItemById?.item]).get(issue.number);assert.ok(row&&row.content.id===issue.contentId,'Added issue identity differs');client.itemIds.add(row.id);}}
    for(const [name,key,value] of [['Status','status',issue.status],['Work type','workType',issue.kind]]){
      const field=fields[name],option=field.options.find(o=>o.name.toLowerCase()===value.toLowerCase());
      if(row?.[key]?.optionId!==option.id||row?.[key]?.name!==option.name){operations.push({kind:'field',issue:issue.number,field:name,value});if(apply)await client.mutate('updateProjectV2ItemFieldValue','UpdateProjectV2ItemFieldValueInput',{projectId:board.id,itemId:row.id,fieldId:field.id,value:{singleSelectOptionId:option.id}});}
    }
  }
  if(apply){
    let verified;
    for(let attempt=0;attempt<4;attempt++){
      verified=await client.board(board.id);identity(verified,board.number);assert.equal(verified.id,board.id);assert.equal(verified.public,true);assert.equal(verified.readme,readme);assert.equal(verified.title,TITLE);
      assert.ok(verified.repositories.some(r=>r.nameWithOwner===REPOSITORY));assert.ok(verified.repositories.every(r=>r.nameWithOwner===REPOSITORY&&!r.isPrivate));
      const finalFields=trackingFields(verified.fields),final=members(verified.items);let matches=true;
      for(const issue of wanted){const row=final.get(issue.number);if(!row){matches=false;continue;}assert.equal(row.content.id,issue.contentId);assert.equal(row.isArchived,false,'Managed issue was archived during synchronization');for(const [name,key,value] of [['Status','status',issue.status],['Work type','workType',issue.kind]]){const option=finalFields[name].options.find(o=>o.name.toLowerCase()===value.toLowerCase());if(row[key]?.name!==option.name||row[key]?.optionId!==option.id)matches=false;}}
      if(matches)break;assert.ok(attempt<3,'Project changes are not observable after bounded readback; inspect before retrying');await client.pause(1000*(attempt+1));
    }
    assert.equal(await client.tip(),sourceCommit,'Main changed during Project synchronization');board=verified;
  }
  return {schemaVersion:1,repository:REPOSITORY,sourceCommit,applied:apply,readbackVerified:apply,project:{id:board.id,number:board.number,url:board.url},template:{id:context.template.id,number:2,url:projectUrl(2)},views:board.views.map(v=>({name:v.name,layout:v.layout,filter:v.filter})),managedIssues:wanted.map(({number,status,kind})=>({number,status,workType:kind})),operations};
}
function saveReceipt(receipt){
  const directory=path.join(ROOT,'artifacts');if(fs.existsSync(directory))assert.ok(!fs.lstatSync(directory).isSymbolicLink(),'Artifacts directory is a symlink');else fs.mkdirSync(directory);
  const target=path.join(directory,'project-board.json');if(fs.existsSync(target))assert.ok(fs.lstatSync(target).isFile()&&!fs.lstatSync(target).isSymbolicLink());fs.writeFileSync(target,JSON.stringify(receipt,null,2)+'\n');
}
async function main(args){
  assert.ok(args.every(a=>a==='--apply'),'Only --apply is supported');assert.ok(args.length<=1);const apply=args.includes('--apply');
  const sourceCommit=process.env.TRACKING_SOURCE_SHA??execFileSync('git',['rev-parse','HEAD'],{cwd:ROOT,encoding:'utf8'}).trim();
  const announce=project=>{console.log(`Created Project: ${project.url}`);if(process.env.GITHUB_STEP_SUMMARY)fs.appendFileSync(process.env.GITHUB_STEP_SUMMARY,`Created public tracking Project: ${project.url}\n\n`);};
  const receipt=await syncBoard(new BoardGitHub(),loadPlan(),loadConfig(),{apply,sourceCommit,onCreated:announce});saveReceipt(receipt);
  if(receipt.project)console.log('Verified Project identity: '+JSON.stringify({project:receipt.project,template:receipt.template,views:receipt.views}));
  const line=receipt.project?`Project: ${receipt.project.url} — ${receipt.managedIssues.length} managed issues; ${receipt.operations.length} ${apply?'applied':'proposed'} operations.`:'Project creation previewed; no changes applied.';
  console.log(line);if(process.env.GITHUB_STEP_SUMMARY)fs.appendFileSync(process.env.GITHUB_STEP_SUMMARY,line+'\n');
}
if(process.argv[1]&&import.meta.url===pathToFileURL(path.resolve(process.argv[1])).href)main(process.argv.slice(2)).catch(error=>{console.error(error instanceof assert.AssertionError||error instanceof ProjectAPIError?error.message.split('\n')[0]:'Project board operation failed; inspect token access and trusted-main workflow configuration.');process.exitCode=1;});
