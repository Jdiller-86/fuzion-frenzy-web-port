# PFIFO boundary design note: prototype semantics and the minimal responder

November 5, 2026. Design-only result, as directed: validate the default-off pusher prototype against the accepted PFIFO component semantics and record what the experiment touches and what a minimal responder would require. **No runtime hook, guest connection, admission, export, seed or translation change was made this cycle.** The architect's subagent region authorizations remain provisional and this note halts for design review.

## 1. Validation against the accepted components

| Aspect | `pfifo_pusher.c` | Accepted component semantics | Result |
| --- | --- | --- | --- |
| Method header | `(w & 0xE0030003) == 0` increasing, `== 0x40000000` non-increasing | `pfifo-decode.mjs` uses the same masks | match |
| Count / method / subchannel | `count = (w >> 18) & 0x7FF`; no method or subchannel dispatch | decoder extracts the same count and fields | match (dispatch intentionally absent) |
| Jump | `(w & 3) == 1`, target `w & 0xFFFFFFFC` | decoder uses the same test and mask | match |
| Ring jump | `(w & 0xE0000003) == 0x20000000`, target `w & 0x1FFFFFFC` | decoder uses the same variant and mask | match |
| Call / return | `(w & 3) == 2` and `(w & 0xFFFF0003) == 0x00020000` halt | decoder reports call/return and never follows them | match (refusal is the model) |
| Unknown word | refuse | decoder counts it as unknown | match |
| Ring bounds | `base < end`, both 4-aligned, span ≤ 64 MiB | `pfifo_channel.c` and `pfifo-ring.mjs` use the same bounds | match |
| Ring pointers | GET and PUT must both be inside `[base, end]` | channel `set_get` accepts any 4-aligned value ≤ 64 MiB (a software GET of 0 is accepted as uninitialized) | **stricter by design**; GET outside is a refusal, not an advance |
| Packet protection | packet must stay within `end` and must not cross PUT | reference protects on `GET >= dma_len` in its offset space; this model uses addresses | **mapping differs, intent matches** |
| Span source | every 4 KiB page of the packet must be CPU-written | `guest_write_map` is page-granular and diagnostic; no accepted component gates on it | **stricter by design** |
| LOW_MARK ordering | clear before dispatch, set after; two `consumer_update`s | `pfifo_status.c` documents exactly this; the reset leaves LOW_MARK set and makes no update | match |
| DMA classification | USER `0xFD800000` + `0x40/0x44/0x48`, channel 0, 64 KiB | `pfifo_dma.c` and its tests classify exactly these | match |
| Budgets | per-step `count + 1 ≤ 4096`; harness 64 GET steps; 500,000 VM accesses | component tests bound each structure; the run budgets are prototype additions | consistent |

Component evidence: `npm run test:native` **36/36** (optimized and ASan/UBSan), and the PFIFO JS component tests **19/19** (`tests/pfifo-decode.test.mjs`, `pfifo-ring.test.mjs`, `pfifo-scan.test.mjs`).

## 2. Exact MMIO register sequence the experiment touches

The synthetic profile supplies a `0xFD000000 + 0x01000000` window with these PFIFO-relevant values:

| Register | Address | Role in the experiment |
| --- | --- | --- |
| USER DMA_PUT | `0xFD800040` (aliased to CACHE1 `0xFD003240`) | title writes the submitted push pointer; the alias mirrors it |
| USER DMA_GET | `0xFD800044` (aliased to `0xFD003244`) | title reads the consumer pointer; the GET site is here |
| USER DMA_REF | `0xFD800048` (aliased to `0xFD003248`) | reference write, preserved verbatim |
| CACHE1_STATUS | `0xFD003214` | LOW_MARK `0x10`; reset sets it, a dispatch clears then sets it |
| RUNOUT_STATUS | `0xFD002400` | LOW_MARK `0x10`; the reference read always reports it |
| CACHE1_DMA_PUSH | `0xFD003220` | STATE `0x10`, ACCESS `0x01`, SUSPENDED `0x1000`; never produced by the model |
| PFB WBC | `0xFD100410` | self-clearing `0x10000`; untouched by the consumer model |

The profile zeroes `0xFD800000+0x1000`, `0xFD003200+0x100`, `0xFD100200+0x1000`, `0xFD400000+0x1000` and `0xFD002080+4`, lists the remaining registers explicitly, and sets the ring from `pfifo` (`base 0x03B7D000`, `end 0x03BFD000`, `put 0x03B7D000`). The reset adapter writes LOW_MARK into `0xFD003214` and `0xFD002400` once. The experiment then differs from the control by exactly one declared input, `pfifoConsumer`.

### GET-site order in `ff_probe_guest_memory`

1. PCRTC state (not PFIFO) is offered first.
2. Device writes are committed; contained registers are served from storage.
3. For a non-contained address, the profile empty-ring path may serve GET.
4. The gated consumer runs **one** pusher step (up to 64 per run) and may serve the advanced GET.
5. If GET still differs from PUT, the harness stops with **status 13** at `0xFD800044`.

