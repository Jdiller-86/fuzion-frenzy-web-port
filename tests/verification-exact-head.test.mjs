import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import { execFileSync } from 'node:child_process';

import {
  EXTERNAL_GATES,
  containment,
  formatExternalGates,
  formatReport,
  headIsArchitectHead,
  parseSha,
  runExactHeadCheck,
  shortSha,
} from '../tools/verify-exact-head.mjs';

const git = (args, cwd) => execFileSync('git', args, { cwd, encoding: 'utf8', stdio: ['ignore', 'pipe', 'pipe'] });

const commit = (cwd, message) => {
  fs.writeFileSync(path.join(cwd, `${message.replace(/\s+/g, '-')}.txt`), `${message}\n`);
  git(['add', '-A'], cwd);
  git(['-c', 'user.email=lane@example.invalid', '-c', 'user.name=Lane Test',
    'commit', '--quiet', '-m', message], cwd);
};

/**
 * A self-contained repository with three refs:
 *   architect  - the architect branch head
 *   published  - the published merge target
 *   main       - the default branch (the candidate under review)
 */
function makeRepo() {
  const directory = fs.mkdtempSync(path.join(os.tmpdir(), 'exact-head-'));
  git(['init', '--quiet'], directory);
  commit(directory, 'first');
  const mainName = git(['rev-parse', '--abbrev-ref', 'HEAD'], directory).trim();
  git(['branch', 'architect'], directory);
  git(['branch', 'published'], directory);
  commit(directory, 'second');
  git(['switch', '--quiet', 'published'], directory);
  commit(directory, 'third');
  git(['switch', '--quiet', mainName], directory);
  return { directory, mainName };
}

test('only a full lowercase commit id is accepted as a head', () => {
  const sha = 'a'.repeat(40);
  assert.equal(parseSha(sha), sha);
  assert.equal(parseSha(`  ${sha}\n`), sha);
  assert.equal(parseSha('a'.repeat(39)), null);
  assert.equal(parseSha('A'.repeat(40)), null);
  assert.equal(parseSha('not-a-sha'), null);
  assert.equal(parseSha(undefined), null);
  assert.equal(shortSha(sha), 'aaaaaaa');
  assert.equal(shortSha(null), 'unknown');
});

test('a candidate matches the architect head only on an identical commit', () => {
  const head = 'b'.repeat(40);
  assert.equal(headIsArchitectHead(head, head), true);
  assert.equal(headIsArchitectHead(head, 'c'.repeat(40)), false);
  assert.equal(headIsArchitectHead(null, head), false);
});

test('containment needs the base to have no commit of its own', () => {
  assert.deepEqual(containment({ baseOnly: 0, headOnly: 132 }),
    { contained: true, ahead: 132, behind: 0 });
  assert.deepEqual(containment({ baseOnly: 2, headOnly: 228 }),
    { contained: false, ahead: 228, behind: 2 });
});

test('the external promotion gates are listed and never asserted', () => {
  assert.ok(EXTERNAL_GATES.length > 0);
  const listed = formatExternalGates();
  assert.equal(listed.split('\n').length, EXTERNAL_GATES.length);
  for (const gate of EXTERNAL_GATES) {
    assert.ok(gate.includes(':'), `gate is not phrased as a requirement: ${gate}`);
    assert.ok(listed.includes(gate));
    assert.doesNotMatch(gate, /\b(is green|has passed|was merged|approved)\b/i,
      `gate claims an outcome instead of naming the check: ${gate}`);
  }
  assert.match(listed, /ci/);
  assert.match(listed, /pr:/);
  assert.match(listed, /marker:/);
});

