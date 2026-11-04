import { readXbeProfile } from '../xbe.mjs';
import { inspectProofWasm } from './wasm-proof.mjs';

// Curated build/ABI observations; no original instructions or file content.
export const BUILD_REFERENCE = {
  sha256: '45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9',
  entry: 0x30f7f, threadWrapper: 0x3439f, tlsSize: 0x3191b4,
  threadStart: 0x30f10, systemStart: 0x34307, kernelReturn: 0x343de, ordinal: 255
};
const RAM_BYTES = 64 * 1024 * 1024;
const required = ['abi','ram','ram_bytes','stack_top','source_digest','entry','state','trace','trace_count',
  'binding_count','binding_slot','binding_ordinal','reset','bind_kernels','run'];
const sha256 = async buffer => [...new Uint8Array(await crypto.subtle.digest('SHA-256', buffer))]
  .map(n => n.toString(16).padStart(2, '0')).join('');

export function parseLocalConsoleSettings(buffer) {
  if (!(buffer instanceof ArrayBuffer) || buffer.byteLength > 8192) throw new Error('Invalid local settings profile size');
  const profile = JSON.parse(new TextDecoder('utf-8', { fatal: true }).decode(buffer));
  if (profile?.kind !== 'local-console-settings' || profile.synthetic !== true ||
      !Number.isInteger(profile.miscFlags) || profile.miscFlags < 0 || profile.miscFlags > 0xffffffff) {
    throw new Error('Use an explicit synthetic misc-flags test profile');
  }
  return { kind: profile.kind, synthetic: true, miscFlags: profile.miscFlags };
}

export function parseLocalNvConsoleSettings(buffer) {
  const settings=parseLocalConsoleSettings(buffer);
  const parentalControlGames=JSON.parse(new TextDecoder('utf-8',{fatal:true}).decode(buffer)).parentalControlGames;
  if(!Number.isInteger(parentalControlGames) || parentalControlGames<0 || parentalControlGames>0xffffffff) {
    throw new Error('Select an explicit synthetic parental-control-games DWORD; no original console default is assumed');
  }
  return {...settings,parentalControlGames};
}

export function proofTraceCapacity(api) {
  const capacity=typeof api.ff_probe_trace_capacity==='function' ? api.ff_probe_trace_capacity() : 64;
  if (![64,128,1024,4096,8192].includes(capacity) ||
      (capacity===4096 || capacity===8192) && ![16,17,18,19,20,21,22,23,24].includes(api.ff_probe_abi?.()))
    throw new Error('Unsupported bounded trace capacity');
  return capacity;
}
export function proofKernelCapacity(api) {
  const capacity=typeof api.ff_probe_kernel_capacity==='function'?api.ff_probe_kernel_capacity():32;
  if(![32,128,256,512].includes(capacity) || capacity===256 && api.ff_probe_abi?.()!==15 || capacity===512 && ![16,17,18,19,20,21,22,23,24].includes(api.ff_probe_abi?.())) throw new Error('Unsupported bounded kernel-record capacity');
  return capacity;
}

export function parseLocalVolumeProfile(buffer) {
  parseLocalConsoleSettings(buffer);
  const volume = JSON.parse(new TextDecoder('utf-8', { fatal: true }).decode(buffer)).volume;
  const count = value => typeof value === 'string' && /^(0|[1-9][0-9]{0,18})$/.test(value) ? BigInt(value) : -1n;
  const total = count(volume?.totalAllocationUnits), available = count(volume?.availableAllocationUnits);
  const sectors = volume?.sectorsPerAllocationUnit, bytes = volume?.bytesPerSector;
  if (volume?.kind !== 'local-volume-profile' || volume.synthetic !== true || volume.device !== 'harddisk0-partition1' ||
      total <= 0n || total > 0x7fffffffffffffffn || available < 0n || available > total ||
      !Number.isInteger(sectors) || sectors < 1 || sectors > 128 || (sectors & (sectors - 1)) ||
      ![512,1024,2048,4096].includes(bytes) || total * BigInt(sectors) * BigInt(bytes) > 0xffffffffffffffffn) {
    throw new Error('Select explicit synthetic volume capacity and geometry; no original disk default is assumed');
  }
  return { kind: volume.kind, synthetic: true, device: volume.device, totalAllocationUnits: total.toString(),
    availableAllocationUnits: available.toString(), sectorsPerAllocationUnit: sectors, bytesPerSector: bytes };
}

