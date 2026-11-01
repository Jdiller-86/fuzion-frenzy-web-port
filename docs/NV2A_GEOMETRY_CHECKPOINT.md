# Filled-triangle geometry component checkpoint

The product target remains the actual original game executing entirely in the browser with the full planned features. No original instructions have executed here and no xemu executable has linked. This checkpoint covers one post-vertex geometry component, not the complete NV2A renderer or original-game fidelity.

## Observations

| Evidence | Result and scope |
| --- | --- |
| Pinned xemu `dc10b4cea96e403d30c17ed53551fdaf023bc17f`, `hw/xbox/nv2a/pgraph/glsl/geom.c` and `common.c` | The filled-triangle stage forwards attributes, selects provoking colors, carries three raster-space positions and calculates depth slope. Geometry shaders and `precise`/`fma` appear in this path. |
| Isolated actual generator | Compiled the unchanged pure generation/varying-declaration functions with existing Wasm GLib. Only hardware-state readers/includes were omitted; fixture state supplies their values. Original and isolated source digests are in `.tools/emulation/geom-oracle-02/build.json`. The first native preparation attempt lacked GLib development headers and is retained separately; no packages were installed. |
| Actual generated native shaders | 24 variants cover smooth/flat colors, first/last provoking vertex, ordinary/perspective depth and three cyclic rotations. The verified local GL context captured 40 source-owned triangles per variant through transform feedback: 960 records, 146,880 scalar outputs. Inputs include seeded normal cases, degenerate triangles and zero-W cases. Raw shaders/data stay ignored. |
| Source-owned C candidate | `src/runtime/nv2a_triangle.c/.h` implements filled-triangle expansion with the 42-float post-vertex / 51-float raster-vertex layouts. It preserves clip position, point size, four colors, fog, four texture coordinates, the position triple and sanitized depth slope. Null pointers and invalid state/profile are rejected without output writes; callers must supply valid three-element arrays. It performs no vertex program execution, rasterization or guest execution. |
| Initial exact comparison | All 144,000 forwarded values matched. The initially fused depth path differed in 840 of 2,880 slope outputs, up to 11 ULP. This failed comparison is preserved; no tolerance was added to make it pass. |
| Arithmetic diagnosis | Direct cancellation-sensitive shader probes show the local llvmpipe geometry compiler evaluating these `fma` calls as separate multiply/add operations. A separate-operation model matches all 960 reference records; a fused model explains the initial differences. This is observed host behavior, not an original Xbox measurement. |
| Explicit host arithmetic profiles | The caller must choose fused or separate arithmetic. The separate profile matches the recorded shader reference exactly: zero mismatches across all 146,880 outputs. Independently, the fused Wasm profile matches a native-libm model for all 2,880 slope comparisons. Analytic native and Wasm tests cover both profiles, including an independently captured cancellation case. |
| Real browser GPU handoff | The Wasm output passes through a WebGL2 vertex stage with 15 attributes and transform feedback. All 960 cases / 146,880 GPU-readback values match the native shader reference exactly under the measured separate profile. Two runs passed with no current-build warnings/errors. A separate source-owned visible triangle returns pixel `[71,73,111,255]`; this drawing uses a diagnostic fragment shader and is not an original image comparison. |

