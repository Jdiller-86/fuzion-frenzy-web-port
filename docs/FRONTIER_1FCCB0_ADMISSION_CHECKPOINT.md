> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# The 0x1FCCB0 admission advances the stop to 0x1F7080

November 4, 2026. The architect directive from integration head `b5294fa` authorized the `0x1FCCB0` admission after a read-only mixed-envelope resync proof. This cycle follows it and stops here. **There is no controllable original screen.**

## Resync fixture (synthetic, read-only)

A source-owned 96-byte synthetic fixture uses a prologue dispatcher, an 8-entry absolute jump table at `0x11030`, four `ret 8` case blocks and padding. The pinned translator emitted all four case targets with no `UNIMPLEMENTED`; fixture SHA-256 `402462c08d414a58196bb7529c9cb36a2b0f3350e892c4a39df10e372383b12b`, generated C SHA-256 `e11bc5905f4115371d7c0752f02d607c2f72091fffaec4f918f69aaaac61ad38`. The check used no original content and did not execute anything. The fixture and the LF-normalized generated C are committed as `tests/mixed_dispatch_fixture_test.py` and `tests/fixtures/mixed-dispatch.generated.c`; the regression reproduces the fixture digest, the generated-text digest, the four targets and the layout/dispatch census without the private toolchain. The retained Windows copy has the same text with CRLF endings and hashes to `f7d4011cc2522568ccd863b4224427b34a4602fc5592cdd80cab6184fc3a96b4`, so the recorded `e11bc590…` digest is the LF-normalized text, not that file's raw bytes.

## Selection and translation

- Table-aware seed: **112-byte envelope**, 18 instructions, 8 four-byte pointer entries at `0x1FCCFC`, 2 undefined spans; SHA-256 `be1dc290a49c30dfc2b3aa108e82f4efb55f112d2579eb4ea3d235252f042f2c`.
- Merged into the front seed (29 functions) to keep **32 exports**; merged seed SHA-256 `3d2597e06a51748f9f113bedd82ee6448edb6f408ea29c92e828e7b99730190e`.
- Translation `recomp-title-recomp-title-post-memory-53`: **422 functions / 17,593 instructions** with `--guard-memory`; `0x1FCCB0` is a mixed layout (32 data bytes, 5 padding bytes, 1 table/site/anchor).
- Source-owned guards: `verify-translation` re-runs at **517 functions, 0 rewrites**; `verify-seeds` re-runs at **32 recorded / 32 offered / 0 unexpected / 0 missing**. The one missing seed (`ca2e5d57…`, generation index 20) was re-exported read-only from the retained Ghidra project and matches the recorded digest exactly, so the 32/32 result is reproducible again.

## Module and controlled pair

