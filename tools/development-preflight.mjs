import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import {createHash} from 'node:crypto';
import {pathToFileURL} from 'node:url';
import {ROOT, loadState, sourceCommit} from './development-state.mjs';

export const REQUIRED = ['xbe', 'analysisProject', 'seedIndex', 'preparedProfile', 'module', 'controlReport', 'inertReport', 'previousReport', 'experimentReport', 'audit', 'toolchainManifest'];
const sha = /^[0-9a-f]{64}$/;

export function validateBaseline(m, state) {
  assert.equal(m.schemaVersion, 1, 'Unsupported private-baseline schema');
  assert.equal(m.checkpointSourceCommit, state.checkpoint.sourceCommit, 'Baseline source differs from the recorded checkpoint');
  assert.equal(m.profile, state.checkpoint.profile, 'Baseline profile differs from the recorded checkpoint');
  assert.ok(Array.isArray(m.files) && m.files.length === REQUIRED.length, 'List every required baseline file exactly once');
  const ids = new Set();
  for (const f of m.files) {
    assert.ok(REQUIRED.includes(f.id) && !ids.has(f.id), 'Unknown or duplicate baseline file identity');
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

export async function inspectBaseline(manifestPath, state) {
  const stat = fs.statSync(manifestPath);
  assert.ok(stat.isFile() && stat.size <= 64 * 1024, 'Baseline manifest must be a regular JSON file of at most 64 KiB');
  const m = validateBaseline(JSON.parse(fs.readFileSync(manifestPath, 'utf8')), state);
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
  return results;
}

export function baselineTemplate(state) {
  return {schemaVersion: 1, checkpointSourceCommit: state.checkpoint.sourceCommit, profile: state.checkpoint.profile,
    files: REQUIRED.map(id => ({id, path: `REPLACE_WITH_LOCAL_${id}_PATH`, bytes: id === 'xbe' ? state.checkpoint.inputBytes : null,
      sha256: ({xbe: state.checkpoint.inputSha256, module: state.checkpoint.moduleSha256, audit: state.checkpoint.auditSha256})[id] ?? 'REPLACE_WITH_RETAINED_SHA256'}))};
}

export async function main(args = process.argv.slice(2)) {
  let manifestPath, requirePrivate = false, template = false;
  for (let i = 0; i < args.length; i++) {
    if (args[i] === '--manifest' && args[i+1] && !args[i+1].startsWith('--')) manifestPath = path.resolve(args[++i]);
    else if (args[i] === '--require-private') requirePrivate = true;
    else if (args[i] === '--template') template = true;
    else throw new Error('Usage: development-preflight.mjs [--manifest LOCAL_JSON] [--require-private] | --template');
  }
  const state = loadState();
  if (template) { assert.ok(!manifestPath && !requirePrivate, '--template cannot check a manifest'); console.log(JSON.stringify(baselineTemplate(state), null, 2)); return; }
  const conventional = path.join(ROOT, 'research/verification/development-baseline.json');
  manifestPath ??= fs.existsSync(conventional) ? conventional : null;
  const results = manifestPath ? await inspectBaseline(manifestPath, state) : REQUIRED.map(id => ({id, status: 'manifest-not-supplied'}));
  const verified = results.every(r => r.status === 'verified');
  console.log(JSON.stringify({schemaVersion: 1, checkoutCommit: sourceCommit(), checkpointSourceCommit: state.checkpoint.sourceCommit,
    profile: state.checkpoint.profile, task: state.nextTask.id, publicSourceAvailable: true, baselineIntegrityVerified: verified,
    runtimeExecutedByCommand: false, milestoneAcceptanceChangedByCommand: false, files: results,
    nextAction: verified ? 'Reproduce the named retained browser cases and verify tool versions before implementation.' : 'Use the local handoff; restore and identify the retained baseline locally. Unaffected source work can continue.',
    limits: 'Digest checks establish named-file integrity only. They do not verify complete Ghidra databases, tool behavior, compiler availability, original execution, or acceptance. Paths and file contents are not reported.'}, null, 2));
  if (requirePrivate && !verified) process.exitCode = 2;
}

if (process.argv[1] && import.meta.url === pathToFileURL(path.resolve(process.argv[1])).href) {
  try { await main(); } catch (error) { console.error(error.code === 'ENOENT' ? 'Private baseline manifest is missing.' : error.message); process.exitCode = 1; }
}