export function parseLocalDirectoryProfile(buffer) {
  parseLocalVolumeProfile(buffer);
  const state=JSON.parse(new TextDecoder('utf-8',{fatal:true}).decode(buffer)).volume.directoryState;
  if(state!=='fresh') throw new Error('Select an explicit fresh synthetic directory state; no original save state is assumed');
  return {synthetic:true,state:'fresh',persistent:false};
}
export function parseLocalFileProfile(buffer) {
  parseLocalDirectoryProfile(buffer);
  const state=JSON.parse(new TextDecoder('utf-8',{fatal:true}).decode(buffer)).volume.fileState;
  if(state!=='fresh') throw new Error('Select an explicit fresh synthetic file state; no original saved files are assumed');
  return {synthetic:true,state:'fresh',persistent:false,maxFiles:16,maxFileBytes:65536};
}

export function parseLocalDiskProfile(buffer) {
  parseLocalFileProfile(buffer);
  const disk=JSON.parse(new TextDecoder('utf-8',{fatal:true}).decode(buffer)).disk;
  const bytes=typeof disk?.bytes==='string' && /^(0|[1-9][0-9]{0,18})$/.test(disk.bytes)?BigInt(disk.bytes):-1n;
  if(disk?.kind!=='local-disk-profile' || disk.synthetic!==true || disk.device!=='harddisk0' || disk.state!=='fresh' ||
     bytes<=0n || bytes>0x7fffffffffffffffn || ![512,4096].includes(disk.sectorBytes) || bytes%BigInt(disk.sectorBytes) ||
     !Number.isInteger(disk.cachePartitions) || disk.cachePartitions<1 || disk.cachePartitions>41) {
    throw new Error('Select an explicit fresh synthetic disk profile; no original disk contents or cache count is assumed');
  }
  return {kind:disk.kind,synthetic:true,device:disk.device,state:disk.state,bytes:bytes.toString(),
    sectorBytes:disk.sectorBytes,cachePartitions:disk.cachePartitions,persistent:false,maxBackingPages:128};
}

export function parseLocalDiskPartitions(buffer) {
  const profile=parseLocalDiskProfile(buffer);
  const partitions=JSON.parse(new TextDecoder('utf-8',{fatal:true}).decode(buffer)).disk.partitions;
  const count=value=>typeof value==='string' && /^(0|[1-9][0-9]{0,18})$/.test(value)?BigInt(value):-1n;
  if(!Array.isArray(partitions) || partitions.length!==profile.cachePartitions)throw new Error('Select an explicit complete cache-partition layout');
  const result=[];
  for(const part of partitions) {
    const offset=count(part?.offset),bytes=count(part?.bytes),sector=BigInt(profile.sectorBytes);
    if(!Number.isInteger(part?.number) || part.number<3 || part.number>=profile.cachePartitions+3 || offset<0n || bytes<=0n ||
       offset+bytes>BigInt(profile.bytes) || offset%sector || bytes%sector || result.some(old=>old.number===part.number ||
         offset<BigInt(old.offset)+BigInt(old.bytes) && BigInt(old.offset)<offset+bytes)) {
      throw new Error('Select nonoverlapping, sector-aligned cache partitions inside the explicit disk');
    }
    result.push({number:part.number,offset:offset.toString(),bytes:bytes.toString()});
  }
  return result;
}

export function parseLocalDiskGeometry(buffer) {
  const disk=parseLocalDiskProfile(buffer);
  const geometry=JSON.parse(new TextDecoder('utf-8',{fatal:true}).decode(buffer)).disk.geometry;
  const cylinders=typeof geometry?.cylinders==='string' && /^(0|[1-9][0-9]{0,18})$/.test(geometry.cylinders)?BigInt(geometry.cylinders):-1n;
  const tracks=geometry?.tracksPerCylinder,sectors=geometry?.sectorsPerTrack;
  if(cylinders<=0n || !Number.isInteger(tracks) || tracks<1 || tracks>0xffffffff ||
     !Number.isInteger(sectors) || sectors<1 || sectors>0xffffffff ||
     cylinders*BigInt(tracks)*BigInt(sectors)*BigInt(disk.sectorBytes)!==BigInt(disk.bytes)) {
    throw new Error('Select explicit disk geometry consistent with the configured capacity');
  }
  return {cylinders:cylinders.toString(),tracksPerCylinder:tracks,sectorsPerTrack:sectors};
}

