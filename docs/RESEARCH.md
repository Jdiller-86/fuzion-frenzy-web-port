# Research and evidence index

This index covers the retained public checkpoint records and package manifests. Use [current status](PROJECT_STATUS.md) for the latest observed stop, [runtime gates](RUNTIME_MILESTONES.md) for acceptance, and [analysis setup](ANALYSIS_SETUP.md) for private prerequisites. A record's title, “current” paragraph, test count, and next task belong to its own checkpoint.

## How to read the evidence

| Evidence | Establishes | Does not establish |
| --- | --- | --- |
| Static/export findings | What bounded source/instruction/layout analysis recorded | Observed original runtime behavior |
| Independently authored fixture | Behavior of a source-owned component in its tested cases | Complete title compatibility or hardware fidelity |
| Synthetic browser profile | Execution under the explicitly declared modeled inputs | Original-console defaults, complete graphics, or playability |
| Paired browser observation | The exact control/experiment behavior and limits it records | Broader behavior beyond that scenario |
| Package manifest/digest | Identity and admission metadata of the recorded package | Execution or fidelity by itself |
| G0/G1/G2 prototype record | Conversion or independently written prototype evidence | Acceptance of the corrected original-game product |

Generated original-title modules, original bytes/assets, pseudocode, RAM/save states, raw reports, and private analysis output are not distributed. Source-owned fixtures and curated findings permit public review without those files. Runtime acceptance retains two named browser reports plus an independent audit locally; do not infer an absent artifact is a pass.

## Key entry points

- [Current product acceptance ledger](ORIGINAL_EXECUTION_PROGRESS.md) and [route](ORIGINAL_EXECUTION_ROUTE.md).
- [Decision history](DECISIONS.md), including superseded diagnoses and route choices.
- [Delivery metrics](archive/PROGRESS_METRICS_PRIOR_20261106.md), separate from acceptance.
- [G0/G1 conversion proof](G0_G1_CHECKPOINT.md), [G2 prototype audit](G2_ACCEPTANCE.md), and [reference inventory](REFERENCE_INVENTORY.md).
- [Renderer-entry history](RENDERER_ENTRY_CHECKPOINT.md), [timer service evidence](TIMER_QUEUE_CHECKPOINT.md), and [geometry component evidence](NV2A_GEOMETRY_CHECKPOINT.md).
- [Historical snapshots](archive/README.md), including the full pre-rebuild status/handoff and plans.

## Retained checkpoint records

The groups below are navigation categories. Records removed from the latest tree are linked to their historical commit; their automation instructions do not apply. Other record filenames remain stable, with navigation links updated where needed.

<details>
<summary>Reference, conversion, and prototype (6 records)</summary>

- [G0 and G1 evidence checkpoint](G0_G1_CHECKPOINT.md)
- [G2 offline Twisted System: implementation and fidelity audit](G2_ACCEPTANCE.md)
- [Provided Folder Assessment](PROVIDED_FOLDER_ASSESSMENT.md)
- [Public reference observations](PUBLIC_REFERENCE_OBSERVATIONS.md)
- [G0 reference and content inventory](REFERENCE_INVENTORY.md)
- [Twisted System: G2 reference and frozen scenario set](TWISTED_SYSTEM_REFERENCE.md)

</details>

<details>
<summary>Translation and memory admission (46 records)</summary>

