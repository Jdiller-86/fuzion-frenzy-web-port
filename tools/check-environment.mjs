import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const [major, minor] = process.versions.node.split('.').map(Number);
if (major !== 24 || minor < 14) throw new Error('Use Node >=24.14.0 and <25; see .node-version.');
const has = relative => fs.existsSync(path.join(root, relative));
const hasOne = (...paths) => paths.some(has);
const codexDefaults = fs.readFileSync(path.join(root, '.codex/config.toml'), 'utf8');
const modelSetting = key => codexDefaults.match(new RegExp('^' + key + ' = "([^"]+)"$', 'm'))?.[1] ?? null;
console.log(JSON.stringify({
  stage: 'Original-game browser execution feasibility; prior remake is research, not product acceptance',
  node: process.versions.node,
  preferredModel: modelSetting('model'),
  preferredReasoning: modelSetting('model_reasoning_effort'),
  agentDefaults: {
    workerModel: modelSetting('default_subagent_model'),
    workerReasoning: modelSetting('default_subagent_reasoning_effort'),
    maxConcurrentWorkers: Number(codexDefaults.match(/^max_concurrent_threads_per_session = ([0-9]+)$/m)?.[1]) || null,
    note: 'Saved project configuration only; verify actual model availability and worker execution in the local client.'
  },
  modelSelection: 'Project defaults do not replace an existing desktop chat model.',
  optionalLocalAnalysis: {
    ghidra: hasOne('.tools/ghidra_12.1.4_PUBLIC/support/analyzeHeadless.bat', '.tools/ghidra_12.1.4_PUBLIC/support/analyzeHeadless'),
    java: hasOne('.tools/jdk-21.0.13+9/bin/java.exe', '.tools/jdk-21.0.13+9/bin/java'),
    xbeLoader: has('.tools/ghidra_12.1.4_PUBLIC/Ghidra/Extensions/ghidra-xbe'),
    discTool: hasOne('.tools/xdvdfs/xdvdfs.exe', '.tools/xdvdfs/xdvdfs'),
    persistentProject: has('research/ghidra-projects/FuzionFrenzySetup.gpr'),
    note: 'Conventional Windows/Linux path existence only; custom locations may differ. Run npm run dev:preflight for named private-artifact integrity. Neither check proves tool behavior or original execution.'
  },
  dependencies: 'Historical prototype has no npm application dependencies; original-runtime cross-build tools/libraries are separate and pinned under .tools. No production services provisioned.'
}, null, 2));
