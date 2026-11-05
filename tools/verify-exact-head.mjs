#!/usr/bin/env node
/**
 * Exact-head precheck for an integration candidate.
 *
 * Promotion of a candidate is decided at one exact commit, so the first
 * question is mechanical: is the reviewed commit still the architect branch
 * head, and does that head already contain the published merge target? Those
 * two facts are observable from a source-only checkout. The rest of the
 * promotion gate (GitHub Actions status at that exact SHA, the pull request's
 * open/draft/clean/mergeable state, the architect's exact-head marker) lives
 * on GitHub and cannot be observed here, so this tool LISTS those gates and
 * never asserts them.
 *
 * It reads refs, commit metadata and path counts only. It never reads
 * original content, never changes a ref, never issues or accepts a marker,
 * and never decides that a candidate is correct.
 *
 * The candidate must be the architect head. It may be the local architect
 * branch head or the published one (`origin/codex/opencode-architect`); the
 * report names which one matched, because the local branch often carries
 * unpublished work that is ahead of the published head.
 *
 * Usage:
 *   node tools/verify-exact-head.mjs [--repo <dir>] <commit>
 *       [--architect-ref <ref>] [--published-ref <ref>] [--base-ref <ref>]
 *       [--local-ref <ref>]
 *
 * Exit codes: 0 = locally checkable preconditions hold, 1 = a precondition
 * fails, 2 = usage or unresolved-reference error.
 */
import { execFileSync } from 'node:child_process';
import { pathToFileURL } from 'node:url';

export const DEFAULT_ARCHITECT_REF = 'codex/opencode-architect';
export const DEFAULT_PUBLISHED_REF = 'origin/codex/opencode-architect';
export const DEFAULT_BASE_REF = 'origin/main';
export const DEFAULT_LOCAL_REF = 'main';

/**
 * Promotion gates that this tool deliberately does not assert. Each entry
 * states what an external reviewer must confirm at the exact head SHA.
 */
export const EXTERNAL_GATES = Object.freeze([
  'ci: GitHub Actions "Preparation checks" on ubuntu-latest reports success at the exact head SHA',
  'ci: GitHub Actions "Preparation checks" on windows-latest reports success at the exact head SHA',
  'pr: the pull request for the exact head is open, not a draft, and reported clean/mergeable',
  'marker: the architect issued an exact-head marker for this SHA; this tool neither issues nor accepts one',
  'publication: the source publication guard and whitespace gate are re-run against the exact head',
]);

export function parseSha(text) {
  const value = typeof text === 'string' ? text.trim() : '';
  return /^[0-9a-f]{40}$/.test(value) ? value : null;
}

export function shortSha(sha) {
  return parseSha(sha) ? sha.slice(0, 7) : 'unknown';
}

export function headIsArchitectHead(headSha, architectSha) {
  return Boolean(headSha) && headSha === architectSha;
}

/**
 * Counts from `git rev-list --left-right --count base...head`: the first
 * number is base-only commits (base is not contained in the head when it is
 * non-zero), the second is head-only commits.
 */
export function containment({ baseOnly, headOnly }) {
  const behind = Number.isInteger(baseOnly) ? baseOnly : 0;
  const ahead = Number.isInteger(headOnly) ? headOnly : 0;
  return { contained: behind === 0, ahead, behind };
}

/**
 * How the published merge target relates to the candidate head.
 *
 * `ancestor` - the target is itself in the head's history.
 * `parents-contained` - the target is a merge commit and both of its sides are
 *   in the head's history, so the target's content is carried by the head even
 *   though the target commit itself is not an ancestor. This is the normal
 *   shape after the target was created by merging this branch into main.
 * `divergent` - the target is not in the head's history and it is not a merge
 *   whose sides are all in the head's history.
 */
export function classifyBaseRelation({ isAncestor, parentsContained }) {
  if (isAncestor) return 'ancestor';
  if (parentsContained) return 'parents-contained';
  return 'divergent';
}

