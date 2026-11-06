> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# The AV mode table accepts a known domain; a zero result matches no row

November 4, 2026. Read-only analysis of the retained executable and the retained `xboxrecomp-source` replacement clarifies the consumer side of the ordinal-2 AV query. The caller's table accepts only NTSC-M, NTSC-J and PAL-I standards with SCART, HDTV, VGA or wildcard packs; it has **no standard-zero row**, so a zero result cannot match. This captures the consumer domain, not the original kernel's option-6 value. Cxbx maps numeric 6 to `AV_QUERY_AV_CAPABILITIES` and handles it; the xboxrecomp replacement maps it to `AV_OPTION_QUERY_AVPACK`. Ordinal 2 remains unsupported and no service model, default or frontier admission is added. **There is no controllable original screen.**

## Method

- Retained executable `research/reference/default.xbe`, SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`.
- The reached caller `0x1FD42F` scans `0xBF` entries of `0xC` bytes from VA `0x202C38` (`0xBF * 0xC = 0x8F4`, matching the caller's bound).
- XBE section 1 maps VA `0x202C38` to file offset `2046200`; the 2,292-byte table fits inside the section (ends at `2048492`, section ends at `2048988`).
- Each entry's first DWORD carries the video standard in bits 8-15 and the pack in bits 0-7; the caller masks `0xFF00` for the standard and `0xFF` for the pack. Only aggregate counts and addresses are recorded here; no table rows are copied.

## Observed domain

| standard | pack 0 (wildcard) | pack 3 | pack 4 | pack 5 |
| --- | --- | --- | --- | --- |
| `0x01` NTSC-M | 12 | 12 | 21 | 4 |
| `0x02` NTSC-J | 12 | 12 | 19 | 4 |
| `0x03` PAL-I | 30 | 30 | 30 | 4 |

The last entry is the `0xFF`/`0xFF` sentinel. There is **no row with standard `0x00`**.

## Consequences

- A zero result matches no row in this game's table. It is not a compatible default. Cxbx's option 6 is a capabilities query, not a no-op; its `AvQueryAvCapabilities()` result is derived from the SMC video mode and nonvolatile settings.
- The `xboxrecomp-source` value `0x00400104` (standard `0x01`, pack `0x04`) matches 33 candidate rows: 21 exact pack-4 rows plus 12 wildcard pack-0 rows. Matching the table does not make it original-console behavior.
- The table scan uses only the standard and pack fields. A later consumer `0x1FD4A8` also masks the refresh bits `0xC00000`, so the full result word has more than one consumer field.
- The domain narrows the accepted values but does not select the player's actual console mode.

## Provenance

The retained `xboxrecomp-source` is a kernel **replacement** for recompiled games, not an original-kernel implementation. Its `src/kernel/kernel_hal.c:1463-1510` comment says the option-6 word is chosen so D3D mode tables match (citing a Half-Life 2 `0x00480104` row and a North-American HDTV/NTSC-M/60 Hz replacement default). Cxbx at `585c49a50af1255ab155099e06f24505f9c5a800` defines `AV_QUERY_AV_CAPABILITIES` as `6` and returns `AvQueryAvCapabilities()`; it does not define `AV_OPTION_QUERY_AVPACK` and does not fall through to a no-op. Neither reference establishes the original console's behavior.

## Limits

- No original-kernel observation, encoder capability, timing, display mode or playable screen is established.
- The table is original content; only aggregate metadata and addresses are recorded. The source-owned runtime is unchanged.
- The service remains unsupported; no default, fixed word, EAX/NTSTATUS success or binding is introduced.

## Next task

An original-hardware capture remains optional fidelity evidence, not a prerequisite for the component or for keeping ordinal 2 unsupported. Do not add another capture schema or run another replacement-source survey. If a capture becomes available, its measured word may be compared with this domain; do not re-enable the withdrawn model or adopt a synthetic replacement value. Keep ordinal 2 unsupported and `0x1F79A0` unadmitted.