test('the report records a marker only for the architect head and never states a CI outcome', () => {
  const { directory, mainName } = makeRepo();
  const stale = runExactHeadCheck({ repo: directory, commit: 'HEAD', architectRef: 'architect',
    baseRef: 'published', localRef: mainName });
  const staleReport = formatReport(stale);
  assert.match(staleReport, /exact-head marker: none/);
  assert.doesNotMatch(staleReport, /exact-head marker: [0-9a-f]{40}/);
  assert.match(staleReport, /not asserted by this tool/);
  assert.doesNotMatch(staleReport, /\b(checks? (?:are )?green|ci passed|merged successfully)\b/i);

  git(['branch', '-f', 'architect', mainName], directory);
  git(['branch', '-f', 'published', 'published~1'], directory);
  const exact = runExactHeadCheck({ repo: directory, commit: 'HEAD', architectRef: 'architect',
    baseRef: 'published', localRef: mainName });
  const exactReport = formatReport(exact);
  assert.match(exactReport, /exact-head marker: [0-9a-f]{40}/);
  assert.match(exactReport, /not asserted by this tool/);
  assert.doesNotMatch(exactReport, /\b(checks? (?:are )?green|ci passed|merged successfully)\b/i);
});

test('a candidate that is not the architect head is rejected', () => {
  const { directory, mainName } = makeRepo();
  const result = runExactHeadCheck({ repo: directory, commit: 'HEAD', architectRef: 'architect',
    baseRef: 'published', localRef: mainName });
  assert.equal(result.ok, false);
  assert.equal(result.exitCode, 1);
  assert.match(result.errors.join(' '), /is not the architect head/);
  assert.equal(result.facts.head.matchesArchitect, false);
});

test('a candidate that does not contain the published base is rejected', () => {
  const { directory, mainName } = makeRepo();
  git(['branch', '-f', 'architect', mainName], directory);
  const result = runExactHeadCheck({ repo: directory, commit: 'HEAD', architectRef: 'architect',
    baseRef: 'published', localRef: mainName });
  assert.equal(result.ok, false);
  assert.equal(result.exitCode, 1);
  assert.match(result.errors.join(' '), /not contained in the head/);
  assert.equal(result.facts.base.contained, false);
});

test('the architect head containing the published base passes and reports local facts', () => {
  const { directory, mainName } = makeRepo();
  git(['branch', '-f', 'architect', mainName], directory);
  git(['branch', '-f', 'published', 'published~1'], directory);
  const result = runExactHeadCheck({ repo: directory, commit: 'HEAD', architectRef: 'architect',
    baseRef: 'published', localRef: mainName });
  assert.equal(result.ok, true, result.errors.join(' '));
  assert.equal(result.exitCode, 0);
  assert.equal(result.facts.head.matchesArchitect, true);
  assert.equal(result.facts.base.contained, true);
  assert.equal(result.facts.local.contained, true);
  assert.equal(result.facts.trackedPathsAtHead, 2);
  assert.equal(result.notes.length, 0);
});

test('an unresolvable candidate or architect ref is a usage error, not a gate failure', () => {
  const { directory, mainName } = makeRepo();
  const missingCommit = runExactHeadCheck({ repo: directory, commit: 'no-such-ref',
    architectRef: 'architect', baseRef: 'published', localRef: mainName });
  assert.equal(missingCommit.ok, false);
  assert.equal(missingCommit.exitCode, 2);
  assert.match(missingCommit.errors.join(' '), /cannot resolve commit/);

  const missingArchitect = runExactHeadCheck({ repo: directory, commit: 'HEAD',
    architectRef: 'no-such-architect-ref', baseRef: 'published', localRef: mainName });
  assert.equal(missingArchitect.ok, false);
  assert.equal(missingArchitect.exitCode, 2);
  assert.match(missingArchitect.errors.join(' '), /cannot resolve architect ref/);
});

test('a missing base ref is reported as unchecked instead of silently passing', () => {
  const { directory, mainName } = makeRepo();
  git(['branch', '-f', 'architect', mainName], directory);
  const result = runExactHeadCheck({ repo: directory, commit: 'HEAD', architectRef: 'architect',
    baseRef: 'origin/main', localRef: mainName });
  assert.equal(result.ok, true, result.errors.join(' '));
  assert.equal(result.facts.base.checked, false);
  assert.match(result.notes.join(' '), /containment was not checked/);
});
