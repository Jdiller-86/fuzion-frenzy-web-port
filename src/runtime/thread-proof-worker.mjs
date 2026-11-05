import { BUILD_REFERENCE as REF, loadLocalTitleProof, parseLocalConsoleSettings, parseLocalNvConsoleSettings, parseLocalVolumeProfile, parseLocalDirectoryProfile, parseLocalFileProfile, parseLocalDiskProfile, parseLocalDiskPartitions, parseLocalDiskGeometry, parseLocalPartitionInformation, parseLocalRawDiskVolumes, parseLocalMemoryProfile, parseLocalMmioProfile, describeMmioExperiments, parseLocalPfifoProfile, proofTraceCapacity, proofKernelCapacity } from './local-title-proof.mjs';
import { createProofClock } from './browser-clock.mjs';
import { runOriginalCopyProof } from './copy-proof.mjs';
import { inspectOpenFileRequest, inspectCreateFileRequest, inspectWriteFileRequest, inspectLoadSectionRequest, compareWideMetadataFormat, inspectCountedAnsiString } from './file-request.mjs';
import { createDeviceReport } from './device-report.mjs';
import { validateProofReport } from './proof-report.mjs';
import { describeKernelService } from './kernel-service.mjs';

/* A boundary report is evidence for a human reader, so it is validated before it
 * is posted: a malformed or out-of-range report becomes an error message instead
 * of a result. */
function postBoundary(report) {
  try {
    validateProofReport(report);
  } catch (error) {
    self.postMessage({ kind: 'error', message: `Proof report rejected: ${error.message}` });
    return;
  }
  self.postMessage(report);
}

import { createSubmissionProbe } from './submission-probe.mjs';

const fields = ['id','handle','handleOpen','state','object','fs','stackLow','stackTop','tls','tlsBytes',
  'routine','context','system','initialSp'];
