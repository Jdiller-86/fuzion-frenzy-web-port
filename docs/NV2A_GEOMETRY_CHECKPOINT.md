# Filled-triangle geometry component checkpoint

The product target remains the actual original game executing entirely in the browser with the full planned features. No original instructions have executed here and no xemu executable has linked. This checkpoint covers one post-vertex geometry component, not the complete NV2A renderer or original-game fidelity.

Directive status, November 3 (integration head `a57b9780cb2e3e3214a376a58406338d2f2981c3`): fixture and topology expansion are paused pending an actionable retained reference and a completed whole-chain review. Drafts #2/#3 are retained as-is; draft #2 remains unapproved. PR #1 needs refreshed-head checks and review and has no merge authorization. The Sol architect review completed at 2026-11-03T19:47:36.9741198-05:00 produced this directive; it is an architect-level review, not a geometry whole-chain or retained-reference review, and older Sol-unavailability reports are superseded. The geometry and architect heads remain unmerged. Reported startup progress under synthetic inputs is not rendering, fidelity or playability, and no controllable original screen exists. No component is integration-approved.

Next-task update, November 1: the user confirmed no local boot firmware and asked about a native web port. The [native-web checkpoint](NATIVE_WEB_FEASIBILITY.md) now scopes an original initialization-function proof. A native line/quad reference has been captured; its browser prepass remains pending. Preserve this triangle component and both references for either runtime route.

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

Source-only checkpoint `402fa29` is published. [CI run 41273163025](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41273163025) completed successfully on Windows and Linux, including the new Linux native triangle compile/run. CI does not execute the original game or the private shader/browser comparison. This documentation follow-up records that result without changing the checked implementation.

With the pinned local generator/reference and compiled module already prepared, a new report can be reproduced with:

```powershell
node tools/compare-nv2a-geometry.mjs .tools/emulation/triangle-prepass-02/triangle.cjs .tools/emulation/geom-reference-01/reference.json research/verification/new-triangle-report.json separate
```

The output must be unused. `fused` intentionally differs from this particular software-shader reference; it is tested separately against native fused arithmetic. Do not overwrite or silently relabel either reference. The compiler/toolchain and generated fixtures are not installed by `npm ci`.

## Source-owned line component

`src/runtime/nv2a_line.c/.h` expands one two-vertex line using the pinned public xemu `pgraph/glsl/geom.c` `emit_line`/`emit_vertex` behavior: forwarded clip/point/fog/texcoords, flat or smooth provoking colors, the 90-degree rotated interpolation row `(-delta.y, delta.x) + v0.xy` with `v0.zw`, and zero depth slope for a standalone line. It reuses the 42-float post-vertex / 51-float raster-vertex layouts and performs no vertex program, rasterization or guest execution.

`ff_nv2a_expand_line_strip` and `ff_nv2a_expand_line_loop` assemble host vertex streams into the same two-vertex line body. A strip of `count` inputs emits `2 * (count - 1)` raster vertices for adjacent pairs; a loop additionally closes `input[count - 1]` to `input[0]` and emits `2 * count`. Both reject `count < 2`, a `capacity` smaller than the required raster-vertex count, invalid state, null pointers and 32-bit size overflow before writing. The generator uses `layout(lines)` for `PRIM_TYPE_LINES`, `PRIM_TYPE_LINE_STRIP` and `PRIM_TYPE_LINE_LOOP` with no strip/loop parity, so flat provoking is resolved within each assembled pair (first or second pair vertex) and smooth color follows each emitted input.

Analytic tests cover attribute forwarding, first/last provoking colors, smooth colors, the rotated helper row, negative deltas, strip and loop pair order including the closing wrap pair, per-pair provoking, capacity/count rejection and invalid-state rejection without output writes. Optimized and ASan/UBSan native runs pass; CI adds a Linux line compile/run. This is a candidate implementation, not the captured native line reference comparison or a browser prepass. Original-hardware arithmetic calibration and renderer integration remain unimplemented.

Review status (November 3, free-model-only cycle): the strip/loop assembly was checked against the pinned public generator source and analytic native tests without an independent whole-chain geometry review. Material correctness input for the primary lane: the selected pair order and per-pair provoking follow the generator's host-supplied `layout(lines)` contract, but the actual NV2A host decomposition of line strips and the loop closing segment has not been compared with a captured reference. Line-loop degeneracy rules (for example count 2 or repeated vertices) and clipping-induced segment splitting are likewise unproved here.