/** Paths tracked by the base that the head does not track at all. */
export function missingPaths(basePaths, headPaths) {
  const present = new Set(headPaths);
  return basePaths.filter((file) => !present.has(file));
}

export function formatExternalGates(gates = EXTERNAL_GATES) {
  return gates.map((gate) => `  - ${gate}`).join('\n');
}

function git(args, repo) {
  return execFileSync('git', args, {
    cwd: repo,
    encoding: 'utf8',
    timeout: 60000,
    stdio: ['ignore', 'pipe', 'pipe'],
  });
}

function resolveCommit(ref, repo) {
  if (typeof ref !== 'string' || ref.trim() === '') return null;
  try {
    const out = git(['rev-parse', '--verify', '--end-of-options', `${ref.trim()}^{commit}`], repo);
    return parseSha(out);
  } catch {
    return null;
  }
}

function commitSubject(sha, repo) {
  try {
    return git(['log', '-1', '--format=%s', sha], repo).trim();
  } catch {
    return '';
  }
}

function revListCounts(baseSha, headSha, repo) {
  try {
    const out = git(['rev-list', '--left-right', '--count', `${baseSha}...${headSha}`], repo).trim();
    const [baseOnly, headOnly] = out.split(/\s+/).map((value) => Number.parseInt(value, 10));
    return { baseOnly, headOnly };
  } catch {
    return { baseOnly: null, headOnly: null };
  }
}

function isAncestor(ancestor, descendant, repo) {
  try {
    execFileSync('git', ['merge-base', '--is-ancestor', ancestor, descendant], {
      cwd: repo,
      timeout: 60000,
      stdio: ['ignore', 'pipe', 'pipe'],
    });
    return true;
  } catch {
    return false;
  }
}

function commitParents(sha, repo) {
  try {
    const out = git(['rev-list', '--parents', '-n', '1', sha], repo).trim();
    return out.split(/\s+/).slice(1).filter(Boolean);
  } catch {
    return [];
  }
}

function trackedPaths(sha, repo) {
  try {
    const out = git(['ls-tree', '-r', '--name-only', sha], repo);
    return out.split('\n').filter(Boolean);
  } catch {
    return [];
  }
}

function trackedPathCount(sha, repo) {
  try {
    const out = git(['ls-tree', '-r', '--name-only', sha], repo);
    return out.split('\n').filter(Boolean).length;
  } catch {
    return null;
  }
}

/**
 * Runs the locally observable precheck for one candidate commit.
 *
 * @returns {{ok: boolean, exitCode: number, errors: string[], notes: string[],
 *            facts: object}}
 */