export function parseLocalPartitionInformation(buffer) {
  const disk=parseLocalDiskProfile(buffer),parts=parseLocalDiskPartitions(buffer);
  const raw=JSON.parse(new TextDecoder('utf-8',{fatal:true}).decode(buffer)).disk.partitions;
  return parts.map((part,i)=>{
    const info=raw[i]?.information;
    if(!Number.isInteger(info?.hiddenSectors) || info.hiddenSectors<0 || info.hiddenSectors>0xffffffff ||
       BigInt(info.hiddenSectors)!==BigInt(part.offset)/BigInt(disk.sectorBytes) ||
       !Number.isInteger(info.partitionType) || info.partitionType<0 || info.partitionType>255 ||
       ['bootIndicator','recognizedPartition','rewritePartition'].some(key=>typeof info[key]!=='boolean')) {
      throw new Error('Select explicit partition information consistent with the configured layout');
    }
    return {number:part.number,hiddenSectors:info.hiddenSectors,partitionType:info.partitionType,
      bootIndicator:info.bootIndicator,recognizedPartition:info.recognizedPartition,rewritePartition:info.rewritePartition};
  });
}

export function parseLocalRawDiskVolumes(buffer) {
  const parts=parseLocalDiskPartitions(buffer);
  const volumes=JSON.parse(new TextDecoder('utf-8',{fatal:true}).decode(buffer)).disk.rawVolumes;
  if(!Array.isArray(volumes) || volumes.length!==parts.length || new Set(volumes).size!==volumes.length ||
     volumes.some(number=>!Number.isInteger(number) || !parts.some(part=>part.number===number))) {
    throw new Error('Select explicit fresh RAW attachments for the configured cache partitions');
  }
  return [...volumes];
}

export function parseLocalMmioProfile(buffer) {
  parseLocalConsoleSettings(buffer);
  const device = JSON.parse(new TextDecoder('utf-8',{fatal:true}).decode(buffer)).device;
  /* A misspelled experimental flag would silently leave the run on a different
   * model than the profile intends, so unknown device keys fail closed. */
  const deviceKeys = ['kind', 'synthetic', 'window', 'registers', 'zeroRanges',
    'writeClearing', 'pmcZero', 'pcrtcVblanks', 'engineWait'];
  if (device && typeof device === 'object' &&
      Object.keys(device).some(key => !deviceKeys.includes(key))) {
    throw new Error('Unknown synthetic register profile key');
  }
  const entries = device?.registers, window = device?.window;
  if (device?.kind !== 'synthetic-register-storage' || device.synthetic !== true ||
      !Number.isInteger(window?.base) || !Number.isInteger(window?.size) || (window.base & 3) ||
      window.size < 4096 || window.size > 0x1000000 || (window.size & 3) ||
      !Array.isArray(entries) || entries.length < 2 || entries.length > 64) {
    throw new Error('Select an explicit synthetic register window with 2-64 supplied entries');
  }
  const seen = new Set();
  for (const entry of entries) {
    if (!Number.isInteger(entry.address) || (entry.address & 3) || !Number.isInteger(entry.initial) ||
        entry.initial < 0 || entry.initial > 0xffffffff || entry.address < window.base ||
        entry.address - window.base >= window.size) {
      throw new Error('Register entries require an aligned in-window address and explicit 32-bit value');
    }
    seen.add(entry.address);
  }
  if (seen.size !== entries.length || !seen.has(0xfd001804) || !seen.has(0xfd600140)) {
    throw new Error('Register storage must declare distinct reached device addresses');
  }
  const rawRanges = device.zeroRanges ?? [];
  if (!Array.isArray(rawRanges) || rawRanges.length > 8)
    throw new Error('Use at most eight explicit synthetic zero ranges');
  const zeroRanges = [];
  for (const range of rawRanges) {
    if (!Number.isInteger(range?.address) || (range.address & 3) || !Number.isInteger(range?.bytes) ||
        range.bytes < 4 || (range.bytes & 3) || range.address < window.base ||
        range.bytes > window.size - (range.address - window.base) ||
        zeroRanges.some(old => old.address === range.address)) {
      throw new Error('Zero ranges require an aligned in-window base and bounded byte length');
    }
    zeroRanges.push({ address: range.address, bytes: range.bytes });
  }
  const writeClearing = device.writeClearing === true;
  /* Experimental only: serving NV_PMC_INTR_0 as a zero status register is a
   * diagnostic, not evidence that the device delivers interrupts. The reached
   * code never acknowledges this register (its only store is zero), so nothing
   * here may be treated as a completion or acknowledge rule. */
  const pmcZero = device.pmcZero === true;
  const pcrtcVblanks = Number.isInteger(device.pcrtcVblanks) && device.pcrtcVblanks >= 0 &&
    device.pcrtcVblanks <= 64 ? device.pcrtcVblanks : 0;
  const engineWait = device.engineWait === true;
  return { kind: device.kind, synthetic: true, window: { base: window.base, size: window.size },
    registers: entries.map(e => ({ address: e.address, initial: e.initial })), zeroRanges, writeClearing, pmcZero,
    pcrtcVblanks, engineWait };
}

