const HEADER_INDEX = 0xffffffff;
const CAPACITY = 4;
const PROFILE_KEYS = ['kind', 'synthetic', 'capacity'];
const STATE_KEYS = [...PROFILE_KEYS, 'objectCount', 'handleCount', 'status', 'objects', 'handles'];
const STATUSES = new Set([0, 0x40000000, 0xc000000d, 0xc00000bb, 0xc0000005, 0xc000009a, 0xc0000024]);
// Replacement model scope: ASCII counted names with case-folded lookup, Win32 root -4,
// OPENIF semantics, type-1 synchronization events initially nonsignaled. No wait/signal/reset/close.

export function parseEventProfile(value) {
  if (value === undefined) return undefined;
  if (!value || typeof value !== 'object' || Array.isArray(value) || Reflect.ownKeys(value).length !== 3 ||
      PROFILE_KEYS.some(key => !Object.hasOwn(value, key)) || value.kind !== 'synthetic-win32-named-events' ||
      value.synthetic !== true || value.capacity !== CAPACITY) throw new Error('Invalid synthetic named event profile');
  return {kind:'synthetic-win32-named-events', synthetic:true, capacity:CAPACITY};
}
function u32(getter,index,field,label) {
  const value=getter(index,field);
  if(!Number.isInteger(value)||value<0||value>0xffffffff) throw new Error(`Invalid named event ${label}`);
  return value;
}
function exact(value,keys,label) {
  if(!value||typeof value!=='object'||Array.isArray(value))throw new Error(`Invalid named event ${label}`);
  const actual=Reflect.ownKeys(value);
  if(actual.length!==keys.length||keys.some(k=>!Object.hasOwn(value,k))||actual.some(k=>!keys.includes(k)))throw new Error(`Invalid named event ${label} shape`);
}
export function readEventState(getter,profile) {
  const selected=parseEventProfile(profile);
  if(typeof getter!=='function'||!selected)throw new Error('Named event profile and state getter are required');
  const h=f=>u32(getter,HEADER_INDEX,f,`header ${f}`), ready=h(0),objectCount=h(1),handleCount=h(2),capacity=h(3),status=h(4);
  if(ready!==1||objectCount>4||handleCount>4||objectCount>handleCount||capacity!==4||!STATUSES.has(status))throw new Error('Invalid named event state header');
  const objects=[];
  for(let i=0;i<objectCount;i++) {
    const row={object:u32(getter,i,0,'object'),nameBytes:u32(getter,i,1,'name length'),type:u32(getter,i,2,'type'),signaled:u32(getter,i,3,'signal state')};
    if(row.object!==i+1||row.nameBytes<1||row.nameBytes>63||row.type!==1||row.signaled!==0)throw new Error('Invalid named event object row');
    objects.push({...row,signaled:false});
  }
  const handles=[];
  for(let i=0;i<handleCount;i++) {
    const row={handle:u32(getter,i,4,'handle'),object:u32(getter,i,5,'handle object')};
    if(row.handle!==0x90000+4*(i+1)||row.object<1||row.object>objectCount)throw new Error('Invalid named event handle row');
    handles.push(row);
  }
  const seen=new Set();let nextObject=1;
  for(const row of handles) {
    if(!objects[row.object-1])throw new Error('Named event handle refers to a missing object');
    if(!seen.has(row.object)) { if(row.object!==nextObject++)throw new Error('Named event objects must appear in first-reference order'); seen.add(row.object); }
  }
  if(seen.size!==objectCount)
    throw new Error('Named event objects must be referenced in first-appearance order');
  if(status===0x40000000 && (handleCount<2||!handles.slice(0,-1).some(row=>row.object===handles.at(-1).object)))
    throw new Error('Informational named event status requires an existing-object open');
  if(status===0 && handleCount>0 && handles.slice(0,-1).some(row=>row.object===handles.at(-1).object))
    throw new Error('Successful named event state must end with a fresh-object creation');
  return {...selected,objectCount,handleCount,status,objects,handles};
}
export function validateEventState(value,profile) {
  const selected=parseEventProfile(profile); if(!selected)throw new Error('Named event profile is required');
  exact(value,STATE_KEYS,'serialized state');
  if(PROFILE_KEYS.some(k=>value[k]!==selected[k]))throw new Error('Named event profile mismatch');
  if(!Number.isInteger(value.objectCount)||value.objectCount<0||value.objectCount>4||!Number.isInteger(value.handleCount)||value.handleCount<0||value.handleCount>4||
     value.objectCount>value.handleCount||!Array.isArray(value.objects)||value.objects.length!==value.objectCount||!Array.isArray(value.handles)||value.handles.length!==value.handleCount)
    throw new Error('Invalid bounded named event tables');
  value.objects.forEach((r,i)=>exact(r,['object','nameBytes','type','signaled'],`object ${i}`));
  value.handles.forEach((r,i)=>exact(r,['handle','object'],`handle ${i}`));
  const canonical=readEventState((index,field)=>{
    if(index===HEADER_INDEX)return [1,value.objectCount,value.handleCount,4,value.status][field];
    if(field<4)return [value.objects[index]?.object,value.objects[index]?.nameBytes,value.objects[index]?.type,value.objects[index]?.signaled?1:0][field];
    return [value.handles[index]?.handle, value.handles[index]?.object][field-4];
  },selected);
  if(canonical.objectCount!==value.objectCount||canonical.handleCount!==value.handleCount||canonical.status!==value.status||
     canonical.objects.some((r,i)=>r.object!==value.objects[i].object||r.nameBytes!==value.objects[i].nameBytes||r.type!==value.objects[i].type||r.signaled!==value.objects[i].signaled)||
     canonical.handles.some((r,i)=>r.handle!==value.handles[i].handle||r.object!==value.handles[i].object))throw new Error('Named event state is inconsistent');
  return canonical;
}
