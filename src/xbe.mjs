// Independently written bounded XBE admission/profile reader.
// Public layout reference: https://xboxdevwiki.net/Xbe
// This reads metadata only. It neither executes nor recompiles game instructions.
const VARIANTS=[['retail',0xa8fc57ab,0x5b6d40b6],['debug',0x94859d4b,0xefb1f152],['beta',0xe682f45b,0x46437dcd]];
const MAX_FILE=16*1024*1024,MAX_IMAGE=128*1024*1024;

export function readXbeProfile(buffer){
  if(!(buffer instanceof ArrayBuffer)||buffer.byteLength<0x178||buffer.byteLength>MAX_FILE)throw new Error('Invalid XBE size');
  const v=new DataView(buffer),length=v.byteLength;
  const range=(offset,size)=>{if(!Number.isSafeInteger(offset)||!Number.isSafeInteger(size)||offset<0||size<0||offset>length||size>length-offset)throw new Error('XBE file range outside input');return offset;};
  const u32=offset=>v.getUint32(range(offset,4),true);
  if(u32(0)!==0x48454258)throw new Error('Invalid XBE signature');
  const base=u32(0x104),headerBytes=u32(0x108),imageBytes=u32(0x10c),imageHeaderBytes=u32(0x110),sectionCount=u32(0x11c);
  if(base<0x1000||base>MAX_IMAGE||headerBytes<0x178||headerBytes>length||imageHeaderBytes<0x178||imageHeaderBytes>headerBytes||imageBytes<headerBytes||imageBytes>MAX_IMAGE||base+imageBytes>0x100000000||sectionCount<1||sectionCount>128)throw new Error('Unsupported XBE layout');
  const imageEnd=base+imageBytes;
  const inImage=(address,size)=>Number.isSafeInteger(address)&&address>=base&&size>=0&&address<=imageEnd&&size<=imageEnd-address;
  const table=u32(0x120)-base;
  if(table<imageHeaderBytes||table+sectionCount*0x38>headerBytes)throw new Error('Invalid XBE section table');
  range(table,sectionCount*0x38);
  const sections=[];
  for(let i=0;i<sectionCount;i++){
    const o=table+i*0x38,flags=u32(o),address=u32(o+4),virtualBytes=u32(o+8),fileOffset=u32(o+12),fileBytes=u32(o+16);
    if(!virtualBytes||!inImage(address,virtualBytes)||address<base+headerBytes||fileBytes>virtualBytes||fileBytes&&fileOffset<headerBytes)throw new Error('Invalid XBE section range');
    range(fileOffset,fileBytes);
    if(sections.some(s=>address<s.address+s.virtualBytes&&s.address<address+virtualBytes))throw new Error('Overlapping XBE virtual sections');
    sections.push({index:i,flags,address,virtualBytes,fileOffset,fileBytes,executable:!!(flags&4)});
  }
  function offsetFor(address,size){
    if(!inImage(address,size))throw new Error('XBE address outside image');
    if(address-base<headerBytes&&size<=headerBytes-(address-base))return range(address-base,size);
    const s=sections.find(s=>address>=s.address&&address-s.address<=s.fileBytes&&size<=s.fileBytes-(address-s.address));
    if(!s)throw new Error('XBE metadata is not file backed');
    return range(s.fileOffset+address-s.address,size);
  }
  function importsAt(address){
    if(!address)return [];
    const imports=[];
    for(let i=0;i<512;i++){
      const slot=address+i*4,value=u32(offsetFor(slot,4));if(!value)return imports;
      if((value>>>16)!==0x8000||(value&0xffff)===0)throw new Error('Invalid XBE kernel ordinal');
      imports.push({address:slot,ordinal:value&0xffff});
    }
    throw new Error('Unterminated XBE kernel imports');
  }
  const candidates=[];
  for(const [variant,entryKey,thunkKey]of VARIANTS){
    const entry=(u32(0x128)^entryKey)>>>0,kernelThunk=(u32(0x158)^thunkKey)>>>0;
    if(!sections.some(s=>s.executable&&entry>=s.address&&entry-s.address<s.fileBytes))continue;
    try{candidates.push({variant,entry,kernelThunk,imports:importsAt(kernelThunk)});}catch{/* A candidate must validate both fields with the same variant. */}
  }
  if(candidates.length!==1)throw new Error('Unknown or ambiguous XBE entry/import profile');
  let tls=null;const tlsAddress=u32(0x12c);
  if(tlsAddress){
    const o=offsetFor(tlsAddress,24),start=u32(o),end=u32(o+4),index=u32(o+8),callback=u32(o+12),zeroFillBytes=u32(o+16),characteristics=u32(o+20);
    if(end<start||end-start+zeroFillBytes>MAX_IMAGE||start&&!inImage(start,end-start)||index&&!inImage(index,4))throw new Error('Invalid XBE TLS layout');
    tls={address:tlsAddress,start,end,index,callback,zeroFillBytes,characteristics};
  }
  const libraryCount=u32(0x160),libraryAddress=u32(0x164),libraries=[];
  if(libraryCount>128)throw new Error('XBE library count exceeds bound');
  if(libraryCount){
    const o=offsetFor(libraryAddress,libraryCount*16);
    for(let i=0;i<libraryCount;i++){
      const p=o+i*16,nameBytes=new Uint8Array(buffer,p,8);let name='';
      for(const b of nameBytes){if(!b)break;if(b<0x20||b>0x7e)throw new Error('Invalid XBE library name');name+=String.fromCharCode(b);}
      libraries.push({name,major:v.getUint16(p+8,true),minor:v.getUint16(p+10,true),build:v.getUint16(p+12,true),flags:v.getUint16(p+14,true)});
    }
  }
  return {base,headerBytes,imageBytes,stackBytes:u32(0x130),initializationFlags:u32(0x124),nonKernelImportsAddress:u32(0x15c),sections,tls,libraries,...candidates[0]};
}