export function runExactHeadCheck(options = {}) {
  const repo = options.repo ?? process.cwd();
  const commit = options.commit;
  const architectRef = options.architectRef ?? DEFAULT_ARCHITECT_REF;
  const publishedRef = options.publishedRef ?? DEFAULT_PUBLISHED_REF;
  const baseRef = options.baseRef ?? DEFAULT_BASE_REF;
  const localRef = options.localRef ?? DEFAULT_LOCAL_REF;

  const errors = [];
  const notes = [];
  const facts = {
    repo,
    architectRef,
    publishedRef,
    baseRef,
    localRef,
    head: null,
    architect: null,
    publishedArchitect: null,
    base: null,
    local: null,
    trackedPathsAtHead: null,
  };

  if (typeof commit !== 'string' || commit.trim() === '') {
    errors.push('a candidate commit is required');
    return { ok: false, exitCode: 2, errors, notes, facts };
  }

  const headSha = resolveCommit(commit, repo);
  if (!headSha) {
    errors.push(`cannot resolve commit ${commit} in ${repo}`);
    return { ok: false, exitCode: 2, errors, notes, facts };
  }
  facts.head = { requested: commit.trim(), sha: headSha, subject: commitSubject(headSha, repo) };

  const architectSha = resolveCommit(architectRef, repo);
  if (!architectSha) {
    errors.push(`cannot resolve architect ref ${architectRef} in ${repo}`);
    return { ok: false, exitCode: 2, errors, notes, facts };
  }
  facts.architect = { ref: architectRef, sha: architectSha };
  facts.head.matchesArchitect = headIsArchitectHead(headSha, architectSha);

  const publishedSha = resolveCommit(publishedRef, repo);
  if (!publishedSha) {
    notes.push(`published ref ${publishedRef} is not resolvable here; only ${architectRef} was compared`);
    facts.publishedArchitect = { ref: publishedRef, sha: null, checked: false };
  } else {
    facts.publishedArchitect = {
      ref: publishedRef,
      sha: publishedSha,
      checked: true,
      matchesHead: headIsArchitectHead(headSha, publishedSha),
    };
  }

  const matchesPublished = facts.publishedArchitect.checked && facts.publishedArchitect.matchesHead;
  facts.head.matchesPublishedArchitect = matchesPublished;
  if (!facts.head.matchesArchitect && !matchesPublished) {
    errors.push(
      `candidate ${shortSha(headSha)} is not the architect head ${shortSha(architectSha)} (${architectRef})`,
    );
  }
  if (!facts.head.matchesArchitect && matchesPublished) {
    notes.push(
      `candidate ${shortSha(headSha)} is the published architect head (${publishedRef}); the local `
        + `${architectRef} is ahead at ${shortSha(architectSha)} with unpublished work`,
    );
  }

  const baseSha = resolveCommit(baseRef, repo);
  if (!baseSha) {
    notes.push(`base ref ${baseRef} is not resolvable here; containment was not checked`);
    facts.base = { ref: baseRef, sha: null, checked: false };
  } else {
    const counts = revListCounts(baseSha, headSha, repo);
    const state = containment(counts);
    const strictAncestor = isAncestor(baseSha, headSha, repo);
    const parents = commitParents(baseSha, repo);
    const parentsContained = parents.length > 1
      && parents.every((parent) => isAncestor(parent, headSha, repo));
    const relation = classifyBaseRelation({ isAncestor: strictAncestor, parentsContained });
    const dropped = missingPaths(trackedPaths(baseSha, repo), trackedPaths(headSha, repo));
    facts.base = {
      ...state,
      ref: baseRef,
      sha: baseSha,
      checked: true,
      relation,
      strictlyContained: relation === 'ancestor',
      contained: relation !== 'divergent',
      parents,
      missingPaths: dropped.length,
    };
    if (relation === 'divergent') {
      errors.push(
        `published base ${baseRef} (${shortSha(baseSha)}) is not contained in the head `
          + `(${state.behind} commit(s) only on ${baseRef})`,
      );
    }
    if (relation === 'parents-contained') {
      notes.push(
        `published base ${baseRef} (${shortSha(baseSha)}) is a merge commit that is not itself an `
          + 'ancestor of the head; both of its sides are in the head history',
      );
    }
    if (dropped.length > 0) {
      errors.push(
        `the head does not track ${dropped.length} path(s) that ${baseRef} tracks `
          + `(${dropped.slice(0, 5).join(', ')})`,
      );
    }
  }

  const localSha = resolveCommit(localRef, repo);
  if (!localSha) {
    notes.push(`local ref ${localRef} is not resolvable here; its divergence was not compared`);
    facts.local = { ref: localRef, sha: null, checked: false };
  } else {
    const counts = revListCounts(localSha, headSha, repo);
    const state = containment(counts);
    facts.local = { ref: localRef, sha: localSha, checked: true, ...state };
    if (!state.contained) {
      notes.push(
        `local ${localRef} (${shortSha(localSha)}) has ${state.behind} commit(s) that the head does not `
          + 'contain; recorded for the architect, not decided here',
      );
    }
  }

  facts.trackedPathsAtHead = trackedPathCount(headSha, repo);

  const ok = errors.length === 0;
  return { ok, exitCode: ok ? 0 : 1, errors, notes, facts };
}

