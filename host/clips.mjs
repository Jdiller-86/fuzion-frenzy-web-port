// Bounded metadata with one statically corroborated frontend playback profile.
export const selectionIdleId=0x2dbdfb0f;
export function clipPreviewRate(clip){return clip.id===selectionIdleId?60:30;}
export function clipPreviewTime(clip,elapsedMs){
  if(!Number.isFinite(elapsedMs)||elapsedMs<0||!Number.isFinite(clip.start)||!Number.isFinite(clip.end)||clip.end<=clip.start)throw new Error('Invalid clip preview clock');
  // The original fixed-point loop includes one additional 1/1024 key unit.
  const duration=clip.end-clip.start+1/1024;
  return clip.start+(elapsedMs*clipPreviewRate(clip)/1000)%duration;
}
export function readAnimationClips(buffer) {
  if(!(buffer instanceof ArrayBuffer)||buffer.byteLength<128||buffer.byteLength>16*1024*1024)throw new Error('Invalid animation clip resource');
  const v=new DataView(buffer),offset=v.getUint32(0x54,true),count=v.getUint32(0x58,true);
  if(!count||count>512||offset<128||offset>buffer.byteLength||count*32>buffer.byteLength-offset)throw new Error('Invalid animation clip table');
  const clips=[];
  for(let i=0;i<count;i++){
    const o=offset+i*32,start=v.getInt32(o+4,true),end=v.getInt32(o+8,true);
    if(start<0||end<=start||end>65535*1024)throw new Error('Invalid animation clip range');
    clips.push({id:v.getUint32(o,true),start:start/1024,end:end/1024,rawStep:v.getInt32(o+12,true)});
  }
  return clips;
}
