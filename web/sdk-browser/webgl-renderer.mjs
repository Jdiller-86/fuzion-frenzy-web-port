import { pixelGlsl } from './pixel-glsl.mjs';
import { vertexGlsl } from './vertex-glsl.mjs';

// Runs on the runtime worker. Commands synchronously consume the checked,
// owned SDK buffers while the title thread waits at its bridge callback.
export class WebGLRenderer {
  constructor(canvas, memory, report, gpuVertices=true) {
    this.canvas = canvas; this.memory = memory; this.report = report;
    const gl = this.gl = canvas.getContext('webgl2', { alpha: false, antialias: false, depth: false, stencil: false, preserveDrawingBuffer: false, powerPreference: 'high-performance' });
    if (!gl) throw new Error('WebGL 2 is unavailable. Select Software reference or enable browser hardware acceleration.');
    this.gpuVertices=gpuVertices&&gl.getParameter(gl.MAX_VERTEX_ATTRIBS)>=16&&gl.getParameter(gl.MAX_VERTEX_UNIFORM_VECTORS)>=195;
    this.vertexPrograms=new Map();
    this.targets = new Map(); this.images = new Map(); this.programs = new Map(); this.frame = 0;
    this.fbo = gl.createFramebuffer(); this.readFbo = gl.createFramebuffer();
    this.vao = gl.createVertexArray(); this.vbo = gl.createBuffer(); this.ibo = gl.createBuffer();
    this.samplers=Array.from({length:4},()=>gl.createSampler());
    this.samplerKeys=[];this.boundTextures=[];this.activeUnit=0;
    this.samplers.forEach((s,i)=>{gl.bindSampler(i,s);gl.samplerParameteri(s,gl.TEXTURE_COMPARE_MODE,gl.NONE);});
    this.vertexSource = `#version 300 es
precision highp float;
layout(location=0) in vec4 aPos;
layout(location=1) in vec4 aD0; layout(location=2) in vec4 aD1;
layout(location=3) in vec4 aB0; layout(location=4) in vec4 aB1;
layout(location=5) in vec4 aFog;
layout(location=6) in vec4 aTc0; layout(location=7) in vec4 aTc1;
layout(location=8) in vec4 aTc2; layout(location=9) in vec4 aTc3;
uniform vec4 surf;
out vec4 vD0; out vec4 vD1; out vec4 vB0; out vec4 vB1;
out vec4 vTc0; out vec4 vTc1; out vec4 vTc2; out vec4 vTc3; out float vFog;
void main() {
  vec3 p = vec3(aPos.xy * 2.0 / surf.xy - 1.0, aPos.z * surf.z * 2.0 - 1.0);
  // Keep the console's top row at texture y=0. Presentation flips once.
  gl_Position = vec4(p * aPos.w, aPos.w);
  vD0=aD0;vD1=aD1;vB0=aB0;vB1=aB1;vFog=aFog.x;
  vTc0=aTc0;vTc1=aTc1;vTc2=aTc2;vTc3=aTc3;
}`;
    this.vertexShader = this.shader(gl.VERTEX_SHADER, this.vertexSource);
    canvas.addEventListener('webglcontextlost', e => { e.preventDefault(); report('GPU context lost. Stop and restart the game to recreate resources.'); });
    const info = gl.getExtension('WEBGL_debug_renderer_info');
    report(`WebGL 2 triangle renderer: ${info ? gl.getParameter(info.UNMASKED_RENDERER_WEBGL) : gl.getParameter(gl.RENDERER)}`);
    report(`Programmable vertices: ${this.gpuVertices?'GPU':'CPU reference'}`);
  }
  bytes(ptr, length) {
    const heap = this.memory().HEAPU8;
    if (!Number.isInteger(ptr) || !Number.isSafeInteger(length) || ptr < 0 || length < 0 || ptr + length > heap.length) throw new Error('GPU bridge span outside WASM memory');
    return heap.subarray(ptr,ptr+length);
  }
  words(ptr,n) { return new Uint32Array(this.bytes(ptr,n*4).slice().buffer); }
  floats(ptr,n) { return new Float32Array(this.bytes(ptr,n*4).slice().buffer); }
  sameWords(a,b,count) {
    if(a===b)return true;
    const av=new Uint32Array(this.bytes(a,count*4).buffer,a,count),bv=new Uint32Array(this.bytes(b,count*4).buffer,b,count);
    for(let i=0;i<count;i++)if(av[i]!==bv[i])return false;
    return true;
  }
  bindTexture(unit,texture,needActive=false) {
    if(this.boundTextures[unit]===texture&&!needActive)return;
    if(this.activeUnit!==unit){this.gl.activeTexture(this.gl.TEXTURE0+unit);this.activeUnit=unit;}
    if(this.boundTextures[unit]!==texture){this.gl.bindTexture(this.gl.TEXTURE_2D,texture);this.boundTextures[unit]=texture;}
  }
  string(ptr) {
    const heap=this.memory().HEAPU8; let end=ptr;
    if (!Number.isInteger(ptr) || ptr<0 || ptr>=heap.length) throw new Error('Invalid shader pointer');
    while(end<heap.length && end-ptr<1000000 && heap[end]) ++end;
    if(end===heap.length || end-ptr===1000000) throw new Error('Unterminated shader');
    return new TextDecoder().decode(heap.slice(ptr,end));
  }
  shader(kind, source) {
    const g=this.gl,s=g.createShader(kind);g.shaderSource(s,source);g.compileShader(s);
    if(!g.getShaderParameter(s,g.COMPILE_STATUS)) {
      const e=g.getShaderInfoLog(s),lines=source.split('\n');
      const context=[...new Set([...e.matchAll(/ERROR:\s*\d+:(\d+)/g)].map(m=>Number(m[1])))].slice(0,10).map(n=>`${n}: ${lines[n-1]}`).join('\n');
      g.deleteShader(s);throw new Error(`GPU shader: ${e}\n${context}`);
    }
    return s;
  }
  commands(ptr,count) {
    try {
    const started=performance.now();
    if(count>65536)throw new Error('GPU command budget exceeded');
    const words=this.words(ptr,count*12);
    // One streaming geometry upload per batch rather than two driver-buffer
    // allocations per glyph/draw. Rebase indices; guest vertices stay intact.
    let vertexBytes=0,indexCount=0;
    for(let i=0;i<count;i++){const q=words.subarray(i*12,i*12+12);if(q[0]===1){vertexBytes+=q[2]*256;indexCount+=q[5];}}
    if(vertexBytes>256*1024*1024||indexCount>16*1024*1024)throw new Error('GPU geometry budget exceeded');
    const vertices=new Uint8Array(vertexBytes),elements=new Uint32Array(indexCount);
    const vertexWords=new Uint32Array(vertices.buffer);
    const totalVertices=vertexBytes/256;
    this.indexOffsets=new Map();let vo=0,io=0;
    for(let i=0;i<count;i++){
      const q=words.subarray(i*12,i*12+12);if(q[0]!==1)continue;
      const metadata=this.words(q[6],40),raw=!!metadata[37],mask=metadata[39];
      if(!raw&&q[3]!==160)throw new Error('Unexpected SDK shaded vertex stride');
      let registers=0;for(let r=0;r<16;r++)if(mask&(1<<r))registers++;
      if(raw&&q[3]!==registers*16)throw new Error('Unexpected raw vertex register span');
      const sourceBytes=this.bytes(q[1],q[2]*q[3]);
      const input=new Uint32Array(sourceBytes.buffer,sourceBytes.byteOffset,sourceBytes.length/4);
      for(let v=0;v<q[2];v++) {
        let from=v*q[3]/4;
        for(let r=0;r<(raw?16:10);r++)if(!raw||(mask&(1<<r))){
          const to=(r*totalVertices+vo/256+v)*4;
          vertexWords[to]=input[from++];vertexWords[to+1]=input[from++];vertexWords[to+2]=input[from++];vertexWords[to+3]=input[from++];
        }
      }
      const order=this.words(q[4],q[5]),base=vo/256,flat=metadata[32];
      if(flat)for(let j=0;j+2<order.length;j+=3){const first=order[j];order[j]=order[j+1];order[j+1]=order[j+2];order[j+2]=first;}
      for(let j=0;j<order.length;j++){if(order[j]>=q[2])throw new Error('GPU index exceeds vertex span');elements[io+j]=order[j]+base;}
      this.indexOffsets.set(q[4],io*4);vo+=q[2]*256;io+=q[5];
    }
    const g=this.gl;g.bindVertexArray(this.vao);g.bindBuffer(g.ARRAY_BUFFER,this.vbo);g.bufferData(g.ARRAY_BUFFER,vertices,g.STREAM_DRAW);
    // Attribute planes avoid WebGL's 255-byte vertex stride limit.
    for(let a=0;a<16;a++){g.enableVertexAttribArray(a);g.vertexAttribPointer(a,4,g.FLOAT,false,16,a*totalVertices*16);}
    g.bindBuffer(g.ELEMENT_ARRAY_BUFFER,this.ibo);g.bufferData(g.ELEMENT_ARRAY_BUFFER,elements,g.STREAM_DRAW);
    let pending;
    const emit=()=>{if(pending){if(!this.draw(...pending.subarray(1,10)))throw new Error('GPU draw rejected');pending=undefined;}};
    for(let i=0;i<count;i++) {
      const q=words.subarray(i*12,i*12+12);
      if(q[0]===1) {
        if(pending&&pending[8]===q[8]&&this.indexOffsets.get(pending[4])+pending[5]*4===this.indexOffsets.get(q[4])&&
           this.sameWords(pending[6],q[6],40)&&this.sameWords(pending[7],q[7],152)&&this.sameWords(pending[9],q[9],56)) {
          pending[5]+=q[5];continue;
        }
        emit();pending=q.slice();continue;
      }
      emit();
      if(q[0]===0)this.target(...q.subarray(1));
      else if(q[0]===2){const z=new Float32Array(q.buffer,q.byteOffset+7*4,1)[0];this.clear(...q.subarray(1,7),z,q[8],q[9]);}
      else throw new Error('Unknown GPU command');
    }
    emit();
    const error=this.gl.getError();
    if(error)throw new Error(`GPU command batch error 0x${error.toString(16)}`);
    this.submitMs=(this.submitMs||0)+performance.now()-started;
    return true;
    } catch(error) {this.report(error.message);postMessage({type:'gpu-error',text:error.message});return false;}
  }
  program(source,kinds,shadows,flat,vertexPointer=0,regs=0) {
    const key=source+':'+kinds+':'+shadows+':'+flat+':'+vertexPointer;
    if(this.programs.has(key)) return this.programs.get(key);
    if(this.programs.size>=2048)throw new Error('GPU shader cache budget exceeded');
    const g=this.gl, p=g.createProgram(), f=this.shader(g.FRAGMENT_SHADER,pixelGlsl(source,kinds,shadows,flat));
    let vertexShader;
    if(vertexPointer){
      const vk=`${vertexPointer}:${flat}`;vertexShader=this.vertexPrograms.get(vk);
      if(!vertexShader){
        const hlsl=this.string(vertexPointer);
        try{vertexShader=this.shader(g.VERTEX_SHADER,vertexGlsl(hlsl,regs,flat));}
        catch(error){postMessage({type:'vertex-diagnostic',hlsl,regs,flat});throw error;}
        this.vertexPrograms.set(vk,vertexShader);
      }
    } else {
      if(flat&&!this.flatVertexShader)this.flatVertexShader=this.shader(g.VERTEX_SHADER,this.vertexSource.replace(/out vec4 v([DB][01])/g,'flat out vec4 v$1'));
      vertexShader=flat?this.flatVertexShader:this.vertexShader;
    }
    g.attachShader(p,vertexShader);g.attachShader(p,f);g.linkProgram(p);g.deleteShader(f);
    if(!g.getProgramParameter(p,g.LINK_STATUS)) throw new Error(`GPU program: ${g.getProgramInfoLog(p)}`);
    const result={p,locations:new Map()};this.programs.set(key,result);return result;
  }
  target(id,w,h,format,pitch,linear,depth,ptr,size,hashLo,hashHi) {
    if(!id || !w || !h || w>4096 || h>4096) throw new Error('Invalid GPU surface');
    const old=this.targets.get(id);
    if(!old&&this.targets.size>=256)throw new Error('GPU target budget exceeded');
    if(old && old.w===w && old.h===h && old.format===format && old.depth===!!depth && old.hashLo===hashLo && old.hashHi===hashHi) {old.last=this.frame;return;}
    if(old) this.invalidate(id);
    const g=this.gl,t={id,w,h,format,depth:!!depth,texture:g.createTexture(),last:this.frame,hashLo,hashHi};
    g.bindTexture(g.TEXTURE_2D,t.texture);this.boundTextures[this.activeUnit]=t.texture;g.texParameteri(g.TEXTURE_2D,g.TEXTURE_MIN_FILTER,g.NEAREST);g.texParameteri(g.TEXTURE_2D,g.TEXTURE_MAG_FILTER,g.NEAREST);
    g.texParameteri(g.TEXTURE_2D,g.TEXTURE_WRAP_S,g.CLAMP_TO_EDGE);g.texParameteri(g.TEXTURE_2D,g.TEXTURE_WRAP_T,g.CLAMP_TO_EDGE);
    if(depth) {
      if(![0x2a,0x2c,0x2e,0x30].includes(format))throw new Error(`Unsupported depth encoding 0x${format.toString(16)}`);
      // WebGL restricts DEPTH_STENCIL uploads to null. Initial CPU depth writes
      // are admitted only through the normal clear/draw lifecycle for now.
      g.texImage2D(g.TEXTURE_2D,0,g.DEPTH24_STENCIL8,w,h,0,g.DEPTH_STENCIL,g.UNSIGNED_INT_24_8,null);
    }
    else {
      if(![0x06,0x07,0x12,0x1e,0x05,0x11].includes(format)) throw new Error(`Unsupported GPU target format 0x${format.toString(16)}`);
      const input=this.bytes(ptr,size), rgba=new Uint8Array(w*h*4), bpp=[0x05,0x11].includes(format)?2:4;
      for(let y=0;y<h;y++) for(let x=0;x<w;x++) {
        let o=0,bit=0;
        if(linear) o=y*pitch+x*bpp;
        else {for(let m=1;m<w||m<h;m<<=1) {if(m<w) {if(x&m)o|=1<<bit;bit++;}if(m<h){if(y&m)o|=1<<bit;bit++;}}o*=bpp;}
        const q=(y*w+x)*4;if(o+bpp>input.length)throw new Error('Surface span mismatch');
        if(bpp===4) {rgba[q]=input[o+2];rgba[q+1]=input[o+1];rgba[q+2]=input[o];rgba[q+3]=[7,0x1e].includes(format)?255:input[o+3];}
        else {const v=input[o]|input[o+1]<<8;rgba[q]=(v>>11)*255/31;rgba[q+1]=(v>>5&63)*255/63;rgba[q+2]=(v&31)*255/31;rgba[q+3]=255;}
      }
      g.texImage2D(g.TEXTURE_2D,0,g.RGBA8,w,h,0,g.RGBA,g.UNSIGNED_BYTE,rgba);
    }
    this.targets.set(id,t);
  }
  attach(color,depth) {
    const g=this.gl;
    if(!this.fboBound){g.bindFramebuffer(g.FRAMEBUFFER,this.fbo);this.fboBound=true;}
    if(this.attachmentsValid&&this.attachedColor===color&&this.attachedDepth===depth)return;
    g.framebufferTexture2D(g.FRAMEBUFFER,g.COLOR_ATTACHMENT0,g.TEXTURE_2D,color?.texture||null,0);
    g.framebufferTexture2D(g.FRAMEBUFFER,g.DEPTH_STENCIL_ATTACHMENT,g.TEXTURE_2D,depth?.texture||null,0);
    g.drawBuffers([color?g.COLOR_ATTACHMENT0:g.NONE]);g.readBuffer(color?g.COLOR_ATTACHMENT0:g.NONE);
    if(g.checkFramebufferStatus(g.FRAMEBUFFER)!==g.FRAMEBUFFER_COMPLETE)throw new Error('GPU framebuffer is incomplete');
    this.attachedColor=color;this.attachedDepth=depth;
    this.attachmentsValid=true;
  }
  clear(rt,zs,n,rects,flags,color,z,stencil,viewport) {
    this.lastState=undefined;
    const g=this.gl,r=this.targets.get(rt),d=this.targets.get(zs),v=this.words(viewport,4);
    if((flags&0xf0)&&!r)throw new Error('Clear references an unregistered color target');
    if((flags&3)&&!d)throw new Error('Clear references an unregistered depth target');
    this.attach(flags&0xf0?r:null,flags&3?d:null);
    g.enable(g.SCISSOR_TEST);g.colorMask(!!(flags&0x10),!!(flags&0x20),!!(flags&0x40),!!(flags&0x80));
    g.depthMask(true);g.stencilMask(255);g.clearColor((color>>>16&255)/255,(color>>>8&255)/255,(color&255)/255,(color>>>24)/255);g.clearDepth(z);g.clearStencil(stencil);
    const list=n?this.words(rects,n*4):[v[0],v[1],v[0]+v[2],v[1]+v[3]];
    for(let i=0;i<(n||1);i++){const [x,y,x1,y1]=Array.from(list).slice(i*4,i*4+4);g.scissor(x,y,Math.max(0,x1-x),Math.max(0,y1-y));g.clear((flags&0xf0?g.COLOR_BUFFER_BIT:0)|(flags&1?g.DEPTH_BUFFER_BIT:0)|(flags&2?g.STENCIL_BUFFER_BIT:0));}
  }
  draw(v,count,stride,indices,n,state,uniforms,source,textures) {
    try {
      const g=this.gl,s=this.words(state,40),tx=this.words(textures,4*14),u=this.floats(uniforms,152);
      const kinds=[0,1,2,3].map(t=>tx[t*14]),shadows=[0,1,2,3].map(t=>tx[t*14+1]);
      if(kinds.some(k=>k===2||k===3))throw new Error('GPU 3D/cube texture admission is not implemented');
      this.pointerPrograms ??= new Map();
      const programKey=`${source}:${s[37]}`;
      let program=this.pointerPrograms.get(programKey);
      if(!program){program=this.program(this.string(source),kinds,shadows,!!s[32],s[37],s[39]);this.pointerPrograms.set(programKey,program);}
      this.attach(this.targets.get(s[0]&0x1fffffff),this.targets.get(s[1]&0x1fffffff));
      if(this.lastProgram!==program.p){g.useProgram(program.p);this.lastProgram=program.p;}
      const loc=name=>{if(!program.locations.has(name))program.locations.set(name,g.getUniformLocation(program.p,name));return program.locations.get(name);};
      const previous=program.uniforms;
      const uniform=(name,offset,num)=>{if(!previous||u.subarray(offset,offset+num).some((v,i)=>v!==previous[offset+i]))g.uniform4fv(loc(name),u.subarray(offset,offset+num));};
      uniform('surf',0,4);uniform('fogcolor',4,4);uniform('misc',8,4);uniform('pc',12,72);uniform('tfactor',84,4);uniform('texsize',88,16);uniform('bumpmat',104,16);uniform('bumplum',120,16);uniform('uvrect',136,16);
      program.uniforms=u;
      if(s[37]) {
        const constants=this.floats(s[38],772),old=program.vertexConstants;
        if(!old||constants.subarray(0,768).some((value,i)=>value!==old[i]))g.uniform4fv(loc('c'),constants.subarray(0,768));
        if(!old||constants.subarray(768).some((value,i)=>value!==old[768+i]))g.uniform4fv(loc('fogp'),constants.subarray(768));
        program.vertexConstants=constants;
      }
      const addresses=new Float32Array(16),borders=new Float32Array(16);
      for(let t=0;t<4;t++){
        const q=tx.subarray(t*14,t*14+14),o=t*4,c=q[13];
        addresses.set([q[7],q[8],q[9],q[10]],o);
        borders.set([(c>>>16&255)/255,(c>>>8&255)/255,(c&255)/255,(c>>>24)/255],o);
      }
      if(!program.addresses||addresses.some((v,i)=>v!==program.addresses[i])){g.uniform4fv(loc('addresses'),addresses);program.addresses=addresses;}
      if(!program.borders||borders.some((v,i)=>v!==program.borders[i])){g.uniform4fv(loc('border'),borders);program.borders=borders;}
      for(let t=0;t<4;t++) {
        const q=tx.subarray(t*14,t*14+14); if(!q[0])continue;
        let image=this.targets.get(q[2]&0x1fffffff);
        if(shadows[t]&&!image)throw new Error('CPU depth-texture uploads are not admitted; depth sampling requires a GPU-owned surface');
        if(!image) {
          // SDK decoded image storage is frame-local. Re-upload on the next
          // frame, including CPU changes and allocation reuse at the same VA.
          const key=`${q[2]}:${q[3]}:${q[4]}`;
          image=this.images.get(key);
          if(!image){if(this.images.size>=4096)throw new Error('GPU texture cache budget exceeded');image={texture:g.createTexture(),frame:-1};this.images.set(key,image);}
          this.bindTexture(t,image.texture);
          if(image.hashLo!==q[11] || image.hashHi!==q[12]){
            this.bindTexture(t,image.texture,true);
            const pixels=this.floats(q[6],q[3]*q[4]*4), rgba=new Uint8Array(pixels.length);
            for(let j=0;j<pixels.length;j++)rgba[j]=Math.round(Math.max(0,Math.min(1,pixels[j]))*255);
            g.texImage2D(g.TEXTURE_2D,0,g.RGBA8,q[3],q[4],0,g.RGBA,g.UNSIGNED_BYTE,rgba);image.hashLo=q[11];image.hashHi=q[12];
          }
          image.frame=this.frame;
        } else this.bindTexture(t,image.texture);
        if(image===this.targets.get(s[0]&0x1fffffff)||image===this.targets.get(s[1]&0x1fffffff))throw new Error('GPU feedback texture overlaps an active attachment');
        const address=m=>m===1?g.REPEAT:m===2?g.MIRRORED_REPEAT:g.CLAMP_TO_EDGE;
        const sampler=this.samplers[t],samplerKey=`${q[7]}:${q[8]}:${q[9]}:${q[10]}`;
        if(this.samplerKeys[t]!==samplerKey){
        g.samplerParameteri(sampler,g.TEXTURE_WRAP_S,address(q[7]));g.samplerParameteri(sampler,g.TEXTURE_WRAP_T,address(q[8]));
        g.samplerParameteri(sampler,g.TEXTURE_MIN_FILTER,q[9]===1?g.NEAREST:g.LINEAR);g.samplerParameteri(sampler,g.TEXTURE_MAG_FILTER,q[10]===1?g.NEAREST:g.LINEAR);
        this.samplerKeys[t]=samplerKey;
        }
        if(!program.samplersSet)g.uniform1i(loc(`tex${t}`),t);
      }
      program.samplersSet=true;
      if(!this.lastState||u[2]!==this.lastZscale||s.subarray(0,37).some((value,i)=>value!==this.lastState[i])) {
      g.viewport(0,0,s[2],s[3]);g.enable(g.SCISSOR_TEST);
      let rect=[s[22],s[23],s[22]+s[24],s[23]+s[25]];
      if(s[26])rect=[Math.max(rect[0],s[27]),Math.max(rect[1],s[28]),Math.min(rect[2],s[29]),Math.min(rect[3],s[30])];
      g.scissor(rect[0],rect[1],Math.max(0,rect[2]-rect[0]),Math.max(0,rect[3]-rect[1]));
      const enable=(cap,on)=>on?g.enable(cap):g.disable(cap);
      enable(g.BLEND,s[4]);g.blendFunc(s[5],s[6]);g.blendEquation(s[7]||g.FUNC_ADD);
      g.blendColor((s[31]>>>16&255)/255,(s[31]>>>8&255)/255,(s[31]&255)/255,(s[31]>>>24)/255);
      g.colorMask(!!(s[8]&0x10000),!!(s[8]&0x100),!!(s[8]&1),!!(s[8]&0x1000000));
      enable(g.DEPTH_TEST,s[9]);g.depthMask(!!s[10]);g.depthFunc(s[11]);
      enable(g.STENCIL_TEST,s[12]);g.stencilFunc(s[13],s[14],s[15]);g.stencilMask(s[16]);g.stencilOp(s[17],s[18],s[19]);
      enable(g.CULL_FACE,s[20]===0x900||s[20]===0x901);
      // GPU coordinates have y down, matching the reference winding convention.
      g.frontFace(s[21]===0x901?g.CW:g.CCW);g.cullFace(s[20]===s[21]?g.FRONT:g.BACK);
      enable(g.POLYGON_OFFSET_FILL,s[34]);const f=new Float32Array(s.buffer);g.polygonOffset(f[35],f[36]*16777215*u[2]);
      this.lastState=s;
      this.lastZscale=u[2];
      }
      g.drawElements(g.TRIANGLES,n,g.UNSIGNED_INT,this.indexOffsets.get(indices));
      this.issuedDraws=(this.issuedDraws||0)+1;
      return true;
    } catch(error) {this.report(error.message);postMessage({type:'gpu-error',text:error.message});return false;}
  }
  present(id) {
    const t=this.targets.get(id);if(!t||t.depth)return false;
    const g=this.gl;if(this.canvas.width!==t.w||this.canvas.height!==t.h){this.canvas.width=t.w;this.canvas.height=t.h;}
    g.bindFramebuffer(g.READ_FRAMEBUFFER,this.fbo);g.framebufferTexture2D(g.READ_FRAMEBUFFER,g.COLOR_ATTACHMENT0,g.TEXTURE_2D,t.texture,0);g.framebufferTexture2D(g.READ_FRAMEBUFFER,g.DEPTH_STENCIL_ATTACHMENT,g.TEXTURE_2D,null,0);g.readBuffer(g.COLOR_ATTACHMENT0);
    g.bindFramebuffer(g.DRAW_FRAMEBUFFER,null);g.disable(g.SCISSOR_TEST);g.blitFramebuffer(0,0,t.w,t.h,0,t.h,t.w,0,g.COLOR_BUFFER_BIT,g.NEAREST);g.flush();
    this.fboBound=false;this.attachmentsValid=false;this.attachedColor=undefined;this.attachedDepth=undefined;this.lastState=undefined;
    ++this.frame;
    this.lastSubmitMs=this.submitMs||0;this.lastIssuedDraws=this.issuedDraws||0;this.submitMs=0;this.issuedDraws=0;
    for(const [key,image] of this.images)if(image.frame<this.frame-2){g.deleteTexture(image.texture);this.images.delete(key);}
    postMessage({type:'gpu-frame',width:t.w,height:t.h});return true;
  }
  pixels(ptr,w,h,pitch) {
    const g=this.gl;
    let t=this.targets.get(-1);
    if(!t){t={w,h,texture:g.createTexture()};this.targets.set(-1,t);}
    t.w=w;t.h=h;g.bindTexture(g.TEXTURE_2D,t.texture);this.boundTextures[this.activeUnit]=t.texture;
    g.texParameteri(g.TEXTURE_2D,g.TEXTURE_MIN_FILTER,g.LINEAR);g.texParameteri(g.TEXTURE_2D,g.TEXTURE_MAG_FILTER,g.LINEAR);
    const input=this.bytes(ptr,(pitch||w*4)*h);let rgba=input.slice();
    if(pitch && pitch!==w*4){rgba=new Uint8Array(w*h*4);for(let y=0;y<h;y++)rgba.set(input.subarray(y*pitch,y*pitch+w*4),y*w*4);}
    g.texImage2D(g.TEXTURE_2D,0,g.RGBA8,w,h,0,g.RGBA,g.UNSIGNED_BYTE,rgba);this.present(-1);
  }
  read(id,ptr) {
    const t=this.targets.get(id);if(!t)return false;
    if(t.depth)throw new Error('Guest depth readback is not yet admitted by the WebGL backend');
    const g=this.gl;this.attach(t,null);const pixels=new Uint8Array(t.w*t.h*4);g.readPixels(0,0,t.w,t.h,g.RGBA,g.UNSIGNED_BYTE,pixels);this.bytes(ptr,pixels.length).set(pixels);return true;
  }
  invalidate(id) {const t=this.targets.get(id);if(t){this.gl.deleteTexture(t.texture);this.targets.delete(id);}}
}
