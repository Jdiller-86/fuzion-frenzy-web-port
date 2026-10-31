// Read-only file identities for the supplied folder; never extracts or copies content.
import {createHash} from 'node:crypto';
import {createReadStream} from 'node:fs';
import {readdir,writeFile,stat,realpath} from 'node:fs/promises';
import path from 'node:path';

const [folder,output]=process.argv.slice(2);
if(!folder||!output||process.argv.length!==4)throw new Error('Usage: node tools/inventory-reference.mjs <supplied-folder> <new-manifest.json>');
const root=await realpath(path.resolve(folder)),requested=path.resolve(output);
const destination=path.join(await realpath(path.dirname(requested)),path.basename(requested));
const relativeOutput=path.relative(root,destination);
if(!relativeOutput||(!path.isAbsolute(relativeOutput)&&relativeOutput!=='..'&&!relativeOutput.startsWith('..'+path.sep)))throw new Error('Output must be outside supplied content');
const expected='45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9';
async function digest(file){const hash=createHash('sha256');for await(const chunk of createReadStream(file))hash.update(chunk);return hash.digest('hex');}
if(await digest(path.join(root,'default.xbe'))!==expected)throw new Error('Unexpected executable identity');
const files=[];
async function visit(directory){
  const entries=await readdir(directory,{withFileTypes:true});
  for(const entry of entries){
    if(entry.isSymbolicLink())throw new Error('Links are not supported');
    const absolute=path.join(directory,entry.name);
    if(entry.isDirectory()){await visit(absolute);continue;}
    if(!entry.isFile()||files.length>=256)throw new Error('Unsupported input inventory');
    const before=await stat(absolute);
    const sha256=await digest(absolute),after=await stat(absolute);
    if(before.size!==after.size||before.mtimeMs!==after.mtimeMs)throw new Error('Input changed while hashing');
    files.push({path:path.relative(root,absolute).split(path.sep).join('/'),bytes:before.size,sha256});
  }
}
await visit(root);
if(files.length!==136)throw new Error('Unexpected file count for the recorded supplied folder');
files.sort((a,b)=>a.path<b.path?-1:a.path>b.path?1:0);
const manifest={schemaVersion:1,kind:'file-identity-only',executableSha256:expected,fileCount:files.length,totalBytes:files.reduce((sum,file)=>sum+file.bytes,0),limitations:['Digests identify supplied bytes; they do not establish retail authenticity or compatibility of another build.','File paths and extensions do not establish selectable activities or decoded assets.','No original or converted game bytes are included.'],files};
await writeFile(destination,JSON.stringify(manifest,null,2)+'\n',{flag:'wx'});
console.log(`Wrote ${files.length} file identities (${manifest.totalBytes} bytes of input); original content was only read.`);
