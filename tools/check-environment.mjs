import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const [major, minor] = process.versions.node.split('.').map(Number);
if (major !== 24 || minor < 14) throw new Error('Use Node >=24.14.0 and <25; see .node-version.');
const has = relative => fs.existsSync(path.join(root, relative));
console.log(JSON.stringify({
  stage: 'G2 offline Twisted System; implementation verification is separate from fidelity acceptance',
  node: process.versions.node,
  preferredModel: 'gpt-6.1-terra',
  modelSelection: 'Project defaults do not replace an existing desktop chat model.',
  optionalLocalAnalysis: {
    ghidra: has('.tools/ghidra_12.1.4_PUBLIC/support/analyzeHeadless.bat'),
    java: has('.tools/jdk-21.0.13+9/bin/java.exe'),
    xbeLoader: has('.tools/ghidra_12.1.4_PUBLIC/Ghidra/Extensions/ghidra-xbe'),
    discTool: has('.tools/xdvdfs/xdvdfs.exe'),
    persistentProject: has('research/ghidra-projects/FuzionFrenzySetup.gpr'),
    note: 'Existence checks only; see saved verification evidence for actual analysis tests.'
  },
  dependencies: 'Dependency-free browser ESM/WebGL 2 implementation; no production services provisioned.'
}, null, 2));
