// Shared issue delivery plans. They describe work; they never record acceptance.
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import {fileURLToPath} from 'node:url';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const BASE = 'https://github.com/Jdiller-86/fuzion-frenzy-web-port';
const START = '<!-- fuzion:delivery:start -->', END = '<!-- fuzion:delivery:end -->';
const SIZES = ['S', 'M', 'L', 'S–M', 'M–L'];
const text = (value, limit = 2500) => assert.ok(typeof value === 'string' && value.length > 0 && value.length <= limit && !/[<>\r\n|]/.test(value), 'Invalid issue delivery text');
function keys(record, allowed) {
  assert.ok(record && typeof record === 'object' && !Array.isArray(record), 'Expected an issue delivery record');
  assert.ok(Object.keys(record).every(key => allowed.includes(key)), 'Unknown issue delivery field');
}
function list(values) {
  assert.ok(Array.isArray(values) && values.length > 0 && values.length <= 10, 'Invalid delivery list');
  values.forEach(value => text(value));
}

export function validateBrief(brief, ownerId) {
  keys(brief, ['context', 'firstStep', 'prerequisites', 'deliverables', 'validation', 'outOfScope', 'stopCondition', 'references']);
  text(brief.context); text(brief.firstStep); text(brief.stopCondition);
  for (const field of ['prerequisites', 'validation', 'outOfScope']) list(brief[field]);
  assert.ok(Array.isArray(brief.deliverables) && brief.deliverables.length > 0 && brief.deliverables.length <= 12);
  const ids = new Set();
  for (const item of brief.deliverables) {
    keys(item, ['id', 'title', 'size', 'doneWhen']);
    assert.match(item.id, /^[a-z0-9-]+$/); assert.ok(item.id.startsWith(ownerId + '-'), 'Delivery slice belongs to another work package');
    assert.ok(!ids.has(item.id), 'Duplicate delivery slice'); ids.add(item.id);
    text(item.title, 200); text(item.doneWhen); assert.ok(SIZES.includes(item.size), 'Invalid delivery size');
  }
  assert.ok(Array.isArray(brief.references) && brief.references.length > 0 && brief.references.length <= 8);
  for (const reference of brief.references) {
    keys(reference, ['path', 'purpose']); text(reference.purpose, 800);
    assert.match(reference.path, /^(?:(?:docs|host|translator|runtime|renderer|web|profiles|tools|tests)\/[A-Za-z0-9_./-]+|\.github\/(?:workflows\/[A-Za-z0-9_-]+\.ya?ml|PULL_REQUEST_TEMPLATE\.md)|(?:package\.json|README\.md|CONTRIBUTING\.md|AGENTS\.md|plans\.md))$/);
    assert.ok(!reference.path.split('/').some(part => part === '..' || part === ''), 'Invalid source reference');
    assert.ok(fs.existsSync(path.join(ROOT, reference.path)) && fs.statSync(path.join(ROOT, reference.path)).isFile(), 'Issue reference must name an existing public file');
  }
  return brief;
}

const bullets = values => values.map(value => '- ' + value).join('\n');
function sourceBase(ref) { assert.match(ref, /^(?:main|[a-f0-9]{40})$/); return `${BASE}/blob/${ref}/`; }
function issueLink(task, lookup) {
  const number = lookup?.get(task.id)?.number ?? task.issueNumber;
  return number ? `[${task.title} (#${number})](${BASE}/issues/${number})` : task.title;
}

export function renderBrief(task, {ref = 'main', localLinks = false} = {}) {
  const brief = validateBrief(task.brief, task.id), base = sourceBase(ref);
  let body = '### Current foundation and gap\n\n' + brief.context + '\n\n### First bounded assignment\n\n' + brief.firstStep;
  body += '\n\n### Inputs and prerequisites\n\n' + bullets(brief.prerequisites);
  body += '\n\n### Delivery slices\n\nChoose the largest ready bounded slice per assignment. Sizes describe engineering breadth, not hours or extra native issues/forecast packages. Required enabling work can move ahead of a larger slice.\n\n';
  body += '| Stable slice / deliverable | Size | Observable exit |\n| --- | --- | --- |\n';
  for (const item of brief.deliverables) body += `| **${item.id}** — ${item.title} | ${item.size} | ${item.doneWhen} |\n`;
  body += '\n### Validation and evidence\n\n' + bullets(brief.validation);
  body += '\n\n### Boundaries\n\n' + bullets(brief.outOfScope) + '\n\n**Stop and report:** ' + brief.stopCondition;
  body += '\n\n### Entry points\n\n';
  for (const reference of brief.references) {
    const target = localLinks ? path.posix.relative('docs', reference.path) : base + reference.path;
    body += `- [${reference.path}](${target}) — ${reference.purpose}\n`;
  }
  return body.trimEnd();
}

