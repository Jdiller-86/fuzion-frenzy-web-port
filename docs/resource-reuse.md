# M1 development workflow and resource reuse

The user-directed priority is development speed toward one playable original round. Use available project artifacts and relevant upstream solutions before writing another compatibility layer. Keep the verified browser checkpoint as a comparison baseline while integrating the SDK route. Original execution remains entirely in the browser; original inputs and generated title code stay local.

## Required work streams

| Work | Resources and implementation boundary |
| --- | --- |
| Bulk C++ generation | Reuse the matching [Fuzion configuration](https://github.com/abilliontokens/fuzionfrenzy-recomp/tree/071dbd97bdc098c4647cc24c99f9eae693fde1eb) and [XboxXBERecomp](https://github.com/abilliontokens/XboxXBERecomp/tree/58757ab255c0a6b3929f405d7a5f1567fcd7fda3). Run the local generator, retain its inventory and reuse unchanged output. Generated quantity does not establish valid function boundaries or execution admission. |
| Browser runtime interfaces | Adapt checked memory access, function/kernel dispatch and thread contexts once at their shared interfaces. Reuse existing virtual-memory, boot-thread and service models. Preserve explicit stops for unsupported access, calls and scheduling. Avoid per-function rewrites when a common adapter fixes the incompatibility. |
| Browser platform | Use [OpenCE's web implementation](https://github.com/OpenCommunityEdition/OpenCE/tree/fc9deb0b10eaae1d62caedb0710968b4e77e6e16/port/web) where applicable: worker execution, WebGL2 restrictions, SDL audio/input and local OPFS storage. Reuse existing project SDL/gamepad/clock/file components and their retained checks. Keep the current memory budget; OpenCE's large Xbox address window is not a drop-in layout. |
| Routine preparation and regression | Use reusable commands and computed manifests for seed packing, identities, guard preservation, binding deltas and generated-body comparisons. Investigate actual reached incompatibilities; do not manually implement every statically discovered trap or repeat one-off setup scripts. |

Source pins are recorded in [the toolchain manifest](../tools/original-execution-toolchain.json). Inspect retained artifacts before downloading, rebuilding or regenerating. Keep component licensing/provenance with reused source; upstream claims and native results do not establish this browser port's compatibility.

## Fast iteration before M1

**Use simple, targeted verification. Defer detailed testing, broad matrices, refinements and polish until after M1.** For each change, check the affected behavior and use one meaningful browser comparison when applicable. Reuse passing unchanged checks. A failure or a specific unresolved correctness concern can justify a deeper check; routine work should not become a repeated full audit.

Keep brief independent correctness review for changed CPU, memory, ABI or GPU behavior. Run required preparation/publication checks at coherent source checkpoints. The final M1 gate still needs the three original menu/round/results/menu sequences, audiovisual/input evidence and reproducibility review; intermediate fixtures and compilation cannot substitute for that result.

The [bulk SDK command](../tools/prepare-sdk-title.py) validates the matching local tool/configuration/XBE and generates only into a fresh ignored directory, recording hashes, inventory and static-only status. The [bounded batch command](../translator/extend-recomp-batch.py) extends an existing checked profile from an actual browser stop, an explicit seed index and a read-only addition seed. It wraps the existing preparation checks, packs into an existing export, computes retention/binding audits and stops before module build or execution. Both commands require explicit private inputs and do not download tools or execute the game. A bulk SDK output is a separate offline candidate; it does not automatically enlarge the active browser admission limits.

## Current resource evidence

The matching configured XBE has been generated locally into 64 C++ partitions. The SDK reports 10,589 functions and 2,165,441 of 2,368,328 image-code bytes; overlapping decodes and unsupported sites remain. These are static generator statistics, not complete source recovery, fidelity or playability.

The SDK also generated the existing source-owned arithmetic fixture. Its unchanged C++ compiled to a fixed 16 MiB, zero-import Wasm module and passed two 25-pair runs in Chrome 156.0.8078.80. The checked adapter and one separately reviewed original-function sentinel branch subsequently passed controlled browser checks. These do not establish full startup, flags fidelity, general memory/thread support or title compatibility. The SDK adapter and OpenCE platform integration remain in progress. [Status](status.md) owns the current original-program boundary and acceptance limits.