/* Every experimental device input the selected profile turns on, so a report can
 * state what model produced it instead of leaving it implicit. */
export function describeMmioExperiments(profile) {
  if (!profile) return [];
  const active = [];
  if (profile.writeClearing) active.push('writeClearing');
  if (profile.pmcZero) active.push('pmcZero');
  if (profile.pcrtcVblanks) active.push(`pcrtcVblanks:${profile.pcrtcVblanks}`);
  if (profile.engineWait) active.push('engineWait');
  return active;
}
export function parseLocalMemoryProfile(buffer) {
  parseLocalConsoleSettings(buffer);
  const memory=JSON.parse(new TextDecoder('utf-8',{fatal:true}).decode(buffer)).memory;
  if(memory?.kind!=='local-memory-profile' || memory.synthetic!==true ||
     memory.fixedArena!=='committed-runtime-pool' || !Number.isInteger(memory.physicalPages) ||
     memory.physicalPages<1 || memory.physicalPages>16384 || !Number.isInteger(memory.baselinePoolPages) ||
     memory.baselinePoolPages<0 || memory.baselinePoolPages>memory.physicalPages) {
    throw new Error('Select explicit modeled physical capacity, pool baseline and fixed-arena ownership');
  }
  return {kind:memory.kind,synthetic:true,physicalPages:memory.physicalPages,baselinePoolPages:memory.baselinePoolPages,
    fixedArena:memory.fixedArena};
}

export function parseLocalPfifoProfile(buffer) {
  if (!buffer) return undefined;
  const root = JSON.parse(new TextDecoder('utf-8', { fatal: true }).decode(buffer));
  const pfifo = root.pfifo;
  if (!pfifo) return undefined;
  const u32 = value => Number.isInteger(value) && value >= 0 && value <= 0xffffffff ? value >>> 0 : null;
  const base = u32(pfifo.base), end = u32(pfifo.end), put = u32(pfifo.put);
  if (pfifo.kind !== 'local-pfifo-profile' || pfifo.synthetic !== true ||
      base === null || end === null || put === null || (base & 3) || (end & 3) || (put & 3) ||
      end <= base || put !== base) {
    throw new Error('Select an explicit synthetic empty PFIFO ring profile');
  }
  return { kind: pfifo.kind, synthetic: true, base, end, put };
}