export function renderDeliveryPlan(def, lookup, {ref = 'main'} = {}) {
  const base = sourceBase(ref);
  let body = '## Delivery plan\n\nPlanning detail decomposes the existing work package(s). It does not record progress or change the acceptance criteria above.\n\n';
  if (def.type === 'milestone') {
    body += '| Owning work package | Concrete delivery areas | First useful assignment |\n| --- | --- | --- |\n';
    for (const task of def.tasks) {
      validateBrief(task.brief, task.id);
      body += `| ${issueLink(task, lookup)} | ${task.brief.deliverables.map(item => item.title + ' (' + item.size + ')').join('; ')} | ${task.brief.firstStep} |\n`;
    }
    body += '\n**Handoff:** choose one work-package slice, name its stable ID and owner, record inputs/files/checks and stop condition, and report delivered behavior plus concrete remaining features. Acceptance dependencies govern milestone closure; independently useful source-owned analysis may proceed earlier when assigned.\n\n';
    body += '**Milestone review:** reconcile every parent criterion with matching-build evidence and the prerequisite acceptance records, then obtain explicit independent review. Completed children or a merged PR do not accept this gate.\n';
  } else body += renderBrief(def, {ref}) + '\n';
  body += `\n[All work-package briefs](${base}docs/work-packages.md) · [Canonical scope](${base}profiles/project.json) · [Status and acceptance rules](${base}docs/management.md). Keep original content and raw private evidence local.\n`;
  return body.trimEnd() + '\n';
}

export function upsertDeliveryPlan(body, content) {
  const starts = body.split(START).length - 1, ends = body.split(END).length - 1;
  assert.ok(starts === ends && starts <= 1, 'Malformed or duplicate issue delivery markers');
  const managedEnd = body.indexOf('<!-- fuzion:managed:end -->');
  assert.ok(managedEnd >= 0, 'Issue must retain its managed scope boundary');
  const block = START + '\n' + content.trim() + '\n' + END;
  if (starts) {
    const start = body.indexOf(START), end = body.indexOf(END);
    assert.ok(start > managedEnd && end > start, 'Delivery block must follow managed scope');
    return body.slice(0, start) + block + body.slice(end + END.length);
  }
  const boundary = managedEnd + '<!-- fuzion:managed:end -->'.length;
  return body.slice(0, boundary) + '\n\n' + block + body.slice(boundary);
}

export function renderWorkPackages(plan) {
  let body = '# Work-package delivery briefs\n\nGenerated from [the reviewed project scope](../profiles/project.json). Edit that source and run `npm run project:docs`; CI checks this view. These are delivery slices inside the ten existing native work packages, not new issue counts or calibrated effort estimates. The [README chart](../README.md#project-progress) remains the overview of five milestone gates and native task status.\n\n';
  body += 'Live status, ownership and implementation evidence remain in the native issues. Use one bounded assignment at a time, preserve accepted criteria and human notes, and update affected scope/evidence in the same PR. For M1, the [24-connection map](m1-browser-work.md) and its largest-first order refine the implementation package.\n';
  for (const milestone of plan.milestones) {
    body += `\n## ${milestone.id.toUpperCase()} — ${milestone.title}\n\n${milestone.goal}\n\n`;
    for (const task of milestone.tasks) {
      body += `### ${task.id}\n\n**${task.title}** · ${issueLink(task)}\n\n${task.scope}\n\n`;
      body += '**Acceptance dependencies:** ' + (task.dependsOn.length ? task.dependsOn.join(', ') : 'no earlier acceptance gate; respect the inputs and prerequisites below') + '.\n\n';
      body += renderBrief(task, {localLinks: true}).replaceAll('### ', '#### ') + '\n\n**Full work-package acceptance:**\n\n' + bullets(task.criteria.map(criterion => criterion.text)) + '\n\n';
    }
  }
  return body.trimEnd() + '\n';
}
