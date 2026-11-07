# Current decisions

This record keeps decisions that define the baseline. Older investigation transcripts, superseded diagnoses, automation directives, and duplicate decision rows remain in Git history.

| ID | Decision | Basis and limits |
| --- | --- | --- |
| D016 | Run the original game entirely in the browser; a fallback replica must be identical in every minute detail | The approximate reconstruction and native/streamed execution do not satisfy the product requirement |
| D023 | Pursue static recompilation of the original executable to WebAssembly | Preserve original behavior and player-local content; compatibility, fidelity, performance, and online authority still need proof |
| D429 | Record the bounded legacy runtime M6 stack/return-address repair as met | Commit `ad0bffa` consumes the missing return word and advances past the former fault with control invariance; it does not establish an original screen or complete runtime |
| D445 | Reset to a static main-branch baseline | User-directed November 6 cleanup: close pending PR #21 without merging, retire non-main branches, remove obsolete docs/metadata and snapshots, consolidate current facts, and pause implementation pending the next objective |
| D446 | Replace the runtime ladder with five verifiable project milestones | User-directed planning modeled after CTR-2-WUP: original browser execution, complete local game/conversion, online/recovery, player experience and release readiness. All five remain unaccepted at the planning baseline. |
| D447 | Adopt domain source layout and repository-native tracking | Host, translator, renderer, portable/browser runtime and web boundaries; five parent issues with a small work breakdown, native relationships and verified progress exports. Metadata workflows start no coding agents or game execution. |
| D448 | Add a public GitHub Project modeled after CTR-2-WUP | User-authorized board creation using an owner-configured Actions secret. Copy field/view configuration, import only this repository's managed issues, pin native Project identity and verify membership/status. Issue evidence owns milestone acceptance; board setup starts no implementation. |
| D449 | Embed a derived tracking graph in the README | User-requested CTR-style SVG overview and interactive graph, refreshed from validated native issue status. Publish only report files to GitHub Pages, preserve source/snapshot provenance, keep task completion separate from accepted gates, and exclude original/private content. |
| D450 | Persist the agreed local session agent structure | Project-local defaults select Sol High and up to two Luna Low workers; repository instructions define ownership, review and task boundaries. Verify model access and actual execution on each local client. Active sessions, global settings, private-input gates and unattended scheduling remain separate. |
| D451 | Route the reached DOS-drive create through the mounted FATX provider | Chrome observed ordinal 190 with disposition 5 and a title-created link to the mounted synthetic utility partition. Admit the missing-file case in its owned single-cluster root. Guard existing targets and other file operations until their behavior is established. |
| D452 | Keep one original envelope for the reached CRT cleanup entry | The direct call at `0x001E4DC2` reaches an eight-byte entry already checked inside `0x001E4D58`. The opt-in emitter validates its single caller and stack shape, charges the extra emission to the existing budgets, and excludes it from indirect lookup. |

## Contracts that remain in force

- Each browser supplies compatible files locally; original files/assets, generated title code, raw analysis, RAM and original-containing save states are not published, uploaded, or shared.
- Browser-local end-user conversion remains a product requirement; the private native preparation workflow is research.
- Original behavior and presentation are the fidelity baseline. Prototype/component fixtures and synthetic profiles are not original-runtime or parity proof.
- Online results must be authoritative; accounts, browser connections, profiles, and seats are distinct, with four total seats. The execution/authority/content contract remains unresolved.
- New project code has no selected license. Public visibility does not establish reuse or original-content rights.

See [the roadmap](../plans.md), [scope](compatibility.md), [content policy](content.md), and [project milestones](milestones.md). No runtime implementation or pending branch feature was accepted by the cleanup decision.

Record future consequential decisions with a unique ID, the concrete behavior/choice, evidence or user direction, and remaining limits. Superseded decisions belong in Git history rather than an active task log.