export function formatReport(result) {
  const { facts } = result;
  const lines = ['exact-head precheck'];
  if (!facts.head) {
    for (const error of result.errors) lines.push(`  ERROR           ${error}`);
    return lines.join('\n');
  }
  lines.push(`  head            ${facts.head.sha}  ${facts.head.subject}`);
  lines.push(`  architect head  ${facts.architect.sha}  (${facts.architect.ref})  `
    + `${facts.head.matchesArchitect ? 'match' : 'MISMATCH'}`);
  if (facts.publishedArchitect.checked) {
    lines.push(`  published head  ${facts.publishedArchitect.sha}  (${facts.publishedArchitect.ref})  `
      + `${facts.publishedArchitect.matchesHead ? 'match' : 'MISMATCH'}`);
  } else {
    lines.push(`  published head  unresolved (${facts.publishedArchitect.ref}); not compared`);
  }
  if (facts.base.checked) {
    lines.push(`  published base  ${facts.base.sha}  (${facts.base.ref})  `
      + `${facts.base.relation} `
      + `(head ahead ${facts.base.ahead}, behind ${facts.base.behind}, `
      + `missing base paths ${facts.base.missingPaths})`);
  } else {
    lines.push(`  published base  unresolved (${facts.base.ref}); containment not checked`);
  }
  if (facts.local.checked) {
    lines.push(`  local ref       ${facts.local.sha}  (${facts.local.ref})  `
      + `${facts.local.contained ? 'contained in head' : 'not contained in head'} `
      + `(head ahead ${facts.local.ahead}, behind ${facts.local.behind})`);
  } else {
    lines.push(`  local ref       unresolved (${facts.local.ref}); divergence not compared`);
  }
  lines.push(`  tracked paths at head  ${facts.trackedPathsAtHead ?? 'unknown'}`);
  for (const note of result.notes) lines.push(`  note            ${note}`);
  for (const error of result.errors) lines.push(`  ERROR           ${error}`);
  lines.push('external promotion gates (listed here, not asserted by this tool):');
  lines.push(formatExternalGates());
  lines.push(facts.head.matchesArchitect || facts.head.matchesPublishedArchitect
    ? `exact-head marker: ${facts.head.sha}`
    : 'exact-head marker: none (the candidate is not the architect head)');
  return lines.join('\n');
}

function parseArgs(argv) {
  const options = { repo: process.cwd(), commit: null, architectRef: DEFAULT_ARCHITECT_REF,
    publishedRef: DEFAULT_PUBLISHED_REF, baseRef: DEFAULT_BASE_REF, localRef: DEFAULT_LOCAL_REF };
  for (let index = 0; index < argv.length; index += 1) {
    const arg = argv[index];
    if (arg === '--repo') options.repo = argv[++index];
    else if (arg === '--architect-ref') options.architectRef = argv[++index];
    else if (arg === '--published-ref') options.publishedRef = argv[++index];
    else if (arg === '--base-ref') options.baseRef = argv[++index];
    else if (arg === '--local-ref') options.localRef = argv[++index];
    else if (arg.startsWith('--')) {
      throw new Error(`unknown option ${arg}`);
    } else if (options.commit === null) options.commit = arg;
    else throw new Error(`unexpected argument ${arg}`);
  }
  return options;
}

function main(argv) {
  let options;
  try {
    options = parseArgs(argv);
  } catch (error) {
    console.error(`usage: node tools/verify-exact-head.mjs [--repo <dir>] <commit> `
      + '[--architect-ref <ref>] [--published-ref <ref>] [--base-ref <ref>] [--local-ref <ref>]');
    console.error(error.message);
    return 2;
  }
  const result = runExactHeadCheck(options);
  console.log(formatReport(result));
  if (!result.ok) {
    console.error('exact-head precheck failed');
    return result.exitCode;
  }
  console.log('exact head matches the architect head and contains the published base');
  return 0;
}

if (process.argv[1] && import.meta.url === pathToFileURL(process.argv[1]).href) {
  process.exitCode = main(process.argv.slice(2));
}
