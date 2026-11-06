#!/usr/bin/env node
// Read-only preflight: never replace an unrelated existing Pages publication.
import assert from 'node:assert/strict';
import path from 'node:path';
import {pathToFileURL} from 'node:url';
import {REPOSITORY} from './project.mjs';
import {SITE_URL,SITE_MARKER} from './publish-progress.mjs';

export async function checkProgressPages({fetchImpl=fetch,token=process.env.GH_TOKEN}={}){
  assert.ok(token,'Pages preflight requires the workflow credential');
  const response=await fetchImpl(`https://api.github.com/repos/${REPOSITORY}/pages`,{headers:{Authorization:`Bearer ${token}`,Accept:'application/vnd.github+json','X-GitHub-Api-Version':'2022-11-28'},signal:AbortSignal.timeout(30000)});
  if(response.status===404)return {existing:false,url:SITE_URL};assert.ok(response.ok,'Cannot inspect existing GitHub Pages configuration');
  const pages=await response.json();assert.equal(pages.cname??'', '', 'An existing custom-domain site needs review before publication');assert.equal(String(pages.html_url).replace(/\/?$/,'/'),SITE_URL,'An existing Pages URL differs from the reviewed destination');assert.equal(pages.build_type,'workflow','Preserve the existing branch-based Pages publication for review');
  const current=await fetchImpl(SITE_URL,{signal:AbortSignal.timeout(30000)});if(current.status===404)return {existing:true,published:false,url:SITE_URL};
  assert.ok(current.ok&&current.url.startsWith(SITE_URL)&&(await current.text()).includes(SITE_MARKER),'Preserve the unrelated existing Pages site for review');return {existing:true,published:true,url:SITE_URL};
}
if(process.argv[1]&&import.meta.url===pathToFileURL(path.resolve(process.argv[1])).href)checkProgressPages().then(result=>console.log(result.existing?'Verified the owned tracking publication destination.':'Verified a new tracking publication destination.')).catch(error=>{console.error(error.message.split('\n')[0]);process.exitCode=1;});
