# Continue from the M39 wrapper post-call guard

Follow [status](status.md), the [scope ledger](m1-work-scope.md) and [fast M1 workflow](resource-reuse.md). M1 remains active and incomplete; original/generated content stays local.

## Current boundary

M39 runs the matching generated1E4F99 parent (35 instructions) once and1E2038 child (41 instructions) twice, on the reviewed argument5/low-IRQL/PTD-table-hit path. Child RETs are observed at D58→D5C with return sites1E4F9F/1E4FD4. Parent plain RET is D60→D64, EAX00330690/ECX0000000D; actual caller observations reach pre-POP D64 and post-cleanup D68, ECX5, negative EAX=ESI. Bounded helper CPU/PTD/TLS checks pass; only PTD offsets+8/+C change to DWORD13/5, TLS20 is unchanged. The public FP summary is unchanged, but the later full FP restoration guard was not reached. C EFLAGS preservation is justified only by dead arithmetic flags through the later ADD ESP,0x14, not general/native flags fidelity.

The outcome is wrapper post-call guard status9 at target03C10E58, ordinal1E01AD, ESP03C10E58, report RA0025C908, after the C body exit. This is not an unsupported kernel or accepted original function stop; adapter/outer returns remain unaccepted. Independent review `PASS_BOUNDED_HELPERS_WITH_ENCLOSING_GUARD_STOP` (no blocking findings) confirms exactly one first-guard failure: saved EBP at03C10E40 contains001E01A2 where the old predicate expects EBP03C10E58; all other first-guard register/stack predicates pass. Run02 readback leaves source/module unchanged. The later full FP guard was not reached.

Default38 is full-record exact after case/module normalization; controls/inert retain prior boundaries. Totals468 distinct /887 kernel /882 raw zero-valued records; prior3038 CPU rows and373 kernel rows remain exact, with25 CPU rows and two actual kernel records added. Module1,392,689B SHA-256 `7632fb8d92e93067ff753154f1e17d7e3773adee51be5ed4dde76b32a59d64c6`;79/88 objects reused,9 compiled. Charges510/23332/32/last31/29 maps/64 bindings; fixed128MiB, sole clock import,186 exports, no start. No new group/import/export. Observed browser aggregates are VM+231/PFIFO+141/clock+12, not per-access tracing.

Independent source review passes for the bounded source/build recipe and build review passes compile-only; the bounded actual-browser review passes. Separate erratum corrects historical rejected-attempt pins only. Forecast remains17/29/50, low confidence; no allowance or M1 gate closes.

## Next bounded task

**SCOPE-32:** correct only the bounded wrapper-owned stack-lifetime predicate using the original epilogue and actual 03C10E40..03C10E5F snapshot evidence. Preserve other register, FP, ABI and stack checks. Keep the M38/M39 reports exact and stop at any new unsupported behavior. Do not broaden stack lifetime, infer the outer return, or mark M1 accepted.