- [The 0x1F6EB0 admission advances the stop to 0x1FC780](FRONTIER_1F6EB0_ADMISSION_CHECKPOINT.md)
- [Frontier: `0x1F6EB0` read-only characterization](FRONTIER_1F6EB0_CHECKPOINT.md)
- [The `0x1F7080` admission observes the taken branch at `0x1F70E0`](FRONTIER_1F7080_ADMISSION_CHECKPOINT.md)
- [Frontier: `0x1F7080` read-only characterization](FRONTIER_1F7080_CHECKPOINT.md)
- [Frontier: `0x1F70E0` and `0x1F5B70` static continuation candidates](FRONTIER_1F70E0_1F5B70_CHECKPOINT.md)
- [The 0x1FC780 admission advances the stop to 0x1FCCB0](FRONTIER_1FC780_ADMISSION_CHECKPOINT.md)
- [Frontier: `0x1FC780` read-only characterization](FRONTIER_1FC780_CHECKPOINT.md)
- [The 0x1FCCB0 admission advances the stop to 0x1F7080](FRONTIER_1FCCB0_ADMISSION_CHECKPOINT.md)
- [Frontier: `0x1FCCB0` read-only characterization (indirect dispatch)](FRONTIER_1FCCB0_CHECKPOINT.md)
- [Frontier: callees of the post-`0x1F7080` candidates](FRONTIER_BATCH_CALLEES_CHECKPOINT.md)
- [The `0x1F65F0` batch closes the last render-state translation gap](FRONTIER_DEP_1F65F0_CHECKPOINT.md)
- [The `0x1F6620` symbol-entry batch advances the stop to `0x1F6BB0`](FRONTIER_DEP_1F6620_CHECKPOINT.md)
- [The `0x1F6730` batch advances the stop to `0x1F75B0`](FRONTIER_DEP_1F6730_CHECKPOINT.md)
- [The `0x1F6760` batch advances the stop to `0x1F7460`](FRONTIER_DEP_1F6760_CHECKPOINT.md)
- [The `0x1F6790` batch advances the stop to `0x1F6AC0`](FRONTIER_DEP_1F6790_CHECKPOINT.md)
- [The `0x1F6850` tail-call batch advances the stop to `0x1F68C0`](FRONTIER_DEP_1F6850_CHECKPOINT.md)
- [The `0x1F6890` batch advances the stop to `0x1F7300`](FRONTIER_DEP_1F6890_CHECKPOINT.md)
- [The `0x1F68C0` batch advances the stop to `0x1F69F0`](FRONTIER_DEP_1F68C0_CHECKPOINT.md)
- [The `0x1F6910` dependency batch advances the stop to `0x1F5B70`](FRONTIER_DEP_1F6910_CHECKPOINT.md)
- [The `0x1F6970` dependency batch advances the stop to `0x1F6620`](FRONTIER_DEP_1F6970_CHECKPOINT.md)
- [The `0x1F69F0` batch advances the stop to `0x1F6A70`](FRONTIER_DEP_1F69F0_CHECKPOINT.md)
- [The `0x1F6A70` batch advances the stop to `0x1F6730`](FRONTIER_DEP_1F6A70_CHECKPOINT.md)
- [The `0x1F6AC0` batch advances the stop to `0x1F6B00`](FRONTIER_DEP_1F6AC0_CHECKPOINT.md)
- [The `0x1F6B00` batch advances the stop to `0x1F6B50`](FRONTIER_DEP_1F6B00_CHECKPOINT.md)
- [The `0x1F6B50` batch advances the stop to `0x1F6890`](FRONTIER_DEP_1F6B50_CHECKPOINT.md)
- [The `0x1F6BB0` symbol-entry batch advances the stop to `0x1F6790`](FRONTIER_DEP_1F6BB0_CHECKPOINT.md)
- [The `0x1F7300` batch advances the stop to `0x1F7370`](FRONTIER_DEP_1F7300_CHECKPOINT.md)
- [The `0x1F7370` batch advances the stop to `0x1F73F0`](FRONTIER_DEP_1F7370_CHECKPOINT.md)
- [The `0x1F73F0` batch advances the stop to `0x1F6850`](FRONTIER_DEP_1F73F0_CHECKPOINT.md)
- [The `0x1F7460` batch advances the stop to `0x1F7490`](FRONTIER_DEP_1F7460_CHECKPOINT.md)
- [The `0x1F7490` batch advances the stop to `0x1F74F0`](FRONTIER_DEP_1F7490_CHECKPOINT.md)
- [The `0x1F74F0` batch advances the stop to `0x1F7550`](FRONTIER_DEP_1F74F0_CHECKPOINT.md)
- [The `0x1F7550` batch advances the stop to `0x1F7570`](FRONTIER_DEP_1F7550_CHECKPOINT.md)
- [The `0x1F7570` batch completes the render-state table and stops at `D3DDevice_Present`](FRONTIER_DEP_1F7570_CHECKPOINT.md)
- [The `0x1F75B0` batch advances the stop to `0x1F7600`](FRONTIER_DEP_1F75B0_CHECKPOINT.md)
- [The `0x1F7600` batch advances the stop to `0x1F6760`](FRONTIER_DEP_1F7600_CHECKPOINT.md)
- [The `0x202150` batch admits `D3DDevice_Present` and stops at the PFIFO pushbuffer boundary](FRONTIER_DEP_202150_CHECKPOINT.md)
- [The Path A admission advances the stop to `0x1F6910`](FRONTIER_PATH_A_ADMISSION_CHECKPOINT.md)
- [Path A preflight: `0x1F70E0` and its two untranslated callees](FRONTIER_PATH_A_PREFLIGHT_CHECKPOINT.md)
- [The Path B batch advances the stop to `0x1F6970`](FRONTIER_PATH_B_ADMISSION_CHECKPOINT.md)
- [Recomposition is byte-identical, and the guard emitter is deterministic and idempotent](RECOMP_DETERMINISM_CHECKPOINT.md)
- [Explicit translation budget extended to 32,768; execution reaches 0x1F78E0](TRANSLATION_BUDGET_CHECKPOINT.md)
- [Read-only characterization of the 0x202150 call surface](TRANSLATION_FRONTIER_CHECKPOINT.md)
- [A001 closed: generated vector ops are non-writable, and denied ops stop before mutation in compiled code](VECTOR_ACCESS_A001_CHECKPOINT.md)
- [Checked XMM memory surface admits the matrix helper; execution reaches 0x1FCB30](XMM_MEMORY_SURFACE_CHECKPOINT.md)
- [XMM type ownership repaired; fail-closed helper admission and owned boundary test](XMM_TYPE_OWNERSHIP_CHECKPOINT.md)

