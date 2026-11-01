// Read-only local execution dependencies; metadata only, no code/pseudocode export.
import {readFile,stat,writeFile,realpath} from 'node:fs/promises';
import {createHash} from 'node:crypto';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
import {readXbeProfile} from '../src/xbe.mjs';
const [input,output]=process.argv.slice(2);
if(!input||!output||process.argv.length!==4)throw new Error('Usage: node tools/profile-execution.mjs <local.xbe> <new-report.json>');
const research=await realpath(fileURLToPath(new URL('../research/',import.meta.url)));
const destination=path.join(await realpath(path.dirname(path.resolve(output))),path.basename(output));
const relative=path.relative(research,destination);
if(!relative||path.isAbsolute(relative)||relative==='..'||relative.startsWith('..'+path.sep))throw new Error('Execution reports must stay inside ignored research/');
if(await realpath(input)===destination)throw new Error('Never overwrite input');
const before=await stat(input);if(!before.isFile()||before.size>16*1024*1024)throw new Error('Unsupported executable size');
const bytes=await readFile(input),after=await stat(input);if(before.size!==after.size||before.mtimeMs!==after.mtimeMs)throw new Error('Input changed during read');
const profile=readXbeProfile(bytes.buffer.slice(bytes.byteOffset,bytes.byteOffset+bytes.byteLength));
const result={schemaVersion:1,kind:'static-execution-dependencies',sha256:createHash('sha256').update(bytes).digest('hex'),bytes:bytes.length,profile,limitations:['Static declarations are not observed calls or instruction coverage.','No game instructions, pseudocode or resources are included.','No execution, firmware acquisition, extraction or upload was performed.']};
await writeFile(destination,JSON.stringify(result,null,2)+'\n',{flag:'wx'});
console.log(JSON.stringify({sha256:result.sha256,entry:profile.entry.toString(16),variant:profile.variant,sections:profile.sections.length,kernelImports:profile.imports.length,uniqueOrdinals:new Set(profile.imports.map(i=>i.ordinal)).size,tls:!!profile.tls,libraries:profile.libraries.map(l=>`${l.name} ${l.major}.${l.minor}.${l.build}`)}));