The captured native line/quad reference and browser comparison remain private and unavailable in this source-only worktree. Do not treat the analytic line tests as original-hardware or reference parity.

## Source-owned quad-fill component

`src/runtime/nv2a_quad.c/.h` expands one four-vertex quad into the pinned public xemu `geom.c` fill order `1,2,0` and `2,3,0` using `calc_quadz`/`calc_triz`/`emit_vertex`. It forwards clip/point/fog/texcoords, uses the quad's provoking color (input 3) or per-emitted-vertex smooth colors, copies the two triangle position row sets, and computes each triangle's depth slope with the explicit fused/separate arithmetic profile. The 42/51-float layouts are reused.

Analytic tests cover attribute forwarding, provoking/smooth colors, both row sets, tilted/planar/perspective depth and invalid-state rejection without output writes. Optimized and ASan/UBSan native runs pass; CI adds a Linux quad compile/run. This is a candidate implementation, not the captured native quad reference comparison or a browser prepass. Quad-strip stream assembly, original-hardware arithmetic calibration and renderer integration remain unimplemented; the captured reference is private and unavailable here.

## Source-owned quad-strip component

`ff_nv2a_expand_quad_strip` uses the public quad-strip fill body: odd adjacency primitive IDs emit nothing; even IDs emit `0,1,2` from `calc_triz(2,0,1)` and `2,1,3` from `calc_triz(2,1,3)`. Attribute forwarding, flat provoking color (input 3), smooth per-emitted-vertex colors, the two position row sets and per-triangle depth slope reuse the quad-fill path. Analytic tests cover even/odd parity, forwarded attributes, colors, row sets, depth slopes and invalid state. The captured quad-strip reference comparison and browser prepass remain private and unavailable; further primitive assembly and renderer integration are unimplemented.

## Source-owned polygon line and point modes

`ff_nv2a_expand_triangle_lines`/`_points`, `ff_nv2a_expand_quad_lines`/`_points` and `ff_nv2a_expand_quad_strip_lines`/`_points` implement the pinned public `geom.c` `POLY_MODE_LINE`/`POLY_MODE_POINT` branches. Triangle LINE emits the three edges `0-1, 1-2, 2-0` in the selected rotation, all carrying the triangle's `calc_triz` slope; triangle POINT emits one point per rotated vertex with the position row repeated three times. Quad LINE emits four edges with the first triangle's slope on `0-1`/`1-2` and the second triangle's on `2-3`/`3-0`; quad POINT emits inputs 0–2 from the first triangle and repeats the second triangle's third row for input 3. Quad-strip LINE/POINT skip odd adjacency primitives and follow the shader's rotated `calc_quadz(2,0,1,3)` edges `0-1, 1-3, 3-2, 2-0` and row mapping `pz[1], pz[2], pz[0], pz2[2]`, with alternating triangle slopes. Flat color uses the primitive's provoking index (input 3 for quads/quad strips, the first/last triangle vertex for triangles); smooth color follows each emitted input. The 42/51-float layouts are reused.

Analytic tests cover emitted counts/order, clip/point/fog/texcoord forwarding, provoking and smooth colors, rotated helper rows, per-edge and per-point depth slopes, even/odd strip parity, and invalid-state rejection without output writes. Optimized and ASan/UBSan native runs pass. This remains a source-owned candidate: no captured-reference comparison, browser prepass or renderer integration was performed here.

`PRIM_TYPE_POINTS` has no geometry stage in the pinned generator. `PRIM_TYPE_POLYGON` fill and line are implemented in the next sections; its point mode is explicitly unsupported by the generator (an assertion). Triangle strip/fan rotation, provoking parity and whole-stream assembly are implemented below; line-strip/loop pair assembly is implemented in the line section above; quad-strip stream assembly and vertex-program execution remain unimplemented.

## Source-owned polygon primitive fill and line

`ff_nv2a_expand_polygon_fill` implements the pinned `PRIM_TYPE_POLYGON`/`POLY_MODE_FILL` branch: `calc_triz` over the rotated input, three emitted vertices, and the branch's fixed flat provoking vertex at the rotated third input (`first_vertex_is_provoking` does not apply). `ff_nv2a_expand_polygon_line` implements the `POLY_MODE_LINE` branch, where the generator's input is already a two-vertex line segment: it forwards the rotated helper row with a zero depth slope and fixes the flat provoking vertex to index 0. The generator itself notes that this line form has no triangle plane and can select the wrong flat vertex; the candidate preserves that stated limitation instead of inventing a plane or provoking rule. `PRIM_TYPE_POLYGON`/`POLY_MODE_POINT` is unsupported by the generator.

