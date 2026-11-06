> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Read-only NV2A packet decoder; PFIFO boundary unchanged

November 3, 2026. Following architect guidance, the next non-geometry renderer-path prerequisite is a source-owned, read-only NV2A pushbuffer packet decoder. It reports command headers, method fields, control-flow records and arguments from an explicitly supplied word range. It never executes a method, follows a jump/call/return, mutates memory or advances `DMA_GET`. **The original startup still stops at the PFIFO DMA wait; there is no playable screen, renderer or device model.**

## Why this milestone

- The retained report `post-memory-browser-20261103-46.json` still stops at status 13 (`pushbuffer consumption required`) at `0xFD800044`.
- The 4 KB window before `PUT` (`0x03B7C000`..`0x03B7D000`) is entirely zero and the 20 KB claim is unwritten, so there is nothing for a consumer to consume.
- The called kernel ordinals include no GPU/display channel-setup service (`187` handle close, `277`/`294` critical enter/leave, `184` virtual allocation, `168` GPU claim, `181` memory statistics). The missing initial GET belongs to unobserved kernel/D3D channel setup.
- Decoding is the smallest useful next step on that path; channel-state mutation and consumption remain deferred until initialization is understood.

## Source-owned change

`src/runtime/pfifo-decode.mjs` exports `decodePushbuffer(view, start, end, { maxWords = 4096, maxPackets = 4096 })`. Encoding follows the pinned bridge survey (`nv2a_pb_scan.c`):

- `(w & 0xE0030003) == 0` increasing methods; `== 0x40000000` non-increasing.
- `(w & 3) == 1` jump; `(w & 0xE0000003) == 0x20000000` ring-jump variant; `(w & 3) == 2` call; `(w & 0xFFFF0003) == 0x00020000` return.
- Method fields: `count = (w >> 18) & 0x7FF`, `subchannel = (w >> 13) & 7`, `method = w & 0x1FFC`.
- Each method record reports its header, address, subchannel, first method, count, non-increasing flag and exactly `count` argument words. Control-flow records report their encoded target; they are never followed. Targets are not mapped through the guest physical/virtual window.
- Stops are explicit: `complete`, `word-budget`, `packet-budget` or `truncated-arguments`. Views, alignment, signed/negative ranges, range size, modeled memory and budgets are validated before any read. The decoder does not mutate the supplied words.

`tests/pfifo-decode.test.mjs` covers increasing and non-increasing methods, jump/ring-jump/call/return/unknown separation, truncated arguments, word and packet budgets, zero-count methods, empty ranges, input immutability and invalid views/ranges/budgets. It is wired into `npm run test:runtime`.

## Review

An independent correctness review enumerated the classification space and confirmed the masks, field extraction, argument consumption, budget termination and read-only behavior. It found one minor fail-closed gap: a negative aligned start could throw from the view instead of being rejected. The decoder now rejects negative ranges before reading and a test covers it. Control-flow target masks remain reported-as-encoded and unverified for consumer use.

## Validation

`tests/pfifo-decode.test.mjs` passes 9/9; the runtime suite passes 103/103; Python memory-access is 10/10 and title-seed 25/25; `npm run check` and whitespace checks pass. Module `recomp-post-memory-build-51` (526,955 bytes, SHA-256 `cd0bf176…`), ABI 24 and D060 are unchanged.

## Limits

The decoder establishes parsing behavior on synthetic fixtures only. It does not establish title execution, queue ownership, a valid initial GET/ring base, rendering, fidelity or playability. A zero window is not a submitted command stream.

## Next task

Before any consumer integration, perform a focused private evidence pass on channel initialization and the provenance of `DMA_GET`/ring initialization with the retained Ghidra project. Static findings alone cannot establish original runtime state. Otherwise continue another independent runtime prerequisite; do not synthesize queue initialization, consume packets or advance GET.
