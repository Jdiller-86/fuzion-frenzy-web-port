# Original section residency and image initialization

November 2, 2026. The selected route remains static recompilation to WebAssembly for the complete original game and planned custom features.

Original browser startup now demand-loads `$$XTIMAGE` from immutable local executable backing, writes its actual 10,240 bytes to `TitleImage.xbx`, unloads the section and closes every volume handle. The shared page remains resident for the neighboring preloaded section; unshared pages become inaccessible. The exact image payload matches the selected executable, and the earlier 52 metadata bytes remain intact. Both directory helpers and `0x00033c12` return successfully; `U:` retains its real directory reference after handle closure. A further 37-byte / 20-instruction original routine is translated and enters. Fifteen seeds now translate 51 functions / 3,655 instructions; 256 trace events observe 42 distinct original routines. 109 kernel operations complete, with one later ordinal-24 query still pending: index 10 (`XC_P_CONTROL_GAMES`), four-byte buffer, return `0x00030aa9`. The pending request receives no invented value or guest result. Both explicit fresh synthetic profiles pass 88 browser checks and 8,410 copy/ABI cases. A playable screen, full rendering/audio/controllers, disc reads, durable saves/FATX equivalence, browser-local end-user conversion, full original fidelity and every planned online/social/control/authority feature remain unaccepted.

The owned image manager admits the compatible immutable executable from a bounded 16 MiB source block, disjoint from guest RAM. Only sections marked preload initially receive bytes and resident references. A first demand load copies real raw bytes and zero-fills the section tail, updating section/head/tail references. Repeated loads preserve current bytes; last unload denies unshared pages while preserving shared neighbors and retained backing bytes. Reload uses the immutable source again. The guest header remains read-only to translated code; only validated kernel-owned reference fields change. Invalid metadata, overlapping source/guest storage, invalid descriptors and ownership failures do not partially modify guest state. This is the compatible bounded image service, not full physical Xbox memory, signature verification, concurrency or proven original failure ordering.

Actual browser calls 103–108 are load, image write, unload, close image, close directory and create the `U:` link. The load changes references 0→1 and shared head/tail counts to 2/1; unload restores 0 and 1/0. The shared DOLBY page remains read/execute; the two unshared image pages become inaccessible. All nine volume handles have closed; two retained directory objects back `T:` and `U:`. Both files charge one 16,384-byte synthetic allocation unit. RAM persistence, timestamps and logical allocation do not establish durable/original FATX saves.

The following 20-instruction routine now enters and queries ordinal 24 with `[10, 62983988, 62983992, 4, 0]`, return `0x00030aa9`. The existing implementation admits only explicitly supplied index-17 misc flags. This later query stops before a guest return. Its internal record is explicitly unsupported, not a successful zero-valued read; the earlier diagnostic attempt is preserved. The pinned [kernel declarations](https://github.com/sp00nznet/xboxrecomp/blob/f0f8a461496a4f23d9b0aeaa284b8d70ba7ab82b/src/kernel/kernel.h) identify index `0x0a` as `XC_P_CONTROL_GAMES`. No actual console setting/default was inferred.

The [nxdk section API](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h#L1519) and [Cxbx comparator](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlXe.cpp#L49) informed the independently authored service. They do not establish controlled original-console fidelity.

## Validation and retained evidence

Validation: owned image-section fixtures pass native optimized, address/undefined-behavior sanitizer and Wasm modes; they cover preload/demand bytes and zero-fill, repeated load, last unload/reload, shared-page protection, invalid descriptors/source metadata and failure atomicity. The actual new browser path passes both profiles; one prior ABI-12 boundary was checked, with empty warning/error logs and Clear verified. Current compilation identities match all 28 runtime sources/declarations and the six owned fixture inputs. Independent local comparison matches the actual file payload with executable section bytes. Thirty-three runtime/admission/delivery cases pass; CI/publication results are recorded after the source review. No full regression campaign was repeated for this service.

All artifacts are ignored under `research/verification/`. Original/generated code, selected bytes, payloads, raw exports, traces and screenshots remain local and are never HTTP-served, committed or uploaded. Fixed 128 MiB Wasm / 64 MiB guest memory, one clock import, 16 files / 64 KiB each, 1,024 trace / 128 call limits and all translation/metadata budgets remain. ABI 13 adds local staging/owned lifecycle exports, not a host filesystem or network provider.

| Artifact | SHA-256 |
| --- | --- |
| `recomp-title-post-image-01/generation.json` | `683de0d914bf67fa96d42c17d15c32f8d5e8eae12375753f43736f7ae5560c7f` |
| `recomp-post-image-build-02/build.json` | `2de981656bf2c179f2b4fdefe6656d6c03db32ca97f5b1b0b1971d876e8e1aff` |
| `recomp-post-image-build-02/startup.wasm` | `d62fa0fe5054f1e5aef663c5dc303b65291b93bde660c200cd4f3fc33973345c` |
| `image-sections-owned-01/checks.json` | `12f60144357546d70df90e55e60ac6761ca019d8888adc4dbffd154269582cc3` |
| `recomp-sections-next-01/seed.json` | `09a60517eeb3e319e740f1b4a39fb2c71d2c50a55d5aabf670839fe2e7a84907` |
| `post-image-browser-20261102-02.json` | `ad3413f98fadccfb5fc7e2977a7cd776ca55e7b653e9b9cbec20d10b221eb2a8` |
| `post-image-browser-20261102-03.json` | `129fa6a2f2906f84a8dc57cac2758d2e1232b30c9effe6ad1ad5bc81c2b7014b` |
| `post-image-browser-20261102-03-context.json` | `4ac28a975bd80313c718b61fb64d68cadfed261ffddd59fb711cbdc178ce563f` |
| `post-image-browser-20261102-03.jpg` | `4f3b96626c7e3ae8b8b308df18c686a9b14a9c1cf37497bd6757d858e45171aa` |
| `section-residency-evidence-20261102-01.json` | `b5114c0d11bc2f2f1bc95e83d5304f051a4def075b4aa1d8f4d4149dcc101c5d` |

Initial `sections-browser-20261102-02/04` pass the 87-check image-initializer boundary; `03` preserves one earlier ABI-12 regression. After the additional original routine is admitted, final `post-image-browser-20261102-02/03` pass 88 checks. The final screenshot is inspected, warning/error logs are empty and Clear empties every selection/output. Temporary tabs and verified source-only hosts were closed; all original files and prior evidence are preserved.

## Next task

Execution priority, user updated November 2: focus on new breakthrough developments and advancing actual original execution. Use targeted validation for each changed runtime contract; repeat broad legacy/audit campaigns only when a regression, related change or unresolved risk justifies them.

Next extend the explicit local console-settings profile and checked query service for the actually reached index 10. Preserve the Xbox five-argument ABI, configured DWORD/type/length and output/failure semantics; require an explicit supplied test value rather than inventing an original console default. Then continue original `0x00030a91` in the browser and follow its next genuine dependency. Reuse `recomp-title-post-image-01`, its fifteen retained seeds and the existing project. No further extraction, import or Ghidra export is needed for this reached service. Retain byte/decoder/layout/memory guards, fixed resources and the local-content boundary. The complete original-game/custom-feature goal stays active and unfinished.