</details>

<details>
<summary>Graphics, device state, and presentation (34 records)</summary>

- [Ordinal 2: establish the reached call contract before emulating an encoder](AV_ENCODER_BOUNDARY_REVIEW.md)
- [The option-6 AV model is withdrawn; ordinal 2 remains unsupported](AV_ENCODER_SERVICE_CHECKPOINT.md)
- [The AV mode table accepts a known domain; a zero result matches no row](AV_MODE_DOMAIN_CHECKPOINT.md)
- [Ordinal-2 original-hardware observation request](AV_ORIGINAL_CAPTURE_REQUEST.md)
- [One default-off synthetic raw-DWORD binding completes the ordinal-2 query](AV_QUERY_BINDING_CHECKPOINT.md)
- [Display-register traffic is now auditable in the device log](DISPLAY_TRAFFIC_LOG_CHECKPOINT.md)
- [Filled-triangle geometry component checkpoint](NV2A_GEOMETRY_CHECKPOINT.md)
- [A bounded PCRTC interrupt component releases the PMC wait only when a profile supplies display events](PCRTC_INTERRUPT_COMPONENT_CHECKPOINT.md)
- [PFIFO boundary design note: prototype semantics and the minimal responder](PFIFO_BOUNDARY_DESIGN_NOTE.md)
- [PFIFO channel setup evidence: the title writes GET=0 itself; the pushbuffer claim is still empty](PFIFO_CHANNEL_SETUP_CHECKPOINT.md)
- [PFIFO completion-flag characterization (read-only)](PFIFO_COMPLETION_FLAG_CHECKPOINT.md)
- [PFIFO pusher prototype: verified component, unbounded consumer run](PFIFO_CONSUMER_PROTOTYPE_CHECKPOINT.md)
- [Device-context observation: the ring is 512 KB at 0x03B7D000; the title waits for GET=base](PFIFO_CONTEXT_CHECKPOINT.md)
- [Read-only NV2A packet decoder; PFIFO boundary unchanged](PFIFO_DECODE_CHECKPOINT.md)
- [Empty-ring initialization passes the PFIFO wait; execution reaches 0x1FB3B0](PFIFO_EMPTY_RING_CHECKPOINT.md)
- [PFIFO engine/consumer handoff](PFIFO_ENGINE_HANDOFF.md)
- [The remaining exit predicates are PFIFO engine status, not display state](PFIFO_EXIT_PREDICATE_CHECKPOINT.md)
- [Serving the observed DMA_GET makes the original wait poll to its limit](PFIFO_GET_READ_CHECKPOINT.md)
- [PFIFO interrupt acknowledge joins PGRAPH; the boundary moves to a PFIFO engine-state wait](PFIFO_INTERRUPT_ACKNOWLEDGE_CHECKPOINT.md)
- [Next routine 0x1FB3B0 needs 29 D3D dependencies; guard-memory admission is blocked at 0x1FA460](PFIFO_NEXT_ROUTINE_CHECKPOINT.md)
- [Empty-ring override guarded against a changed PUT; profile path still reaches 0x1FB3B0](PFIFO_OVERRIDE_GUARD_CHECKPOINT.md)
- [The PFIFO engine predicates have no producer in the retained reference either](PFIFO_PREDICATE_PROVENANCE_CHECKPOINT.md)
- [Empty-ring profile adopted; default build reaches 0x1FB3B0 with the profile and stays fail-closed without it](PFIFO_PROFILE_ADOPTION_CHECKPOINT.md)
- [The pinned reset state passes the PFIFO low-mark wait; execution reaches 0x1FCB00](PFIFO_RESET_STATE_CHECKPOINT.md)
- [Pushbuffer ring pointer model; ring base/end and command source still unresolved](PFIFO_RING_CHECKPOINT.md)
- [The PFIFO status seam exists; the contract owner does not](PFIFO_STATUS_SEAM_CHECKPOINT.md)
- [Acknowledged PGRAPH interrupt store clears the recurrence; the next stop is an unmodeled PMC interrupt-status read](PGRAPH_INTERRUPT_ACKNOWLEDGE_CHECKPOINT.md)
- [Vector/port hardening is accepted; the next stop is a PGRAPH interrupt read-back recurrence](PGRAPH_INTERRUPT_RECURRENCE_CHECKPOINT.md)
- [PMC bit 24 is the PCRTC (display) interrupt; no host producer exists, so delivery stays fail-closed](PMC_BIT24_PROVENANCE_CHECKPOINT.md)
- [PMC zero status is an explicit experiment; the default stays fail-closed](PMC_ZERO_EXPERIMENT_CHECKPOINT.md)
- [Read-only characterization of `D3DDevice_Present` and its call surface](PRESENT_STATIC_CHARACTERIZATION_CHECKPOINT.md)
- [Original graphics initialization requests contiguous kernel memory](RENDERER_ENTRY_CHECKPOINT.md)
- [The ordinal-159 wait object is a D3D device field, not a KEVENT](VBLANK_OBJECT_CHECKPOINT.md)
- [Ordinal-159 vblank wait model: gate refused, event layout unconfirmed](VBLANK_WAIT_MODEL_CHECKPOINT.md)

