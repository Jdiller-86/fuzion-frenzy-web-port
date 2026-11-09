import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import {spawnSync} from 'node:child_process';
import {pathToFileURL} from 'node:url';
import {ROOT, sourceCommit, workingTreeDirty} from './development-state.mjs';

export const TASKS = Object.freeze({workflow: ['test:workflow'], runtime: ['test:runtime'], native: ['test:native'],
  evidence: ['test:evidence'], formats: ['test:formats'], gameplay: ['test:gameplay'], project: ['test:project']});
const PHASES = ['setup','prepare','build','browser','review','report','source-checks'];
const OUTCOMES = ['passed','failed','blocked'];
const FAILURES = ['none','harness','runtime','prerequisite','source-check','unknown'];
const scopePattern = /^[A-Za-z0-9][A-Za-z0-9_.:-]{0,79}$/;

export function validateRecord(r) {
  assert.equal(r.schemaVersion, 1);
  assert.ok(['automated-source-checks','reported-local-work'].includes(r.kind));
  assert.match(r.scope, scopePattern);
  assert.ok(PHASES.includes(r.phase) && OUTCOMES.includes(r.outcome));
  assert.ok(FAILURES.includes(r.failureClass));
  assert.equal(r.outcome === 'passed', r.failureClass === 'none', 'Only passed samples use failure class none');
  assert.ok(Number.isSafeInteger(r.durationMs) && r.durationMs >= 0 && r.durationMs <= 7 * 24 * 3600 * 1000);
  assert.match(r.sourceCommit, /^[0-9a-f]{40}$/);
  assert.equal(typeof r.workingTreeDirty, 'boolean');
  assert.ok(typeof r.timestamp === 'string' && Number.isFinite(Date.parse(r.timestamp)));
  assert.equal(r.acceptance, false, 'Timing records cannot accept milestones');
  return r;
}

function recordPath(root) {
  const dir = path.join(root, 'artifacts/development');
  const localStat = location => {
    try { return fs.lstatSync(location); }
    catch (error) { if (error.code === 'ENOENT') return null; throw error; }
  };
  for (const relative of ['artifacts','artifacts/development']) {
    const location = path.join(root, relative);
    const stat = localStat(location);
    if (stat) assert.ok(stat.isDirectory() && !stat.isSymbolicLink(), 'Timing directory must not be a symlink');
    else fs.mkdirSync(location);
  }
  const file = path.join(dir, 'iterations.jsonl');
  const stat = localStat(file);
  if (stat) assert.ok(stat.isFile() && !stat.isSymbolicLink(), 'Timing log must be a regular file');
  return file;
}

export function appendRecord(root, record) {
  validateRecord(record);
  // Explicit projection prevents free-form/private fields entering the log.
  const {schemaVersion,kind,scope,phase,outcome,failureClass,durationMs,sourceCommit,workingTreeDirty,timestamp,acceptance} = record;
  const saved = {schemaVersion,kind,scope,phase,outcome,failureClass,durationMs,sourceCommit,workingTreeDirty,timestamp,acceptance};
  fs.appendFileSync(recordPath(root), JSON.stringify(saved) + '\n', {mode: 0o600});
  return saved;
}

export function summarize(records) {
  const groups = new Map();
  for (const r of records) {
    validateRecord(r);
    const key = `${r.kind}/${r.phase}/${r.outcome}/${r.failureClass}`;
    const group = groups.get(key) ?? {kind:r.kind, phase:r.phase, outcome:r.outcome, failureClass:r.failureClass, samples:0, measuredMs:0};
    group.samples++; group.measuredMs += r.durationMs; groups.set(key, group);
  }
  return {samples: records.length, groups: [...groups.values()], limits: 'Samples may overlap and have unequal scope. Durations are not project elapsed time, velocity, a forecast, or evidence of runtime acceptance.'};
}