Analytic tests cover rotation, the fixed provoking and smooth colors, forwarded attributes, position rows, planar and perspective depth slope, zero line slope, and invalid-state rejection without output writes. Optimized and ASan/UBSan native runs pass. This remains a source-owned candidate: no captured-reference comparison, browser prepass or renderer integration was performed here.

## Source-owned triangle strip and fan parity

`ff_nv2a_expand_triangle_strip` models one host-assembled strip triangle. Even primitive IDs select the `rotation` order and odd IDs `rotation_odd`, matching the generator's `tri_rot0`/`tri_rot1` selection; the flat provoking vertex is the rotated first vertex when `first_vertex_is_provoking` is set and otherwise the rotated `2 - (primitive_id & 1)`, matching the strip branch. `ff_nv2a_expand_triangle_fan` uses one fan rotation for every triangle and selects the rotated first vertex (first provoking) or rotated second vertex as the flat provoking vertex. Both reuse the triangle fill emission for attributes, position rows and depth slope. `ff_nv2a_expand_triangle_strip_lines`/`_points` and `ff_nv2a_expand_triangle_fan_lines`/`_points` apply the same rotation and provoking selection to the generator's triangle-strip/fan `POLY_MODE_LINE`/`POLY_MODE_POINT` bodies: three shared-slope edges or three repeated-row points per host-assembled triangle.

`ff_nv2a_assemble_triangle_strip` and `ff_nv2a_assemble_triangle_fan` expand whole post-vertex streams. A strip of `count` inputs forms triangles `input[t], input[t + 1], input[t + 2]` with primitive ID `t`; a fan forms `input[0], input[t + 1], input[t + 2]` with the single fan rotation. Both accept `FF_NV2A_POLYGON_FILL`/`_LINE`/`_POINT` and emit three, six or three raster vertices per triangle through the per-triangle functions, preserving their parity, provoking, row and slope behavior. They reject `count < 3`, unknown modes, an undersized capacity, invalid state/profile, null pointers and 32-bit size overflow before writing. Strip parity assumes the stream starts at primitive 0 of the draw; the fan ignores the strip-only `rotation_odd` field.

The stream assemblers now form strip/fan triangles; the caller still supplies the measured winding orders. Line-strip and line-loop assembly is handled by `ff_nv2a_expand_line_strip`/`ff_nv2a_expand_line_loop` in the line section, reusing the same two-vertex line body per adjacent (and closing) pair; the generator treats all three line primitive types with the same two-vertex line shader and has no strip/loop parity. Analytic tests cover even/odd rotation and provoking selection, first-vertex selection, smooth colors, position rows, depth slope, stream triangle windows and per-mode output counts, and invalid-state/capacity rejection without output writes across fill, line and point modes. Optimized and ASan/UBSan native runs pass; no captured-reference comparison, browser prepass or renderer integration was performed here.

Review status (November 3, free-model-only cycle): the stream assembly was checked against the pinned public generator source and analytic native tests without an independent whole-chain geometry review, per the current runner directive. Remaining correctness inputs for primary-lane review: the caller must supply host-measured winding orders, the stream must begin at the draw's primitive 0 for strip parity, and strip/fan primitive-restart, degenerate-triangle and draw-origin behavior is not compared with a captured reference. No captured strip/fan reference or browser prepass exists here, and GLSL division rounding is assumed correctly rounded because the fused/separate profiles cover only the determinant terms.

## Retained-reference comparison contract

Directive update (integration head `d31e071a3202024574af997d14f8251b3537acec`): new topology expansion is on hold until this contract and a commit-level review are satisfied. This section records the contract and the current gate; it adds no geometry behavior.

### Admission of a retained reference

A reference is admissible only when all of the following are recorded with it:

- the pinned xemu commit (currently `dc10b4cea96e403d30c17ed53551fdaf023bc17f`), checked by the comparator against `tools/original-execution-toolchain.json`;
- the generator inputs and their digests (source shader and varying headers) and the isolated-generator build identity;
- the capture environment: GL implementation, driver and version, the transform-feedback procedure, and the exact arithmetic behavior the capture represents;
- the reference `kind`, input/output strides, a nonzero record count and the case/seed inventory, including degenerate and zero-`w` cases;
- a SHA-256 of the reference bytes, retained in every comparison report;
- no original game or franchise content.

