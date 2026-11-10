// Check maintained Markdown links without network access or generated artifacts.
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import {execFileSync} from 'node:child_process';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const files=[...new Set(execFileSync('git',['ls-files','-co','--exclude-standard','-z'],{cwd:root,encoding:'utf8'}).split('\0').filter(p=>p.endsWith('.md')))];
const anchors=new Map();
function headings(file){
  if(anchors.has(file))return anchors.get(file);
  const ids=new Set(),seen=new Map();
  for(const line of fs.readFileSync(file,'utf8').split('\n')){
    for(const anchor of line.matchAll(/<a\s+(?:id|name)=["']([a-zA-Z0-9_-]+)["']\s*><\/a>/g))ids.add(anchor[1]);
    const match=line.match(/^ {0,3}#{1,6}\s+(.+?)\s*#*$/);if(!match)continue;
    const plain=match[1].replace(/\[([^\]]+)\]\([^)]*\)/g,'$1').replace(/<[^>]+>/g,'');
    const slug=plain.toLowerCase().replace(/[^\p{L}\p{N}_\- ]/gu,'').replaceAll(' ','-');
    const count=seen.get(slug)??0;seen.set(slug,count+1);ids.add(slug+(count?'-'+count:''));
  }
  anchors.set(file,ids);return ids;
}
let count=0;const errors=[];
for(const name of files){
  const absolute=path.join(root,name);assert.ok(fs.existsSync(absolute),'Missing maintained Markdown: '+name);
  const body=fs.readFileSync(absolute,'utf8').replace(/^\s*(```|~~~)[\s\S]*?^\s*\1\s*$/gm,'');
  for(const match of body.matchAll(/\[[^\]\n]*\]\((<[^>]+>|[^\s)]+)(?:\s+["'][^\n]*["'])?\)/g)){
    const raw=match[1].replace(/^<|>$/g,'');if(/^[a-zA-Z][a-zA-Z0-9+.-]*:/.test(raw)||raw.startsWith('//'))continue;
    const [target,fragment='']=raw.split('#');const decoded=decodeURIComponent(target.split('?')[0]);
    const file=decoded?path.resolve(decoded.startsWith('/')?root:path.dirname(absolute),decoded.replace(/^\//,'')):absolute;count++;
    if(!fs.existsSync(file))errors.push(name+': missing '+raw);
    else if(fragment&&file.endsWith('.md')&&!headings(file).has(decodeURIComponent(fragment)))errors.push(name+': missing heading '+raw);
  }
}
assert.equal(errors.length,0,errors.join('\n'));console.log(`Documentation passed: ${files.length} Markdown files, ${count} local links.`);
