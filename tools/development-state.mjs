import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import {execFileSync} from 'node:child_process';
import {fileURLToPath, pathToFileURL} from 'node:url';

export const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
export const DOCUMENTS = ['AGENTS.md', 'plans.md', 'docs/README.md', 'docs/agents.md', 'docs/handoff.md', 'docs/status.md'];
const START = '<!-- development-state:start -->';
const END = '<!-- development-state:end -->';
const sha = /^[0-9a-f]{64}$/;
const text = value => typeof value === 'string' && value.length > 0 && value.length <= 600 && !/[\r\n<>]/.test(value);

export function validateState(s) {
  assert.equal(s.schemaVersion, 1, 'Unsupported development-state schema');
  assert.match(s.milestone, /^M[1-5]$/);
  assert.ok(['in-progress', 'paused', 'blocked', 'awaiting-evidence'].includes(s.status), 'Invalid development status');
  assert.ok(Number.isSafeInteger(s.checkpoint.profile) && s.checkpoint.profile > 0);
  assert.match(s.checkpoint.sourceCommit, /^[0-9a-f]{40}$/);
  for (const name of ['inputSha256', 'moduleSha256', 'auditSha256']) assert.match(s.checkpoint[name], sha, name);
  assert.ok(Number.isSafeInteger(s.checkpoint.inputBytes) && s.checkpoint.inputBytes > 0);
  assert.equal(s.checkpoint.evidenceGuide, 'docs/status.md');
  assert.ok(text(s.checkpoint.summary));
  assert.match(s.nextTask.id, /^SCOPE-[0-9]+$/);
  assert.ok(text(s.nextTask.goal) && text(s.nextTask.stopCondition));
  assert.equal(typeof s.nextTask.requiresPrivateBaseline, 'boolean');
  for (const name of ['functions', 'instructions', 'bindings']) {
    const n = s.admission[name];
    assert.ok(Number.isSafeInteger(n.used) && Number.isSafeInteger(n.limit) && n.used >= 0 && n.limit > 0 && n.used <= n.limit, `Invalid ${name} admission`);
  }
  return s;
}

export function loadState(root = ROOT) {
  return validateState(JSON.parse(fs.readFileSync(path.join(root, 'profiles/development-state.json'), 'utf8')));
}

export function stateBlock(state, filename) {
  validateState(state);
  const prefix = filename.startsWith('docs/') ? '' : 'docs/';
  const manifest = filename.startsWith('docs/') ? '../profiles/' : 'profiles/';
  const sdk = state.milestone === 'M1';
  return `${START}\nRecorded development state: **${state.milestone} / ${state.status}**, profile **${state.checkpoint.profile}**. ${sdk ? 'Retained startup resume task' : 'Next local runtime task'}: **${state.nextTask.id}**.\n\n${state.checkpoint.summary}\n\n[Current runtime evidence](${prefix}status.md) owns detailed observations and limits. [Structured state](${manifest}development-state.json) owns this generated pointer; [workflow](${prefix}development-workflow.md) explains preflight and updates. ${sdk ? `[Goal G](${prefix}local-agent-handoff.md#g-sustained-browser-execution-and-host-delivery-mb05) owns the SDK integration assignment; the retained startup stop is a reference boundary. ` : ''}Source-only work can proceed without private inputs; original execution requires the matching local baseline. No milestone is accepted by this record.\n${END}`;
}

export function replaceStateBlock(body, block) {
  const starts = body.split(START).length - 1, ends = body.split(END).length - 1;
  assert.ok(starts === ends && starts <= 1, 'Malformed or duplicated development-state markers');
  if (starts) {
    const start = body.indexOf(START), end = body.indexOf(END);
    assert.ok(start < end, 'Reversed development-state markers');
    return body.slice(0, start) + block + body.slice(end + END.length);
  }
  const headingEnd = body.indexOf('\n');
  assert.ok(body.startsWith('# ') && headingEnd >= 0, 'Expected document title');
  return body.slice(0, headingEnd + 1) + '\n' + block + '\n' + body.slice(headingEnd + 1);
}

export function syncDocuments(root = ROOT, {check = false} = {}) {
  const state = loadState(root), drift = [];
  for (const name of DOCUMENTS) {
    const file = path.join(root, name), body = fs.readFileSync(file, 'utf8');
    const wanted = replaceStateBlock(body, stateBlock(state, name));
    if (body !== wanted) { drift.push(name); if (!check) fs.writeFileSync(file, wanted); }
    if (state.status === 'in-progress' && ['AGENTS.md', 'plans.md', 'docs/agents.md'].includes(name)) {
      assert.doesNotMatch(body, /Implementation is paused pending|Runtime implementation remains paused|bounded `FUN_001e93e4` goal remains blocked/, `Stale entry guidance in ${name}`);
    }
  }
  if (check) assert.equal(drift.length, 0, `Development state drift: ${drift.join(', ')}. Run npm run dev:state:docs.`);
  return drift;
}

export function sourceCommit(root = ROOT) {
  return execFileSync('git', ['rev-parse', 'HEAD'], {cwd: root, encoding: 'utf8'}).trim();
}

export function workingTreeDirty(root = ROOT) {
  return execFileSync('git', ['status', '--porcelain', '--untracked-files=normal'], {cwd: root, encoding: 'utf8'}).length > 0;
}

export function main(args = process.argv.slice(2)) {
  const [command = 'status', ...rest] = args;
  assert.equal(rest.length, 0, 'Usage: development-state.mjs status|check|docs');
  if (command === 'status') {
    const s = loadState();
    console.log(JSON.stringify({checkoutCommit: sourceCommit(), workingTreeDirty: workingTreeDirty(), ...s,
      browserExecutionWorkMap: 'docs/m1-browser-work.md',
      ...(s.milestone === 'M1' ? {recordedTaskRole: 'retained-startup-resume',
        implementationGuide: 'docs/local-agent-handoff.md#g-sustained-browser-execution-and-host-delivery-mb05'} : {}),
      headroom: Object.fromEntries(Object.entries(s.admission).map(([k,v]) => [k, v.limit - v.used])),
      runtimeExecutedByCommand: false, milestoneAcceptanceChangedByCommand: false}, null, 2));
  } else if (command === 'check' || command === 'docs') {
    const changed = syncDocuments(ROOT, {check: command === 'check'});
    console.log(command === 'check' ? 'Development-state pointers agree.' : `Updated ${changed.length} development-state pointers.`);
  } else throw new Error('Usage: development-state.mjs status|check|docs');
}

if (process.argv[1] && import.meta.url === pathToFileURL(path.resolve(process.argv[1])).href) {
  try { main(); } catch (error) { console.error(error.message); process.exitCode = 1; }
}
