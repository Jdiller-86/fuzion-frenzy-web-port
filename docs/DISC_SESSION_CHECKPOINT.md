> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# A bounded browser-local read-only disc session is working

November 4, 2026. The browser can now open one locally selected bare XISO in a dedicated worker, identify the supported executable by its explicit size/digest, and return requested byte ranges. The session never uploads, serves or converts content, and it is not connected to the guest runtime. The Sol architect reviewed the next-step options this cycle at `2026-11-04T06:16:22.4039478-05:00` and selected this bounded disc-session prerequisite. **There is no controllable original screen.**

## Delivered boundary

- `src/runtime/disc-session-core.mjs` owns the bounded session: it opens a local `Blob`, uses the existing `openXiso` reader, requires an explicit `default.xbe` size/SHA-256, and serves byte ranges only.
- `src/runtime/disc-session-worker.mjs` is the browser worker bridge. The selected `File`/`Blob` is cloned into the worker; the page receives only requested ranges.
- `src/runtime/disc-session.mjs` is the main-thread handle. It allows one outstanding request, ignores late results after clear, times out bounded requests and terminates the worker on clear.
- Reads are limited to **65,536 bytes per response** and one outstanding read per session. The adapter does not call `arrayBuffer()` on the whole image; `openXiso` reads the header/directory spans, identity reads the executable span, and `read()` slices only the requested range.
- A bounded **directory inventory** accepts the root or one explicit directory path and returns only names, sizes and directory flags, with a default 256 entries and a configurable 1..1024 limit; it never returns file offsets or content. It shares the one-request-at-a-time rule.
- Fail-closed cases: non-Blob/empty image, malformed XDVDFS, missing/wrong `default.xbe` identity, unsafe path, directory target, negative/zero/oversized range, range beyond the file, truncated read, busy session, oversized inventory and cleared session.
- Clear terminates the worker and invalidates the session; a new `DiscSession.open` creates an independent worker/session.

## Browser fixture

`proof/disc-session.html` + `proof/disc-session.mjs` generate a small source-owned XDVDFS image at runtime with a synthetic `default.xbe` and `data.bin`. The fixture-specific identity is computed from the generated executable; production build identification is unchanged. The development server allowlist includes only the fixture and source modules.

Observed in the in-app browser at `http://127.0.0.1:4173/disc-session`:

```json
{
  "ok": true,
  "inventory": ["default.xbe", "data.bin", "PACKAGES"],
  "packages": ["a.bin"],
  "notDirectory": true,
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

## Review correction

An independent GLM review at `2026-11-04T06:35:19.3594903-05:00` found that `DiscSessionCore.open()` had no busy guard and could overwrite a cleared session if `close()` ran during one of its awaits. It now sets a busy guard, re-checks `closed` after each await and no longer resets `closed` on success. Tests now cover clear-during-open, concurrent open, clear while a read is pending, request timeout, `worker.onerror` and an unexpected response type.

## Production identity validation

The pinned `BUILD_REFERENCE` now carries the 2,834,432-byte XBE size. `tests/disc-session.test.mjs` conditionally builds a synthetic XISO around the retained `research/reference/default.xbe` when it is present, opens the session with the pinned size/SHA-256, checks the root inventory and reads the first 16 bytes. It skips cleanly when the local XBE is absent; no original content is committed. It passed in this worktree.

## Tests and limits

- `tests/disc-session.test.mjs` covers open/read, production-XBE identity when the local file is present, root and explicit subdirectory inventory (names/sizes only), inventory entry limit, non-directory/missing paths, identity mismatch, malformed image, unsafe/missing path, invalid/oversized ranges, one outstanding read, clear-during-read/open, concurrent open, independent reopen, client busy/clear/timeout/worker.onerror/protocol rejection and worker error replies.
- Runtime suite 157/157, evidence 4/4, native 18/18, title-seed 25/25, `npm run check` 346 source files and whitespace checks pass.
- This is not a guest disc service, not a full importer/converter, and not proof of conversion completeness, title fidelity or playability. It does not read the whole image or upload it. The inventory is shallow: the root or one explicit directory, with no recursive listing or content.
- `dataBytesRead` is informational; there is no cumulative session cap. A same-origin caller can retrieve an entire image in 64 KiB slices over time, though no egress path exists in these files.
- The session trusts the caller-supplied identity; any runtime wiring must supply the pinned production digest.
- The module-worker URL has been proven against the allowlisted research server, not a Vite production build. The worker's explicit `close` branch is currently unreachable from the shipped client, which terminates the worker instead.
- Ordinal 2 remains unsupported, `0x1F79A0` remains unadmitted, and no CPU/stack, geometry or frontier work is involved.

## Next task

Connect the session only when a reached original disc/file path or an explicit product decision requires it. Keep the selected image local, retain the explicit identity and 64 KiB/one-outstanding limits, and do not weaken production build identification for fixtures. Continue other unowned runtime prerequisites; do not treat this adapter as original execution.
