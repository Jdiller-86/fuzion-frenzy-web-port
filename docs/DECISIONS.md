# Current decisions

This record keeps decisions that define the baseline. Older investigation transcripts, superseded diagnoses, automation directives, and duplicate decision rows remain in Git history.

| ID | Decision | Basis and limits |
| --- | --- | --- |
| D016 | Run the original game entirely in the browser; a fallback replica must be identical in every minute detail | The approximate reconstruction and native/streamed execution do not satisfy the product requirement |
| D023 | Pursue static recompilation of the original executable to WebAssembly | Preserve original behavior and player-local content; compatibility, fidelity, performance, and online authority still need proof |
| D429 | Record the bounded M6 stack/return-address repair as met | Commit `ad0bffa` consumes the missing return word and advances past the former fault with control invariance; it does not establish an original screen or complete runtime |
| D445 | Reset to a static main-branch baseline | User-directed November 6 cleanup: close pending PR #21 without merging, retire non-main branches, remove obsolete docs/metadata and snapshots, consolidate current facts, and pause implementation pending the next objective |

## Contracts that remain in force

- Each browser supplies compatible files locally; original files/assets, generated title code, raw analysis, RAM and original-containing save states are not published, uploaded, or shared.
- Browser-local end-user conversion remains a product requirement; the private native preparation workflow is research.
- Original behavior and presentation are the fidelity baseline. Prototype/component fixtures and synthetic profiles are not original-runtime or parity proof.
- Online results must be authoritative; accounts, browser connections, profiles, and seats are distinct, with four total seats. The execution/authority/content contract remains unresolved.
- New project code has no selected license. Public visibility does not establish reuse or original-content rights.

See [the roadmap](../DEVELOPMENT_PLAN.md), [scope](SCOPE_AND_FIDELITY.md), [content policy](PLAYER_SUPPLIED_CONTENT.md), and [runtime gates](RUNTIME_MILESTONES.md). No runtime implementation or pending branch feature was accepted by the cleanup decision.

Record future consequential decisions with a unique ID, the concrete behavior/choice, evidence or user direction, and remaining limits. Superseded decisions belong in Git history rather than an active task log.