## 3. What the gated run observes at the GET site

- **Control** (consumer absent), module 106 and 107: one stop, **status 13 at `0xFD800044`**, **339 routines / 405 kernel operations**, `pushbuffer consumption required`, no consumer state. The site is reached exactly as before the prototype.
- **Consumer enabled**, module 106: no report and no termination (300 s, earlier 900 s); the 64-step GET budget did not fire, so the wait was passed.
- **Consumer enabled**, module 107 (500,000 VM-access budget): the worker writes `{"kind":"error","message":"RuntimeError: unreachable"}`. That is a Wasm trap from a `stop()`, but it is surfaced as a generic error, so the stop record and `pfifoConsumerState` are not classified.
- **Module 108** scopes the VM budget to guarded accesses with a real call site (probe reads pass site 0). The control still reproduces **339/405 at `0xFD800044`**, but the consumer run still writes the same `RuntimeError: unreachable` at about 180 s. The error is therefore **not** a post-run probe read consuming the budget; the worker's trap classification remains the open gap.
- Consequence: the GET site is reached and the pusher runs, but the post-consumption sequence is **not yet observable as a classified stop**. The model cannot claim an advance, a refusal class, or a next stop from these runs.

### Observed control sequence (retained device log, module 107)

The control report's device log contains **93** PFIFO accesses. In order:

1. **Channel setup** — writes to CACHE1 `0xFD003210`, `0xFD003270`, `0xFD003240`, `0xFD003244`, `0xFD003258`, `0xFD003228`, `0xFD002410`, `0xFD002420`; then `0xFD003224 = 0x000F0078`, `0xFD003200/250/220 = 0`; a read-back pass over `0xFD003200..0xFD00326C`; then `0xFD003204 = 0x100`, `0xFD00322C = 0x11C`, `0xFD003224 = 0x00086078`, `0xFD003220 = 1`, `0xFD003250 = 1`, `0xFD003200 = 1`.
2. **PFB flush** — `0xFD100410` read `0`, write `0x00010000`, read `0`.
3. **Empty-ring GET** — USER PUT `0xFD800040 = 0x03B7D000`, then GET read `0xFD800044 = 0x03B7D000` at site `0x1FBEAE`, served by the profile empty-ring path.
4. **Status wait** — CACHE1_STATUS `0xFD003214 = 0x10`, RUNOUT `0xFD002400 = 0x10`, DMA_PUSH `0xFD003220 = 1`; DMA_PUSH then toggles `1 → 0 → 1` twice.
5. **Submissions** — USER PUT `0xFD800040 = 0x03B7D7B4`, PFB flush, then USER PUT `= 0x03B7D7CC`.
6. **Stop** — the next GET read is the status-13 stop at `0xFD800044`; it is not logged because the harness stops before recording the access.

Interpretation: GET is served as the ring base only while PUT equals the base. Once the title submits at `0x03B7D7B4` / `0x03B7D7CC`, the empty-ring condition no longer holds, so the next GET read observes `0x03B7D000 != PUT` and stops. At the consumer's GET site the inputs are therefore: base `0x03B7D000`, GET `0x03B7D000`, PUT `0x03B7D7CC`, and a span of just under 2 KiB to check for CPU-written pages.

## 4. What a minimal gated PFIFO responder would require

1. **Preconditions** (not checked by the prototype today): PUSH0.ACCESS, DMA_PUSH.ACCESS set, STATUS not suspended, channel MODE = DMA. Without them a responder must refuse.
2. **Fetch/decode**: one method packet, jump or ring jump per step; call/return and unknown words halt. No method execution, no PGRAPH writes.
3. **Protection**: GET inside the ring, packet within `end` and not crossing PUT, span CPU-written.
4. **Status ordering**: clear LOW_MARK before dispatch, set after; RUNOUT stays LOW_MARK; DMA_PUSH state never produced.
5. **Budgets and accounting**: per-step word budget, run step budget, VM-access budget; report classification plus words/updates for every run.
6. **Refusal-first**: any unmodeled condition keeps status 13 with a classification; never initialize GET, never synthesize kernel channel setup, never widen the model to pass a refusal.
7. **Report plumbing**: classify an expected stop trap in the worker (the current gap) so a budget or refusal stop is a record, not a generic error.
8. **Evidence gate**: the pinned reference lines, the retained channel-programming values, and no claim beyond the synthetic harness.

## 5. Halt

This is a design result only. No hook was added, no guest connection exists, and no admission/export/seed/translation change was made. The consumer stays default-off. The next step is a fresh architect decision: either authorise the responder preconditions plus the worker trap classification as one bounded set, or hold the PFIFO boundary.

Main `4b94a64` is still not an ancestor of this branch and this session denies `git merge`; the new integration head `460549e` is published on the architect branch but not integrated here.
