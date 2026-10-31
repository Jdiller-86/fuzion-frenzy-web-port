import {mkdir,copyFile,readdir} from 'node:fs/promises';
import {webFiles} from './web-files.mjs';
const root=new URL('../',import.meta.url),destination=new URL('../dist/',import.meta.url);
// Preserve unexpected local files: never clean a directory containing unreviewed paths.
async function inspect(dir,prefix=''){
  let entries;try{entries=await readdir(dir,{withFileTypes:true});}catch(e){if(e.code==='ENOENT')return;throw e;}
  for(const e of entries){const p=prefix+e.name;if(e.isSymbolicLink())throw new Error('Build destination contains a symlink');if(e.isDirectory())await inspect(new URL(e.name+'/',dir),p+'/');else if(!webFiles.includes(p))throw new Error('Build destination contains an unexpected file; preserve and inspect it before rebuilding');}
}
await inspect(destination);
for(const path of webFiles){const target=new URL(path,destination);await mkdir(new URL('./',target),{recursive:true});await copyFile(new URL(path,root),target);}
console.log(`Built ${webFiles.length} allowlisted source files in dist/. No game content included. Serve with npm run preview.`);