export function runScripts(root, scripts, run = spawnSync) {
  const pkg = JSON.parse(fs.readFileSync(path.join(root, 'package.json'), 'utf8'));
  // Use npm's JS entrypoint with the active Node, without shell interpolation.
  const npm = process.env.npm_execpath;
  assert.ok(npm && fs.statSync(npm).isFile(), 'Run through npm run dev:task so the pinned npm entrypoint is available');
  for (const script of scripts) {
    assert.ok(Object.values(TASKS).flat().includes(script), 'Unknown check script');
    assert.equal(typeof pkg.scripts[script], 'string', `Missing ${script}`);
    const result = run(process.execPath, [npm, 'run', script], {cwd:root, stdio:'inherit', shell:false});
    if (result.error || result.status !== 0) return result.status || 1;
  }
  return 0;
}

export function main(args = process.argv.slice(2)) {
  const [command = 'help', ...rest] = args;
  if (command === 'help') { console.log('npm run dev:task -- <workflow|runtime|native|evidence|formats|gameplay|project> [--scope SCOPE-ID]\nnpm run dev:task -- record --scope SCOPE-ID --phase <setup|prepare|build|browser|review|report> --seconds N --outcome <passed|failed|blocked> [--failure-class CLASS]\nnpm run dev:task -- summary\nRuns source checks only. Manual records contain timings, never raw reports or acceptance.'); return; }
  if (command === 'summary') {
    assert.equal(rest.length,0);
    const file = recordPath(ROOT);
    const records = fs.existsSync(file) ? fs.readFileSync(file,'utf8').split('\n').filter(Boolean).map(line => JSON.parse(line)) : [];
    console.log(JSON.stringify(summarize(records),null,2)); return;
  }
  const options = {};
  for (let i=0; i<rest.length; i+=2) {
    const flag = rest[i], value = rest[i+1];
    assert.ok(['--scope','--phase','--seconds','--outcome','--failure-class'].includes(flag) && value && !value.startsWith('--') && !(flag in options), 'Invalid or duplicate task option');
    options[flag] = value;
  }
  const scope = options['--scope'] ?? `source:${command}`;
  assert.match(scope, scopePattern);
  const base = {schemaVersion:1, scope, timestamp:new Date().toISOString(), sourceCommit:sourceCommit(), workingTreeDirty:workingTreeDirty(), acceptance:false};
  if (command === 'record') {
    assert.ok(options['--scope'] && options['--phase'] && options['--seconds'] && options['--outcome'], 'Manual records require scope, phase, seconds and outcome');
    assert.ok(/^(?:0|[1-9][0-9]*)(?:\.[0-9]{1,3})?$/.test(options['--seconds']), 'Use nonnegative seconds with at most three decimal places');
    const record = appendRecord(ROOT, {...base, kind:'reported-local-work', phase:options['--phase'], outcome:options['--outcome'],
      failureClass:options['--failure-class'] ?? (options['--outcome'] === 'passed' ? 'none' : 'unknown'), durationMs:Math.round(Number(options['--seconds']) * 1000)});
    console.log(JSON.stringify(record,null,2)); return;
  }
  assert.ok(Object.hasOwn(TASKS, command), 'Unknown task; run npm run dev:task -- help');
  assert.ok(Object.keys(options).every(k => k === '--scope'), 'Automated tasks accept only --scope');
  const start = performance.now(), code = runScripts(ROOT, TASKS[command]);
  const record = appendRecord(ROOT, {...base, kind:'automated-source-checks', phase:'source-checks', outcome:code ? 'failed' : 'passed',
    failureClass:code ? 'source-check' : 'none', durationMs:Math.round(performance.now()-start)});
  console.log(JSON.stringify(record,null,2)); process.exitCode = code;
}

if (process.argv[1] && import.meta.url === pathToFileURL(path.resolve(process.argv[1])).href) {
  try { main(); } catch (error) { console.error(error.message); process.exitCode = 1; }
}