</details>

<details>
<summary>Startup, filesystem, and runtime services (35 records)</summary>

- [Bounded Xbox controller packet, adapter, port, browser source and four-port device; PFIFO boundary unchanged](CONTROLLER_PACKET_CHECKPOINT.md)
- [Original copy-helper and mixed-layout checkpoint](COPY_STARTUP_CHECKPOINT.md)
- [CRT startup and explicit settings checkpoint](CRT_STARTUP_CHECKPOINT.md)
- [Declared device-status waits are reported instead of silently filling the event log](DEVICE_STATUS_WAIT_CHECKPOINT.md)
- [Original title-data directories and next drive-link contract](DIRECTORY_STARTUP_CHECKPOINT.md)
- [A bounded browser-local read-only disc session is working](DISC_SESSION_CHECKPOINT.md)
- [0x1FCCB0 admitted; next stop 0x1F7080](FATX_UTILITY_MOUNT_CHECKPOINT.md)
- [Original directory-backed drive link and next metadata-file request](FILE_LINK_STARTUP_CHECKPOINT.md)
- [Original metadata write and handle closure](FILE_WRITE_STARTUP_CHECKPOINT.md)
- [Original hexadecimal-format return and next startup dependency](FORMAT_STARTUP_CHECKPOINT.md)
- [Original heap return and first file request](HEAP_STARTUP_CHECKPOINT.md)
- [Deterministic fixed-step host loop pacing; original execution still at the PFIFO boundary](HOST_STEP_CHECKPOINT.md)
- [Original initialization and disk-cache checkpoint](INITIALIZER_EXECUTION_CHECKPOINT.md)
- [Checked guest memory and original heap-startup checkpoint](MEMORY_STARTUP_CHECKPOINT.md)
- [Modeled memory statistics return and original code enters renderer initialization](MEMORY_STATISTICS_CHECKPOINT.md)
- [Original wide metadata formatting and pending write](METADATA_FORMAT_STARTUP_CHECKPOINT.md)
- [Original drive link and next startup helper](NAMESPACE_STARTUP_CHECKPOINT.md)
- [Native web port feasibility checkpoint](NATIVE_WEB_FEASIBILITY.md)
- [Original startup execution checkpoint](ORIGINAL_STARTUP_CHECKPOINT.md)
- [Original cache-partition formatting writes](PARTITION_FORMAT_CHECKPOINT.md)
- [Bounded port input for 0x80C0; execution reaches a new register read at 0xFD002080](PORT_INPUT_CHECKPOINT.md)
- [Full-range scan: the GET..PUT span includes the game image/heap, not a queue](POST_MEMORY_EXECUTION_CHECKPOINT.md)
- [Original formatter returns after a real volume dismount](RAW_VOLUME_DISMOUNT_CHECKPOINT.md)
- [Original metadata-file creation and query](REGULAR_FILE_STARTUP_CHECKPOINT.md)
- [Original section residency and image initialization](SECTION_RESIDENCY_CHECKPOINT.md)
- [Service ordinal 159 characterization (read-only)](SERVICE_ORDINAL_159_CHECKPOINT.md)
- [Service ordinal 1 is AvGetSavedDataAddress](SERVICE_ORDINAL_1_CHECKPOINT.md)
- [The later ordinal-2 call is an unknown AV option 0x0F (read-only)](SERVICE_ORDINAL_2_SECOND_QUERY_CHECKPOINT.md)
- [Service ordinal 3 is AvSetDisplayMode (read-only)](SERVICE_ORDINAL_3_CHECKPOINT.md)
- [Original thread-entry checkpoint](THREAD_STARTUP_CHECKPOINT.md)
- [Timer diagnostic identifies the DPC-list head mismatch at the ordinal-149 stop](TIMER_DPC_DIAGNOSTIC_CHECKPOINT.md)
- [One-shot timer queue component checkpoint](TIMER_QUEUE_CHECKPOINT.md)
- [Browser clock and original timer-arming checkpoint](TIMER_STARTUP_CHECKPOINT.md)
- [Original title-image section-load boundary](TITLE_IMAGE_SECTION_CHECKPOINT.md)
- [Original volume lifecycle and drive-link request](VOLUME_STARTUP_CHECKPOINT.md)