Module `recomp-post-memory-build-74`: **663,028 bytes**, SHA-256 `c8c27cb4ec232cff478d16752d649767a34c30ebca45cb8c38195c552aa67657`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x1F7080`** | 285 | 405 |

- Same module and full supplied profile; `avQueryWord` is the only added input.
- `tools/compare-proof-profiles.mjs` exits 0: **+6 routines, +1 kernel operation**.
- `0x1FCCB0` and the prior helpers are in `originalFunctionsReached`; the stop returns to `0x1F57C9` (call site `0x1F57C4`), matching the earlier static candidate.
- Frontier on translation 53: **95 placeholders, 1 entered** — `0x1F7080` called by executed `0x1F5640`; 71 called by an executed caller, 22 static-only.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Table-aware seed | `be1dc290a49c30dfc2b3aa108e82f4efb55f112d2579eb4ea3d235252f042f2c` |
| Merged front seed | `3d2597e06a51748f9f113bedd82ee6448edb6f408ea29c92e828e7b99730190e` |
| `generation.json` | `f8caad7fa190529482b8a99eb75ab6ed3d742171de069b1a349bcd8a8e140024` |
| `title-functions.inc` | `6c8ab6eaba2f856aa76c2200085b6e9b12fc10180d0be4a9cb39604b80d43546` |
| `title-profile.h` | `aa162aaa0c232e422c582766a3ae0faf654639277a9a0ebd51d6f17ff36c9fc1` |
| Module | `c8c27cb4ec232cff478d16752d649767a34c30ebca45cb8c38195c552aa67657` |
| Control profile | `d01ac221ccc107ebd21379953e8124e866a9baf46273dd73f4e9f85a9e4d4419` |
| Experiment profile | `351383c33e37e51baed18fead2f4621c037aa3ca74fef246bbfb8f97ad2c58af` |
| Control report | `db2937a2f44357c6a2fa1812e1b34276f45e39224bbe2d440c12ab9a1427281b` |
| Experiment report | `97a539801de0522631511fac8b76f8a2d2809ef94ebffe1e183b2ce987ee4a21` |

## Review-package revalidation

Before review, the declared artifacts were rechecked against the retained local copies: the module, the three translation outputs, both settings profiles and both reports all match the digests below. `tools/compare-proof-profiles.mjs` on `browser-74-reset3.json` / `browser-74-reset-avquery.json` exits 0 with the pair table above. The frontier tool on translation 53 reports 517 functions, 95 placeholders and 94 referenced placeholders, with `0x1F7080` referenced by `0x1F5640`, matching the checkpoint. The shared report contract now also rejects an unconfigured ordinal-2 call record whose status is neither completed (0) nor denied (1), and requires an actual denial record for any unsupported ordinal-2 stop; a completed call alone cannot satisfy the stop. These close the remaining unexercised control paths noted by the narrow review, and the retained pair still validates. No translation, module or profile content changed.

`tools/verify-recomp-package.mjs` reruns the package with `--manifest docs/FRONTIER_1FCCB0_PACKAGE_MANIFEST.json`: it discovers the recorded seed files by digest, runs `verify-seeds.py` and `verify-translation.py`, applies `compareProofProfiles`, and checks the ten declared artifact digests. On the retained package it reports 32/32 seeds, 422 admitted / 517 generated / 0 rewrites, 10/10 digests, the controlled pair ok (+6 routines / +1 operation), next stop `0x1F7080`, exit 0. `tests/verify-recomp-package.test.mjs` covers seed discovery, a missing root, and manifest digest matching/malformed input.

## Seed retention recovery

The translation's `generation.json` records 32 seed digests; only 31 seed files were retained, and the missing digest was `ca2e5d57df7e25270dbc2ce2bc3ab4d326bd1d0b62310ac57d9adc0d0b593b4b` at generation index 20. Comparing the retained seed function set with the translation's 422 admitted functions isolated the missing seed's only contribution: `0x002022E0` (8-byte body, 2 instructions).

A read-only `ExportRecompilationSeed.java` run on the retained `FuzionFrenzySetup` project (`-noanalysis -readOnly`) exported that function to `research/verification/recovered-seed-002022e0/seed.json` (916 bytes). Its SHA-256 is exactly `ca2e5d57…`, matching the recorded digest. `tools/verify-seeds.py` now reports **32 recorded / 32 offered / 0 unexpected / 0 missing** and exits 0, so the original 32/32 claim is reproducible again. The recovered seed contains original instruction bytes and stays local/ignored; no original content is committed and no seed was substituted or regenerated with different bytes.

## Review outcome

The architect integrated this slice at `92c31ef` (merge of source `33cfb36`, CI run `41940691880`, clean ancestry and source-only delta) after the report-contract and guard hardening. The slice is no longer pending review. The post-integration seed-retention gap was resolved by the exact re-export, and `verify-seeds` now reproduces 32/32. The next stop remains untranslated `0x1F7080`, unadmitted; no further admission follows from the integration.

## Next and limits

The next genuine stop was untranslated **`0x1F7080`**, called by executed `0x1F5640`. It was subsequently admitted at translation 54/module 75; see [the 0x1F7080 admission checkpoint](FRONTIER_1F7080_ADMISSION_CHECKPOINT.md). Per that directive, the lane stopped there.

These are synthetic startup observations with a synthetic avQuery word. They are not console settings, fidelity, renderer, gameplay or original service semantics. No host/Wasm memory substitution was used.