The host arithmetic distinction is material: the [desktop GLSL specification](https://registry.khronos.org/OpenGL/specs/gl/GLSLangSpec.4.60.pdf) distinguishes explicit `fma` from separate operations under `precise` but permits implementation-dependent precision. [GLSL ES 3.00](https://registry.khronos.org/OpenGL/specs/es/3.0/GLSL_ES_Specification_3.00.pdf) does not expose the same `fma` facility and allows other precision differences. The candidate performs the geometry arithmetic in Wasm with explicit operation ordering and keeps the host choice separate from guest state. A product calibration/baseline policy has not been selected.

## Retained evidence and checks

- Generator: `.tools/emulation/geom-oracle-02/`; its 24 generated geometry shaders and four varying headers come from the pinned source. No original game files were inputs.
- Native reference: `.tools/emulation/geom-reference-01/reference.json`, SHA-256 `474497d8fb629580352390ec7705b4ed0ea47aa635b8064e9ad2f393d7b530e5`; driver/version and per-shader digests are included.
- Failed initial comparison: `research/verification/triangle-prepass-reference-20261031-01.json`; initial module/source snapshots remain under `.tools/emulation/triangle-prepass-01/`.
- Diagnosis: `.tools/emulation/geom-numerics-01.json` and `geom-fma-01/result.json`.
- Native/Wasm analytic builds: `.tools/emulation/triangle-prepass-02/result.json` and `check.log`; both compile with warnings treated as errors, fast math disabled and `-ffp-contract=off`.
- Exact separate-profile comparison: `research/verification/triangle-prepass-reference-20261031-02.json` (SHA-256 `e775d0ebe13b4d9049b7317d4fa4ef14e2b0ff3642b340d59486886d6f0f085a`); `tools/compare-nv2a-geometry.mjs` checks the pin/schema, uses private compiled-tool inputs, and exclusively creates an ignored research report.
- Independent fused-profile check: `.tools/emulation/fused-profile-01.json`; 960 records / 2,880 slopes, zero mismatches against the native-libm model.
- Browser build/source identities: `.tools/emulation/sdl-browser-geometry-01/build.json`. The fixture is `tools/fixtures/nv2a_geometry_check.html`; it embeds only synthetic reference data and uses the existing restricted four-file loopback server.
- Browser screenshot and DOM result: `research/verification/nv2a-triangle-browser-20261101-01.jpg` (SHA-256 `e5a5058aa22e09bc068e7b5f2e48b7783fdc35f0980a0be4131f7a25a1e02b61`) and matching `.json`, kept local/ignored. Full-page capture was unavailable; the viewport capture and DOM result succeeded. The temporary browser tab and the verified fixture server were closed after validation.
- CI adds a Linux native compile/run of `tests/nv2a_triangle_test.c`. The Windows/Linux source and delivery checks remain separate from the private shader/Wasm/browser comparisons. CI does not execute the original game.
- Local publication checks passed for 104 source files, together with comparator syntax and whitespace checks. The pinned xemu checkout has no tracked changes. The supplied `default.xbe` still hashes to `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`; it was not executed or modified.

With the pinned local generator/reference and compiled module already prepared, a new report can be reproduced with:

```powershell
node tools/compare-nv2a-geometry.mjs .tools/emulation/triangle-prepass-02/triangle.cjs .tools/emulation/geom-reference-01/reference.json research/verification/new-triangle-report.json separate
```

The output must be unused. `fused` intentionally differs from this particular software-shader reference; it is tested separately against native fused arithmetic. Do not overwrite or silently relabel either reference. The compiler/toolchain and generated fixtures are not installed by `npm ci`.

## Inference, unknowns and next task

The evidence supports a CPU prepass followed by WebGL2 as a candidate replacement for this filled-triangle geometry stage. It does not select the final graphics architecture or establish original-hardware fidelity. The source-owned C component is not integrated into xemu yet.

Unknown/unimplemented: other primitives and polygon modes, primitive assembly, vertex-program/fixed-function translation, original fragment/texture/depth/blend behavior, shared-context/thread integration, complete GL host adaptation, performance, release-browser coverage and original-title execution. The synthetic forwarded positions are post-vertex inputs, not a substitute for that missing vertex path. An Xbox test/capture would still be needed to establish original behavior; no game/firmware download or native-game streaming is authorized.

Next concrete task: extend the same actual-generator/reference comparison to line and quad geometry output and their assembly rules, preserving the triangle regression. Keep arithmetic choice explicit. Do not integrate a partial geometry path as if it covered the renderer or advertise the synthetic drawing as Fuzion Frenzy. Boot inputs remain unresolved; the full browser/online/social objective stays active and incomplete.