</details>

<details>
<summary>Integration and verification (5 records)</summary>

- [Architect integration checkpoint: vector coverage and interrupt-policy gates remain](https://github.com/Jdiller-86/fuzion-frenzy-web-port/blob/85b28e073422b8ee0206f31bbdaa3c31a6e6b182/docs/ARCHITECT_INTEGRATION_CHECKPOINT.md)
- [Architect PR and execution review](https://github.com/Jdiller-86/fuzion-frenzy-web-port/blob/85b28e073422b8ee0206f31bbdaa3c31a6e6b182/docs/ARCHITECT_PR_REVIEW_CHECKPOINT.md)
- [Exact-head integration candidate review](INTEGRATION_CANDIDATE_REVIEW.md)
- [Integration evidence index and hold](INTEGRATION_EVIDENCE_INDEX.md)
- [Integration verification checkpoint](https://github.com/Jdiller-86/fuzion-frenzy-web-port/blob/85b28e073422b8ee0206f31bbdaa3c31a6e6b182/docs/INTEGRATION_VERIFICATION_CHECKPOINT.md)

</details>

## Package manifests and reference metadata

The frontier/preflight manifests contain curated package/admission metadata; reference-files.json records the reference inventory. These are identity and evidence records, not downloadable title modules.

- [FRONTIER 1FCCB0 — manifest](FRONTIER_1FCCB0_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F65F0 — manifest](FRONTIER_DEP_1F65F0_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F6620 — manifest](FRONTIER_DEP_1F6620_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F6730 — manifest](FRONTIER_DEP_1F6730_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F6760 — manifest](FRONTIER_DEP_1F6760_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F6790 — manifest](FRONTIER_DEP_1F6790_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F6850 — manifest](FRONTIER_DEP_1F6850_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F6890 — manifest](FRONTIER_DEP_1F6890_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F68C0 — manifest](FRONTIER_DEP_1F68C0_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F6910 — manifest](FRONTIER_DEP_1F6910_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F6970 — manifest](FRONTIER_DEP_1F6970_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F69F0 — manifest](FRONTIER_DEP_1F69F0_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F6A70 — manifest](FRONTIER_DEP_1F6A70_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F6AC0 — manifest](FRONTIER_DEP_1F6AC0_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F6B00 — manifest](FRONTIER_DEP_1F6B00_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F6B50 — manifest](FRONTIER_DEP_1F6B50_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F6BB0 — manifest](FRONTIER_DEP_1F6BB0_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F7300 — manifest](FRONTIER_DEP_1F7300_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F7370 — manifest](FRONTIER_DEP_1F7370_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F73F0 — manifest](FRONTIER_DEP_1F73F0_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F7460 — manifest](FRONTIER_DEP_1F7460_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F7490 — manifest](FRONTIER_DEP_1F7490_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F74F0 — manifest](FRONTIER_DEP_1F74F0_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F7550 — manifest](FRONTIER_DEP_1F7550_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F7570 — manifest](FRONTIER_DEP_1F7570_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F75B0 — manifest](FRONTIER_DEP_1F75B0_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 1F7600 — manifest](FRONTIER_DEP_1F7600_PACKAGE_MANIFEST.json)
- [FRONTIER DEP 202150 — manifest](FRONTIER_DEP_202150_PACKAGE_MANIFEST.json)
- [FRONTIER PATH A — manifest](FRONTIER_PATH_A_PACKAGE_MANIFEST.json)
- [FRONTIER PATH B — manifest](FRONTIER_PATH_B_PACKAGE_MANIFEST.json)
- [reference-files](reference-files.json)