References stay in ignored local storage (`.tools/emulation/**`, reports under `research/**`). They are never committed, uploaded, relabeled or overwritten. A new capture uses a new path.

### Record schema and current comparator scope

Each filled-triangle record carries 126 raw float32 input words, four state words and 153 expected output words. State words 0-2 must be 0 or 1 and word 3 must be 0-2. Output is compared as raw bits; depth slope is word 50 of each 51-word record. The reference file is capped at 16 MiB and 4096 records.

`tools/compare-nv2a-geometry.mjs` currently drives only the `_ff_nv2a_expand_triangle` export with that four-word state. Comparing any other primitive or mode requires an equivalent bounded export, state layout and reference schema before it can be called a reference comparison.

The reference-compatible state is exactly the four 32-bit words `smooth`, `first_provoking`, `perspective_depth` and `rotation`. `FFNv2aTriangleState` appends `rotation_odd` as a fifth word for strip/stream parity; the list fill/line/point paths do not read it, which keeps the current four-word allocation safe. Any adapter that drives strip, fan or stream exports must allocate the full struct or a five-word state and define how the fifth word is encoded.

### Comparison procedure and rules

Run:

```powershell
node tools/compare-nv2a-geometry.mjs <compiled.cjs> <reference.json> <new-research-report.json> <fused|separate>
```

The tool requires the compiled module and its `.wasm` inside `.tools/emulation`, requires a new JSON report inside ignored `research/` opened exclusively (`wx`), verifies the pinned commit, schema and limits before loading tool code, allocates the bounded buffers, calls the export with the explicit arithmetic profile, compares every word, and writes digests plus mismatch counts and a `status` of `exact-match` or `differences-found`. It exits nonzero on any mismatch.

- The arithmetic profile is part of the evidence. A fused result is checked against the native fused model, not against a separate-operation software-shader capture; the fused profile cannot excuse differences from a separate-profile reference.
- No tolerance is added to make a comparison pass. A mismatch is retained as a failed report.
- Analytic tests are not a reference comparison. A component without an admissible reference makes no fidelity claim.

### Whole-chain review scope

The chain awaiting review is: host primitive assembly (line strip/loop, triangle strip/fan streams) → post-vertex adapter → per-primitive geometry expansion (triangle, quad, line, polygon fill/line/point) → the 51-float raster vertices consumed by the next stage. A whole-chain review must cover:

- the 42/51-float member offsets, not only total sizes; these are now statically asserted in `src/runtime/nv2a_triangle.c`;
- the four-word reference state ABI and the five-word strip extension;
- caller-supplied winding orders, draw origin and arithmetic profile;
- assembly counts, capacity bounds and untouched-output rejection;
- the explicit absence of vertex-program execution, rasterization and guest execution.

Whole-chain review and retained-reference comparison are both outstanding. No component is integrated.

### Documented write bounds

Every public geometry entry point now has a source-owned write-bounds check in the CI-compiled native tests:

- triangle list fill/line/point, strip fill/line/point, fan fill/line/point, polygon fill/line and both stream assemblers in `tests/nv2a_triangle_test.c`;
- single line, line strip and line loop in `tests/nv2a_line_test.c`;
- quad fill, quad strip fill, quad line/point and quad-strip line/point in `tests/nv2a_quad_test.c`.

Each call receives a heap buffer with four extra raster-vertex slots poisoned to `0xa5`. The test asserts that the declared output count was actually written, that no spare slot changed, and that an over-large `capacity` does not increase the write count. Odd quad-strip primitives must leave every slot untouched while reporting `emitted == 0`. Heap allocation also lets ASan catch an overrun past the documented count. This pins the count contract only; it does not validate vertex values against a retained reference or establish rendering fidelity.

### Integration gate and commit-level review

A geometry component may enter the original-execution/renderer path only after:

1. an admissible retained reference exists for that primitive and mode;
2. the comparator reports `exact-match` for the component's supported surface under a stated profile, or every difference has a recorded explanation accepted by the primary lane;
3. a commit-level review records the changed paths, the primitive/mode covered, the reference identity and digest, the resulting report, the checks actually run, the untested surface, and the remaining correctness inputs (winding orders, draw origin, division rounding, host adaptation);
4. the primary lane confirms the component is not presented as original rendering or complete renderer coverage.

