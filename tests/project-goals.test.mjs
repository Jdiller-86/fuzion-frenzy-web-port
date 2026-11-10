import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import {ROOT,loadPlan,definitions,renderIssue} from '../tools/project.mjs';
import {loadBrowserWork,validateBrowserWork,syncGoalDocuments,replaceGeneratedBlock,renderProjectOverview} from '../tools/project-goals.mjs';

test('browser map keeps stable unique IDs, conditional scope, structural sizes and supported evidence states', () => {
  const work = loadBrowserWork();
  assert.equal(work.items.length, 24); assert.equal(work.priorities.length, 7);
  assert.deepEqual(work.items.map(item => item.id), Array.from({length:24}, (_, i) => 'MB' + String(i + 1).padStart(2, '0')));
  for (const mutate of [w => w.items.push(w.items[0]), w => w.items.pop(), w => w.items[0].priority = 8, w => w.items[1].size = '3 days', w => w.items[1].section = 'missing', w => w.items[1].state = 'Accepted', w => w.items[1].evidenceRef = 'https://example.org', w => w.privateNote = 'never publish', w => w.items[0].title = '<script>']) {
    const invalid = structuredClone(work); mutate(invalid); assert.throws(() => validateBrowserWork(invalid));
  }
  const resolved = structuredClone(work); resolved.items[0].state = 'Verified connection';
  assert.throws(() => validateBrowserWork(resolved));
  resolved.items[0].resolution = 'The selected recipe is recorded and independently reviewed.';
  assert.throws(() => validateBrowserWork(resolved), /specific curated/);
  resolved.items[0].evidenceRef = 'docs/status.md#selected-recipe';
  assert.doesNotThrow(() => validateBrowserWork(resolved));
});

test('scope edits invalidate generated map and summaries while generation preserves outside prose', () => {
  const root = fs.mkdtempSync(path.join(os.tmpdir(), 'fuzion-goal-drift-'));
  try {
    for (const filename of ['README.md', 'docs/README.md', 'docs/m1-browser-work.md', 'profiles/development-state.json']) {
      fs.mkdirSync(path.dirname(path.join(root, filename)), {recursive:true});
      fs.copyFileSync(path.join(ROOT, filename), path.join(root, filename));
    }
    const plan = structuredClone(loadPlan());
    assert.deepEqual(syncGoalDocuments(plan, {root, check:true}), []);
    plan.browserWork.items.find(item => item.id === 'MB15').title = 'Changed required shader feature';
    assert.throws(() => syncGoalDocuments(plan, {root, check:true}), /Goal documentation drift/);
    const readme = path.join(root, 'README.md'); fs.appendFileSync(readme, '\nHuman contribution note.\n');
    const changed = syncGoalDocuments(plan, {root}); assert.ok(changed.includes('docs/m1-browser-work.md'));
    assert.match(fs.readFileSync(readme, 'utf8'), /Human contribution note/);
    assert.deepEqual(syncGoalDocuments(plan, {root, check:true}), []);
    plan.browserWork.priorities[0].title = 'Changed execution priority';
    assert.throws(() => syncGoalDocuments(plan, {root, check:true}), /README.md/);
    syncGoalDocuments(plan, {root});
    assert.match(fs.readFileSync(readme, 'utf8'), /Changed execution priority/);
  } finally { fs.rmSync(root, {recursive:true, force:true}); }
});

test('missing, duplicate and reversed generated markers cannot hide stale projections', () => {
  for (const body of ['', '<!-- x:start --><!-- x:end --><!-- x:start -->', '<!-- x:end --><!-- x:start -->']) {
    assert.throws(() => replaceGeneratedBlock(body, 'x', 'updated'));
  }
});

test('Project overview and native M1 card expose all scope without creating issues or altering acceptance', () => {
  const plan = loadPlan(), defs = definitions(plan), owner = defs.find(item => item.id === 'm1-build');
  assert.equal(defs.length, 15); assert.equal(plan.milestones.flatMap(milestone => milestone.tasks).length, 10);
  const original = renderIssue(owner, null, new Map()) + '\nHuman evidence remains.\n';
  const current = {body:original.replace('- [ ]', '- [x]')};
  const updated = renderIssue(owner, current, new Map());
  for (const item of plan.browserWork.items) assert.ok(updated.includes(item.id + ': ' + item.title));
  assert.ok(updated.includes('- [x]')); assert.ok(updated.endsWith('Human evidence remains.\n'));
  assert.match(updated, /not additional forecast packages/);
  const overview = renderProjectOverview(plan);
  for (const milestone of plan.milestones) assert.ok(overview.includes(milestone.goal));
  for (const priority of plan.browserWork.priorities) assert.ok(overview.includes(priority.title));
  assert.doesNotMatch(overview, /implementation remains paused|[0-5]\/5 accepted/i);
});

test('refresh workflows observe all main pushes and reconcile missed events from current trusted main', () => {
  const tracking = fs.readFileSync(path.join(ROOT, '.github/workflows/project.yml'), 'utf8');
  assert.match(tracking, /push:\s+branches: \[main\]\s+schedule:/);
  assert.match(tracking, /cron: '11 \* \* \* \*'/);
  assert.match(tracking, /ref: \$\{\{ steps.trusted.outputs.sha \}\}/);
  assert.doesNotMatch(tracking, /TRACKING_SOURCE_SHA: \$\{\{ github.sha \}\}/);
  assert.match(tracking, /\[ "\$TRACKING_EVENT" != workflow_dispatch \]/);
  const publish = fs.readFileSync(path.join(ROOT, '.github/workflows/publish-progress.yml'), 'utf8');
  for (const filename of ['profiles/m1-browser-work.json', 'profiles/development-state.json', 'tools/project-goals.mjs']) assert.ok(publish.includes(filename));
});
