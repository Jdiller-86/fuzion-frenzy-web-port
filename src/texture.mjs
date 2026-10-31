// Narrow Xbox swizzled texture profiles used by the selected samples.
// Provenance and limitations: docs/REFERENCE_INVENTORY.md.
export function decodeTexture(buffer,{frame=0}={}) {
 if(!(buffer instanceof ArrayBuffer)||buffer.byteLength<256)throw new Error('Invalid texture resource');
 const v=new DataView(buffer),format=v.getUint16(0x2c,true),w=v.getUint16(0x2e,true),h=v.getUint16(0x30,true),frames=v.getUint8(0x37);
 if(![1,3,5].includes(format)||w<2||h<2||w>1024||h>1024||(w&(w-1))!==0||(h&(h-1))!==0||frames<1||frames>32||!Number.isInteger(frame)||frame<0||frame>=frames)throw new Error('Unsupported texture profile');
 // Native initialization at 0x41890: 24-byte headers, aligned to 128,
 // shared palette, then aligned per-frame payload. Return the selected base mip.
 const headerEnd=128+Math.ceil(frames*24/128)*128,bytes=format===1?4:format===3?2:1;
 const stride=Math.ceil(v.getUint32(0x28,true)*bytes/128)*128;
 if(frames>1&&(stride<w*h*bytes||stride>4*1024*1024||buffer.byteLength<headerEnd+(format===5?1024:0)+stride*frames))throw new Error('Invalid texture frame ranges');
 const pixelStart=headerEnd+(format===5?1024:0)+frame*stride;
 if(buffer.byteLength<pixelStart+w*h*bytes)throw new Error('Truncated texture resource');
 const rgba=new Uint8Array(w*h*4);
 for(let y=0;y<h;y++)for(let x=0;x<w;x++){
   let morton=0,slot=0;for(let bit=0;(1<<bit)<w||(1<<bit)<h;bit++){if((1<<bit)<w)morton|=((x>>bit)&1)<<slot++;if((1<<bit)<h)morton|=((y>>bit)&1)<<slot++;}
   const p=4*(y*w+x);let c;
   if(format===3){c=v.getUint16(pixelStart+2*morton,true);rgba[p]=Math.round(((c>>10)&31)*255/31);rgba[p+1]=Math.round(((c>>5)&31)*255/31);rgba[p+2]=Math.round((c&31)*255/31);rgba[p+3]=c&0x8000?255:0;}
   else {c=format===5?v.getUint32(headerEnd+v.getUint8(pixelStart+morton)*4,true):v.getUint32(pixelStart+4*morton,true);rgba[p]=(c>>>16)&255;rgba[p+1]=(c>>>8)&255;rgba[p+2]=c&255;rgba[p+3]=c>>>24;}
 }
 return {width:w,height:h,rgba};
}
