# A bounded browser-local read-only disc session is working

November 4, 2026. The browser can now open one locally selected bare XISO in a dedicated worker, identify the supported executable by its explicit size/digest, and return requested byte ranges. The session never uploads, serves or converts content, and it is not connected to the guest runtime. The Sol architect reviewed the next-step options this cycle at `2026-11-04T06:16:22.4039478-05:00` and selected this bounded disc-session prerequisite. **There is no controllable original screen.**

## Delivered boundary

- `src/runtime/disc-session-core.mjs` owns the bounded session: it opens a local `Blob`, uses the existing `openXiso` reader, requires an explicit `default.xbe` size/SHA-256, and serves byte ranges only.
- `src/runtime/disc-session-worker.mjs` is the browser worker bridge. The selected `File`/`Blob` is cloned into the worker; the page receives only requested ranges.
- `src/runtime/disc-session.mjs` is the main-thread handle. It allows one outstanding request, ignores late results after clear, times out bounded requests and terminates the worker on clear.
- Reads are limited to **65,536 bytes per response** and one outstanding read per session. The adapter does not call `arrayBuffer()` on the whole image; `openXiso` reads the header/directory spans, identity reads the executable span, and `read()` slices only the requested range.
- A bounded root **inventory** returns only names, sizes and directory flags, with a default 1..1024 entry limit; it never returns file offsets or content. It shares the one-request-at-a-time rule.
- Fail-closed cases: non-Blob/empty image, malformed XDVDFS, missing/wrong `default.xbe` identity, unsafe path, directory target, negative/zero/oversized range, range beyond the file, truncated read, busy session, oversized inventory and cleared session.
- Clear terminates the worker and invalidates the session; a new `DiscSession.open` creates an independent worker/session.

## Browser fixture

`proof/disc-session.html` + `proof/disc-session.mjs` generate a small source-owned XDVDFS image at runtime with a synthetic `default.xbe` and `data.bin`. The fixture-specific identity is computed from the generated executable; production build identification is unchanged. The development server allowlist includes only the fixture and source modules.

Observed in the in-app browser at `http://127.0.0.1:4173/disc-session`:

```json
{
  "ok": true,
  "inventory": ["default.xbe", "data.bin"],
  "data": [32, 48],
  "executable": [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16],
  "oversized": true,
  "unsafe": true,
  "busy": true,
  "cleared": true,
  "reopened": [16, 32, 48, 64]
}
```

No console errors were reported. The network log contained only the fixture page and source modules (`disc-session.mjs`, `disc-session-core.mjs`, `disc-session-worker.mjs`, `xdvdfs.mjs`, `identity.mjs`); the image was a local generated `File`, so there was no content-bearing request. The worker module appears twice because the fixture opens a second independent session after clear.

## Tests and limits

- `tests/disc-session.test.mjs` covers open/read, root inventory (names/sizes only), inventory entry limit, identity mismatch, malformed image, unsafe/missing path, invalid/oversized ranges, one outstanding read, clear-during-read, independent reopen, client busy/clear/late-result isolation and worker errors.
- Runtime suite 151/151, evidence 4/4, native 18/18, title-seed 25/25, `npm run check` 346 source files and whitespace checks pass.
- This is not a guest disc service, not a full importer/converter, and not proof of conversion completeness, title fidelity or playability. It does not read the whole image or upload it. The inventory is root-only and contains no content.
- Ordinal 2 remains unsupported, `0x1F79A0` remains unadmitted, and no CPU/stack, geometry or frontier work is involved.

## Next task

Connect the session only when a reached original disc/file path or an explicit product decision requires it. Keep the selected image local, retain the explicit identity and 64 KiB/one-outstanding limits, and do not weaken production build identification for fixtures. Continue other unowned runtime prerequisites; do not treat this adapter as original execution.
