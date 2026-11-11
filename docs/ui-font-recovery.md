# Native UI font recovery checkpoint

This is a bounded investigation record for font-named XBP resources and the retained native source. Resource names and byte hashes remain in the ignored private report; no resource bytes, font rendering, or generated game code are added to the repository.

## Verified package evidence

`tools/ui/font-recovery.mjs` scans the complete XBP census for case-insensitive `.ttf` suffixes. It checks the matching XBE identity, each package's full SHA-256 against the census, and every indexed resource range before recording a digest and metadata. The private report is `artifacts/ui-native/font-recovery.json`.

The corpus has 30 named rows across 15 packages: five rows in the already selected package and 25 additional metadata leads across 14 other packages. All 30 exact slices have a common 42-byte zero prefix. The 64-byte windows immediately after that prefix vary, and this investigation did not recover their schema. None contains a validated standalone SFNT/TTC font or a gzip/zlib-wrapped SFNT/TTC font. The source tree's existing XBP reader explicitly parses package tables and does not decode resource payloads. The subsequent native-handler comparison below identifies a matching texture-resource structure in these slices, but does not establish why the title gives those resources font suffixes or whether they encode glyphs.

The 25 additional rows come from unselected packages and have not been connected to executable name references. The scanner marks these as outside the selected-package source-reference index; that is a coverage limit, not a no-reference result.

## Static text/resource path evidence

The selected-package source-reference index ties the five `Generic.xbp` font names to exact strings in the XBE and push-address references in generated code. The references pass names to the shared resource loader at `0x0004BB30`. This is static evidence of resource requests, not evidence that the corresponding code ran or that a returned object is a font or is drawn as text.

Two bounded examples from the retained generated source:

- `x86_recomp.0.cpp`, analyst-labeled `sub_000133C0`: address `0x000133C8` pushes one TTF-suffixed resource-name address and calls `0x0004BB30` at `0x000133D1`; the returned value is stored at object offset `+0x5C`. The same function pushes a second TTF-suffixed resource name at `0x000133DD`, calls the loader at `0x000133E7`, and stores the result at `+0x60`.
- `x86_recomp.50.cpp`, analyst-labeled `sub_001C9120`: address `0x001C936E` pushes a TTF-suffixed resource-name address and calls `0x0004BB30` at `0x001C9380`. The captured sequence stores the returned value in a stack temporary. Its further use and mode lifecycle are not assigned here.

The other selected font names also have generated push references to `0x0004BB30`, retained in the private JSON report. Generated labels are analyst labels; repeated source-shard copies do not count as distinct runtime calls. The current source-reference scan does not recover a font renderer, glyph lookup, text ID mapping, or resource-object methods.

The retained read-only Ghidra analysis resolves one layer of the loader. `0x0004BB30` forwards the requested resource type to the type-indexed service at `0x0004B720`; type index 3 dispatches to `0x00042E60`. That factory obtains a resource view and passes its `+0x780` region to `0x00041890`. The latter reads descriptor fields at `+0x2C`, `+0x2E`, `+0x30`, `+0x36`, and `+0x37`, then registers texture surfaces through `XGSetTextureHeader` and `D3DResource_Register`. All 30 suffix-matched slices agree with the consumer's explicit `+0x2C == 5` branch; the recorded dimension and count fields are plausible texture-header values. In that branch, the consumer selects texture format `0x0B` and 8 bits per pixel. This is byte-to-handler evidence that these payloads contain the custom texture structure expected by this path. It does not show that the payload is a glyph atlas or explain its glyph metrics. The inspected calls in `sub_000133C0` store returned objects at `+0x5C` and `+0x60`; the bounded neighboring method group contains an indirect call through `+0x64`, but no direct read of those fields or recovered text draw. These are static decompilation findings, not observed runtime behavior.

## Renderer evidence and limits

The retained Windows renderer comments identify a 512×512 font-atlas menu case and calculate an inset UV rectangle from actual assembled quad vertices. The matching retained frame lists a stage-0 512×512 guest texture, but its draw rows do not include usable UV samples and its guest address is not joined to an XBP resource. This renderer-side clue does not recover the title's font format, glyph atlas, advances, bearings, baseline, line height, or draw function.

For development UI, the existing ImGui ProggyClean face remains available as a host-overlay fallback. It is unrelated to the original game and supplies no original glyph metrics. For an interim original-text fidelity fallback, retain original rendered atlas data and observed per-glyph advances, bearings, and baselines from a controlled native capture, joined to the actual draw UVs where possible. Do not infer those metrics from a font filename or substitute guessed system-font metrics. The currently retained capture does not yet supply the required UV/atlas join, so the original metrics remain unresolved.

## Next operation

Resolve the indirect object method reached through `+0x64` and trace whether it consumes the `+0x5C`/`+0x60` resources. Obtain a controlled native draw capture retaining the guest texture bytes and assembled vertex UVs for the 512×512 menu-texture case. Stop if the resource method, object layout, or draw inputs remain ambiguous; do not identify glyphs from screenshots alone.