self.onmessage = async event => {
  try {
    const clock = createProofClock();
    const { api, memory, range, view, ram, profile, imageInitialState, sourceSha256, moduleSha256, stackTop } =
      await loadLocalTitleProof(event.data, [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24], ['thread_count','parent_returned','kernel_count','kernel_word','thread_word'], { clock });
    const timerAbi = api.ff_probe_abi() >= 4, memoryAbi = api.ff_probe_abi() >= 5, volumeAbi = api.ff_probe_abi() >= 6, namespaceAbi = api.ff_probe_abi() >= 7;
    const directoryAbi = api.ff_probe_abi() >= 8, fileLinkAbi = api.ff_probe_abi() >= 9, regularAbi=api.ff_probe_abi()>=10, metadataAbi=api.ff_probe_abi()>=11, writeAbi=api.ff_probe_abi()>=12,sectionAbi=api.ff_probe_abi()>=13,nvAbi=api.ff_probe_abi()>=14;
    const abi=api.ff_probe_abi();const statisticsAbi=abi>=23,fatxAbi=abi>=22,dismountAbi=abi>=21,informationAbi=abi>=20,geometryAbi=abi>=19,partitionAbi=abi>=18,diskAbi=abi>=17,initializerAbi=abi>=16;
    if(api.ff_probe_abi()>=15 && (proofKernelCapacity(api)!==(initializerAbi?512:256) || typeof api.ff_probe_fp_word!=='function')) throw new Error('Expanded startup requires its declared bounded observation ABI');
    if(metadataAbi && ['metadata_format_word','metadata_format_byte'].some(name=>typeof api['ff_probe_'+name]!=='function')) throw new Error('Missing bounded metadata-format observation ABI');
    if(metadataAbi && proofTraceCapacity(api)!==(initializerAbi?8192:1024)) throw new Error('Metadata-format proof requires its explicit bounded trace capacity');
    if(regularAbi && ['file_storage_capacity','regular_word','regular_name','regular_byte','file_information_word','create_request_word','create_request_name'].some(name=>typeof api['ff_probe_'+name]!=='function')) throw new Error('Missing regular-file storage ABI');
    if(fileLinkAbi && ['file_word','file_resolution'].some(name=>typeof api['ff_probe_'+name]!=='function')) throw new Error('Missing retained file-link ABI');
    if (directoryAbi && ['directory_word','directory_name'].some(name=>typeof api['ff_probe_'+name]!=='function')) throw new Error('Missing owned directory ABI');
    if (timerAbi && ['timer_count','timer_word','dpc_count','dpc_delivered','poll','dpc_recover'].some(name =>
      typeof api['ff_probe_' + name] !== 'function')) throw new Error('Missing timer proof ABI');
    let consoleSettings, volumeProfile, directoryProfile, fileStorageProfile, diskProfile, localDisk;
    let memoryProfile, mmioProfile;
    if (api.ff_probe_abi() >= 3) {
      if (typeof api.ff_probe_misc_flags !== 'function') throw new Error('Missing local settings ABI');
      if (!event.data.settingsBytes) throw new Error('Select an explicit local test console-settings profile');
      consoleSettings = nvAbi?parseLocalNvConsoleSettings(event.data.settingsBytes):parseLocalConsoleSettings(event.data.settingsBytes);
      api.ff_probe_misc_flags(consoleSettings.miscFlags);
      if(nvAbi) {
        if(typeof api.ff_probe_parental_games!=='function') throw new Error('Missing explicit console-settings ABI');
        api.ff_probe_parental_games(consoleSettings.parentalControlGames);
      }
    } else if (event.data.settingsBytes) throw new Error('The earlier proof ABI does not consume a settings profile');
    if(api.ff_probe_abi()===24) {
      mmioProfile=parseLocalMmioProfile(event.data.settingsBytes);
      const primary=mmioProfile.registers.find(entry=>entry.address===0xfd001804);
      const secondary=mmioProfile.registers.find(entry=>entry.address===0xfd600140);
      /* Write-1-to-clear register behaviour is an explicitly experimental model:
       * the selected profile must opt in, and the same profile without it is the
       * control that must reproduce the un-acknowledged guest loop. */
      if(typeof api.ff_probe_mmio_write_clearing!=='function' ||
         api.ff_probe_mmio_write_clearing(mmioProfile.writeClearing?1:0)) {
        throw new Error('The private module cannot set the experimental write-clearing control');
      }
      if(typeof api.ff_probe_mmio_pmc_zero!=='function' ||
         api.ff_probe_mmio_pmc_zero(mmioProfile.pmcZero?1:0)) {
        throw new Error('The private module cannot set the experimental PMC zero-status control');
      }
      if(typeof api.ff_probe_mmio_engine_wait!=='function' ||
         api.ff_probe_mmio_engine_wait(mmioProfile.engineWait?1:0)) {
        throw new Error('The private module cannot set the experimental engine-wait control');
      }
      if(mmioProfile.pfifoReset) {
        if(typeof api.ff_probe_mmio_pfifo_reset!=='function' ||
           api.ff_probe_mmio_pfifo_reset(1)) {
          throw new Error('The private module cannot set the experimental PFIFO reference-reset control');
        }
      }
      if(mmioProfile.pfifoConsumer) {
        if(typeof api.ff_probe_pfifo_consumer!=='function' || api.ff_probe_pfifo_consumer(1)) {
          throw new Error('The private module cannot enable the experimental PFIFO consumer');
        }
      }
      if(mmioProfile.displayMode) {
        if(typeof api.ff_probe_display_mode!=='function' || api.ff_probe_display_mode(1)) {
          throw new Error('The private module cannot enable the experimental display-mode declaration');
        }
      }
      if(mmioProfile.avUnknownOption) {
        if(typeof api.ff_probe_av_unknown!=='function' || api.ff_probe_av_unknown(1)) {
          throw new Error('The private module cannot enable the experimental unknown AV option model');
        }
      }
      if(mmioProfile.avSavedData) {
        if(typeof api.ff_probe_av_saved_data!=='function' || api.ff_probe_av_saved_data(1)) {
          throw new Error('The private module cannot enable the experimental saved-data model');
        }
      }
      if(mmioProfile.avNullResult) {
        if(typeof api.ff_probe_av_null_result!=='function' || api.ff_probe_av_null_result(1)) {
          throw new Error('The private module cannot enable the experimental null-result AV model');
        }
      }
      if(mmioProfile.mmFreeContiguous) {
        if(typeof api.ff_probe_mm_free_contiguous!=='function' || api.ff_probe_mm_free_contiguous(1)) {
          throw new Error('The private module cannot enable the experimental contiguous-free model');
        }
      }
      if(mmioProfile.guestWatch) {
        if(typeof api.ff_probe_guest_watch!=='function' || api.ff_probe_guest_watch(1)) {
          throw new Error('The private module cannot enable the diagnostic guest write watch');
        }
      }
      if(mmioProfile.guestWatch && mmioProfile.guestWatchAddr !== undefined) {
        if(typeof api.ff_probe_guest_watch_target!=='function' || api.ff_probe_guest_watch_target(mmioProfile.guestWatchAddr)) {
          throw new Error('The private module cannot retarget the diagnostic guest watch');
        }
      }
      if(mmioProfile.siteWatch) {
        if(typeof api.ff_probe_site_watch!=='function' || api.ff_probe_site_watch(1)) {
          throw new Error('The private module cannot enable the diagnostic site watch');
        }
      }
      if(mmioProfile.avQueryWord !== undefined) {
        if(typeof api.ff_probe_av_query_word!=='function' ||
           api.ff_probe_av_query_word(mmioProfile.avQueryWord)) {
          throw new Error('The private module cannot set the declared synthetic AV query word');
        }
      }
      if(mmioProfile.pcrtcVblanks) {
        if(typeof api.ff_probe_pcrtc!=='function' || api.ff_probe_pcrtc(mmioProfile.pcrtcVblanks)) {
          throw new Error('The private module cannot admit the declared display events');
        }
      }
      if(api.ff_probe_mmio_profile(mmioProfile.window.base,mmioProfile.window.size,
        primary.initial,secondary.initial)) {
        throw new Error('Cannot admit the explicit synthetic register window');
      }
      for(const entry of mmioProfile.registers.filter(entry=>entry.address!==0xfd001804 && entry.address!==0xfd600140)) {
        if(typeof api.ff_probe_mmio_add!=='function' || api.ff_probe_mmio_add(entry.address,entry.initial)) {
          throw new Error('The private module cannot seed the explicit extra register profile');
        }
      }
      for(const range of mmioProfile.zeroRanges ?? []) {
        if(typeof api.ff_probe_mmio_zero!=='function' || api.ff_probe_mmio_zero(range.address,range.bytes)) {
          throw new Error('The private module cannot seed the explicit zero range');
        }
      }
    }
    let pfifoProfile;
    if (typeof api.ff_probe_pfifo_profile === 'function') {
      pfifoProfile = parseLocalPfifoProfile(event.data.settingsBytes);
      if (pfifoProfile && api.ff_probe_pfifo_profile(pfifoProfile.base, pfifoProfile.end, pfifoProfile.put))
        throw new Error('Cannot admit the explicit synthetic empty PFIFO ring profile');
    }
    if(statisticsAbi) {
      memoryProfile=parseLocalMemoryProfile(event.data.settingsBytes);
      if(api.ff_probe_memory_profile(memoryProfile.physicalPages,memoryProfile.baselinePoolPages)) {
        throw new Error('Cannot admit explicit modeled physical-memory accounting');
      }
    }
    if (volumeAbi) {
      if (['volume_profile','volume_word','volume_handle_word'].some(name => typeof api['ff_probe_' + name] !== 'function')) throw new Error('Missing local volume ABI');
      volumeProfile = parseLocalVolumeProfile(event.data.settingsBytes);
      if (directoryAbi) directoryProfile = parseLocalDirectoryProfile(event.data.settingsBytes);
      if (regularAbi) {
        fileStorageProfile=parseLocalFileProfile(event.data.settingsBytes);
        if(api.ff_probe_file_storage_capacity(0)!==fileStorageProfile.maxFiles || api.ff_probe_file_storage_capacity(1)!==fileStorageProfile.maxFileBytes) throw new Error('Unsupported bounded file storage capacity');
      }
      const total = BigInt(volumeProfile.totalAllocationUnits), available = BigInt(volumeProfile.availableAllocationUnits);
      if (api.ff_probe_volume_profile(Number(total & 0xffffffffn), Number(total >> 32n), Number(available & 0xffffffffn),
          Number(available >> 32n), volumeProfile.sectorsPerAllocationUnit, volumeProfile.bytesPerSector)) throw new Error('Cannot admit explicit local volume profile');
    }
    if(diskAbi) {
      diskProfile=parseLocalDiskProfile(event.data.settingsBytes);
      if(partitionAbi)diskProfile.partitions=parseLocalDiskPartitions(event.data.settingsBytes);
      if(geometryAbi)diskProfile.geometry=parseLocalDiskGeometry(event.data.settingsBytes);
      if(informationAbi)diskProfile.partitionInformation=parseLocalPartitionInformation(event.data.settingsBytes);
      if(dismountAbi)diskProfile.rawVolumes=parseLocalRawDiskVolumes(event.data.settingsBytes);
      const bytes=BigInt(diskProfile.bytes);
      if(api.ff_probe_disk_profile(Number(bytes&0xffffffffn),Number(bytes>>32n),diskProfile.sectorBytes,diskProfile.cachePartitions)) {
        throw new Error('Cannot admit explicit local disk-device profile');
      }
      if(partitionAbi)for(const part of diskProfile.partitions) {
        const offset=BigInt(part.offset),length=BigInt(part.bytes);
        if(api.ff_probe_disk_partition_profile(part.number,Number(offset&0xffffffffn),Number(offset>>32n),Number(length&0xffffffffn),Number(length>>32n))) {
          throw new Error('Cannot admit explicit disk-partition backing');
        }
      }
      if(geometryAbi) {
        const cylinders=BigInt(diskProfile.geometry.cylinders);
        if(api.ff_probe_disk_geometry_profile(Number(cylinders&0xffffffffn),Number(cylinders>>32n),diskProfile.geometry.tracksPerCylinder,diskProfile.geometry.sectorsPerTrack)) {
          throw new Error('Cannot admit explicit disk geometry');
        }
      }
      if(informationAbi)for(const info of diskProfile.partitionInformation) {
        if(api.ff_probe_disk_partition_information(info.number,info.hiddenSectors,info.partitionType,
          (info.bootIndicator?1:0)|(info.recognizedPartition?2:0)|(info.rewritePartition?4:0))) {
          throw new Error('Cannot admit explicit partition information');
        }
      }
      if(dismountAbi)for(const partition of diskProfile.rawVolumes) {
        if(api.ff_probe_disk_raw_volume(partition))throw new Error('Cannot admit fresh RAW-volume attachment');
      }
    }
    if (namespaceAbi) {
      if (['namespace_init','namespace_count','namespace_word','namespace_name','namespace_target_name','namespace_resolution'].some(name =>
        typeof api['ff_probe_' + name] !== 'function')) throw new Error('Missing object namespace ABI');
      if (api.ff_probe_namespace_init()) throw new Error('Cannot admit guest device namespace');
    }
    if (profile.sections.some(s => s.address + s.virtualBytes > 0x03c00000)) throw new Error('Image overlaps boot-thread arena');
    const fileClockWindow=regularAbi?{beforeSystemTicks:clock.sample().system.toString()}:undefined;
    let trapped = false;
    try { api.ff_probe_run(); } catch (error) {
      if (!(error instanceof WebAssembly.RuntimeError)) throw error;
      trapped = true;
    } finally { if (timerAbi) api.ff_probe_dpc_recover(); }
    if(fileClockWindow) fileClockWindow.afterSystemTicks=clock.sample().system.toString();
    /* The run is over; disarm the consumer so later diagnostic guest code
     * (copy proof, denial probes) cannot consume the run budget. The report
     * still marks the experiment through the sticky configured flag. */
    if (mmioProfile.pfifoConsumer && typeof api.ff_probe_pfifo_consumer === 'function' &&
        api.ff_probe_pfifo_consumer(0))
      throw new Error('Cannot disarm the experimental PFIFO consumer');
    const floatingPointState=api.ff_probe_abi()>=15?Object.fromEntries(
      ['x87ControlWord','mxcsr','representedConditionBits','x87StackTop'].map((name,i)=>[name,api.ff_probe_fp_word(i)>>>0])):undefined;
    const count = api.ff_probe_trace_count(), threadCount = api.ff_probe_thread_count(), callCount = api.ff_probe_kernel_count();
    const traceCapacity=proofTraceCapacity(api), kernelCapacity=proofKernelCapacity(api);
    if (count < 0 || count > traceCapacity || threadCount > 4 || callCount < 0 || callCount > kernelCapacity) throw new Error('Invalid boot-proof counts');
    const words = new Uint32Array(memory, range(api.ff_probe_trace(), count * 12, 4), count * 3);
    const hasCpuTrace = typeof api.ff_probe_trace_cpu === 'function';
    const trace = Array.from({ length: count }, (_, i) => ({ kind: words[i * 3] === 1 ? 'enter' : 'exit',
      address: words[i * 3 + 1], stack: words[i * 3 + 2], ...(hasCpuTrace ? { cpu: {
        eax: api.ff_probe_trace_cpu(i, 0) >>> 0, fs: api.ff_probe_trace_cpu(i, 1) >>> 0,
        exceptionHead: api.ff_probe_trace_cpu(i, 2) >>> 0 } } : {}) }));
    const state = Array.from(new Uint32Array(memory, range(api.ff_probe_state(), 64, 4), 16));
    const threads = Array.from({ length: threadCount }, (_, i) => Object.fromEntries(fields.map((key, n) => [key, api.ff_probe_thread_word(i, n) >>> 0])));
    const calls = Array.from({ length: callCount }, (_, i) => ({ ordinal: api.ff_probe_kernel_word(i, 0),
      status: api.ff_probe_kernel_word(i, 1) >>> 0,
      arguments: Array.from({ length: ({ 2: 4, 255: 10, 107: 3, 113: 2, 24: 5, 149: 4, 168: 2, 184: 5, 190: 9, 196:10, 200:10, 202: 6, 211: 5, 218: 5, 219:8, 236: 8, 279: 3, 289: 2, 305: 2, 67: 2 })[api.ff_probe_kernel_word(i, 0)] ?? 1 }, (_, n) => api.ff_probe_kernel_word(i, n + 2) >>> 0) }));
    calls.forEach((call, i) => {
      if (call.ordinal === 2 && call.status === 0 && mmioProfile.avQueryWord !== undefined)
        call.output = {
          rawWord: api.ff_probe_kernel_word(i, 6) >>> 0,
          address: api.ff_probe_kernel_word(i, 11) >>> 0,
          eaxBefore: api.ff_probe_kernel_word(i, 7) >>> 0,
          espBefore: api.ff_probe_kernel_word(i, 8) >>> 0,
          eaxAfter: api.ff_probe_kernel_word(i, 9) >>> 0,
          espAfter: api.ff_probe_kernel_word(i, 10) >>> 0,
        };
      if (call.ordinal === 24 && call.status!==0xc00000bb) call.output = { type: api.ff_probe_kernel_word(i, 7),
        value: api.ff_probe_kernel_word(i, 8) >>> 0, requiredBytes: api.ff_probe_kernel_word(i, 9) };
      if(call.ordinal===128 && !call.status)call.output={systemTicks:((BigInt(api.ff_probe_kernel_word(i,4)>>>0)<<32n)|BigInt(api.ff_probe_kernel_word(i,3)>>>0)).toString()};
      if(statisticsAbi && call.ordinal===181 && !call.status)call.output={words:Array.from({length:9},(_,n)=>api.ff_probe_kernel_word(i,n+3)>>>0),
        scope:'modeled-guest-memory-statistics'};
      if (call.ordinal === 149) call.output = { previousInsertion: api.ff_probe_kernel_word(i, 6),        sampledInterruptTicks: ((BigInt(api.ff_probe_kernel_word(i, 8) >>> 0) << 32n) | BigInt(api.ff_probe_kernel_word(i, 7) >>> 0)).toString(),
        sampledSystemTicks: ((BigInt(api.ff_probe_kernel_word(i, 10) >>> 0) << 32n) | BigInt(api.ff_probe_kernel_word(i, 9) >>> 0)).toString() };
      if (call.ordinal === 184 && !call.status) call.output = { baseAddress: api.ff_probe_kernel_word(i, 7) >>> 0, regionBytes: api.ff_probe_kernel_word(i, 8) >>> 0 };
      if (call.ordinal === 168 && !call.status) call.output = { baseAddress: api.ff_probe_kernel_word(i, 4) >>> 0 };
      if (call.ordinal === 305 && !call.status) call.output = { fields: Array.from({length:8},(_,n)=>api.ff_probe_kernel_word(i,n+4)>>>0) };
      if (call.ordinal === 202 && !call.status) call.output = { handle: api.ff_probe_kernel_word(i, 8) >>> 0,
        ioStatus: api.ff_probe_kernel_word(i, 9) >>> 0, information: api.ff_probe_kernel_word(i, 10) >>> 0 };
      if (call.ordinal === 218 && !call.status) call.output = { ioStatus: api.ff_probe_kernel_word(i, 7) >>> 0, information: api.ff_probe_kernel_word(i, 8) >>> 0,
        sizeWords:Array.from({length:6}, (_, n) => api.ff_probe_kernel_word(i, n + 9) >>> 0) };
      if (call.ordinal === 67 && !call.status) call.output = { link:api.ff_probe_kernel_word(i,4), target:api.ff_probe_kernel_word(i,5),
        targetReferences:api.ff_probe_kernel_word(i,6), deviceKind:api.ff_probe_kernel_word(i,7), mediaReady:api.ff_probe_kernel_word(i,8) };
      if (call.ordinal === 67 && !call.status && fileLinkAbi) Object.assign(call.output,{targetKind:api.ff_probe_kernel_word(i,9),
        providerObject:api.ff_probe_kernel_word(i,10),directory:api.ff_probe_kernel_word(i,11),fileReferences:api.ff_probe_kernel_word(i,12),
        fileHandle:api.ff_probe_kernel_word(i,13),objects:api.ff_probe_kernel_word(i,14),files:api.ff_probe_kernel_word(i,15),
        opened:api.ff_probe_kernel_word(i,16),closed:api.ff_probe_kernel_word(i,17),active:api.ff_probe_kernel_word(i,18)});
      if (call.ordinal === 190) call.output = { handle:api.ff_probe_kernel_word(i,11)>>>0, ioStatus:api.ff_probe_kernel_word(i,12)>>>0,
        information:api.ff_probe_kernel_word(i,13), directories:api.ff_probe_kernel_word(i,14), created:api.ff_probe_kernel_word(i,15), active:api.ff_probe_kernel_word(i,16) };
      if(call.ordinal===190 && regularAbi) Object.assign(call.output,{dataFiles:api.ff_probe_kernel_word(i,17),dataFilesCreated:api.ff_probe_kernel_word(i,18),dataFile:api.ff_probe_kernel_word(i,19)});
      if(call.ordinal===211 && regularAbi && !call.status) call.output={ioStatus:api.ff_probe_kernel_word(i,7),information:api.ff_probe_kernel_word(i,8),
        words:Array.from({length:14},(_,n)=>api.ff_probe_file_information_word(i,n)>>>0)};
      const diskTransfer=diskAbi && [219,236].includes(call.ordinal) && call.arguments[0]>=0x60000 && call.arguments[0]<0x70000;
      if(diskTransfer)call.output={backend:'local-synthetic-disk',ioStatus:api.ff_probe_kernel_word(i,10)>>>0,information:api.ff_probe_kernel_word(i,11)};
      if(diskTransfer && !call.status && call.output.information && api.ff_probe_disk_transfer_details?.()===1) {
        const wide=(low,high)=>((BigInt(api.ff_probe_kernel_word(i,high)>>>0)<<32n)|BigInt(api.ff_probe_kernel_word(i,low)>>>0)).toString();
        Object.assign(call.output,{partition:api.ff_probe_kernel_word(i,12)>>>0,position:wide(13,14),relativeOffset:wide(15,16),physicalOffset:wide(17,18)});
      }
      if(geometryAbi && call.ordinal===196 && !call.status)call.output={backend:'local-synthetic-disk',ioStatus:api.ff_probe_kernel_word(i,12)>>>0,
        information:api.ff_probe_kernel_word(i,13)>>>0,words:Array.from({length:api.ff_probe_kernel_word(i,13)/4},(_,n)=>api.ff_probe_disk_control_word(i,n)>>>0)};
      if(dismountAbi && call.ordinal===200 && !call.status)call.output={backend:'local-synthetic-raw-volume',
        ioStatus:api.ff_probe_kernel_word(i,12)>>>0,information:api.ff_probe_kernel_word(i,13)>>>0,
        volume:api.ff_probe_kernel_word(i,14)>>>0,state:api.ff_probe_kernel_word(i,15)>>>0,
        references:api.ff_probe_kernel_word(i,16)>>>0,dismounts:api.ff_probe_kernel_word(i,17)>>>0};
      if(call.ordinal===236 && writeAbi && !diskTransfer) call.output={ioStatus:api.ff_probe_kernel_word(i,10)>>>0,information:api.ff_probe_kernel_word(i,11),
        dataFile:api.ff_probe_kernel_word(i,12),position:api.ff_probe_kernel_word(i,13),fileBytes:api.ff_probe_kernel_word(i,14),
        allocatedBytes:((BigInt(api.ff_probe_kernel_word(i,16)>>>0)<<32n)|BigInt(api.ff_probe_kernel_word(i,15)>>>0)).toString(),
        writeTime:((BigInt(api.ff_probe_kernel_word(i,18)>>>0)<<32n)|BigInt(api.ff_probe_kernel_word(i,17)>>>0)).toString(),initialPosition:api.ff_probe_kernel_word(i,19)};
      if(sectionAbi && [327,328].includes(call.ordinal)) call.output={beforeReferences:api.ff_probe_kernel_word(i,3),references:api.ff_probe_kernel_word(i,4),
        headReferences:api.ff_probe_kernel_word(i,5),tailReferences:api.ff_probe_kernel_word(i,6),address:api.ff_probe_kernel_word(i,7),
        virtualBytes:api.ff_probe_kernel_word(i,8),fileBytes:api.ff_probe_kernel_word(i,9),fileOffset:api.ff_probe_kernel_word(i,10)};
      if (call.ordinal === 289) call.output = {length:api.ff_probe_kernel_word(i,4),maximumLength:api.ff_probe_kernel_word(i,5),buffer:api.ff_probe_kernel_word(i,6)>>>0};
      if (call.ordinal === 279) call.output = {equal:api.ff_probe_kernel_word(i,5),firstBytes:api.ff_probe_kernel_word(i,6),secondBytes:api.ff_probe_kernel_word(i,7)};
    });
    const thread = threads[0];
    if (!thread || thread.object < 0x03c00000 || thread.fs < thread.object || thread.fs + 64 > thread.stackLow ||
        thread.stackTop > 0x03e00000 || thread.stackLow >= thread.initialSp || thread.initialSp + 12 > thread.tls ||
        thread.tlsBytes > 65536 || thread.tls + thread.tlsBytes !== thread.stackTop) throw new Error('Invalid boot-thread ranges');
    const tlsBytes = (((profile.tls.end - profile.tls.start + profile.tls.zeroFillBytes + 15) & 0xfffffff0) + 4) >>> 0;
    const threadLocalStorage={base:thread.tls+4,templateBytes:profile.tls.end-profile.tls.start,zeroFillBytes:profile.tls.zeroFillBytes,
      data:Array.from(ram.slice(thread.tls+4,thread.tls+4+profile.tls.end-profile.tls.start+profile.tls.zeroFillBytes))};
    const expectedCreate = [stackTop - 28, 0, profile.stackBytes, tlsBytes, 0, REF.threadStart, 0, 0, 0, REF.systemStart];
    const entered = trace.filter(row => row.kind === 'enter').map(row => row.address);
    const loaderExit = trace.findIndex(row => row.kind === 'exit' && row.address === REF.entry);
    const childEnter = trace.findIndex(row => row.kind === 'enter' && row.address === REF.systemStart);
    const reachedThreadEntry = state[0] === 2 && state[1] === 0x33ccc;
    const reachedTimerRequest = trapped && state[0] === 1 && state[2] === 149 && !!consoleSettings;
    const reachedTimerArmed = timerAbi && calls[8]?.ordinal === 149 && calls[8].status === 0;
    const reachedNextHelper = reachedTimerArmed && trapped && state[0] === 2 && state[1] === 0x1df9c0 && state[5] === 0x32327;
    const copyReturned = trace.some(row => row.kind === 'exit' && row.address === 0x1df9c0);
    const bindingIndex = (state[1] - 0xfe000000) / 4;
    const reachedAllocation = reachedTimerArmed && copyReturned && trapped && state[0] === 1 && state[2] === 184 &&
      Number.isInteger(bindingIndex) && bindingIndex >= 0 && bindingIndex < api.ff_probe_binding_count() &&
      api.ff_probe_binding_ordinal(bindingIndex) === 184 && state[5] === 0x32530 &&
      state.slice(6, 11).every((n, i) => n === [62983944, 0, 62983988, 8192, 4][i]);
    const reachedHeapDependency = memoryAbi && reachedTimerArmed && copyReturned && trapped && state[0] === 2 &&
      state[1] === 0x31da0 && state[5] === 0x32696 && calls.length === 12;
    const reachedFileRequest = memoryAbi && reachedTimerArmed && copyReturned && trapped && state[0] === 1 && state[2] === 202 &&
      Number.isInteger(bindingIndex) && bindingIndex >= 0 && bindingIndex < api.ff_probe_binding_count() &&
      api.ff_probe_binding_ordinal(bindingIndex) === 202 && state[5] === 0x337ca && calls.length === 12 && entered.includes(0x33794);
    const reachedVolumeLifecycle = volumeAbi && reachedTimerArmed && copyReturned && trapped && state[0] === 1 && state[2] === 67 &&
      Number.isInteger(bindingIndex) && bindingIndex >= 0 && bindingIndex < api.ff_probe_binding_count() &&
      api.ff_probe_binding_ordinal(bindingIndex) === 67 && state[5] === 0x33d41 && calls.length === 15;
    const reachedNamespaceDependency = namespaceAbi && reachedTimerArmed && copyReturned && trapped && state[0] === 2 &&
      state[1] === 0x1dee6e && state[5] === 0x33c29 && calls.length === 16 && entered.includes(0x33c12);
    const reachedFormatDependency = namespaceAbi && reachedTimerArmed && copyReturned && trapped && state[0] === 2 &&
      state[1] === 0x338de && state[5] === 0x33c45 && calls.length === 16 && entered.includes(0x1e2433) &&
      trace.some(row=>row.kind==='exit' && row.address===0x1dee6e);
    const reachedDirectoryLink = directoryAbi && reachedTimerArmed && copyReturned && trapped && state[0] === 9 &&
      state[1] === 0xc000000d && state[2] === 67 && state[5] === 0x33c05 && calls.length === 22 && entered.includes(0x338de);
    const reachedFileMetadataHelper = regularAbi && reachedTimerArmed && copyReturned && trapped && state[0]===2 &&
      state[1]===0x1df4f0 && state[2]===0 && state[5]===0x33b6e && calls.length===98 && entered.includes(0x3105c);
    const reachedMetadataWrite = metadataAbi && reachedTimerArmed && copyReturned && trapped && state[0]===1 && state[2]===236 &&
      Number.isInteger(bindingIndex) && bindingIndex>=0 && bindingIndex<api.ff_probe_binding_count() && api.ff_probe_binding_ordinal(bindingIndex)===236 &&
      state[5]===0x33b8c && calls.length===98 && trace.some(row=>row.kind==='exit' && row.address===0x1df4f0);
    const reachedWrittenFileHelper = writeAbi && reachedTimerArmed && copyReturned && trapped && state[0]===2 && state[1]===0x337f7 && state[2]===0 &&
      state[5]===0x33bc4 && calls.length===100 && calls[98]?.ordinal===236 && calls[98].status===0 && calls[99]?.ordinal===187 && calls[99].status===0;
    const reachedImageSectionRequest = writeAbi && reachedTimerArmed && copyReturned && trapped && state[0]===1 && state[2]===327 &&
      Number.isInteger(bindingIndex) && bindingIndex>=0 && bindingIndex<api.ff_probe_binding_count() && api.ff_probe_binding_ordinal(bindingIndex)===327 &&
      state[5]===0x34607 && calls.length===103 && calls[100]?.ordinal===289 && calls[101]?.ordinal===190 && calls[102]?.ordinal===211 &&
      calls.slice(98).every(call=>call.status===0);
    const reachedPostImageSetting=sectionAbi && trapped && state[0]===9 && state[1]===10 && state[2]===24 && state[5]===0x30aa9 && calls.length===110 &&
      calls[109]?.ordinal===24 && calls[109].status===0xc00000bb && calls[109].arguments[0]===10;
    const completedImageFlow=sectionAbi && reachedTimerArmed && copyReturned && trapped &&
      (state[0]===2 && state[1]===0x30a91 && state[2]===0 && state[5]===0x33dc5 && calls.length===109 || reachedPostImageSetting) &&
      calls.slice(103,109).every((call,n)=>call.ordinal===[327,236,328,187,187,67][n] && call.status===0);
    const completedMetadataWrite=reachedWrittenFileHelper || reachedImageSectionRequest || completedImageFlow;
    const pastFileMetadataQuery=reachedFileMetadataHelper || reachedMetadataWrite || completedMetadataWrite;
    const reachedRegularFileRequest = pastFileMetadataQuery || !regularAbi && fileLinkAbi && reachedTimerArmed && copyReturned && trapped && state[0] === 9 &&
      state[1] === 0xc00000bb && state[2] === 190 && state[5] === 0x33ac9 && calls.length === 97 &&
      [0x338de,0x34594,0x31127,0x3105c].every(address=>entered.includes(address));
    const expectedTlsData=new Uint8Array(profile.tls.zeroFillBytes);
    // The reached original lookup passes 1168 to its TLS accessor twice.
    // Initial zero-fill is not an invariant after those original writes.
    if(reachedRegularFileRequest && expectedTlsData.length>=8) new DataView(expectedTlsData.buffer).setUint32(4,1168,true);
    const beyondThreadEntry = reachedThreadEntry || reachedTimerRequest || reachedTimerArmed;
    const checks = {
      realThreadCreation: calls.length === (reachedPostImageSetting ? 110 : completedImageFlow ? 109 : reachedImageSectionRequest ? 103 : reachedWrittenFileHelper ? 100 : pastFileMetadataQuery ? 98 : reachedRegularFileRequest ? 97 : reachedDirectoryLink ? 22 : reachedNamespaceDependency || reachedFormatDependency ? 16 : reachedVolumeLifecycle ? 15 : reachedHeapDependency || reachedFileRequest ? 12 : reachedTimerArmed ? 9 : reachedTimerRequest ? 8 : reachedThreadEntry ? 4 : 2) && calls[0].ordinal === 255 && calls[0].status === 0 &&
        calls[0].arguments.every((n, i) => n === expectedCreate[i]) && threadCount === 1,
      closedHandleKeepsRunnableThread: calls[1]?.ordinal === 187 && calls[1].status === 0 &&
        calls[1].arguments[0] === thread.handle && thread.handleOpen === 0 && thread.state === 3,
      loaderReturnedBeforeChild: api.ff_probe_parent_returned() === 1 && loaderExit >= 0 && childEnter > loaderExit,
      childSystemAndSehEntered: entered.includes(REF.systemStart) && entered.includes(0x1e04cc),
      fsAndThreadTlsAgree: view.getUint32(thread.fs + 0x28, true) === thread.object &&
        view.getUint32(thread.object + 0x28, true) === thread.tls && view.getUint32(thread.fs + 4, true) === thread.stackTop &&
        view.getUint32(profile.tls.index, true) === (-tlsBytes / 4) >>> 0 && thread.tlsBytes === tlsBytes,
      originalTlsSelfReference: view.getUint32(thread.tls, true) === thread.tls + 4 && !((thread.tls + 4) & 15) &&
        ram.slice(thread.tls + 4, thread.tls + 4 + profile.tls.zeroFillBytes).every((n,i) => n === expectedTlsData[i]),
      twoArgumentSystemFrame: view.getUint32(thread.initialSp + 4, true) === REF.threadStart &&
        view.getUint32(thread.initialSp + 8, true) === 0 && thread.routine === REF.threadStart && thread.system === REF.systemStart,
      explicitNextUnsupportedStop: (trapped && state[0] === 2 && (state[1] === 0x34198 || reachedThreadEntry)) || reachedTimerRequest || reachedNextHelper || reachedAllocation || reachedHeapDependency || reachedFileRequest || reachedVolumeLifecycle || reachedNamespaceDependency || reachedFormatDependency || reachedDirectoryLink || reachedRegularFileRequest
    };
    if (beyondThreadEntry) {
      checks.originalThreadStartEntered = entered.includes(0x34198) && entered.includes(REF.threadStart);
      checks.notificationCriticalSectionBalanced = calls[2]?.ordinal === 277 && calls[3]?.ordinal === 294 &&
        calls[2].status === 0 && calls[3].status === 0 && calls[2].arguments[0] === 0x2845b0 && calls[3].arguments[0] === 0x2845b0 &&
        view.getInt32(0x2845c0, true) === -1 && view.getUint32(0x2845c4, true) === 0 && view.getUint32(0x2845c8, true) === 0;
    }
    let objectInitialization, timerQueue;
    if (reachedTimerRequest || reachedTimerArmed) {
      const dpc = calls[4], timer = calls[5], setting = calls[6], conversion = calls[7];
      const valid = (address, size) => Number.isInteger(address) && address > 0 && address % 4 === 0 && address <= ram.length - size;
      const da = dpc?.arguments[0], ta = timer?.arguments[0];
      checks.originalCrtAndHelpersReached = [0x33ccc, 0x346a7, 0x309b2, 0x34720, 0x3467a].every(address => entered.includes(address));
      checks.deferredCallbackObjectInitialized = dpc?.ordinal === 107 && dpc.status === 0 && valid(da, 28) &&
        view.getUint16(da, true) === 0x13 && view.getUint8(da + 2) === 0 &&
        view.getUint32(da + 12, true) === dpc.arguments[1] && view.getUint32(da + 16, true) === dpc.arguments[2];
      checks.timerObjectInitialized = timer?.ordinal === 113 && timer.status === 0 && valid(ta, 40) &&
        view.getUint8(ta) === 8 + timer.arguments[1] && view.getUint8(ta + 2) === 10 && view.getUint8(ta + 3) === (reachedTimerArmed ? 1 : 0) &&
        view.getInt32(ta + 4, true) === 0 && view.getUint32(ta + 8, true) === ta + 8 && view.getUint32(ta + 12, true) === ta + 8 &&
        (reachedTimerArmed ? view.getBigUint64(ta + 16, true) > 0n : view.getBigUint64(ta + 16, true) === 0n &&
        view.getUint32(ta + 24, true) === 0 && view.getUint32(ta + 28, true) === 0) &&
        view.getInt32(ta + 36, true) === 0;
      checks.explicitSyntheticSettingRead = setting?.ordinal === 24 && setting.status === 0 && setting.arguments[0] === 17 &&
        setting.arguments[3] === 4 && setting.output.type === 4 && setting.output.requiredBytes === 4 && setting.output.value === consoleSettings.miscFlags;
      checks.exactSuccessStatusConversion = conversion?.ordinal === 301 && conversion.arguments[0] === 0 && conversion.status === 0;
      const arming = reachedTimerArmed ? calls[8] : undefined;
      const due = arming ? BigInt.asIntN(64, (BigInt(arming.arguments[2]) << 32n) | BigInt(arming.arguments[1]))
        : BigInt.asIntN(64, (BigInt(state[8]) << 32n) | BigInt(state[7]));
      checks.originalSixHourTimerRequest = due === -216000000000n && (arming
        ? arming.arguments[0] === ta && arming.arguments[3] === da : state[6] === ta && state[9] === da && state[5] === 0x34694);
      objectInitialization = { dpc: da, deferredRoutine: dpc?.arguments[1], deferredContext: dpc?.arguments[2], timer: ta,
        timerType: timer?.arguments[1], requestedDueTicks: due.toString(), requestedDelaySeconds: Number(-due) / 10000000,
        callbackStateWord: view.getUint32(0x3191a8, true) };
      if (arming) {
        const timerCount = api.ff_probe_timer_count();
        if (timerCount > 32 || api.ff_probe_dpc_count() > 32) throw new Error('Invalid timer queue count');
        timerQueue = { active: timerCount, pendingCallbacks: api.ff_probe_dpc_count(), deliveredCallbacks: api.ff_probe_dpc_delivered(),
          entries: Array.from({ length: timerCount }, (_, i) => ({ timer: api.ff_probe_timer_word(i, 0), dpc: api.ff_probe_timer_word(i, 1),
            absolute: api.ff_probe_timer_word(i, 2), deadlineTicks: ((BigInt(api.ff_probe_timer_word(i, 4) >>> 0) << 32n) |
              BigInt(api.ff_probe_timer_word(i, 3) >>> 0)).toString() })) };
        checks.realClockBackedTimerQueued = clock.describe().samples > 0 && arming.output.previousInsertion === 0 && timerCount === 1 && timerQueue.entries[0].timer === ta &&
          timerQueue.entries[0].dpc === da && timerQueue.entries[0].absolute === 0 &&
          BigInt(timerQueue.entries[0].deadlineTicks) === BigInt(arming.output.sampledInterruptTicks) - due &&
          view.getBigUint64(ta + 16, true).toString() === timerQueue.entries[0].deadlineTicks && view.getUint32(ta + 32, true) === da;
        checks.timerLinksAndNoEarlyCallback = view.getUint32(ta + 24, true) === 0x03e00000 && view.getUint32(ta + 28, true) === 0x03e00000 &&
          view.getUint32(0x03e00000, true) === ta + 24 && timerQueue.pendingCallbacks === 0 && timerQueue.deliveredCallbacks === 0 &&
          view.getUint8(thread.fs + 0x24) === 0 && view.getUint32(thread.fs + 0x58, true) === 0;
        checks.originalArmingWrapperReturned = trace.some(row => row.kind === 'exit' && row.address === 0x3467a) &&
          trace.some(row => row.kind === 'exit' && row.address === 0x346a7);
        checks.originalNextDependencyReached = (reachedNextHelper || reachedAllocation || reachedHeapDependency || reachedFileRequest || reachedVolumeLifecycle || reachedNamespaceDependency || reachedFormatDependency || reachedDirectoryLink || reachedRegularFileRequest) && entered.includes(0x322ef);
      }
    }
    let allocationRequest, copyProof, virtualMemory, heapProof, openFileRequest, createFileRequest, imageFileRequest, sectionLoadRequest, imageSectionLifecycle, metadataWriteRequest, metadataFormat, localVolume, volumeAccounting, symbolicLinkRequest, objectNamespace, originalFormat;
    if (typeof api.ff_probe_format_word==='function' && typeof api.ff_probe_format_byte==='function') {
      const words=Array.from({length:13},(_,i)=>api.ff_probe_format_word(i)>>>0);
      const text=(which,limit)=>{ let value=''; for(let i=0;i<limit;++i) { const byte=api.ff_probe_format_byte(which,i); if(!byte)return value;
        if(byte<32 || byte>126)throw new Error('Invalid bounded format observation'); value+=String.fromCharCode(byte); } return value; };
      originalFormat={entries:words[0],exits:words[1],entryStack:words[2],destination:words[3],formatPointer:words[4],value:words[5],
        returnedBytes:words[6],validMask:words[7],entryFs:words[8],entryExceptionHead:words[9],exitStack:words[10],exitFs:words[11],exitExceptionHead:words[12],
        format:text(0,64),output:text(1,128)};
      checks.originalHexFormatReturnedCorrectly = (reachedFormatDependency || reachedDirectoryLink || reachedRegularFileRequest) && originalFormat.entries===1 && originalFormat.exits===1 &&
        originalFormat.validMask===7 && originalFormat.value===0x4d530002 && originalFormat.format==='%08lx' && originalFormat.returnedBytes===8 &&
        originalFormat.output===originalFormat.value.toString(16).padStart(8,'0');
      checks.originalFormatStackAndSehRestored = originalFormat.entryStack===originalFormat.exitStack && originalFormat.entryFs===thread.fs &&
        originalFormat.exitFs===originalFormat.entryFs && originalFormat.exitExceptionHead===originalFormat.entryExceptionHead;
      checks.originalFormatRoutinesReturned = [0x1dee6e,0x1e2433,0x1e6e50,0x1e23fc,0x1e23d8,0x1e23a5].every(address=>
        trace.some(row=>row.kind==='exit' && row.address===address));
    }
    if (namespaceAbi) {
      const objectCount=api.ff_probe_namespace_count();
      if (objectCount > 32) throw new Error('Invalid object namespace count');
      const name = (index,limit,method) => {
        let value='';
        for (let n=0;n<limit;++n) { const byte=api[method](index,n); if (!byte) return value;
          if (byte < 32 || byte > 126) throw new Error('Invalid namespace diagnostic name'); value+=String.fromCharCode(byte); }
        throw new Error('Unterminated namespace diagnostic name');
      };
      objectNamespace={count:objectCount,objects:Array.from({length:32},(_,i)=>({id:api.ff_probe_namespace_word(i,0),parent:api.ff_probe_namespace_word(i,1),
        kind:api.ff_probe_namespace_word(i,2),references:api.ff_probe_namespace_word(i,3),target:api.ff_probe_namespace_word(i,4),
        deviceKind:api.ff_probe_namespace_word(i,5),mediaReady:api.ff_probe_namespace_word(i,6),name:name(i,64,'ff_probe_namespace_name'),
        targetName:name(i,256,'ff_probe_namespace_target_name'),...(fileLinkAbi?{providerObject:api.ff_probe_namespace_word(i,7)}:{})})).filter(row=>row.id),
        driveResolution:{status:api.ff_probe_namespace_resolution(0),object:api.ff_probe_namespace_resolution(1),
          remainingOffset:api.ff_probe_namespace_resolution(2),remainingBytes:api.ff_probe_namespace_resolution(3)}};
      if (objectNamespace.objects.length !== objectCount) throw new Error('Inconsistent namespace object count');
      if(fileLinkAbi) objectNamespace.fileResolution={status:api.ff_probe_file_resolution(0),object:api.ff_probe_file_resolution(1),
        remainingOffset:api.ff_probe_file_resolution(2),remainingBytes:api.ff_probe_file_resolution(3)};
      const root=objectNamespace.objects.find(row=>row.parent===0 && row.kind===1);
      const dos=objectNamespace.objects.find(row=>row.parent===root?.id && row.name==='??' && row.kind===1);
      const devices=objectNamespace.objects.find(row=>row.parent===root?.id && row.name==='Device' && row.kind===1);
      const disc=objectNamespace.objects.find(row=>row.parent===devices?.id && row.name==='CdRom0' && row.kind===2);
      const link=objectNamespace.objects.find(row=>row.parent===dos?.id && row.name==='D:' && row.kind===3);
      const created=calls[15];
      const expectedObjects=completedImageFlow?9:fileLinkAbi?7:5;
      checks.ownedDriveLinkReferencesDevice = objectCount===expectedObjects && new Set(objectNamespace.objects.map(row=>row.id)).size===expectedObjects &&
        !!disc && !!link && disc.references===2 && link.references===1 && link.target===disc.id && link.targetName==='\\Device\\CdRom0' &&
        created?.output?.link===link.id && created.output.target===disc.id && created.output.targetReferences===disc.references;
      checks.caseInsensitiveDriveResolution = !!disc && objectNamespace.driveResolution.status===0 && objectNamespace.driveResolution.object===disc.id &&
        objectNamespace.driveResolution.remainingOffset===7 && objectNamespace.driveResolution.remainingBytes===0;
      checks.noImplicitDiscContentMounted = !!disc && disc.deviceKind===1 && disc.mediaReady===0 && created?.output?.mediaReady===0 &&
        objectNamespace.objects.filter(row=>row.kind===2 && row.deviceKind===1).every(row=>row.mediaReady===0);
      checks.originalNextHelperEntered = reachedNamespaceDependency || reachedFormatDependency || reachedDirectoryLink || reachedRegularFileRequest;
    }
    const admittedGuestRange = (address, bytes, access) => {
      for (let page = Math.floor(address / 4096); page <= Math.floor((address + bytes - 1) / 4096); ++page) {
        if (!api.ff_probe_vm_page_word(page * 4096, 1) || (api.ff_probe_vm_page_word(page * 4096, 2) & access) !== access) return false;
      }
      return true;
    };
    if (namespaceAbi && trapped && [1,9].includes(state[0]) && state[2] === 190) {
      createFileRequest = inspectCreateFileRequest(ram,state.slice(6,15),admittedGuestRange);
    }
    const capturedCreateRequest=index=>{
      const args=calls[index].arguments, captured=Array.from({length:6},(_,n)=>api.ff_probe_create_request_word(index,n)>>>0);
      if(!captured[4] || captured[4]>=256 || captured[4]>captured[5]) throw new Error('Invalid captured create-request length');
      let name='';
      for(let n=0;n<captured[4];++n) {const byte=api.ff_probe_create_request_name(index,n); if(byte<32 || byte>126) throw new Error('Invalid captured create-request name'); name+=String.fromCharCode(byte);}
      const request={fileHandlePointer:args[0],desiredAccess:args[1],objectAttributesPointer:args[2],rootDirectory:captured[0],attributes:captured[1],
        nameDescriptorPointer:captured[2],nameBuffer:captured[3],nameBytes:captured[4],nameMaximumBytes:captured[5],name,
        ioStatusBlockPointer:args[3],allocationSizePointer:args[4],fileAttributes:args[5],shareAccess:args[6],createDisposition:args[7],openOptions:args[8],
        serviceImplemented:true,observation:'captured-at-create'};
      if(admittedGuestRange(captured[2],8,1)) request.descriptorAtStop={nameBytes:view.getUint16(captured[2],true),
        nameMaximumBytes:view.getUint16(captured[2]+2,true),nameBuffer:view.getUint32(captured[2]+4,true)};
      if(admittedGuestRange(captured[3],captured[4],1)) request.bufferChangedAtStop=Array.from(name).some((byte,i)=>byte.charCodeAt(0)!==ram[captured[3]+i]);
      return request;
    };
    if(regularAbi && calls[96]?.ordinal===190 && !calls[96].status) createFileRequest=capturedCreateRequest(96);
    if(reachedImageSectionRequest || completedImageFlow) imageFileRequest=capturedCreateRequest(101);
    if(reachedImageSectionRequest) {
      sectionLoadRequest=inspectLoadSectionRequest(ram,profile,[state[6]],admittedGuestRange);
      const selected=new Uint8Array(event.data.xbeBytes,sectionLoadRequest.fileOffset,sectionLoadRequest.fileBytes);
      const current=ram.subarray(sectionLoadRequest.virtualAddress,sectionLoadRequest.virtualAddress+sectionLoadRequest.fileBytes);
      const digest=async bytes=>Array.from(new Uint8Array(await crypto.subtle.digest('SHA-256',bytes)),n=>n.toString(16).padStart(2,'0')).join('');
      sectionLoadRequest.sourceBytesSha256=await digest(selected);
      sectionLoadRequest.residentBytesSha256=await digest(current);
      sectionLoadRequest.residentBytesMatchSource=selected.every((byte,i)=>byte===current[i]);
      sectionLoadRequest.loaderPolicy='eager-all-sections-proof';
      sectionLoadRequest.lifetimeImplemented=false;
    }
    if(completedImageFlow) {
      const fields=['descriptor','flags','address','virtualBytes','fileOffset','fileBytes','references','headReferences','tailReferences'];
      const final=Object.fromEntries(fields.map((name,n)=>[name,api.ff_probe_image_word(10,n)>>>0]));
      imageSectionLifecycle={initial:imageInitialState.sections[10],final,policy:imageInitialState.policy,
        pagesAfterUnload:Array.from({length:Math.ceil(((final.address%4096)+final.virtualBytes)/4096)},(_,i)=>{
          const address=(final.address&~4095)+i*4096;
          return {address,committed:api.ff_probe_vm_page_word(address,1),permission:api.ff_probe_vm_page_word(address,2)};
        }),wholeSectionReadableAfterUnload:admittedGuestRange(final.address,final.virtualBytes,1)};
    }
    if (volumeAbi && trapped && [1,9].includes(state[0]) && state[2] === 67) {
      symbolicLinkRequest = { link:inspectCountedAnsiString(ram,state[6],admittedGuestRange),
        device:inspectCountedAnsiString(ram,state[7],admittedGuestRange), serviceImplemented:false };
    }
    if (volumeAbi) {
      const total = (BigInt(api.ff_probe_volume_word(5) >>> 0) << 32n) | BigInt(api.ff_probe_volume_word(4) >>> 0);
      const available = (BigInt(api.ff_probe_volume_word(7) >>> 0) << 32n) | BigInt(api.ff_probe_volume_word(6) >>> 0);
      localVolume = { ready:api.ff_probe_volume_word(0), active:api.ff_probe_volume_word(1), opened:api.ff_probe_volume_word(2), closed:api.ff_probe_volume_word(3),
        totalAllocationUnits:total.toString(), availableAllocationUnits:available.toString(), sectorsPerAllocationUnit:api.ff_probe_volume_word(8),
        bytesPerSector:api.ff_probe_volume_word(9), handles:Array.from({length:16}, (_, i) => ({handle:api.ff_probe_volume_handle_word(i,0),
          access:api.ff_probe_volume_handle_word(i,1), share:api.ff_probe_volume_handle_word(i,2), options:api.ff_probe_volume_handle_word(i,3)})).filter(row => row.handle) };
      if (localVolume.active > 16 || localVolume.opened > 32 || localVolume.closed > localVolume.opened) throw new Error('Invalid local volume counters');
      if (directoryAbi) {
        localVolume.directoryCount=api.ff_probe_volume_word(10); localVolume.directoriesCreated=api.ff_probe_volume_word(11);
        localVolume.directories=Array.from({length:32},(_,i)=>{
          let name='';
          for(let n=0;n<64;++n) { const byte=api.ff_probe_directory_name(i,n); if(!byte)break;
            if(byte<32 || byte>126)throw new Error('Invalid directory diagnostic name'); name+=String.fromCharCode(byte); }
          return {id:api.ff_probe_directory_word(i,0),parent:api.ff_probe_directory_word(i,1),name};
        }).filter(directory=>directory.id);
        localVolume.handles.forEach((handle)=>{
          for(let i=0;i<16;++i) if(api.ff_probe_volume_handle_word(i,0)===handle.handle) handle.directory=api.ff_probe_volume_handle_word(i,4);
        });
      }
      if(fileLinkAbi) {
        localVolume.fileCount=api.ff_probe_volume_word(12);
        localVolume.files=Array.from({length:32},(_,i)=>Object.fromEntries(['id','directory','access','share','options','references','handle',...(regularAbi?['dataFile']:[])].map((field,n)=>[field,api.ff_probe_file_word(i,n)>>>0]))).filter(file=>file.id);
        if(localVolume.fileCount>32 || localVolume.files.length!==localVolume.fileCount) throw new Error('Invalid retained file-object count');
      }
      if(regularAbi) {
        localVolume.dataFileCount=api.ff_probe_volume_word(13); localVolume.dataFilesCreated=api.ff_probe_volume_word(14);
        localVolume.dataFiles=Array.from({length:fileStorageProfile.maxFiles},(_,i)=>{
          let name='',terminated=false;
          for(let n=0;n<64;++n) {const byte=api.ff_probe_regular_name(i,n); if(!byte){terminated=true;break;}
            if(byte<32 || byte>126) throw new Error('Invalid regular-file diagnostic name'); name+=String.fromCharCode(byte);}
          if(!terminated) throw new Error('Unterminated regular-file diagnostic name');
          const file={id:api.ff_probe_regular_word(i,0),parent:api.ff_probe_regular_word(i,1),attributes:api.ff_probe_regular_word(i,2),bytes:api.ff_probe_regular_word(i,3),name};
          for(const [field,low] of [['creationTime',4],['accessTime',6],['writeTime',8],['changeTime',10],['allocatedBytes',12]]) {
            file[field]=((BigInt(api.ff_probe_regular_word(i,low+1)>>>0)<<32n)|BigInt(api.ff_probe_regular_word(i,low)>>>0)).toString();
          }
          if(file.bytes>fileStorageProfile.maxFileBytes) throw new Error('Invalid regular-file diagnostic size');
          if(writeAbi) file.payload=Array.from({length:file.bytes},(_,byte)=>api.ff_probe_regular_byte(i,byte));
          return file;
        }).filter(file=>file.id);
        if(localVolume.dataFileCount>fileStorageProfile.maxFiles || localVolume.dataFiles.length!==localVolume.dataFileCount) throw new Error('Invalid regular-file count');
        localVolume.handles.forEach(handle=>{for(let i=0;i<16;++i) if(api.ff_probe_volume_handle_word(i,0)===handle.handle) {
          handle.dataFile=api.ff_probe_volume_handle_word(i,5); handle.position=api.ff_probe_volume_handle_word(i,6);
        }});
      }
      const open = calls[12], query = calls[13], close = calls[14], handle = open?.output?.handle;
      const entry = trace.find(row => row.address === 0x33794 && row.kind === 'enter');
      const returned = trace.find(row => row.address === 0x33794 && row.kind === 'exit');
      const initialTotal=BigInt(volumeProfile.totalAllocationUnits),initialAvailable=BigInt(volumeProfile.availableAllocationUnits);
      const unitBytes=BigInt(volumeProfile.sectorsPerAllocationUnit)*BigInt(volumeProfile.bytesPerSector);
      const chargedUnits=writeAbi?localVolume.dataFiles.reduce((sum,file)=>sum+BigInt(file.allocatedBytes)/unitBytes,0n):0n;
      volumeAccounting={unitBytes,chargedUnits,initialAvailable,available};
      const expectedSize = [Number(initialTotal & 0xffffffffn), Number(initialTotal >> 32n), Number(initialAvailable & 0xffffffffn),
        Number(initialAvailable >> 32n), volumeProfile.sectorsPerAllocationUnit, volumeProfile.bytesPerSector];
      checks.explicitSyntheticVolumeProfileUsed = localVolume.ready === 1 && localVolume.totalAllocationUnits === volumeProfile.totalAllocationUnits &&
        available+chargedUnits===initialAvailable && localVolume.sectorsPerAllocationUnit === volumeProfile.sectorsPerAllocationUnit &&
        localVolume.bytesPerSector === volumeProfile.bytesPerSector;
      checks.realVolumeRootOpened = open?.ordinal === 202 && open.status === 0 && open.output.ioStatus === 0 && open.output.information === 1 &&
        handle >= 0x10000 && handle < 0x20000 && !(handle & 3) && handle !== thread.handle &&
        open.arguments[1] === 0x100001 && open.arguments[4] === 3 && open.arguments[5] === 0x800021;
      checks.realVolumeSizeQueryCompleted = query?.ordinal === 218 && query.status === 0 && query.arguments[0] === handle &&
        query.arguments[1] === open.arguments[3] && query.arguments[3] === 24 && query.arguments[4] === 3 &&
        query.output.ioStatus === 0 && query.output.information === 24 && query.output.sizeWords.every((n,i) => n === expectedSize[i]);
      checks.volumeRootHandleClosed = close?.ordinal === 187 && close.status === 0 && close.arguments[0] === handle &&
        !localVolume.handles.some(row=>row.handle===handle) && (reachedRegularFileRequest ||
        (localVolume.opened === (reachedDirectoryLink ? 3 : 1) && localVolume.closed === (reachedDirectoryLink ? 3 : 1) && localVolume.active === 0 && localVolume.handles.length === 0));
      checks.originalVolumeInitializerReturned = !!entry?.cpu && !!returned?.cpu && entry.stack === returned.stack &&
        entry.cpu.fs === thread.fs && returned.cpu.fs === entry.cpu.fs && returned.cpu.exceptionHead === entry.cpu.exceptionHead;
      checks.nextOriginalDriveLinkReached = namespaceAbi ? (reachedNamespaceDependency || reachedFormatDependency || reachedDirectoryLink || reachedRegularFileRequest) && calls[15]?.ordinal===67 && calls[15].status===0 :
        reachedVolumeLifecycle && symbolicLinkRequest?.link.name === '\\??\\D:' && symbolicLinkRequest.device.name === '\\Device\\CdRom0' && symbolicLinkRequest.serviceImplemented === false;
    }
    if (directoryAbi) {
      const first=calls[16], firstClose=calls[17], string=calls[18], title=calls[19], titleClose=calls[20], link=calls[21];
      const root=localVolume.directories.find(directory=>directory.parent===0);
      const data=localVolume.directories.find(directory=>directory.parent===root?.id && directory.name==='TDATA');
      const owned=localVolume.directories.find(directory=>directory.parent===data?.id && directory.name===originalFormat?.output);
      const user=localVolume.directories.find(directory=>directory.parent===root?.id && directory.name==='UDATA');
      const userTitle=localVolume.directories.find(directory=>directory.parent===user?.id && directory.name===originalFormat?.output);
      const directoryCreates=fileLinkAbi?[first,title,calls[22],calls[25]]:[first,title];
      const expectedDirectories=fileLinkAbi?5:3;
      checks.explicitFreshDirectoryStateSelected = directoryProfile.synthetic===true && directoryProfile.state==='fresh' && directoryProfile.persistent===false;
      checks.originalDirectoryHelperEntered = reachedRegularFileRequest || reachedDirectoryLink && !trace.some(row=>row.kind==='exit' && row.address===0x338de);
      checks.originalDirectoryCreateArguments = directoryCreates.every((call,i)=>call?.ordinal===190 && call.status===0 &&
        call.arguments[1]===[0x100001,0x120117][i%2] && call.arguments[4]===0 && call.arguments[5]===0x80 &&
        call.arguments[6]===3 && call.arguments[7]===3 && call.arguments[8]===0x4021);
      checks.ownedTitleDirectoriesCreated = localVolume.directoryCount===expectedDirectories && localVolume.directoriesCreated===expectedDirectories-1 &&
        localVolume.directories.length===expectedDirectories && new Set(localVolume.directories.map(directory=>directory.id)).size===expectedDirectories && !!root && !!data && !!owned &&
        (!fileLinkAbi || !!user && !!userTitle) && directoryCreates.every((call,i)=>
          call?.output?.ioStatus===0 && call.output.information===2 && call.output.directories===i+2 && call.output.created===i+1);
      checks.directoryHandlesClosedAndNodesRetained = [firstClose,titleClose].every((call,i)=>call?.ordinal===187 && call.status===0 &&
        call.arguments[0]===[first?.output?.handle,title?.output?.handle][i]) && first?.output?.handle!==title?.output?.handle &&
        !localVolume.handles.some(row=>[first?.output?.handle,title?.output?.handle].includes(row.handle)) && !!data && !!owned &&
        (reachedRegularFileRequest || localVolume.active===0 && localVolume.handles.length===0 && localVolume.opened===3 && localVolume.closed===3);
      checks.countedTitlePathUsesOriginalBuffer = string?.ordinal===289 && string.status===0 && string.output.length===43 && string.output.maximumLength===44 &&
        string.output.buffer===string.arguments[1] && (fileLinkAbi?string.arguments[0]===link?.arguments[1]:
          string.arguments[0]===symbolicLinkRequest?.device.nameDescriptorPointer && string.output.buffer===symbolicLinkRequest?.device.nameBuffer);
      if (!fileLinkAbi) checks.directoryTargetDriveLinkStopped = reachedDirectoryLink && link?.ordinal===67 && link.status===0xc000000d &&
          symbolicLinkRequest?.serviceImplemented===false && symbolicLinkRequest.link.name==='\\??\\T:' &&
          symbolicLinkRequest.device.name==='\\Device\\Harddisk0\\partition1\\TDATA\\4d530002' && objectNamespace.count===5 &&
          !objectNamespace.objects.some(object=>object.name==='T:');
      else {
        const drive=objectNamespace.objects.find(object=>object.name==='T:' && object.kind===3);
        const target=objectNamespace.objects.find(object=>object.id===drive?.target && object.kind===4);
        const file=localVolume.files.find(file=>file.id===target?.providerObject);
        const helper=trace.filter(row=>row.address===0x338de), equalCalls=calls.filter(call=>call.ordinal===279);
        const userHandle=localVolume.handles.find(handle=>handle.directory===userTitle?.id);
        const balanced=(address,expected)=>{
          const rows=trace.filter(row=>row.address===address);
          return rows.length===expected*2 && rows.every((row,i)=>row.kind===(i%2?'exit':'enter') && !!row.cpu &&
            row.cpu.fs===thread.fs && (!(i%2) || row.stack===rows[i-1].stack && row.cpu.exceptionHead===rows[i-1].cpu.exceptionHead));
        };
        checks.directoryTargetDriveLinkCreated = reachedRegularFileRequest && link?.ordinal===67 && link.status===0 && !!drive && !!target &&
          drive.targetName==='\\Device\\Harddisk0\\partition1\\TDATA\\4d530002' && drive.references===1 && target.references===1 &&
          link.output.link===drive.id && link.output.target===target.id && link.output.targetKind===4 && link.output.targetReferences===1;
        checks.directoryFileReferenceSurvivesTemporaryClose = !!file && file.directory===owned?.id && file.references===1 && file.handle===0 &&
          file.access===0 && file.share===7 && file.options===1 && localVolume.fileCount===(completedImageFlow?2:1) && link.output.providerObject===file.id &&
          link.output.directory===file.directory && link.output.fileReferences===1 && link.output.fileHandle===0;
        checks.temporaryTargetHandleClosed = link.output.opened===4 && link.output.closed===4 && link.output.active===0 && link.output.files===1;
        checks.caseInsensitiveFileDriveResolution = !!target && objectNamespace.fileResolution.status===0 && objectNamespace.fileResolution.object===target.id &&
          objectNamespace.fileResolution.remainingOffset===7 && objectNamespace.fileResolution.remainingBytes===0;
        checks.linkIdentitySurvivesReusedOriginalBuffer = calls[24]?.ordinal===289 && calls[24].status===0 &&
          calls[24].arguments[0]===string.arguments[0] && calls[24].arguments[1]===string.arguments[1] &&
          calls[24].output.buffer===string.output.buffer && !!file && file.directory===owned?.id && file.directory!==userTitle?.id;
        checks.firstOriginalDirectoryHelperReturned = helper.length===(completedImageFlow?4:3) && helper[0].kind==='enter' && helper[1].kind==='exit' && helper[2].kind==='enter' &&
          helper[1].cpu?.eax===0 && helper[1].stack===helper[0].stack && helper[1].cpu.fs===thread.fs &&
          helper[1].cpu.exceptionHead===helper[0].cpu.exceptionHead && (completedImageFlow || !trace.some(row=>row.address===0x33c12 && row.kind==='exit'));
        checks.originalLookupHelpersReturnedWithRestoredContext = balanced(0x34594,3) && balanced(0x31127,2) && balanced(0x3105c,reachedImageSectionRequest || completedImageFlow?2:1);
        checks.originalLookupUpdatedActualThreadLocalData = trace.filter(row=>row.address===0x31127 && row.kind==='exit').every(row=>row.cpu?.eax===threadLocalStorage.base) &&
          threadLocalStorage.templateBytes===0 && threadLocalStorage.zeroFillBytes===12 && view.getUint32(threadLocalStorage.base+4,true)===1168 &&
          threadLocalStorage.data.every((byte,i)=>byte===expectedTlsData[i]);
        const lookupReturns=trace.filter(row=>row.address===0x34594 && row.kind==='exit').map(row=>row.cpu?.eax);
        checks.originalLookupFoundActualSectionDescriptor = lookupReturns.length===3 && lookupReturns[0]===0xffffffff && lookupReturns[2]===0xffffffff &&
          lookupReturns[1]===view.getUint32(profile.base+0x120,true)+(profile.sections.length-1)*0x38;
        checks.originalCountedComparisonsCompleted = equalCalls.length===33 && equalCalls.every(call=>call.status===0 && call.arguments[2]===1 && [0,1].includes(call.output.equal)) &&
          equalCalls.filter(call=>call.output.equal===1).length===1 && equalCalls.find(call=>call.output.equal===1)?.output.firstBytes===9 &&
          equalCalls.find(call=>call.output.equal===1)?.output.secondBytes===9;
        if(!completedImageFlow) checks.pendingUserDirectoryHandleIsOwned = !!userTitle && !!userHandle && localVolume.opened===(reachedImageSectionRequest?8:regularAbi?7:6) && localVolume.closed===(writeAbi?6:5) &&
          localVolume.active===(reachedImageSectionRequest || regularAbi && !writeAbi?2:1) && localVolume.handles.length===(reachedImageSectionRequest || regularAbi && !writeAbi?2:1) && userHandle.handle===calls[25]?.output?.handle &&
          userHandle.access===0x120117 && userHandle.share===3 && userHandle.options===0x4021 &&
          calls[23]?.ordinal===187 && calls[23].status===0 && calls[23].arguments[0]===calls[22]?.output?.handle;
        if(!regularAbi) checks.nextRegularFileRequestStoppedBeforeGuestReturn = reachedRegularFileRequest && calls.slice(0,96).every(call=>call.status===0) &&
          calls[96]?.ordinal===190 && calls[96].status===0xc00000bb && createFileRequest?.serviceImplemented===false &&
          createFileRequest.name==='\\Device\\Harddisk0\\partition1\\UDATA\\4d530002\\TitleMeta.xbx' && createFileRequest.rootDirectory===0 &&
          createFileRequest.desiredAccess===0x40100000 && createFileRequest.attributes===0x40 && createFileRequest.allocationSizePointer===0 &&
          createFileRequest.fileAttributes===4 && createFileRequest.shareAccess===1 && createFileRequest.createDisposition===3 && createFileRequest.openOptions===0x22;
        else {
          const created=calls[96],query=calls[97],dataFile=localVolume.dataFiles.find(file=>file.id===created?.output?.dataFile);
          const fileHandle=localVolume.handles.find(handle=>handle.handle===created?.output?.handle);
          const words=query?.output?.words;
          const wide=(low,high)=>((BigInt(high>>>0)<<32n)|BigInt(low>>>0)).toString();
          checks.explicitFreshFileStorageSelected = fileStorageProfile.synthetic===true && fileStorageProfile.state==='fresh' && fileStorageProfile.persistent===false &&
            fileStorageProfile.maxFiles===16 && fileStorageProfile.maxFileBytes===65536;
          checks.originalRegularFileCreated = pastFileMetadataQuery && created?.ordinal===190 && created.status===0 && created.output.ioStatus===0 && created.output.information===2 &&
            created.output.dataFiles===1 && created.output.dataFilesCreated===1 && !!dataFile && dataFile.parent===userTitle?.id && dataFile.attributes===4 && dataFile.name==='TitleMeta.xbx' &&
            createFileRequest?.serviceImplemented===true && createFileRequest.name==='\\Device\\Harddisk0\\partition1\\UDATA\\4d530002\\TitleMeta.xbx' &&
            createFileRequest.desiredAccess===0x40100000 && createFileRequest.shareAccess===1 && createFileRequest.createDisposition===3 && createFileRequest.openOptions===0x22 && createFileRequest.allocationSizePointer===0;
          checks.completedCreateRequestCapturedBeforeBufferReuse = createFileRequest?.observation==='captured-at-create' && createFileRequest.bufferChangedAtStop===true &&
            createFileRequest.nameBytes===57 && createFileRequest.nameMaximumBytes===58 && createFileRequest.descriptorAtStop?.nameBuffer===createFileRequest.nameBuffer &&
            createFileRequest.name.endsWith('\\TitleMeta.xbx');
          if(!writeAbi) {
            checks.regularFileHandleHasSeparateIdentity = !!fileHandle && !!dataFile && fileHandle.directory===0 && fileHandle.dataFile===dataFile.id && fileHandle.position===0 &&
              fileHandle.access===0x40100000 && fileHandle.share===1 && fileHandle.options===0x22 && fileHandle.handle!==userHandle?.handle && file?.dataFile===0;
            checks.newFileHasEmptyOwnedBacking = localVolume.dataFileCount===1 && localVolume.dataFilesCreated===1 && localVolume.dataFiles.length===1 && !!dataFile &&
              dataFile.bytes===0 && dataFile.allocatedBytes==='0';
          } else {
            checks.closedRegularFileHadSeparateIdentity = !!dataFile && !fileHandle && created.output.dataFile===dataFile.id && created.output.handle!==userHandle?.handle && file?.dataFile===0;
            checks.preWriteMetadataQuerySawEmptyBacking = !!words && wide(words[8],words[9])==='0' && wide(words[10],words[11])==='0';
          }
          const queriedWriteTime=writeAbi?wide(words[4],words[5]):dataFile?.writeTime,queriedChangeTime=writeAbi?wide(words[6],words[7]):dataFile?.changeTime;
          checks.fileCreationUsesSampledBrowserClock = !!dataFile && dataFile.creationTime===dataFile.accessTime && dataFile.creationTime===queriedWriteTime &&
            dataFile.creationTime===queriedChangeTime && BigInt(dataFile.creationTime)>=BigInt(fileClockWindow.beforeSystemTicks) &&
            BigInt(dataFile.creationTime)<=BigInt(fileClockWindow.afterSystemTicks);
          checks.originalNetworkOpenInformationRequested = query?.ordinal===211 && query.status===0 && query.arguments.length===5 && query.arguments[0]===created?.output?.handle &&
            query.arguments[1]===created.arguments[3] && query.arguments[3]===56 && query.arguments[4]===34;
          checks.fileInformationMatchesOwnedMetadata = !!dataFile && !!words && words.length===14 && query.output.ioStatus===0 && query.output.information===56 &&
            wide(words[0],words[1])===dataFile.creationTime && wide(words[2],words[3])===dataFile.accessTime && wide(words[4],words[5])===queriedWriteTime &&
            wide(words[6],words[7])===queriedChangeTime && wide(words[8],words[9])===(writeAbi?'0':dataFile.allocatedBytes) && wide(words[10],words[11])===(writeAbi?'0':String(dataFile.bytes)) &&
            words[12]===dataFile.attributes && words[13]===0;
          if(!metadataAbi) checks.nextOriginalMetadataHelperStopped = reachedFileMetadataHelper && calls.every(call=>call.status===0) && !entered.includes(0x1df4f0);
          else {
            const words=Array.from({length:21},(_,n)=>api.ff_probe_metadata_format_word(n)>>>0);
            const captured=(which,size)=>Array.from({length:size},(_,n)=>api.ff_probe_metadata_format_byte(which,n));
            const text=(which,size)=>{const bytes=captured(which,size),units=[];for(let i=0;i<size;i+=2){const unit=bytes[i]|bytes[i+1]<<8;if(!unit)return String.fromCharCode(...units);units.push(unit);}throw new Error('Unterminated captured wide string');};
            metadataFormat={entries:words[0],exits:words[1],entryStack:words[2],destination:words[3],formatPointer:words[4],
              returnedUnits:words[6],validMask:words[7],entryFs:words[8],entryExceptionHead:words[9],exitStack:words[10],exitFs:words[11],exitExceptionHead:words[12],
              maximumUnits:words[13],format:text(0,256),output:text(1,520),arguments:[{kind:'character',value:words[5]},
                {kind:'string',value:text(2,256),pointer:words[15],capturedBytes:words[14]},{kind:'character',value:words[16]},
                {kind:'string',value:text(3,256),pointer:words[17],capturedBytes:words[19]},{kind:'string',value:text(4,256),pointer:words[18],capturedBytes:words[20]}]};
            try {metadataFormat.comparison=compareWideMetadataFormat(metadataFormat);}
            catch(error) {metadataFormat.comparison={matches:false,expectedPayload:[],diagnostic:String(error)};}
            if(reachedMetadataWrite) metadataWriteRequest=inspectWriteFileRequest(ram,state.slice(6,14),admittedGuestRange);
            checks.originalMetadataFormatterReturned = (reachedMetadataWrite || completedMetadataWrite) && metadataFormat.entries===1 && metadataFormat.exits===1 && metadataFormat.validMask===63 &&
              metadataFormat.maximumUnits===142 && metadataFormat.returnedUnits===26 && metadataFormat.output.length===26;
            checks.originalMetadataOutputMatchesCapturedArguments = metadataFormat.comparison.matches && metadataFormat.comparison.expectedUnits===26 &&
              metadataFormat.comparison.expectedBytes===52 && metadataFormat.arguments.filter(arg=>arg.kind==='string').every(arg=>arg.capturedBytes===arg.value.length*2);
            checks.originalMetadataStackAndSehRestored = metadataFormat.entryStack===metadataFormat.exitStack && metadataFormat.entryFs===thread.fs &&
              metadataFormat.exitFs===metadataFormat.entryFs && metadataFormat.exitExceptionHead===metadataFormat.entryExceptionHead;
            checks.originalMetadataDependenciesReturned = [[0x1df4f0,1],[0x1e2c35,1],[0x1e2bd3,5],[0x1e2bae,26],[0x1e2bf8,10],[0x1e6ee5,26]]
              .every(([address,count])=>balanced(address,count));
            if(!writeAbi) {
              checks.actualMetadataBufferRequestedForWrite = reachedMetadataWrite && metadataWriteRequest.serviceImplemented===false &&
                metadataWriteRequest.fileHandle===fileHandle?.handle && metadataWriteRequest.eventHandle===0 && metadataWriteRequest.apcRoutine===0 && metadataWriteRequest.apcContext===0 &&
                metadataWriteRequest.ioStatusBlockPointer===query.arguments[1] && metadataWriteRequest.buffer===metadataFormat.destination && metadataWriteRequest.bytes===metadataFormat.returnedUnits*2 &&
                metadataWriteRequest.byteOffsetPointer===0 && metadataWriteRequest.payload.every((byte,n)=>byte===metadataFormat.comparison.expectedPayload[n]);
              checks.writeStoppedBeforeFileMutation = reachedMetadataWrite && calls.every(call=>call.status===0) && dataFile.bytes===0 && dataFile.allocatedBytes==='0' && fileHandle.position===0 &&
                view.getUint32(metadataWriteRequest.ioStatusBlockPointer,true)===0 && view.getUint32(metadataWriteRequest.ioStatusBlockPointer+4,true)===56;
            } else {
              const {unitBytes,chargedUnits,initialAvailable,available}=volumeAccounting;
              const write=calls[98],close=calls[99],args=write?.arguments??[];
              metadataWriteRequest={fileHandle:args[0],eventHandle:args[1],apcRoutine:args[2],apcContext:args[3],ioStatusBlockPointer:args[4],buffer:args[5],bytes:args[6],byteOffsetPointer:args[7],
                serviceImplemented:true,observation:'captured-call-and-result',result:write?.output};
              checks.originalMetadataWriteCompleted = completedMetadataWrite && write?.ordinal===236 && write.status===0 && write.output.ioStatus===0 && write.output.information===52 &&
                args[0]===created.output.handle && args.slice(1,4).every(value=>value===0) && args[4]===query.arguments[1] && args[5]===metadataFormat.destination && args[6]===52 && args[7]===0 &&
                write.output.initialPosition===0 && write.output.position===52 && write.output.fileBytes===52 && write.output.dataFile===dataFile.id;
              checks.originalWritePayloadRetainedAfterClose = !!dataFile && dataFile.bytes===52 && dataFile.payload.length===52 &&
                dataFile.payload.every((byte,n)=>byte===metadataFormat.comparison.expectedPayload[n]);
              checks.regularFileClosePreservesWrittenNode = close?.ordinal===187 && close.status===0 && close.arguments[0]===created.output.handle && !fileHandle &&
                localVolume.dataFileCount===(reachedImageSectionRequest || completedImageFlow?2:1) && localVolume.dataFilesCreated===(reachedImageSectionRequest || completedImageFlow?2:1) &&
                localVolume.opened===(completedImageFlow?9:reachedImageSectionRequest?8:7) && localVolume.closed===(completedImageFlow?9:6) && localVolume.active===(completedImageFlow?0:reachedImageSectionRequest?2:1);
              checks.syntheticAllocationChargedExactlyOnce = BigInt(dataFile.allocatedBytes)===unitBytes && write.output.allocatedBytes===dataFile.allocatedBytes && chargedUnits===(completedImageFlow?2n:1n) &&
                initialAvailable-available===(completedImageFlow?2n:1n);
              checks.writeTimesUseExistingClockSample = dataFile.writeTime===dataFile.changeTime && dataFile.writeTime===write.output.writeTime &&
                BigInt(dataFile.writeTime)>=BigInt(fileClockWindow.beforeSystemTicks) && BigInt(dataFile.writeTime)<=BigInt(fileClockWindow.afterSystemTicks);
              if(!reachedImageSectionRequest && !completedImageFlow) checks.nextOriginalFileHelperStoppedBeforeEntry = reachedWrittenFileHelper && calls.every(call=>call.status===0) && !entered.includes(0x337f7);
              else {
                const imageCreate=calls[101], imageQuery=calls[102], image=localVolume.dataFiles.find(file=>file.id===imageCreate.output.dataFile);
                const imageHandle=localVolume.handles.find(handle=>handle.handle===imageCreate.output.handle), imageWords=imageQuery.output.words;
                if(!completedImageFlow) checks.originalImageHelperEntered = reachedImageSectionRequest && entered.includes(0x337f7) && entered.includes(0x345fb) &&
                  !trace.some(row=>row.kind==='exit' && [0x337f7,0x345fb].includes(row.address)) && !entered.includes(0x3461c) && !entered.includes(0x34670);
                checks.originalImageFileCreated = imageFileRequest.name==='\\Device\\Harddisk0\\partition1\\UDATA\\4d530002\\TitleImage.xbx' &&
                  imageFileRequest.nameBytes===58 && imageFileRequest.nameMaximumBytes===59 && imageFileRequest.observation==='captured-at-create' &&
                  imageFileRequest.desiredAccess===0x40100000 && imageFileRequest.fileAttributes===4 && imageFileRequest.shareAccess===1 &&
                  imageFileRequest.createDisposition===3 && imageFileRequest.openOptions===0x22 && imageFileRequest.allocationSizePointer===0 &&
                  imageCreate.output.ioStatus===0 && imageCreate.output.information===2 && imageCreate.output.dataFiles===2 && imageCreate.output.dataFilesCreated===2 &&
                  !!image && image.parent===userTitle.id && image.name==='TitleImage.xbx' && image.attributes===4;
                if(!completedImageFlow) checks.pendingImageFileOwnsEmptyBacking = !!image && !!imageHandle && image.id!==dataFile.id && imageHandle.handle!==created.output.handle &&
                  imageHandle.handle!==userHandle.handle && imageHandle.dataFile===image.id && imageHandle.directory===0 && imageHandle.position===0 &&
                  imageHandle.access===0x40100000 && imageHandle.share===1 && imageHandle.options===0x22 && image.bytes===0 && image.payload.length===0 && image.allocatedBytes==='0';
                checks.originalImageMetadataQueryCompleted = !!image && imageQuery.arguments[0]===imageCreate.output.handle && imageQuery.arguments[1]===imageCreate.arguments[3] &&
                  imageQuery.arguments[3]===56 && imageQuery.arguments[4]===34 && imageQuery.output.ioStatus===0 && imageQuery.output.information===56 &&
                  [0,2,4,6].every(i=>wide(imageWords[i],imageWords[i+1])===image.creationTime) && imageWords.slice(8).every((word,i)=>word===[0,0,0,0,4,0][i]) &&
                  image.creationTime===image.accessTime && (completedImageFlow || image.creationTime===image.writeTime && image.creationTime===image.changeTime) &&
                  BigInt(image.creationTime)>=BigInt(fileClockWindow.beforeSystemTicks) && BigInt(image.creationTime)<=BigInt(fileClockWindow.afterSystemTicks);
                if(!completedImageFlow) {
                checks.originalImageSectionDescriptorRequested = sectionLoadRequest.descriptor===state[6] && sectionLoadRequest.index===10 && sectionLoadRequest.name==='$$XTIMAGE' &&
                  sectionLoadRequest.flags===0x38 && !sectionLoadRequest.preload && sectionLoadRequest.virtualAddress===profile.sections[10].address &&
                  sectionLoadRequest.virtualBytes===10240 && sectionLoadRequest.fileBytes===10240;
                checks.sectionLoadStoppedBeforeResultOrReferenceChange = sectionLoadRequest.serviceImplemented===false && sectionLoadRequest.lifetimeImplemented===false &&
                  sectionLoadRequest.observation==='stopped-before-kernel-result' && sectionLoadRequest.referenceCount===0 && sectionLoadRequest.headReferences===0 &&
                  sectionLoadRequest.tailReferences===0 && calls.every(call=>call.ordinal!==327 && call.ordinal!==328 && call.status===0);
                checks.eagerSectionResidencyRecordedAsProofLimitation = sectionLoadRequest.loaderPolicy==='eager-all-sections-proof' &&
                  sectionLoadRequest.guestBytesReadableBeforeService && sectionLoadRequest.residentBytesMatchSource &&
                  sectionLoadRequest.sourceBytesSha256===sectionLoadRequest.residentBytesSha256;
                } else {
                  const load=calls[103],imageWrite=calls[104],unload=calls[105],imageClose=calls[106],directoryClose=calls[107],userLink=calls[108];
                  const selected=profile.sections[10],initial=imageSectionLifecycle.initial,final=imageSectionLifecycle.final;
                  const source=new Uint8Array(event.data.xbeBytes,selected.fileOffset,selected.fileBytes);
                  const digest=async bytes=>Array.from(new Uint8Array(await crypto.subtle.digest('SHA-256',bytes)),n=>n.toString(16).padStart(2,'0')).join('');
                  imageSectionLifecycle.sourceBytesSha256=await digest(source);
                  imageSectionLifecycle.fileBytesSha256=await digest(Uint8Array.from(image.payload));
                  imageSectionLifecycle.fileBytesMatchSource=source.length===image.payload.length && source.every((byte,n)=>byte===image.payload[n]);
                  checks.originalImageHelpersReturned = [0x337f7,0x345fb,0x34670,0x3461c].every(address=>balanced(address,1)) &&
                    trace.some(row=>row.address===0x345fb && row.kind==='exit' && row.cpu.eax===selected.address) &&
                    trace.some(row=>row.address===0x34670 && row.kind==='exit' && row.cpu.eax===selected.fileBytes);
                  checks.imageSectionWasDemandLoaded = imageSectionLifecycle.policy==='local-preload-demand' && imageInitialState.sourceCapacity===16*1024*1024 &&
                    imageInitialState.sections.every(section=>section.references===((section.flags&2)?1:0)) && initial.references===0 && initial.lastPageCommitted===0 &&
                    !initial.bytesMatchSource && load.arguments[0]===initial.descriptor && load.output.beforeReferences===0 && load.output.references===1 &&
                    load.output.headReferences===2 && load.output.tailReferences===1 && load.output.address===selected.address && load.output.fileBytes===selected.fileBytes;
                  checks.originalImageWriteUsesLoadedBytes = imageWrite.arguments.every((value,n)=>value===[imageCreate.output.handle,0,0,0,imageQuery.arguments[1],selected.address,10240,0][n]) &&
                    imageWrite.output.ioStatus===0 && imageWrite.output.information===10240 && imageWrite.output.initialPosition===0 && imageWrite.output.position===10240 &&
                    imageWrite.output.dataFile===image.id && imageWrite.output.fileBytes===image.bytes && image.bytes===10240 &&
                    BigInt(image.allocatedBytes)===unitBytes && imageWrite.output.allocatedBytes===image.allocatedBytes && imageSectionLifecycle.fileBytesMatchSource &&
                    imageSectionLifecycle.sourceBytesSha256===imageSectionLifecycle.fileBytesSha256;
                  checks.imageWriteTimesUseExistingClock = image.writeTime===image.changeTime && image.writeTime===imageWrite.output.writeTime &&
                    BigInt(image.writeTime)>=BigInt(fileClockWindow.beforeSystemTicks) && BigInt(image.writeTime)<=BigInt(fileClockWindow.afterSystemTicks);
                  checks.imageSectionUnloadPreservesSharedPage = unload.arguments[0]===load.arguments[0] && unload.output.beforeReferences===1 && unload.output.references===0 &&
                    unload.output.headReferences===1 && unload.output.tailReferences===0 && final.references===0 && final.headReferences===1 && final.tailReferences===0 &&
                    imageSectionLifecycle.pagesAfterUnload[0].committed===1 && imageSectionLifecycle.pagesAfterUnload.slice(1).every(page=>!page.committed && !page.permission) &&
                    !imageSectionLifecycle.wholeSectionReadableAfterUnload;
                  checks.imageAndDirectoryHandlesClosed = !imageHandle && !userHandle && imageClose.arguments[0]===imageCreate.output.handle && directoryClose.arguments[0]===calls[25].output.handle &&
                    localVolume.opened===9 && localVolume.closed===9 && localVolume.active===0 && localVolume.handles.length===0;
                  const drive=objectNamespace.objects.find(object=>object.name==='U:' && object.kind===3);
                  const target=objectNamespace.objects.find(object=>object.id===drive?.target && object.kind===4);
                  const retained=localVolume.files.find(file=>file.id===target?.providerObject);
                  checks.userDriveRetainsRealDirectoryAfterClose = !!drive && !!target && !!retained && drive.targetName==='\\Device\\Harddisk0\\partition1\\UDATA\\4d530002' &&
                    drive.references===1 && target.references===1 && retained.directory===userTitle.id && retained.references===1 && retained.handle===0 &&
                    userLink.output.link===drive.id && userLink.output.target===target.id && userLink.output.providerObject===retained.id && userLink.output.active===0;
                  checks.originalDataInitializerReturned = balanced(0x338de,2) && balanced(0x33c12,1) &&
                    trace.filter(row=>row.kind==='exit' && [0x338de,0x337f7,0x33c12].includes(row.address)).every(row=>row.cpu.eax===0);
                  if(!reachedPostImageSetting) checks.nextOriginalRoutineStopsBeforeEntry = completedImageFlow && !entered.includes(0x30a91) && calls.every(call=>call.status===0);
                  else {
                    checks.originalPostImageRoutineEntered = entered.includes(0x30a91) && !trace.some(row=>row.address===0x30a91 && row.kind==='exit');
                    checks.nextOriginalSettingStopsBeforeGuestResult = reachedPostImageSetting && calls.slice(0,109).every(call=>call.status===0) &&
                      calls[109].output===undefined && calls[109].arguments.every((value,n)=>value===[10,62983988,62983992,4,0][n]);
                  }
                }
              }
            }
          }
        }
      }
    }
    if (memoryAbi) {
      const regionCount = api.ff_probe_vm_region_count();
      if (regionCount > 128) throw new Error('Invalid virtual-memory region count');
      virtualMemory = { checkedAccesses: api.ff_probe_vm_access_count(), fault: Array.from({length:5}, (_, i) => api.ff_probe_vm_fault_word(i)),
        regions: Array.from({length:regionCount}, (_, i) => ({base:api.ff_probe_vm_region_word(i,0), bytes:api.ff_probe_vm_region_word(i,1),
          kind:api.ff_probe_vm_region_word(i,2), protection:api.ff_probe_vm_region_word(i,3)})) };
      for (const region of virtualMemory.regions) {
        if (region.base % 4096 || region.bytes % 4096 || region.base > ram.length - region.bytes) throw new Error('Invalid virtual-memory region');
        if (region.kind === 2) region.pages = Array.from({length:region.bytes / 4096}, (_, i) => ({
          committed:api.ff_probe_vm_page_word(region.base + i*4096,1), permission:api.ff_probe_vm_page_word(region.base + i*4096,2)}));
      }
      const reserve = calls[9], commit = calls[10], critical = calls[11], base = reserve?.output?.baseAddress;
      const region = virtualMemory.regions.find(row => row.kind === 2 && row.base === base);
      checks.realGuestReservation = reserve?.ordinal === 184 && reserve.status === 0 && reserve.arguments[1] === 0 &&
        reserve.arguments[3] === 0x2000 && reserve.arguments[4] === 4 && base % 65536 === 0 &&
        base >= profile.base + profile.imageBytes && base + 1048576 <= 0x03c00000 && reserve.output.regionBytes === 1048576 &&
        region?.bytes === 1048576 && virtualMemory.regions.filter(row => row.kind === 2).length === 1;
      checks.onlyRequestedPageCommitted = !!region && commit?.ordinal === 184 && commit.status === 0 && commit.arguments[1] === 0 &&
        commit.arguments[3] === 0x1000 && commit.arguments[4] === 4 && commit.output.baseAddress === base && commit.output.regionBytes === 4096 &&
        region.pages[0].committed === 1 && region.pages[0].permission === 3 && region.pages.slice(1).every(p => !p.committed && !p.permission);
      const ca = critical?.arguments[0];
      checks.heapCriticalSectionInitialized = !!region && critical?.ordinal === 291 && critical.status === 0 && ca === base + 0x610 &&
        view.getUint8(ca) === 1 && view.getUint8(ca + 2) === 4 && view.getUint32(ca + 4, true) === 0 &&
        view.getUint32(ca + 8, true) === ca + 8 && view.getUint32(ca + 12, true) === ca + 8 &&
        view.getInt32(ca + 16, true) === -1 && view.getUint32(ca + 20, true) === 0 && view.getUint32(ca + 24, true) === 0;
      checks.checkedAccessesHadNoBootFault = virtualMemory.checkedAccesses > 0 && virtualMemory.fault.every(word => word === 0);
      if (reachedFileRequest || volumeAbi) {
        const heapEnter = trace.find(row => row.address === 0x322ef && row.kind === 'enter');
        const heapExit = trace.find(row => row.address === 0x322ef && row.kind === 'exit');
        heapProof = { entry: heapEnter, returned: heapExit };
        checks.originalHeapConstructorReturned = !!heapExit?.cpu && heapExit.cpu.eax === base;
        checks.originalHeapStackAndSehRestored = !!heapEnter?.cpu && !!heapExit?.cpu && heapEnter.cpu.fs === thread.fs &&
          heapExit.cpu.fs === heapEnter.cpu.fs && heapExit.cpu.exceptionHead === heapEnter.cpu.exceptionHead && heapExit.stack === heapEnter.stack;
        checks.originalHeapSupportReturned = [0x31da0, 0x3139a, 0x31259, 0x3185d, 0x1e0505].every(address =>
          trace.some(row => row.kind === 'exit' && row.address === address));
        if (reachedFileRequest) openFileRequest = inspectOpenFileRequest(ram, state.slice(6, 12), admittedGuestRange);
        if (reachedFileRequest) checks.boundedFileRequestDecoded = true;
      }
    }
    if (reachedAllocation) {
      for (const pointer of [state[6], state[8]]) {
        if (pointer % 4 || pointer > ram.length - 4) throw new Error('Invalid original allocation output pointer');
      }
      allocationRequest = { baseAddressPointer: state[6], baseAddress: view.getUint32(state[6], true), zeroBits: state[7],
        regionSizePointer: state[8], regionBytes: view.getUint32(state[8], true), allocationType: state[9], protection: state[10] };
    }
    if (entered.includes(0x1df9c0)) {
      copyProof = runOriginalCopyProof(api, 0x1df9c0);
      checks.originalCopyReturned = copyReturned;
      checks.originalCopyMemoryAndAbi = copyProof.passed && api.ff_probe_trace_count() === count && api.ff_probe_kernel_count() === callCount;
    }
    if (memoryAbi && checks.realGuestReservation && checks.onlyRequestedPageCommitted) {
      const denied = (address, access) => {
        let trapped = false;
        try { api.ff_probe_vm_access_probe(address, access); } catch (error) {
          if (!(error instanceof WebAssembly.RuntimeError)) throw error;
          trapped = true;
        }
        const fault = Array.from({length:5}, (_, i) => api.ff_probe_vm_fault_word(i));
        return trapped && fault.every((word,i) => word === [address,1,0,access][i]) &&
          new Uint32Array(memory, range(api.ff_probe_state(),64,4),16)[0] === 10;
      };
      virtualMemory.denialDiagnostics = { reservedRead:denied(calls[9].output.baseAddress + 4096,1), readOnlyImageWrite:denied(profile.base,2) };
      checks.reservedAndReadOnlyAccessDenied = Object.values(virtualMemory.denialDiagnostics).every(Boolean);
    }
    if(diskAbi) {
      const fields=['ready','active','opened','closed','allocatedPages','bytesLow','bytesHigh','sectorBytes','cachePartitions'];
      localDisk={...Object.fromEntries(fields.map((field,i)=>[field,api.ff_probe_disk_word(i)>>>0])),synthetic:true,persistent:false,
        cacheRecord:Array.from({length:512},(_,i)=>api.ff_probe_disk_byte(2048+i,0))};
      if(localDisk.allocatedPages>diskProfile.maxBackingPages || localDisk.cacheRecord.some(byte=>byte>255))throw new Error('Invalid disk observation bounds');
      const slot=profile.imports.find(entry=>entry.ordinal===40)?.address;
      if(slot===undefined)throw new Error('Missing declared cache-partition-count import');
      const address=view.getUint32(slot,true);
      if(address>ram.length-4 || !admittedGuestRange(address,4,1) || admittedGuestRange(address,4,2)) {
        throw new Error('Kernel data import must point to readable, read-only guest memory');
      }
      localDisk.kernelData={slot,address,value:view.getUint32(address,true),readable:true,writable:false};
      if(localDisk.kernelData.value!==diskProfile.cachePartitions)throw new Error('Kernel data differs from the explicit disk profile');
      if(partitionAbi) {
        const count=api.ff_probe_disk_word(9);
        if(count!==diskProfile.partitions.length)throw new Error('Incomplete disk-partition backing');
        localDisk.partitions=Array.from({length:count},(_,i)=>({number:api.ff_probe_disk_partition_word(i,0),
          offset:((BigInt(api.ff_probe_disk_partition_word(i,2)>>>0)<<32n)|BigInt(api.ff_probe_disk_partition_word(i,1)>>>0)).toString(),
          bytes:((BigInt(api.ff_probe_disk_partition_word(i,4)>>>0)<<32n)|BigInt(api.ff_probe_disk_partition_word(i,3)>>>0)).toString()}));
        if(JSON.stringify(localDisk.partitions)!==JSON.stringify(diskProfile.partitions))throw new Error('Disk-partition backing differs from the explicit layout');
      }
      if(geometryAbi) {
        localDisk.geometry={ready:api.ff_probe_disk_word(10),cylinders:((BigInt(api.ff_probe_disk_word(12)>>>0)<<32n)|BigInt(api.ff_probe_disk_word(11)>>>0)).toString(),
          tracksPerCylinder:api.ff_probe_disk_word(13)>>>0,sectorsPerTrack:api.ff_probe_disk_word(14)>>>0};
        if(localDisk.geometry.ready!==1 || ['cylinders','tracksPerCylinder','sectorsPerTrack'].some(key=>localDisk.geometry[key]!==diskProfile.geometry[key])) {
          throw new Error('Disk geometry differs from the explicit profile');
        }
      }
      if(informationAbi) {
        localDisk.partitionInformation=diskProfile.partitions.map((part,i)=>{
          const flags=api.ff_probe_disk_information_word(i,3)>>>0;
          if(api.ff_probe_disk_information_word(i,0)!==1)throw new Error('Incomplete partition information');
          return {number:part.number,hiddenSectors:api.ff_probe_disk_information_word(i,1)>>>0,
            partitionType:api.ff_probe_disk_information_word(i,2)>>>0,bootIndicator:!!(flags&1),recognizedPartition:!!(flags&2),rewritePartition:!!(flags&4)};
        });
        if(JSON.stringify(localDisk.partitionInformation)!==JSON.stringify(diskProfile.partitionInformation))throw new Error('Partition information differs from the explicit profile');
      }
      if(partitionAbi && api.ff_probe_disk_transfer_details?.()===1) {
        localDisk.partitionSamples=[];
        for(const part of diskProfile.partitions) {
          const writes=calls.filter(call=>call.ordinal===236 && !call.status && call.output?.partition===part.number && call.output.information>0);
          if(!writes.length)continue;
          const positions=[...new Set(writes.map(call=>call.output.relativeOffset))];
          const selected=[...new Set([...positions.slice(0,3),...positions.slice(-5)])];
          const samples=selected.map(value=>{
            const relative=BigInt(value),physical=BigInt(part.offset)+relative;
            const count=Number(BigInt(part.bytes)-relative<4096n?BigInt(part.bytes)-relative:4096n);
            if(count<1 || count>4096)throw new Error('Partition sample lies outside explicit backing');
            return {relativeOffset:value,physicalOffset:physical.toString(),bytes:Array.from({length:count},(_,i)=>{
              const address=physical+BigInt(i);return api.ff_probe_disk_byte(Number(address&0xffffffffn),Number(address>>32n));
            })};
          });
          localDisk.partitionSamples.push({number:part.number,writes:writes.length,transferredBytes:writes.reduce((total,call)=>total+call.output.information,0),
            observation:'post-stop-current-backing',samples});
        }
      }
      if(dismountAbi) {
        if(api.ff_probe_disk_word(15)!==diskProfile.rawVolumes.length)throw new Error('Incomplete RAW-volume attachments');
        localDisk.volumes=diskProfile.rawVolumes.map((partition,i)=>{
          const value={partition:api.ff_probe_disk_volume_word(i,0)>>>0,state:api.ff_probe_disk_volume_word(i,1)>>>0,
            references:api.ff_probe_disk_volume_word(i,2)>>>0,dismounts:api.ff_probe_disk_volume_word(i,3)>>>0};
          if(value.partition!==partition || ![1,2,3].includes(value.state) || value.references>8 || value.dismounts>1 ||
             value.state===3 && value.references)throw new Error('Invalid RAW-volume attachment lifetime');
          return value;
        });
      }
    }
    if(initializerAbi && state[2]===202 && (state[0]===1 || state[0]===9)) {
      openFileRequest={...inspectOpenFileRequest(ram,state.slice(6,12),admittedGuestRange),
        observation:'stopped-before-kernel-result'};
    }
    let localFatx;
    if(fatxAbi) {
      const mountCount=api.ff_probe_fatx_word(1),fileCount=api.ff_probe_fatx_word(2);
      if(api.ff_probe_fatx_word(0)!==1 || mountCount>4 || fileCount>16)throw new Error('Invalid FATX provider state');
      const fieldNames=['id','partition','device','serial','sectorsPerCluster','clusterBytes','entryBytes','rootCluster',
        'clusterCount','freeClusters','rootClusters','fileObjects'];
      const mounts=Array.from({length:mountCount},(_,i)=>{
        const value=Object.fromEntries(fieldNames.map((key,n)=>[key,api.ff_probe_fatx_mount_word(i,n)>>>0]));
        for(const [key,low] of [['offset',12],['bytes',14],['fatBytes',16],['dataOffset',18],['revision',20]]) {
          value[key]=((BigInt(api.ff_probe_fatx_mount_word(i,low+1)>>>0)<<32n)|BigInt(api.ff_probe_fatx_mount_word(i,low)>>>0)).toString();
        }
        const device=objectNamespace.objects.find(object=>object.id===value.device);
        if(!device || device.kind!==2 || device.deviceKind!==2 || device.mediaReady!==1 || device.providerObject!==value.id ||
           ![2,4].includes(value.entryBytes) || !value.rootClusters || value.rootClusters>64 || value.freeClusters>value.clusterCount) {
          throw new Error('Invalid mounted FATX identity/layout');
        }
        return value;
      });
      const fileFields=['id','mount','handle','access','share','options','references'];
      const files=Array.from({length:16},(_,i)=>Object.fromEntries(fileFields.map((key,n)=>[key,api.ff_probe_fatx_file_word(i,n)>>>0]))).filter(file=>file.id);
      if(files.length!==fileCount || files.some(file=>!file.references || !mounts.some(mount=>mount.id===file.mount)))throw new Error('Invalid FATX file lifetime');
      localFatx={ready:true,mountCount,fileCount,active:api.ff_probe_fatx_word(3),opened:api.ff_probe_fatx_word(4),closed:api.ff_probe_fatx_word(5),mounts,files,
        persistent:false,scope:'backed FATX root/size; general file operations unsupported'};
    }
    let memoryAccounting;
    if(statisticsAbi) {
      if(api.ff_probe_memory_word(0)!==1 || api.ff_probe_memory_word(1)!==memoryProfile.physicalPages ||
         api.ff_probe_memory_word(2)!==memoryProfile.baselinePoolPages)throw new Error('Memory accounting differs from its explicit profile');
      const status=api.ff_probe_memory_word(3)>>>0;
      memoryAccounting={status,profile:memoryProfile,scope:'modeled guest pages; no retail-console equivalence',
        ...(status===0?{words:Array.from({length:9},(_,n)=>api.ff_probe_memory_word(n+4)>>>0)}:{})};
    }
    let deviceRegisters;
    if(api.ff_probe_abi()===24) {
      deviceRegisters=createDeviceReport(api, mmioProfile);
    }
    const claim = calls.find(call => call.ordinal === 168 && call.status === 0 && call.output?.baseAddress);
    let guestWritePages;
    if (typeof api.ff_probe_write_map_size === 'function') {
      const words = api.ff_probe_write_map_size() >>> 0, pages = [];
      for (let wordIndex = 0; wordIndex < words; ++wordIndex) {
        const bits = api.ff_probe_write_map_word(wordIndex) >>> 0;
        for (let bit = 0; bit < 32; ++bit) if (bits & (1 << bit)) pages.push(wordIndex * 32 + bit);
      }
      guestWritePages = { pageBytes: 4096, count: api.ff_probe_write_map_count() >>> 0, pages };
    }
    const submissionProbe = deviceRegisters ? createSubmissionProbe({
      status: state[0], entries: deviceRegisters.entries, view, ramBytes: ram.byteLength,
      allocation: claim ? { base: claim.output.baseAddress, bytes: claim.arguments?.[0] ?? 256 } : undefined,
      aliases: deviceRegisters.aliases,
      events: deviceRegisters.log,
      writtenPages: guestWritePages?.pages ?? [],
    }) : { available: false, reason: 'no observed device aperture' };
    let memoryStatisticsRequest;
    if(fatxAbi && state[0]===1 && state[2]===181) {
      const pointer=state[6];
      if(!pointer || (pointer&3) || !admittedGuestRange(pointer,4,1))throw new Error('Invalid memory-statistics request pointer');
      const suppliedLength=view.getUint32(pointer,true);
      memoryStatisticsRequest={pointer,suppliedLength,expectedBytes:36,
        outputWritable:admittedGuestRange(pointer,36,2),serviceImplemented:false,observation:'stopped-before-kernel-result'};
    }
    let originalStartup;
    if(initializerAbi) {
      // Read the immutable selected file, not the reused live stack or a guessed catalog.
      const start=0x281750,end=0x2818a0;
      const section=profile.sections.find(s=>s.address<=start && end<=s.address+s.fileBytes);
      if(!section)throw new Error('Original initializer table is outside selected file backing');
      const table=new DataView(event.data.xbeBytes,section.fileOffset+start-section.address,end-start);
      const callbacks=Array.from({length:(end-start)/4},(_,i)=>table.getUint32(i*4,true)).filter(Boolean);
      const callbackFrame=trace.find(row=>row.kind==='enter' && row.address===callbacks[0]);
      let completed=0,cursor=-1;
      if(callbackFrame)for(const address of callbacks) {
        const entry=trace.findIndex((row,i)=>i>cursor && row.kind==='enter' && row.address===address && row.stack===callbackFrame.stack);
        if(entry<0)break;
        const returned=trace.findIndex((row,i)=>i>entry && row.kind==='exit' && row.address===address && row.stack===callbackFrame.stack);
        if(returned<0 || trace[entry].cpu?.fs!==thread.fs || trace[returned].cpu?.fs!==thread.fs ||
           trace[returned].cpu?.exceptionHead!==trace[entry].cpu?.exceptionHead)break;
        cursor=returned;completed++;
      }
      originalStartup={tableStart:start,tableEndExclusive:end,totalInitializers:callbacks.length,completedInitializers:completed,
        nextInitializer:callbacks[completed],distinctOriginalFunctions:new Set(entered).size,
        completedKernelOperations:calls.filter(call=>call.status===0).length,
        stopReason:state[0]===2 || state[0]===3?'untranslated original routine':state[0]===1 || state[0]===9 && state[2]?'unsupported Xbox service':
          state[0]===4 || state[0]===5?'diagnostic capacity reached':state[0]===10?'guest memory access stopped':state[0]===12?'device poll limit reached':state[0]===13?'pushbuffer consumption required':state[0]===14?'unmodeled port input':state[0]===15?'device register capacity reached':'unsupported execution boundary',
        limitation:'Observed startup progress; no playable screen or original-game fidelity acceptance.'};
    }
    postBoundary({ kind: 'thread-startup-boundary', phase: initializerAbi ? 'original-initializer-bringup' : reachedPostImageSetting ? 'original-image-written-settings-stop' : completedImageFlow ? 'original-image-written-initializer-returned' : reachedImageSectionRequest ? 'original-title-image-section-stop' : reachedWrittenFileHelper ? 'original-metadata-write-next-stop' : reachedMetadataWrite ? 'original-metadata-format-write-stop' : reachedFileMetadataHelper ? 'original-file-create-query-next-stop' : reachedRegularFileRequest ? 'original-file-link-regular-file-stop' : reachedDirectoryLink ? 'original-title-directory-link-stop' : reachedFormatDependency ? 'original-format-next-stop' : reachedNamespaceDependency ? 'original-drive-helper-stop' : reachedVolumeLifecycle ? 'original-volume-drive-stop' : reachedFileRequest ? 'original-heap-file-stop' : reachedHeapDependency ? 'heap-first-page-next-stop' : reachedAllocation ? 'original-copy-allocation-stop' : reachedTimerArmed ? 'timer-armed-next-stop' : reachedTimerRequest ? 'crt-timer-request' : reachedThreadEntry ? 'original-thread-entry' : 'original-child-tls',
      verified: !initializerAbi && Object.values(checks).every(Boolean), checks:initializerAbi?undefined:checks,originalStartup,
      contextWords: typeof api.ff_probe_context_word === 'function'
        ? Array.from({length:32},(_,i)=>api.ff_probe_context_word(i) >>> 0) : undefined,
      sourceSha256, moduleSha256, consoleSettings, floatingPointState, volumeProfile, directoryProfile, fileStorageProfile, diskProfile, localDisk, localFatx, fileClockWindow, localVolume, objectNamespace, originalFormat, biosUsed: false, hostImports: timerAbi ? 1 : 0, clock: timerAbi ? clock.describe() : undefined, originalFunctionsReached: entered,
      trace, traceCapacity, kernelCapacity, calls, memoryAccounting, deviceRegisters, submissionProbe, guestWritePages, memoryStatisticsRequest, kernelOperations:completedImageFlow?{completed:109,pending:reachedPostImageSetting?1:0}:undefined, pfifoProfile, experiments: describeMmioExperiments(mmioProfile), avQueryWord: mmioProfile.avQueryWord, pcrtc: typeof api.ff_probe_pcrtc_word === 'function' && api.ff_probe_pcrtc_word(0) === 1 ? { pending: api.ff_probe_pcrtc_word(1) >>> 0, enabled: api.ff_probe_pcrtc_word(2) >>> 0, events: api.ff_probe_pcrtc_word(3) >>> 0 } : undefined, pfifoResetState: typeof api.ff_probe_pfifo_status_word === 'function' && api.ff_probe_pfifo_status_word(0) === 1 ? { scope: 'seam', cache1: api.ff_probe_pfifo_status_word(1) >>> 0, runout: api.ff_probe_pfifo_status_word(2) >>> 0, dmaPush: api.ff_probe_pfifo_status_word(3) >>> 0, consumerUpdates: api.ff_probe_pfifo_status_word(4) >>> 0, resets: api.ff_probe_pfifo_status_word(5) >>> 0 } : undefined, pfifoConsumerState: typeof api.ff_probe_pfifo_consumer_word === 'function' && api.ff_probe_pfifo_consumer_word(0) === 1 ? { scope: 'prototype', classification: api.ff_probe_pfifo_consumer_word(2) >>> 0, get: api.ff_probe_pfifo_consumer_word(3) >>> 0, put: api.ff_probe_pfifo_consumer_word(4) >>> 0, base: api.ff_probe_pfifo_consumer_word(5) >>> 0, end: api.ff_probe_pfifo_consumer_word(6) >>> 0, wordsConsumed: api.ff_probe_pfifo_consumer_word(7) >>> 0, consumerUpdates: api.ff_probe_pfifo_consumer_word(8) >>> 0, lastHeader: api.ff_probe_pfifo_consumer_word(9) >>> 0, steps: api.ff_probe_pfifo_consumer_word(10) >>> 0, words: api.ff_probe_pfifo_consumer_word(11) >>> 0, budgetHit: api.ff_probe_pfifo_consumer_word(12) >>> 0, vmChecks: api.ff_probe_pfifo_consumer_word(13) >>> 0, releaseAttempted: api.ff_probe_pfifo_consumer_word(14) >>> 0, releaseFired: api.ff_probe_pfifo_consumer_word(15) >>> 0, releaseReason: api.ff_probe_pfifo_consumer_word(16) >>> 0, fenceVa: api.ff_probe_pfifo_consumer_word(17) >>> 0, releaseBefore: api.ff_probe_pfifo_consumer_word(18) >>> 0, releaseWritten: api.ff_probe_pfifo_consumer_word(19) >>> 0, releaseSeq: api.ff_probe_pfifo_consumer_word(20) >>> 0, drainedWords: api.ff_probe_pfifo_consumer_word(21) >>> 0 } : undefined, vblankWaitState: typeof api.ff_probe_vblank_word === 'function' && api.ff_probe_vblank_word(0) ? { calls: api.ff_probe_vblank_word(0) >>> 0, type: api.ff_probe_vblank_word(1) >>> 0, signal: api.ff_probe_vblank_word(2) >>> 0, decision: api.ff_probe_vblank_word(3) >>> 0, declared: api.ff_probe_vblank_word(4) >>> 0 } : undefined, displayModeState: typeof api.ff_probe_display_mode_word === 'function' && api.ff_probe_display_mode_word(9) === 1 ? { accepted: api.ff_probe_display_mode_word(0) >>> 0, addr: api.ff_probe_display_mode_word(1) >>> 0, step: api.ff_probe_display_mode_word(2) >>> 0, mode: api.ff_probe_display_mode_word(3) >>> 0, format: api.ff_probe_display_mode_word(4) >>> 0, pitch: api.ff_probe_display_mode_word(5) >>> 0, fb: api.ff_probe_display_mode_word(6) >>> 0, window: api.ff_probe_display_mode_word(7) >>> 0, decision: api.ff_probe_display_mode_word(8) >>> 0 } : undefined, avUnknownOptionState: typeof api.ff_probe_av_unknown_word === 'function' && api.ff_probe_av_unknown_word(0) === 1 ? { calls: api.ff_probe_av_unknown_word(1) >>> 0, handle: api.ff_probe_av_unknown_word(2) >>> 0, option: api.ff_probe_av_unknown_word(3) >>> 0, param: api.ff_probe_av_unknown_word(4) >>> 0, result: api.ff_probe_av_unknown_word(5) >>> 0, decision: api.ff_probe_av_unknown_word(6) >>> 0, written: api.ff_probe_av_unknown_word(7) >>> 0, eaxBefore: api.ff_probe_av_unknown_word(8) >>> 0, espBefore: api.ff_probe_av_unknown_word(9) >>> 0, eaxAfter: api.ff_probe_av_unknown_word(10) >>> 0, espAfter: api.ff_probe_av_unknown_word(11) >>> 0 } : undefined, avSavedDataState: typeof api.ff_probe_av_saved_word === 'function' && api.ff_probe_av_saved_word(0) === 1 ? { calls: api.ff_probe_av_saved_word(1) >>> 0, value: api.ff_probe_av_saved_word(2) >>> 0, decision: api.ff_probe_av_saved_word(3) >>> 0, eaxBefore: api.ff_probe_av_saved_word(4) >>> 0, espBefore: api.ff_probe_av_saved_word(5) >>> 0, eaxAfter: api.ff_probe_av_saved_word(6) >>> 0, espAfter: api.ff_probe_av_saved_word(7) >>> 0, priorSetter: api.ff_probe_av_saved_word(8) >>> 0 } : undefined, avNullResultState: typeof api.ff_probe_av_null_result_word === 'function' && api.ff_probe_av_null_result_word(0) === 1 ? { calls: api.ff_probe_av_null_result_word(1) >>> 0, handle: api.ff_probe_av_null_result_word(2) >>> 0, option: api.ff_probe_av_null_result_word(3) >>> 0, param: api.ff_probe_av_null_result_word(4) >>> 0, result: api.ff_probe_av_null_result_word(5) >>> 0, decision: api.ff_probe_av_null_result_word(6) >>> 0, writeBefore: api.ff_probe_av_null_result_word(7) >>> 0, writeAfter: api.ff_probe_av_null_result_word(8) >>> 0, checksumBefore: api.ff_probe_av_null_result_word(9) >>> 0, checksumAfter: api.ff_probe_av_null_result_word(10) >>> 0, eaxBefore: api.ff_probe_av_null_result_word(11) >>> 0, espBefore: api.ff_probe_av_null_result_word(12) >>> 0, eaxAfter: api.ff_probe_av_null_result_word(13) >>> 0, espAfter: api.ff_probe_av_null_result_word(14) >>> 0 } : undefined, mmFreeContiguousState: typeof api.ff_probe_mm_free_contiguous_word === 'function' && api.ff_probe_mm_free_contiguous_word(0) === 1 ? { calls: api.ff_probe_mm_free_contiguous_word(1) >>> 0, address: api.ff_probe_mm_free_contiguous_word(2) >>> 0, decision: api.ff_probe_mm_free_contiguous_word(3) >>> 0, eaxBefore: api.ff_probe_mm_free_contiguous_word(4) >>> 0, espBefore: api.ff_probe_mm_free_contiguous_word(5) >>> 0, eaxAfter: api.ff_probe_mm_free_contiguous_word(6) >>> 0, espAfter: api.ff_probe_mm_free_contiguous_word(7) >>> 0, writeBefore: api.ff_probe_mm_free_contiguous_word(8) >>> 0, writeAfter: api.ff_probe_mm_free_contiguous_word(9) >>> 0 } : undefined, guestWatchState: typeof api.ff_probe_guest_watch_word === 'function' && api.ff_probe_guest_watch_word(0) === 1 ? { address: api.ff_probe_guest_watch_word(1) >>> 0, site: api.ff_probe_guest_watch_word(2) >>> 0, count: api.ff_probe_guest_watch_word(3) >>> 0, readSite: api.ff_probe_guest_watch_word(4) >>> 0, readCount: api.ff_probe_guest_watch_word(5) >>> 0 } : undefined, abiViolationState: typeof api.ff_probe_abi_violation_word === 'function' ? { count: api.ff_probe_abi_violation_word(0) >>> 0, first: api.ff_probe_abi_violation_word(1) >>> 0, last: api.ff_probe_abi_violation_word(2) >>> 0, esiCount: api.ff_probe_abi_violation_word(3) >>> 0, esiFirst: api.ff_probe_abi_violation_word(4) >>> 0, esiLast: api.ff_probe_abi_violation_word(5) >>> 0 } : undefined, siteWatchState: typeof api.ff_probe_site_watch_word === 'function' && api.ff_probe_site_watch_word(0) === 1 ? { entryAddr: api.ff_probe_site_watch_word(1) >>> 0, entryEcx: api.ff_probe_site_watch_word(2) >>> 0, entryReturn: api.ff_probe_site_watch_word(3) >>> 0, fieldAddr: api.ff_probe_site_watch_word(4) >>> 0, fieldCount: api.ff_probe_site_watch_word(5) >>> 0, fieldEsi: api.ff_probe_site_watch_word(6) >>> 0, spanEsi: [api.ff_probe_site_watch_word(7) >>> 0, api.ff_probe_site_watch_word(8) >>> 0, api.ff_probe_site_watch_word(9) >>> 0, api.ff_probe_site_watch_word(10) >>> 0], entryFs: api.ff_probe_site_watch_word(11) >>> 0, fieldFs: api.ff_probe_site_watch_word(12) >>> 0, esiChanges: typeof api.ff_probe_esi_change_count === 'function' ? (() => { const n = api.ff_probe_esi_change_count(); const c = Math.min(8, n); const out = []; for (let k = 0; k < c; k++) { const slot = (n - c + k) & 7; out.push([api.ff_probe_esi_change_word(slot*3) >>> 0, api.ff_probe_esi_change_word(slot*3+1) >>> 0, api.ff_probe_esi_change_word(slot*3+2) >>> 0]); } return out; })() : [], entries: typeof api.ff_probe_site_entry_count === 'function' ? Array.from({length: Math.min(8, api.ff_probe_site_entry_count())}, (_, i) => [api.ff_probe_site_entry_word(i*3) >>> 0, api.ff_probe_site_entry_word(i*3+1) >>> 0, api.ff_probe_site_entry_word(i*3+2) >>> 0]) : [] } : undefined, timerCheck: typeof api.ff_probe_timer_check === 'function' ? api.ff_probe_timer_check() : undefined, dpcHead: typeof api.ff_probe_dpc_head === 'function' ? api.ff_probe_dpc_head() : undefined, dpcHeadValue: typeof api.ff_probe_dpc_head_value === 'function' ? api.ff_probe_dpc_head_value() : undefined, timerHead: typeof api.ff_probe_timer_head === 'function' ? api.ff_probe_timer_head() : undefined, timerHeadValue: typeof api.ff_probe_timer_head_value === 'function' ? api.ff_probe_timer_head_value() : undefined, dpcObjectWords: typeof api.ff_probe_dpc_head_value === 'function' && api.ff_probe_guest_word && api.ff_probe_dpc_head_value() ? Array.from({length:8},(_,i)=>api.ff_probe_guest_word(api.ff_probe_dpc_head_value()+i*4)) : undefined,
      threads, threadLocalStorage, objectInitialization, timerQueue, allocationRequest, copyProof, virtualMemory, heapProof, openFileRequest, createFileRequest, symbolicLinkRequest, tlsSelfReference: view.getUint32(thread.tls, true),
      metadataFormat,metadataWriteRequest,imageFileRequest,sectionLoadRequest,imageInitialState,imageSectionLifecycle,stop: { status: state[0], target: state[1], ordinal: state[2], stack: state[3], returnAddress: state[5], stackArguments: state.slice(6, 16), service: state[0] === 1 ? describeKernelService(state[2]) : undefined },
      gameBooted: false, memoryBytes: memory.byteLength,
      limitation: 'Boot-thread dispatch only; no preemption, waits, complete kernel, renderer or playable game. Unsupported original calls stop.' });
  } catch (error) { self.postMessage({ kind: 'error', message: String(error) }); }
};
