> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Exact-head integration candidate review

November 4, 2026. This package lets the architect verify the corrected integration candidate without re-deriving the merge. It is not a promotion request and does not authorize a main merge or any helper admission.

## Refs

| Role | Ref | Notes |
| --- | --- | --- |
| main | `b62989f` | PR #1 merge; contains `cc07556`, governance and `ac207ba` |
| source lane | `b575fc5` | published; PR #2 head |
| candidate | `slice-merge-correct` @ `0b4cae4` | published; not a PR |

## Why this is a merge, not a 20-commit rebase

- The 20-commit-only rebase `b092a6f` passes a reduced tree: check 308 files / runtime 142 tests, with 28 pre-slice source/test files absent (D246).
- The `-X theirs` source merge `88966cc` is governance-lossy: 245 decision rows and only an explicitly re-appended `A023`; main-only `A001-A022` were dropped (D248).
- The candidate is a plain three-way merge of `a16ce50` into `b62989f`. `docs/DECISIONS.md` is a row union; the add/add and living-doc conflicts take the lane-owned source version; main governance docs, architect checkpoints, tools and the `ac207ba` AV-query CI step are preserved.

## Mechanical invariant

```text
node tools/verify-integration.mjs b62989f b575fc5 slice-merge-correct
decision rows: base 166, source 250, candidate 285
missing base rows: 0
missing source rows: 0
duplicate candidate rows: 0
missing source files: 0
integration candidate preserves both decision logs and the source file set
```

The same tool rejects `88966cc` (missing `A001-A022`) and `b092a6f` (missing source files). Its unit tests use synthetic decision tables and file lists; it reads no original content.

## Checks at exact head

| Check | Result |
| --- | --- |
| Candidate `0b4cae4` `npm run check` | 364 files |
| Candidate `test:runtime` | 185 pass + 1 skip (private XBE absent in the temporary worktree) |
| Candidate `test:native` | 34/34 |
| Candidate `test:evidence` | 4/4 |
| Source `b575fc5` `npm run check` | 357 files |
| Source `b575fc5` `test:runtime` | 186/186 |
| `git diff a16ce50 0b4cae4 -- src` | empty |
| CI run `41906707015` on `0b4cae4` | Ubuntu success (native step success), Windows success |
| CI run `41905648969` on `d143a9c` | success |
| CI run `41906133188` on `70d7cf0` | success |

## Retained original-execution pair

- Module `42ee9909ad83092c021226deb94e49999deb64c38d71049e7b7adb76eeb3077b`; executable `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`.
- Control: status 1 at `0xFE000094`, ordinal 2, 279 routines / 404 kernel operations.
- Explicit `avQueryWord=0x00800303`: status 2 at `0x1F6EB0`, 281 / 405.
- Frontier: 98 placeholders, exactly 1 entered: `0x1F6EB0` called by executed `0x1F83C0`.
- These are retained synthetic-startup observations, not fidelity, original service semantics or a playable screen.

## Open decisions for the architect

1. Accept the plain union merge as the integration artifact; the rebase variants are provably incomplete or governance-lossy.
2. Name the landing path: retarget/replace PR #2, open a new main PR from `0b4cae4`, or another reviewed path.
3. State whether this exact-head review permits one bounded `0x1F6EB0` admission.
4. Sol escalation remains unavailable (last attempt `2026-11-04T15:26`, provider usage limit); no approval is claimed.

## Limits

No `0x1F6EB0` admission, no runtime change, no geometry work, no original screen, and no fidelity, online, social or authority claim. Original content remains player supplied and local.
