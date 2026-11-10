import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import {createHash} from 'node:crypto';
import {execFileSync} from 'node:child_process';
import {pathToFileURL} from 'node:url';
import {ROOT, loadState, sourceCommit} from './development-state.mjs';

export const REQUIRED = ['xbe', 'analysisProject', 'seedIndex', 'preparedProfile', 'module', 'controlReport', 'inertReport', 'previousReport', 'experimentReport', 'audit', 'toolchainManifest'];
export const REPLAY_REQUIRED = ['xbe', 'module', 'controlReport', 'inertReport', 'previousReport', 'experimentReport', 'audit', 'toolchainManifest'];
const sha = /^[0-9a-f]{64}$/;
const requiredFor = purpose => {
  if (!['analysis', 'replay'].includes(purpose)) throw new Error('Unknown private preflight purpose');
  return purpose === 'replay' ? REPLAY_REQUIRED : REQUIRED;
};

export function validateBaseline(m, state, {purpose = 'analysis'} = {}) {
  const required = requiredFor(purpose);
  assert.equal(m.schemaVersion, purpose === 'replay' ? 2 : 1, 'Unsupported private-baseline schema for this purpose');
  if (purpose === 'replay') assert.ok(m.purpose === 'replay', 'Private baseline purpose mismatch');
  assert.equal(m.checkpointSourceCommit, state.checkpoint.sourceCommit, 'Baseline source differs from the recorded checkpoint');
  assert.equal(m.profile, state.checkpoint.profile, 'Baseline profile differs from the recorded checkpoint');
  assert.ok(Array.isArray(m.files) && m.files.length === required.length, 'List every required baseline file exactly once');
  const ids = new Set();
  for (const f of m.files) {
    assert.ok(required.includes(f.id) && !ids.has(f.id), 'Unknown or duplicate baseline file identity');
    ids.add(f.id);
    assert.ok(typeof f.path === 'string' && f.path.length > 0 && !f.path.includes('\0'), 'Invalid local path');
    assert.match(f.sha256, sha, `Missing SHA-256 for ${f.id}`);
    assert.ok(Number.isSafeInteger(f.bytes) && f.bytes > 0, `Missing byte count for ${f.id}`);
  }
  for (const [id, field] of [['xbe','inputSha256'], ['module','moduleSha256'], ['audit','auditSha256']]) {
    assert.equal(m.files.find(f => f.id === id).sha256, state.checkpoint[field], `${id} identity differs from the accepted checkpoint`);
  }
  assert.equal(m.files.find(f => f.id === 'xbe').bytes, state.checkpoint.inputBytes);
  return m;
}

async function hashFile(file) {
  const hash = createHash('sha256');
  for await (const bytes of fs.createReadStream(file)) hash.update(bytes);
  return hash.digest('hex');
}

export async function inspectBaseline(manifestPath, state, {purpose = 'analysis'} = {}) {
  const stat = fs.statSync(manifestPath);
  assert.ok(stat.isFile() && stat.size <= 64 * 1024, 'Baseline manifest must be a regular JSON file of at most 64 KiB');
  const m = validateBaseline(JSON.parse(fs.readFileSync(manifestPath, 'utf8')), state, {purpose});
  const results = [];
  // Paths are read locally, never executed or included in the report.
  for (const f of m.files) {
    const file = path.resolve(path.dirname(manifestPath), f.path);
    let status;
    try {
      const before = fs.statSync(file);
      if (!before.isFile() || before.size !== f.bytes) status = 'size-or-type-mismatch';
      else {
        const digest = await hashFile(file), after = fs.statSync(file);
        status = before.size !== after.size || before.mtimeMs !== after.mtimeMs || before.ino !== after.ino ? 'changed-during-check' : digest === f.sha256 ? 'verified' : 'digest-mismatch';
      }
    } catch (e) { status = e.code === 'ENOENT' ? 'missing' : 'unreadable'; }
    results.push({id: f.id, status});
  }
  if (purpose === 'replay') {
    const result = id => results.find(row => row.id === id);
    if (result('audit').status === 'verified') {
      const auditFile = path.resolve(path.dirname(manifestPath), m.files.find(f => f.id === 'audit').path);
      // Rehash the bounded bytes being parsed; an unverified audit never grants provenance.
      const auditSize = fs.statSync(auditFile).size;
      assert.ok(auditSize <= 512 * 1024, 'Replay audit exceeds the metadata budget');
      const bytes = fs.readFileSync(auditFile);
      if (createHash('sha256').update(bytes).digest('hex') !== state.checkpoint.auditSha256) {
        result('audit').status = 'changed-during-check';
      } else {
        const audit = JSON.parse(bytes.toString('utf8'));
        const run = audit.harness?.run;
        const validRun = typeof run === 'string' && new RegExp(`^browser-${state.checkpoint.profile}-run-[0-9]{2}$`).test(run);
        if (!validRun || audit.moduleSha256 !== state.checkpoint.moduleSha256) result('audit').status = 'provenance-mismatch';
        else for (const [id, role] of [['controlReport','control'], ['inertReport','inert'], ['previousReport','default'], ['experimentReport','selected']]) {
          const expected = audit.artifacts?.[`${run}/browser-${role}.json`];
          if (!sha.test(expected ?? '') || m.files.find(f => f.id === id).sha256 !== expected) result(id).status = 'provenance-mismatch';
        }
      }
    }
    // This fixed source path/commit is code-owned, never a manifest command or query.
    let toolchainDigest;
    try {
      const bytes = execFileSync('git', ['show', `${state.checkpoint.sourceCommit}:tools/original-execution-toolchain.json`],
        {cwd: ROOT, maxBuffer: 64 * 1024, stdio: ['ignore', 'pipe', 'ignore']});
      toolchainDigest = createHash('sha256').update(bytes).digest('hex');
    } catch { result('toolchainManifest').status = 'provenance-unavailable'; }
    if (toolchainDigest && m.files.find(f => f.id === 'toolchainManifest').sha256 !== toolchainDigest) result('toolchainManifest').status = 'provenance-mismatch';
  }
  return results;
}

