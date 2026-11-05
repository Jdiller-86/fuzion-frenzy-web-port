# Continue from the contiguous-memory request in renderer initialization

OpenCode model routing and source-only worktree instructions are in [the OpenCode workflow](OPENCODE_WORKFLOW.md); the [continuous development goal](OPENCODE_GOAL.md) defines the bounded advancement loop. This changes development coordination, not the runtime checkpoint below.

Read [the current renderer-entry checkpoint](RENDERER_ENTRY_CHECKPOINT.md). Static recompilation and the complete original-game/custom-feature goal remain active. There is no playable original screen.

Original code now programs a bounded synthetic device aperture and continues past it. Helper `0x001fe05a` reads `0xfd001804`, ORs 4, writes it back and writes zero to `0xfd600140`; the caller then writes `0xfd009140` -- a computed offset that a fixed per-address list could not cover. Both profiles record four events across three addresses (with unknown-before provenance for the first write), execute 234 routines / 396 kernel operations / 3,633 events, keep statistics at `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`, and stop at untranslated original routine `0x1fe085` (return `0x1fe5bd`).

Reuse `recomp-title-post-memory-06` and `recomp-post-memory-build-16`: 344 functions / 12,878 instructions, ABI 24, module `d757d534…` (424,106 bytes). Reports `post-memory-browser-20261102-15.json` / `-16.json`; audit `aperture-checkpoint-20261102-01.json`. The aperture admits [`0xFD000000`, `0xFD800000`) from explicit profile inputs, stores sparsely (1,024 addresses), logs 4,096 events, faults on unknown reads, and rejects every other address, width or direction without mutation. It is inert storage, not NV2A behaviour; D060 remains conditional on that.

**Next task:** admit and translate `0x1fe085` from a bounded read-only export, then observe the next dependency. No device behaviour, DMA, interrupt or rendering may be inferred.

Validation: new aperture native/sanitizer suite passes; `python tests/title_seed_test.py` 25; 40/40 runtime cases and `npm run check` pass. No public CI result is claimed. Full runtime/fidelity, renderer, disc/package reads, general filesystem and durable saves, graphics/audio/controllers, playability, end-user conversion and all planned features remain unfinished. Keep the full goal active.
Prior checkpoint handoff sections are archived at
docs/archive/DEVELOPMENT_HANDOFF_PRIOR_20261105.md.