export async function loadLocalTitleProof({ moduleBytes, xbeBytes } = {}, abi, extraExports = [], { clock } = {}) {
  if (typeof WorkerGlobalScope === 'undefined' || !(self instanceof WorkerGlobalScope)) {
    throw new Error('Original title proof execution requires a browser worker');
  }
  if (!(xbeBytes instanceof ArrayBuffer) || xbeBytes.byteLength > 16 * 1024 * 1024) throw new Error('Invalid local executable size');
  const limits = inspectProofWasm(moduleBytes, { allowClockImport: !!clock }), sourceSha256 = await sha256(xbeBytes);
  if (sourceSha256 !== BUILD_REFERENCE.sha256) throw new Error('This proof requires the verified local game build');
  const profile = readXbeProfile(xbeBytes);
  if (profile.entry !== BUILD_REFERENCE.entry || !profile.tls || profile.nonKernelImportsAddress) throw new Error('Unsupported startup profile');
  const compiled = await WebAssembly.compile(moduleBytes);
  const imports = WebAssembly.Module.imports(compiled);
  if (imports.length !== (limits.clockImport ? 1 : 0) || imports.some(item =>
    item.module !== 'env' || item.name !== 'ff_host_clock' || item.kind !== 'function')) throw new Error('Host imports are not admitted');
  let admittedMemory;
  const api = (await WebAssembly.instantiate(compiled, limits.clockImport ? {
    env: { ff_host_clock: address => clock.write(admittedMemory?.buffer, address) }
  } : {})).exports;
  if (!(api.memory instanceof WebAssembly.Memory) || [...required, ...extraExports].some(name => typeof api['ff_probe_' + name] !== 'function')) {
    throw new Error('Unexpected local proof ABI');
  }
  admittedMemory = api.memory;
  if (typeof api._initialize === 'function') api._initialize();
  if (!(Array.isArray(abi) ? abi : [abi]).includes(api.ff_probe_abi()) || api.ff_probe_entry() !== profile.entry || api.ff_probe_ram_bytes() !== RAM_BYTES) {
    throw new Error('Module ABI/build does not match this proof');
  }
  const proofAbi = api.ff_probe_abi();
  if ([4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24].includes(proofAbi) !== !!limits.clockImport) throw new Error('Only clock proof ABIs use the required clock import');
  if ([5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24].includes(proofAbi) && ['vm_image','vm_map_image','vm_region_count','vm_region_word','vm_page_word','vm_access_count','vm_fault_word','vm_access_probe'].some(name =>
    typeof api['ff_probe_' + name] !== 'function')) throw new Error('Missing checked virtual-memory ABI');
  if(proofAbi>=13 && ['image_source','image_source_capacity','image_init','image_count','image_word'].some(name=>
    typeof api['ff_probe_'+name]!=='function')) throw new Error('Missing local image-section ABI');
  if([17,18,19,20,21,22,23,24].includes(proofAbi) && ['disk_profile','disk_word','disk_byte'].some(name=>typeof api['ff_probe_'+name]!=='function')) {
    throw new Error('Missing local disk-device ABI');
  }
  if([18,19,20,21,22,23,24].includes(proofAbi) && ['disk_partition_profile','disk_partition_word'].some(name=>typeof api['ff_probe_'+name]!=='function')) {
    throw new Error('Missing local disk-partition ABI');
  }
  if([19,20,21,22,23,24].includes(proofAbi) && ['disk_geometry_profile','disk_control_word'].some(name=>typeof api['ff_probe_'+name]!=='function')) {
    throw new Error('Missing local disk-control ABI');
  }
  if([20,21,22,23,24].includes(proofAbi) && ['disk_partition_information','disk_information_word'].some(name=>typeof api['ff_probe_'+name]!=='function')) {
    throw new Error('Missing explicit disk-partition information ABI');
  }
  if([21,22,23,24].includes(proofAbi) && ['disk_raw_volume','disk_volume_word'].some(name=>typeof api['ff_probe_'+name]!=='function')) {
    throw new Error('Missing explicit RAW-volume attachment ABI');
  }
  if([22,23,24].includes(proofAbi) && ['fatx_word','fatx_mount_word','fatx_file_word'].some(name=>typeof api['ff_probe_'+name]!=='function')) {
    throw new Error('Missing backed FATX-root provider ABI');
  }
  if([23,24].includes(proofAbi) && ['memory_profile','memory_word'].some(name=>typeof api['ff_probe_'+name]!=='function')) {
    throw new Error('Missing modeled memory-statistics ABI');
  }
  if(proofAbi===24 && ['mmio_profile','mmio_word'].some(name=>typeof api['ff_probe_'+name]!=='function')) throw new Error('Missing synthetic register-storage ABI');
  if(proofAbi===24 && ['mmio_profile','mmio_word'].some(name=>typeof api['ff_probe_'+name]!=='function')) throw new Error('Missing synthetic register-storage ABI');
  const memory = api.memory.buffer;
  if (memory.byteLength !== limits.minimumPages * 65536) throw new Error('Module memory differs from admission limits');
  const range = (address, size, alignment = 1) => {
    address >>>= 0;
    if (address % alignment || size > memory.byteLength || address > memory.byteLength - size) throw new Error('Proof ABI range outside memory');
    return address;
  };
  const declaredDigest = new TextDecoder().decode(new Uint8Array(memory, range(api.ff_probe_source_digest(), 64), 64));
  if (declaredDigest !== sourceSha256) throw new Error('Local module was prepared for a different executable');
  const ramAddress = range(api.ff_probe_ram(), RAM_BYTES, 4);
  const ram = new Uint8Array(memory, ramAddress, RAM_BYTES), view = new DataView(memory, ramAddress, RAM_BYTES);
  ram.fill(0); api.ff_probe_reset();
  if (profile.base + profile.headerBytes > RAM_BYTES) throw new Error('Headers exceed the proof RAM');
  ram.set(new Uint8Array(xbeBytes, 0, profile.headerBytes), profile.base);
  for (const section of proofAbi>=13?[]:profile.sections) {
    if (section.address + section.virtualBytes > RAM_BYTES) throw new Error('Section exceeds proof RAM');
    ram.set(new Uint8Array(xbeBytes, section.fileOffset, section.fileBytes), section.address);
  }
  let imageInitialState;
  if ([5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24].includes(proofAbi)) {
    if (api.ff_probe_vm_image(profile.base, profile.imageBytes)) {
      throw new Error('Cannot admit image and private runtime ranges');
    }
    if(proofAbi>=13) {
      const capacity=api.ff_probe_image_source_capacity();
      if(capacity!==16*1024*1024) throw new Error('Unexpected bounded local image source capacity');
      const sourceAddress=range(api.ff_probe_image_source(),capacity,16);
      if(sourceAddress<ramAddress+RAM_BYTES && ramAddress<sourceAddress+capacity) throw new Error('Local image source overlaps guest RAM');
      new Uint8Array(memory,sourceAddress,xbeBytes.byteLength).set(new Uint8Array(xbeBytes));
      if(api.ff_probe_image_init(xbeBytes.byteLength) || api.ff_probe_image_count()!==profile.sections.length) throw new Error('Cannot admit local image section ownership');
      const sectionFields=['descriptor','flags','address','virtualBytes','fileOffset','fileBytes','references','headReferences','tailReferences'];
      imageInitialState={policy:'local-preload-demand',sourceCapacity:capacity,
        sections:profile.sections.map((section,i)=>({
          ...Object.fromEntries(sectionFields.map((name,n)=>[name,api.ff_probe_image_word(i,n)>>>0])),
          firstPageCommitted:api.ff_probe_vm_page_word(section.address,1),
          lastPageCommitted:api.ff_probe_vm_page_word(section.address+section.virtualBytes-1,1),
          bytesMatchSource:new Uint8Array(xbeBytes,section.fileOffset,section.fileBytes).every((byte,n)=>byte===ram[section.address+n])}))};
    } else {
      if(api.ff_probe_vm_map_image(profile.base,profile.headerBytes,2)) throw new Error('Cannot admit image header pages');
      for (const section of profile.sections) {
        const protection = section.executable ? section.flags & 1 ? 0x40 : 0x20 : section.flags & 1 ? 4 : 2;
        if (api.ff_probe_vm_map_image(section.address, section.virtualBytes, protection)) throw new Error('Cannot admit section pages');
      }
    }
  }
  const bindingCount = api.ff_probe_binding_count();
  if (!Number.isInteger(bindingCount) || bindingCount < 1 || bindingCount > 64) throw new Error('Invalid proof binding count');
  for (let i = 0; i < bindingCount; i++) {
    const slot = api.ff_probe_binding_slot(i), ordinal = api.ff_probe_binding_ordinal(i);
    const word=Number.isInteger(slot) && slot>=0 && slot<=RAM_BYTES-4?view.getUint32(slot,true):undefined;
    if (!profile.imports.some(entry => entry.address === slot && entry.ordinal === ordinal) ||
        slot % 4 || slot > RAM_BYTES - 4 || word !== (0x80000000 | ordinal) >>> 0) {
      throw new Error(`Kernel-call binding does not match the original import declaration: slot ${slot}, ordinal ${ordinal}, loaded word ${word}`);
    }
  }
  api.ff_probe_bind_kernels();
  const stackTop = api.ff_probe_stack_top();
  if (stackTop !== RAM_BYTES - 4096) throw new Error('Unsupported proof stack');
  return { api, memory, range, view, ram, profile, imageInitialState, sourceSha256, moduleSha256: await sha256(moduleBytes), stackTop };
}