export function baselineTemplate(state, {purpose = 'analysis'} = {}) {
  const required = requiredFor(purpose);
  return {schemaVersion: purpose === 'replay' ? 2 : 1, ...(purpose === 'replay' ? {purpose} : {}), checkpointSourceCommit: state.checkpoint.sourceCommit, profile: state.checkpoint.profile,
    files: required.map(id => ({id, path: `REPLACE_WITH_LOCAL_${id}_PATH`, bytes: id === 'xbe' ? state.checkpoint.inputBytes : null,
      sha256: ({xbe: state.checkpoint.inputSha256, module: state.checkpoint.moduleSha256, audit: state.checkpoint.auditSha256})[id] ?? 'REPLACE_WITH_RETAINED_SHA256'}))};
}

export async function main(args = process.argv.slice(2)) {
  let manifestPath, requirePrivate = false, template = false, purpose = 'analysis';
  for (let i = 0; i < args.length; i++) {
    if (args[i] === '--manifest' && args[i+1] && !args[i+1].startsWith('--')) manifestPath = path.resolve(args[++i]);
    else if (args[i] === '--require-private') requirePrivate = true;
    else if (args[i] === '--template') template = true;
    else if (args[i] === '--purpose' && args[i+1] && !args[i+1].startsWith('--')) purpose = args[++i];
    else throw new Error('Usage: development-preflight.mjs [--purpose analysis|replay] [--manifest LOCAL_JSON] [--require-private] | [--purpose analysis|replay] --template');
  }
  const required = requiredFor(purpose);
  const state = loadState();
  if (template) { assert.ok(!manifestPath && !requirePrivate, '--template cannot check a manifest'); console.log(JSON.stringify(baselineTemplate(state, {purpose}), null, 2)); return; }
  const conventional = path.join(ROOT, `research/verification/${purpose === 'replay' ? 'development-replay-baseline' : 'development-baseline'}.json`);
  manifestPath ??= fs.existsSync(conventional) ? conventional : null;
  const results = manifestPath ? await inspectBaseline(manifestPath, state, {purpose}) : required.map(id => ({id, status: 'manifest-not-supplied'}));
  const verified = results.every(r => r.status === 'verified');
  console.log(JSON.stringify({schemaVersion: 1, checkoutCommit: sourceCommit(), checkpointSourceCommit: state.checkpoint.sourceCommit,
    profile: state.checkpoint.profile, task: state.nextTask.id, purpose, publicSourceAvailable: true, baselineIntegrityVerified: verified,
    verificationScope: purpose === 'replay' ? 'retained-module-replay-inputs' : 'recorded-analysis-baseline-files',
    analysisProjectFileVerified: purpose === 'analysis' && results.find(r => r.id === 'analysisProject')?.status === 'verified',
    completeAnalysisDatabaseVerified: false, harnessClosureVerified: false, newOriginalCodeFactsVerified: false,
    runtimeExecutedByCommand: false, milestoneAcceptanceChangedByCommand: false, files: results,
    nextAction: verified ? 'Verify the reviewed harness dependency closure and tools, then reproduce the named cases. New original-code facts require separate provenance review.' : 'Restore the inputs required for this purpose from retained provenance. Unaffected source work can continue.',
    limits: 'Named-file integrity only. Replay report hashes come from the state-pinned audit and toolchain provenance from the checkpoint Git blob. Harness dependency closure, complete analysis databases, new code facts, tool behavior, execution and acceptance require separate checks. Paths and file contents are not reported.'}, null, 2));
  if (requirePrivate && !verified) process.exitCode = 2;
}

if (process.argv[1] && import.meta.url === pathToFileURL(path.resolve(process.argv[1])).href) {
  try { await main(); } catch (error) {
    const message = error instanceof SyntaxError ? 'Invalid private baseline JSON.' :
      error.code === 'ENOENT' ? 'Private baseline manifest is missing.' :
      typeof error.code === 'string' ? 'Cannot inspect the private baseline manifest.' : error.message.split('\n')[0];
    console.error(message); process.exitCode = 1;
  }
}
