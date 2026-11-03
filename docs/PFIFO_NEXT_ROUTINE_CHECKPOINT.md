# Next routine 0x1FB5B0 needs 29 D3D dependencies; guard-memory admission is blocked at 0x1FA460

November 4, 2026. The adopted empty-ring profile reaches `0x1FB5B0`; that address is a loop body inside `FUN_001FB580`, a D3D device-defaults routine. A bounded closure export recovered 29 new functions / 1,518 instructions, but the guard-memory recomposition rejects a raw matrix-helper access at `0x1FA460`. The new translation is therefore not admitted into the guarded build. The adopted module `recomp-post-memory-build-58` remains the current reachable build. **There is no playable screen, renderer or device model.**

## Export and recomposition

- Read-only Ghidra closure rooted at `001fb580` produced `research/verification/recomp-post-memory-next-01/seed.json` (private): 29 new functions, 1,518 instructions, 12 unresolved frontier entries.
- The closure was merged into one existing export to preserve the 32-export bound; all 32 retained seeds still compose.
- `prepare-title-recomp.py --function-limit 512 --instruction-limit 16384` produced an unguarded translation with 395 functions / 16,092 instructions.
- A `--guard-memory` recomposition rejected `0x001FA460`: "Unadmitted raw guest pointer or memory helper".

## The blocker

`FUN_001FA460` is a generic 4×4 matrix multiply over raw `float *` inputs. The guard-memory emitter cannot admit its pointer arithmetic without a bounded modeled memory surface or a known guest range. The surrounding `FUN_001FB580` calls D3D transform/render-state/texture-stage helpers, so the reached address is not a single small leaf function; it pulls in render-state machinery.

## Consequence

- Do not bypass guard-memory, hand-admit raw pointers or substitute a host matrix helper: that would weaken the checked-translation contract.
- The next milestone is a bounded memory-surface/admission rule for `0x1FA460` (or a narrower closure that avoids unadmitted helpers), then rebuild with `--guard-memory` and rerun the adopted profile.
- The current reachable module remains `recomp-post-memory-build-58` (527,379 bytes, SHA-256 `6b4ddf6f…`): status 2 at `0x1FB5B0` with `settings-pfifo.json`, status 13 without it.

## Limits

The closure/translation output is private and was not executed. The unguarded 395-function translation was not used to claim execution. No new original routine beyond `0x1FB5B0` was observed. There is no renderer, audio, fidelity or playability.

## Validation

Source tree unchanged; runtime suite 110/110, Python memory-access 10/10, title-seed 25/25, `npm run check` and whitespace checks remain the current verification. The adopted build-58 profile/no-profile runs are recorded in [the adoption checkpoint](PFIFO_PROFILE_ADOPTION_CHECKPOINT.md).

## Next task

Add a bounded modeled memory surface or admission rule for `0x1FA460`, recompose with `--guard-memory`, build a new module and rerun the adopted profile. Alternatively choose a narrower closure that avoids the unadmitted helper. Do not consume a non-empty ring or claim rendering/fidelity without evidence.
