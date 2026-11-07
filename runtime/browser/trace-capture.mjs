export const TRACE_CAPTURE_CAPACITY = 8192;
const U32 = value => Number.isInteger(value) && value >= -0x80000000 && value <= 0xffffffff;
const dword = value => { if (!U32(value)) throw new Error('Trace capture API returned a non-DWORD'); return value >>> 0; };
export function readTraceCpuWord(api, row, field) { return dword(api.ff_probe_trace_cpu(row,field)); }

export function parseTraceCaptureStart(value) {
  if (value === undefined) return undefined;
  if (value !== 0 && value !== 8192) throw new Error('Select trace capture start 0 or 8192');
  return value;
}

export function traceCaptureExportsAvailable(api) {
  return ['trace_cpu','trace_capture_profile','trace_capture_word','trace_capture_routine','trace_capture_digest_word']
    .every(name => typeof api[`ff_probe_${name}`] === 'function');
}

export function selectTraceCapture(api, requestedStart) {
  if (!traceCaptureExportsAvailable(api)) throw new Error('Missing selected original trace capture ABI');
  if(api.ff_probe_trace_capture_profile(requestedStart)!==0) throw new Error('Selected original trace capture profile was refused');
}

export function readTraceCapture(api, requestedStart, trace) {
  if (!traceCaptureExportsAvailable(api)) throw new Error('Missing selected original trace capture ABI');
  const signedWord = name => index => dword(api[`ff_probe_trace_capture_${name}`](index));
  const word = signedWord('word');
  if(word(0)!==1) throw new Error('Selected original trace capture profile is not ready');
  const [version, selected, capturedStart, totalCount, capturedCount, totalEntries, prefixEntries,
    routinesCount, prefixRoutines, capacity, limit] = Array.from({length:11},(_,i)=>word(i+1));
  if(version!==1 || selected!==requestedStart || capacity!==TRACE_CAPTURE_CAPACITY || limit!==requestedStart+capacity ||
     capturedStart!==(requestedStart===8192 && totalCount>8192 ? 8192 : 0) || totalCount>limit || capturedCount!==totalCount-capturedStart ||
     capturedCount!==trace.length || routinesCount>512 || prefixRoutines>routinesCount || prefixRoutines>512 ||
     (capturedStart===0 ? prefixEntries!==0 || prefixRoutines!==0 : prefixEntries>totalEntries))
    throw new Error('Selected original trace capture counters are inconsistent');
  const routines=new Array(routinesCount);
  for(let i=0;i<routinesCount;i++) routines[i]=dword(api.ff_probe_trace_capture_routine(i));
  if(routines.some(value=>value===0)||new Set(routines).size!==routines.length) throw new Error('Trace capture routine list is malformed');
  const digest = which => {
    const bytes=new Uint8Array(32), view=new DataView(bytes.buffer);
    for(let i=0;i<8;i++) view.setUint32(i*4,dword(api.ff_probe_trace_capture_digest_word(which,i)),true);
    return [...bytes].map(n=>n.toString(16).padStart(2,'0')).join('');
  };
  const complete=capturedStart===0, sha256=digest(0), prefixSha256=complete?null:digest(1);
  if(complete ? prefixSha256!==null : !/^[0-9a-f]{64}$/.test(prefixSha256)) throw new Error('Trace capture digest metadata is invalid');
  const metadata={kind:'original-trace-window',version,requestedStart:selected,capturedStart,capturedCount,totalCount,totalEntries,
    prefixEntries,capacity,limit,routines,prefixRoutines,sha256,prefixSha256,complete};
  validateTraceCapture(metadata,trace);
  return metadata;
}

export function validateTraceCapture(meta, trace) {
  const keys=['kind','version','requestedStart','capturedStart','capturedCount','totalCount','totalEntries','prefixEntries','capacity','limit','routines','prefixRoutines','sha256','prefixSha256','complete'];
  if(!meta || Object.keys(meta).length!==keys.length || keys.some(k=>!Object.hasOwn(meta,k)) || meta.kind!=='original-trace-window' || meta.version!==1 ||
     ![0,8192].includes(meta.requestedStart) || ![0,8192].includes(meta.capturedStart) || meta.capacity!==8192 || meta.limit!==meta.requestedStart+8192 ||
     typeof meta.complete!=='boolean' || meta.complete!==(meta.capturedStart===0) || !Array.isArray(trace) || trace.length!==meta.capturedCount ||
     ![meta.totalCount,meta.capturedCount,meta.totalEntries,meta.prefixEntries,meta.prefixRoutines].every(value=>Number.isSafeInteger(value)&&value>=0) ||
     meta.totalCount>meta.limit || meta.capturedCount!==meta.totalCount-meta.capturedStart || meta.capturedCount>8192 ||
     (meta.requestedStart===0 && meta.capturedStart!==0) ||
     meta.prefixEntries>meta.totalEntries || meta.prefixEntries>8192 || !Array.isArray(meta.routines) || meta.routines.length>512 || meta.prefixRoutines>meta.routines.length ||
     meta.prefixRoutines>meta.prefixEntries || meta.totalEntries<meta.routines.length || meta.totalEntries>meta.totalCount ||
     ((meta.totalEntries===0)!==(meta.routines.length===0)) || ((meta.prefixEntries===0)!==(meta.prefixRoutines===0)) ||
     meta.routines.length-meta.prefixRoutines>meta.totalEntries-meta.prefixEntries ||
     meta.routines.some(x=>!Number.isInteger(x)||x<=0||x>0xffffffff) || new Set(meta.routines).size!==meta.routines.length ||
     typeof meta.sha256!=='string' || !/^[0-9a-f]{64}$/.test(meta.sha256) ||
     (meta.complete ? meta.prefixSha256!==null || meta.prefixEntries!==0 || meta.prefixRoutines!==0 :
       typeof meta.prefixSha256!=='string' || !/^[0-9a-f]{64}$/.test(meta.prefixSha256)))
    throw new Error('Malformed original trace window metadata');
  const enters=trace.filter(x=>x.kind==='enter');
  if(trace.some(row=>!row||!['enter','exit'].includes(row.kind)||!Number.isInteger(row.address)||row.address<=0||row.address>0xffffffff||
      !Number.isInteger(row.stack)||row.stack<0||row.stack>0xffffffff||!row.cpu||['eax','fs','exceptionHead'].some(k=>!Number.isInteger(row.cpu[k])||row.cpu[k]<0||row.cpu[k]>0xffffffff)))
    throw new Error('Original trace rows require six valid DWORD fields');
  const capturedEntries=enters.length;
  if(meta.totalEntries!==meta.prefixEntries+capturedEntries) throw new Error('Original trace entry totals do not match captured rows');
  const seen=new Set(meta.routines.slice(0,meta.prefixRoutines));
  for(const row of enters) if(!seen.has(row.address)){ if(seen.size>=512) throw new Error('Original routine list exceeds 512'); seen.add(row.address); }
  if(JSON.stringify([...seen])!==JSON.stringify(meta.routines)) throw new Error('Original trace routine order disagrees with captured entries');
  if(meta.complete && JSON.stringify([...new Set(enters.map(row=>row.address))])!==JSON.stringify(meta.routines))
    throw new Error('Complete original trace routine list disagrees with entry sequence');
  if(!meta.complete && meta.totalCount<=8192) throw new Error('Trace omission metadata requires more than 8192 total events');
  return meta;
}
