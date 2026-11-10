// Reviewed scope projections. This module never changes live issue status or acceptance.
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
import {loadState} from './development-state.mjs';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const BASE = 'https://github.com/Jdiller-86/fuzion-frenzy-web-port';
export const README_PROGRESS_CHART = '[![Project progress](https://jdiller-86.github.io/fuzion-frenzy-web-port/progress/gates.svg?v=delivery-issues-1)](https://jdiller-86.github.io/fuzion-frenzy-web-port/progress/)';
export const WORK_STATES = ['Open', 'In progress', 'Awaiting evidence', 'Verified connection', 'Not required'];
export const EVIDENCE_LEVELS = ['source/plan only', 'component evidence', 'bounded title evidence', 'reported local candidate', 'reviewed title result'];
const sizes = ['S', 'M', 'L', 'S–M', 'M–L'];
const keys = (value, allowed) => {
  assert.ok(value && typeof value === 'object' && !Array.isArray(value), 'Expected a scope record');
  assert.ok(Object.keys(value).every(key => allowed.includes(key)), 'Unrecognized browser scope field');
};
const text = (value, limit = 4000) => assert.ok(typeof value === 'string' && value.length > 0 && value.length <= limit && !/[<>\r\n|]/.test(value), 'Invalid public scope text');

export function validateBrowserWork(work) {
  keys(work, ['schemaVersion', 'scope', 'sourceObservation', 'sections', 'priorities', 'items']);
  assert.equal(work.schemaVersion, 1, 'Unsupported browser scope schema');
  text(work.scope); assert.match(work.sourceObservation, /^[a-f0-9]{40}$/);
  assert.ok(Array.isArray(work.sections) && work.sections.length > 0 && work.sections.length <= 20);
  const sections = new Set();
  for (const section of work.sections) {
    keys(section, ['id', 'title']); assert.match(section.id, /^[a-z][a-z0-9-]*$/); text(section.title, 150);
    assert.ok(!sections.has(section.id), 'Duplicate browser scope section'); sections.add(section.id);
  }
  assert.equal(work.priorities?.length, 7, 'Keep the seven reviewed M1 priority groups');
  work.priorities.forEach((priority, index) => {
    keys(priority, ['rank', 'title', 'reason']); assert.equal(priority.rank, index + 1); text(priority.title, 150); text(priority.reason);
  });
  assert.ok(Array.isArray(work.items) && work.items.length > 0 && work.items.length <= 80);
  const ids = new Set();
  for (const item of work.items) {
    keys(item, ['id', 'title', 'section', 'workPackage', 'deliveryIssue', 'priority', 'size', 'state', 'evidence', 'startingPoint', 'completion', 'evidenceRef', 'resolution']);
    assert.match(item.id, /^MB[0-9]{2,3}$/); assert.ok(!ids.has(item.id), 'Duplicate browser work ID'); ids.add(item.id);
    text(item.title, 180); assert.ok(sections.has(item.section), 'Unknown browser scope section');
    assert.match(item.workPackage, /^m1-[a-z0-9-]+$/, 'Browser work must name its owning M1 package');
    assert.match(item.deliveryIssue,/^m1-(?:build|verify)-[a-z0-9-]+$/,'Browser work must name its tracked delivery issue');
    assert.ok(Number.isInteger(item.priority) && item.priority >= 0 && item.priority <= 7);
    assert.ok(sizes.includes(item.size), 'Invalid structural size'); assert.ok(WORK_STATES.includes(item.state), 'Invalid browser work state');
    assert.ok(EVIDENCE_LEVELS.includes(item.evidence), 'Invalid browser evidence level');
    text(item.startingPoint); text(item.completion);
    assert.match(item.evidenceRef, /^docs\/[a-z0-9-]+\.md(?:#[a-z0-9-]+)?$/, 'Evidence must link to a curated repository guide');
    if (['Verified connection', 'Not required'].includes(item.state)) {
      text(item.resolution); assert.ok(item.evidenceRef.includes('#'), 'A resolved row needs a specific curated evidence section');
    } else if (item.resolution !== undefined) text(item.resolution);
  }
  assert.ok(ids.has('MB01'), 'The selected scenario must remain explicit');
  for(let number=1;number<=24;number++)assert.ok(ids.has('MB'+String(number).padStart(2,'0')), 'A reviewed browser work ID is missing; retain it with an evidenced Not required resolution');
  for(const priority of work.priorities)assert.ok(work.items.some(item=>item.priority===priority.rank), 'A reviewed priority group is empty');
  assert.equal(work.items.find(item => item.id === 'MB01').priority, 0);
  assert.ok(work.items.every(item => item.priority !== 0 || item.id === 'MB01'), 'Only scenario selection precedes the priority queue');
  return work;
}

export function loadBrowserWork(root = ROOT) {
  const work = validateBrowserWork(JSON.parse(fs.readFileSync(path.join(root, 'profiles/m1-browser-work.json'), 'utf8')));
  for (const item of work.items) assert.ok(fs.existsSync(path.join(root, item.evidenceRef.split('#')[0])), 'Browser evidence guide is missing');
  return work;
}

export function browserItemLink(work, item, prefix = '') {
  return `${prefix}m1-browser-work.md#${item.id.toLowerCase()}`;
}

export function renderPriorityTable(work, {compact = false, prefix = ''} = {}) {
  validateBrowserWork(work);
  let body = compact ? '| Order | M1 work / structural size |\n| --- | --- |\n' : '| Priority | Main work / structural size | Why it comes here / observable early result |\n| --- | --- | --- |\n';
  for (const priority of work.priorities) {
    const items = work.items.filter(item => item.priority === priority.rank);
    const ids = items.map(item => `[${item.id}](${browserItemLink(work, item, prefix)})`).join(', ');
    const size = [...new Set(items.map(item => item.size))].join(' / ');
    body += `| **${priority.rank} — ${priority.title}** | ${ids} · **${size}** |${compact ? '' : ' ' + priority.reason + ' |'}\n`;
  }
  return body.trimEnd();
}

export function renderBrowserSection(work, section) {
  let body = '| ID / work | Starting point and missing browser connection | Completion check and bounded question | Size / evidence |\n| --- | --- | --- | --- |\n';
  for (const item of work.items.filter(item => item.section === section.id)) {
    const resolution = item.resolution ? ` Resolution: ${item.resolution}` : '';
    const tracked=`${BASE}/issues?q=${encodeURIComponent('is:issue label:work:delivery "'+item.deliveryIssue+'"')}`;
    body += `| <a id="${item.id.toLowerCase()}"></a>**${item.id} — ${item.title}** | ${item.startingPoint}${resolution} | ${item.completion} | **${item.size}** · ${item.state}; [${item.evidence}](${path.posix.relative('docs', item.evidenceRef)}) · [tracked delivery](${tracked}) |\n`;
  }
  return body.trimEnd();
}

export function renderNativeBrowserWork(work) {
  validateBrowserWork(work);
  const prefix = `${BASE}/blob/main/docs/`;
  let body = '## M1 browser work map\n\nLargest ready subsystem first; pull forward only the prerequisite needed for the next large proof. Start by selecting MB01\'s scenario.\n\n';
  body += renderPriorityTable(work, {compact: true, prefix}) + '\n\n';
  body += `These ${work.items.length} scope/integration/acceptance rows decompose M1's implementation and evidence packages. They are not additional forecast packages or automatically assignable defects. S/M/L describe engineering breadth, not hours. Row evidence is distinct from this issue's live status and full M1 acceptance.\n\n`;
  body += '| Browser connection | Size / row state / evidence | Observable completion check |\n| --- | --- | --- |\n';
  for (const item of [...work.items].sort((a, b) => a.priority - b.priority || a.id.localeCompare(b.id))) {
    body += `| [${item.id}: ${item.title}](${browserItemLink(work, item, prefix)}) | ${item.size} · ${item.state}; ${item.evidence} | ${item.completion}${item.resolution ? ' Resolution: ' + item.resolution : ''} |\n`;
  }
  return body;
}

export function renderProjectOverview(plan) {
  const work = plan.browserWork;
  let body = '# Fuzion Frenzy Web Port\n\nOriginal Xbox Fuzion Frenzy running entirely in a browser, followed by the complete local game, online play and player experience.\n\n';
  body += `[All goals and acceptance](${BASE}/blob/main/docs/milestones.md) · [Work-package briefs](${BASE}/blob/main/docs/work-packages.md) · [Detailed M1 work](${BASE}/blob/main/docs/m1-browser-work.md) · [Interactive progress](https://jdiller-86.github.io/fuzion-frenzy-web-port/progress/) · [Agent handoff](${BASE}/blob/main/docs/local-agent-handoff.md)\n\n`;
  body += '| Milestone | Product outcome |\n| --- | --- |\n';
  for (const milestone of plan.milestones) body += `| [${milestone.id.toUpperCase()}: ${milestone.title}](${BASE}/issues/${milestone.issueNumber}) | ${milestone.goal} |\n`;
  if (work) body += '\n## Current M1 priority\n\n' + renderPriorityTable(work, {compact: true, prefix: `${BASE}/blob/main/docs/`}) + '\n\nOpen the M1 implementation card for every browser connection, its evidence level, size and completion check. These rows do not add to the task forecast.\n';
  const count=plan.milestones.flatMap(m=>m.tasks).reduce((total,task)=>total+task.brief.deliverables.length,0);
  body += `\n${count} tracked delivery issues sit under ${plan.milestones.flatMap(m=>m.tasks).length} work packages and five acceptance gates. The chart counts delivery issues, with package status and gate acceptance separate; counts are not remaining-effort estimates.\n\nIssue status labels and reviewed acceptance records own live status. This board mirrors those records; merging code or completing component checks does not accept a milestone. Runtime evidence and checkpoint identity remain in [current status](${BASE}/blob/main/docs/status.md). Keep original content and private evidence local.\n`;
  return body;
}

const marked = (key, body) => `<!-- ${key}:start -->\n${body.trim()}\n<!-- ${key}:end -->`;
export function replaceGeneratedBlock(body, key, content) {
  const start = `<!-- ${key}:start -->`, end = `<!-- ${key}:end -->`;
  assert.equal(body.split(start).length, 2, `Missing or duplicate generated block ${key}`);
  assert.equal(body.split(end).length, 2, `Missing or duplicate generated block ${key}`);
  assert.ok(body.indexOf(start) < body.indexOf(end), 'Reversed generated scope markers');
  return body.slice(0, body.indexOf(start)) + marked(key, content) + body.slice(body.indexOf(end) + end.length);
}

export function renderGoalSummary(plan, state, filename) {
  const prefix = filename.startsWith('docs/') ? '' : 'docs/';
  let body = '## Goals and next work\n\n| Milestone | Remaining product outcome | Acceptance and live status |\n| --- | --- | --- |\n';
  for (const milestone of plan.milestones) body += `| **${milestone.id.toUpperCase()}: ${milestone.title}** | ${milestone.goal} | [Criteria](${prefix}milestones.md#${milestone.id}--${milestone.title.toLowerCase().replaceAll(' ', '-')}) · [Native issue](${BASE}/issues/${milestone.issueNumber}) |\n`;
  body += `\n**Recorded development:** ${state.milestone} / ${state.status}, runtime checkpoint **${state.checkpoint.profile}**. [Runtime evidence](${prefix}status.md) distinguishes the accepted checkpoint from local candidates. Native issues own live milestone acceptance.\n\n`;
  body += '**M1 order: largest ready item first.** Select one scenario (MB01), settle the required interfaces, and pull small prerequisites forward only to enable the next large proof.\n\n';
  body += renderPriorityTable(plan.browserWork, {compact: true, prefix}) + '\n\n';
  body += `[All ${plan.browserWork.items.length} browser connections, evidence and exit checks](${prefix}m1-browser-work.md) · [Largest-first details](${prefix}m1-browser-work.md#largest-first-implementation-order) · [All work-package delivery briefs](${prefix}work-packages.md) · [Next local-agent assignment](${prefix}local-agent-handoff.md#bounded-goals-and-selection). These rows decompose existing scope; they are not an additional task estimate or a completion percentage.\n`;
  body += `\n**Tracked scope:** ${plan.milestones.flatMap(m=>m.tasks).reduce((total,task)=>total+task.brief.deliverables.length,0)} delivery issues in ${plan.milestones.flatMap(m=>m.tasks).length} work packages under five milestone gates. Counts are not effort estimates.\n`;
  body += `\n[Tracked delivery issues](${BASE}/issues?q=is%3Aissue+label%3Awork%3Adelivery) are native children of the work-package issues. The README chart and Project use their live status; package closure and milestone acceptance remain separate.\n`;
  return body;
}

export function syncGoalDocuments(plan, {root = ROOT, check = false} = {}) {
  const work = validateBrowserWork(plan.browserWork), state = loadState(root), drift = [];
  const outputs = new Map();
  for (const filename of ['README.md', 'docs/README.md']) {
    const body = fs.readFileSync(path.join(root, filename), 'utf8');
    if(filename==='README.md')assert.equal(body.split(README_PROGRESS_CHART).length,2,'Keep the README progress chart and its interactive-report link');
    outputs.set(filename, replaceGeneratedBlock(body, 'project-goals', renderGoalSummary(plan, state, filename)));
  }
  const filename = 'docs/m1-browser-work.md';
  let body = fs.readFileSync(path.join(root, filename), 'utf8');
  body = replaceGeneratedBlock(body, 'browser-work:provenance', `Initial source observation: \`${work.sourceObservation}\`. Scope: ${work.scope} The current development record is **${state.milestone} / ${state.status}**, runtime checkpoint **${state.checkpoint.profile}**. [Status](status.md) owns accepted evidence and separately identified local candidates. Row state/evidence below describe individual connections; generating this map executes no title and accepts no checkpoint or milestone.`);
  body = replaceGeneratedBlock(body, 'browser-work:inventory', `There are **${work.items.length} named work items**, including scope selection, integration and acceptance. They decompose the existing capability allowances; do not add them to the package forecast or derive a completion percentage. Edit the reviewed [scope data](../profiles/m1-browser-work.json), then run \`npm run project:docs\`; CI checks this map and the README/docs summaries against that source. Native issues own work-package status; each row records its own connection evidence. A verified row does not accept M1.`);
  body = replaceGeneratedBlock(body, 'browser-work:priorities', renderPriorityTable(work));
  for (const section of work.sections) {
    assert.ok(body.includes(`## ${section.title}\n`), 'Browser work section heading is missing');
    body = replaceGeneratedBlock(body, 'browser-work:' + section.id, renderBrowserSection(work, section));
  }
  outputs.set(filename, body);
  for (const [name, wanted] of outputs) {
    if (fs.readFileSync(path.join(root, name), 'utf8') !== wanted) {
      drift.push(name); if (!check) fs.writeFileSync(path.join(root, name), wanted);
    }
  }
  if (check) assert.equal(drift.length, 0, `Goal documentation drift: ${drift.join(', ')}. Run npm run project:docs.`);
  return drift;
}