The free-model-only geometry directive forbids delegating to paid or other-model agents, so this lane cannot itself obtain the independent commit-level review. That review is a material blocker for the primary lane/architect; this lane must not claim it occurred. Self-review of staged source-only paths still applies but does not satisfy item 3.

### Current reference status

| Component | Retained reference | Comparison state |
| --- | --- | --- |
| Filled triangle (`ff_nv2a_expand_triangle`) | `.tools/emulation/geom-reference-01/reference.json`, SHA-256 `474497d8fb629580352390ec7705b4ed0ea47aa635b8064e9ad2f393d7b530e5` (960 records) | exact under the separate profile; fused checked separately against the native-libm model |
| Line and quad | captured locally, private and unavailable in this worktree | not compared here; browser prepass pending |
| Triangle strip/fan fill/line/point, polygon fill/line, quad line/point, quad-strip, stream assembly | none | analytic tests only |

A source-owned ABI guard in `tests/nv2a_triangle_test.c` allocates the four-word reference state on the heap and exercises the list fill/line/point paths under ASan, so a future read of the strip-only extension fails the boundary check rather than silently changing the comparator contract.

The status above is unchanged: no actionable reference was added or accessed since the pause began. The private triangle reference and the line/quad captures remain outside this source-only worktree.

### Pause status and resumption criteria

Per the current directive the lane is paused for:

- any new primitive, polygon mode, strip/loop/stream assembler or other topology expansion;
- new geometry fixtures, comparator exports or reference-capture work;
- any change that would make a component look integration-ready without the required review.

Retained and unchanged: drafts #2 and #3, the existing source-owned components and the write-bounds checks. Draft #2 remains unapproved; this lane has not re-evaluated either draft. PR #1 has not been checked, reviewed or merged by this lane. The newest source/test change is `818380b` (write-bounds checks); subsequent lane changes have been documentation only.

Provisional review status: a provisional hourly review (reported with Sol unavailable, remote-read verification only, nothing executed or merged) described geometry as documentation-only with the pause respected and assets/references preserved. That is not an approved design decision and does not change this lane's scope or resumption criteria; its primary-lane findings are outside this lane. Independently checked this cycle: the directive/PR head `a57b9780cb2e3e3214a376a58406338d2f2981c3` is not an ancestor of `main` (`ebd1ebf`), and this lane performed no PR check, review, merge or push.

Resumption requires all of:

1. an actionable retained reference admissible under the contract above and covering one existing component;
2. an exact-bit comparison result for that component under a stated arithmetic profile;
3. a recorded whole-chain review of the scope above, performed outside this lane under a permitted model or by the primary lane/architect, including the five-word strip adapter decision;
4. a primary-lane integration decision.

Until then the only in-scope changes are corrections to a demonstrated defect or to evidence that is already false. No new fixture or topology should be committed while paused.

## Inference, unknowns and next task

The evidence supports a CPU prepass followed by WebGL2 as a candidate replacement for this filled-triangle geometry stage. It does not select the final graphics architecture or establish original-hardware fidelity. The source-owned C component is not integrated into xemu yet.

Unknown/unimplemented: primitive assembly from vertex streams for quad strips (triangle strip/fan and line strip/loop assembly are now source-owned), vertex-program/fixed-function translation, original fragment/texture/depth/blend behavior, shared-context/thread integration, complete GL host adaptation, performance, release-browser coverage and original-title execution. The synthetic forwarded positions are post-vertex inputs, not a substitute for that missing vertex path. An Xbox test/capture would still be needed to establish original behavior; no game/firmware download or native-game streaming is authorized.

Next concrete task: paused by the current directive. No actionable retained reference or whole-chain review exists in this source-only worktree, and this lane may not request another model. The next actor is the primary lane/architect: supply an admissible reference and the five-word strip adapter decision, perform the whole-chain review, and decide integration. This lane should not add fixtures or topology while paused; if the directive changes, resume with a component the review names. Keep arithmetic choice explicit. Do not integrate a partial geometry path as if it covered the renderer or advertise the synthetic drawing as Fuzion Frenzy. Boot inputs remain unresolved; the full browser/online/social objective stays active and incomplete.
