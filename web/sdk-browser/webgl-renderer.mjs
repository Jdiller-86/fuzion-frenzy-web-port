import { FIXED_PIXEL_KEY_WORDS, fixedFallbackPixelGlsl, pixelGlsl, prepareFixedFallbackPixelKey } from './pixel-glsl.mjs';
import { vertexGlsl } from './vertex-glsl.mjs';
import { fixedFallbackVertexGlsl } from './fixed-vertex-fallback.mjs';
import { decodeFallbackVertexCode, programmableFallbackVertexGlsl, FALLBACK_VERTEX_CODE_BYTES } from './programmable-vertex-fallback.mjs';

const MAX_LINKED_PROGRAMS = 2048;
const FRAME_ERROR_CHECK_DELAY_MS = 250;
const MIN_GEOMETRY_REUSE_BYTES = 64 * 1024;
const WARMUP_STARTUP_BUDGET_MS = 400;
const WARMUP_POLL_INTERVAL_MS = 8;
const WARMUP_DRAW_BATCH = 4;
const PROMOTION_UPLOAD_BURST_BYTES = 1024 * 1024;
const PROMOTION_SETTLE_FRAMES = 8;
const PIXEL_UNIFORMS = [
  ['surf',0,4],['fogcolor',4,4],['misc',8,4],['pc',12,72],['tfactor',84,4],
  ['texsize',88,16],['bumpmat',104,16],['bumplum',120,16],['uvrect',136,16],
];
const DRAW_UNIFORMS = [
  ...PIXEL_UNIFORMS.map(([name]) => name),
  'c', 'fogp', 'wvp', 'wv', 'wvn', 'tm', 'mat', 'amb', 'lt', 'addresses', 'border',
  'tex0', 'tex1', 'tex2', 'tex3', 'fallbackVertexKey[0]', 'fallbackPixelKey[0]', 'fallbackInstructionCount',
];

function decodeColorTarget(input, width, height, format, pitch, linear) {
  const bytesPerPixel = format === 0x05 || format === 0x11 ? 2 : 4;
  const opaque = format === 0x07 || format === 0x1e;
  let columns, rows;
  if (!linear) {
    let xMask = 0, yMask = 0, bit = bytesPerPixel;
    for (let size = 1; size < width || size < height; size *= 2) {
      if (size < width) { xMask |= bit; bit *= 2; }
      if (size < height) { yMask |= bit; bit *= 2; }
    }
    // Increment the interleaved address bits separately for each axis.
    columns = new Uint32Array(width);
    rows = new Uint32Array(height);
    for (let x = 1; x < width; x++) columns[x] = (columns[x - 1] - xMask) & xMask;
    for (let y = 1; y < height; y++) rows[y] = (rows[y - 1] - yMask) & yMask;
  }
  const last = linear ? (height - 1) * pitch + (width - 1) * bytesPerPixel
    : rows[height - 1] + columns[width - 1];
  if (last + bytesPerPixel > input.length) throw new Error('Surface span mismatch');

  const rgba = new Uint8Array(width * height * 4);
  let dst = 0;
  for (let y = 0; y < height; y++) {
    const row = linear ? y * pitch : rows[y];
    for (let x = 0; x < width; x++, dst += 4) {
      const src = row + (linear ? x * bytesPerPixel : columns[x]);
      if (bytesPerPixel === 4) {
        rgba[dst] = input[src + 2];
        rgba[dst + 1] = input[src + 1];
        rgba[dst + 2] = input[src];
        rgba[dst + 3] = opaque ? 255 : input[src + 3];
      } else {
        const pixel = input[src] | (input[src + 1] << 8);
        rgba[dst] = (pixel >> 11) * 255 / 31;
        rgba[dst + 1] = ((pixel >> 5) & 63) * 255 / 63;
        rgba[dst + 2] = (pixel & 31) * 255 / 31;
        rgba[dst + 3] = 255;
      }
    }
  }
  return rgba;
}

function fixedShaderKey(source, kind, length) {
  const match = /^\/\/ ff_browser_fixed_(vertex|pixel)=([0-9,]+)$/m.exec(source);
  if (match?.[1] !== kind) return null;
  const words = match[2].split(',').map(Number);
  if (words.length !== length || words.some(word => !Number.isInteger(word) || word < 0 || word > 0xffffffff)) return null;
  return Uint32Array.from(words);
}

const RAW_VERTEX_FORMATS = new Map([
  [41,{components:1,componentBytes:4,type:'FLOAT',normalized:false,integer:false}],
  [16,{components:2,componentBytes:4,type:'FLOAT',normalized:false,integer:false}],
  [6,{components:3,componentBytes:4,type:'FLOAT',normalized:false,integer:false}],
  [2,{components:4,componentBytes:4,type:'FLOAT',normalized:false,integer:false}],
  [87,{components:4,componentBytes:1,type:'UNSIGNED_BYTE',normalized:true,integer:false,bgra:true}],
  [58,{components:1,componentBytes:2,type:'SHORT',normalized:true,integer:false}],
  [37,{components:2,componentBytes:2,type:'SHORT',normalized:true,integer:false}],
  [13,{components:4,componentBytes:2,type:'SHORT',normalized:true,integer:false}],
  [61,{components:1,componentBytes:1,type:'UNSIGNED_BYTE',normalized:true,integer:false}],
  [49,{components:2,componentBytes:1,type:'UNSIGNED_BYTE',normalized:true,integer:false}],
  [28,{components:4,componentBytes:1,type:'UNSIGNED_BYTE',normalized:true,integer:false}],
  [59,{components:1,componentBytes:2,type:'SHORT',normalized:false,integer:true}],
  [38,{components:2,componentBytes:2,type:'SHORT',normalized:false,integer:true}],
  [14,{components:4,componentBytes:2,type:'SHORT',normalized:false,integer:true}],
  [42,{components:1,componentBytes:4,type:'UNSIGNED_INT',normalized:false,integer:true}],
]);

// Runs on the runtime worker. Commands synchronously consume the checked,
// owned SDK buffers while the title thread waits at its bridge callback.
export class WebGLRenderer {
  constructor(canvas, memory, report, gpuVertices=true, shaderPrewarm=true, gpuTiming=false, shaderFallback=false, shaderPromotion=true) {
    this.canvas = canvas; this.memory = memory; this.report = report;
    const gl = this.gl = canvas.getContext('webgl2', { alpha: false, antialias: false, depth: false, stencil: false, preserveDrawingBuffer: false, powerPreference: 'high-performance' });
    if (!gl) throw new Error('WebGL 2 is unavailable. Select Software reference or enable browser hardware acceleration.');
    this.maxTextureSize = gl.getParameter(gl.MAX_TEXTURE_SIZE);
    this.gpuVertices=gpuVertices&&gl.getParameter(gl.MAX_VERTEX_ATTRIBS)>=16&&gl.getParameter(gl.MAX_VERTEX_UNIFORM_VECTORS)>=195;
    this.parallelShaderCompile=shaderPrewarm?gl.getExtension('KHR_parallel_shader_compile'):null;
    this.shaderStats={enabled:!!this.parallelShaderCompile,prepassMs:0,compileMs:0,linkMs:0,waitMs:0,prepassPrograms:0,completedPrograms:0,programReuses:0,readyWithoutWait:0,blockingPrograms:0,maxWaitMs:0,fragmentCompiles:0,fragmentReuses:0};
    Object.assign(this.shaderStats,{warmupLinks:0,warmupHits:0,warmupPrimed:0,warmupPrimeMs:0,warmupWaitMs:0,warmupSkipped:0,warmupDraws:0,warmupDrawMs:0});
    Object.assign(this.shaderStats,{fixedFallbackEnabled:false,fixedFallbackStartupWaitMs:0,fixedFallbackDraws:0,fixedFallbackFrames:0});
    Object.assign(this.shaderStats,{programmableFallbackEnabled:false,programmableFallbackStartupWaitMs:0,programmableFallbackDraws:0,programmableFallbackFrames:0});
    Object.assign(this.shaderStats,{liveLinks:0,residentVariants:0,inlineTranslations:0,inlineTranslationMaxMs:0,fastCombinerDraws:0});
    Object.assign(this.shaderStats,{backgroundLinks:0,promotedVariants:0,promotionFailures:0,promotionMaxStepMs:0,promotionUploadDeferrals:0});
    this.shaderPromotionResumeFrame = 0;
    if (shaderFallback && shaderPromotion && this.parallelShaderCompile) this.shaderPromotionCandidates = new Set();
    this.shaderPrewarmSeen=new Set();this.shaderPrewarmPending=new Map();this.shaderPrewarmDiscovered=new Map();this.shaderPrewarmPairCount=0;
    this.vertexPrograms=new Map();
    this.fragmentShaders=new Map();
    this.programVariants=new Map();
    this.targets = new Map(); this.images = new Map(); this.programs = new Map(); this.frame = 0;
    if(gpuTiming) {
      const ext=gl.getExtension('EXT_disjoint_timer_query_webgl2');
      const bits=ext?gl.getQuery(ext.TIME_ELAPSED_EXT,ext.QUERY_COUNTER_BITS_EXT):0;
      this.gpuTimerExtension=bits>0?ext:null;this.gpuTimerPending=[];
      this.gpuTimingStats={supported:bits>0,enabled:bits>0,counterBits:bits,periodFrames:6,samples:0,pending:0,disjointEvents:0,dropped:0,skippedStarts:0,results:[]};
      report(`GPU elapsed timing: ${bits>0?'enabled (one sample per six frames)':'unavailable; rendering unchanged'}`);
    }
    this.stateSnapshot = new Uint32Array(40);
    this.readFbo = gl.createFramebuffer(); this.framebuffers = new Map(); this.validatedFramebufferLayouts = new Map(); this.boundAttachments = null;
    this.vao = gl.createVertexArray();
    this.vertexBuffer = {buffer:gl.createBuffer(), capacity:0, limit:256*1024*1024, shadowLimit:8*1024*1024, validWords:0};
    this.indexBuffer = {buffer:gl.createBuffer(), capacity:0, limit:64*1024*1024, shadowLimit:2*1024*1024, validWords:0};
    const commandFloatBuffer=new ArrayBuffer(4);this.commandFloatBits=new Uint32Array(commandFloatBuffer);this.commandFloatValue=new Float32Array(commandFloatBuffer);
    this.samplers=Array.from({length:4},()=>gl.createSampler());
    this.imageUpload=new Uint8Array(0);
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
    this.defaultVertexStage = {source:this.vertexSource,shader:this.compileShader(gl.VERTEX_SHADER,this.vertexSource)};
    if (shaderFallback && shaderPrewarm && this.gpuVertices) this.beginFixedFallbacks();
    canvas.addEventListener('webglcontextlost', e => {
      e.preventDefault();
      this.contextLost = true;
      this.deferGpuErrors = false;
      this.cancelShaderPromotion();
      this.cancelFrameErrorCheck();
      this.disposeGpuTiming();
      postMessage({type:'gpu-error',text:'The graphics context was lost. Start the game again to recreate its resources.'});
    });
    const info = gl.getExtension('WEBGL_debug_renderer_info');
    report(`WebGL 2 triangle renderer: ${info ? gl.getParameter(info.UNMASKED_RENDERER_WEBGL) : gl.getParameter(gl.RENDERER)}`);
    report(`Programmable vertices: ${this.gpuVertices?'GPU':'CPU reference'}`);
  }
  clearGpuTimerQueries() {
    const g=this.gl;
    if(this.gpuTimerActive) {
      try {g.endQuery(this.gpuTimerExtension.TIME_ELAPSED_EXT);} catch {}
      g.deleteQuery(this.gpuTimerActive[0]);this.gpuTimerActive=undefined;
      this.gpuTimingStats.dropped++;
    }
    for(const [query] of this.gpuTimerPending||[])g.deleteQuery(query);
    if(this.gpuTimingStats){this.gpuTimingStats.dropped+=this.gpuTimerPending.length;this.gpuTimingStats.pending=0;}
    if(this.gpuTimerPending)this.gpuTimerPending.length=0;
  }
  disposeGpuTiming() {
    this.clearGpuTimerQueries();this.gpuTimerExtension=null;
    if(this.gpuTimingStats)this.gpuTimingStats.enabled=false;
  }
  beginGpuTimer() {
    const g=this.gl,ext=this.gpuTimerExtension,stats=this.gpuTimingStats;
    if(!ext)return;
    try {
      if(g.getParameter(ext.GPU_DISJOINT_EXT)) {
        stats.disjointEvents++;this.clearGpuTimerQueries();this.gpuTimerSkipFrame=this.frame;return;
      }
      while(this.gpuTimerPending.length) {
        const [query,frame]=this.gpuTimerPending[0];
        if(!g.getQueryParameter(query,g.QUERY_RESULT_AVAILABLE)) {
          if(this.frame-frame<=120)break;
          g.deleteQuery(query);this.gpuTimerPending.shift();stats.dropped++;continue;
        }
        const ns=g.getQueryParameter(query,g.QUERY_RESULT);
        g.deleteQuery(query);this.gpuTimerPending.shift();
        if(Number.isSafeInteger(ns)&&ns>=0) {
          stats.samples++;stats.results.push([frame,ns/1e6]);
          if(stats.results.length>120)stats.results.shift();
        } else stats.dropped++;
      }
      stats.pending=this.gpuTimerPending.length+(this.gpuTimerActive?1:0);
      if(this.gpuTimerActive||this.frame%6||this.gpuTimerSkipFrame===this.frame)return;
      if(this.gpuTimerPending.length>=4){stats.skippedStarts++;return;}
      const query=g.createQuery();if(!query){stats.skippedStarts++;return;}
      this.gpuTimerActive=[query,this.frame+1];g.beginQuery(ext.TIME_ELAPSED_EXT,query);stats.pending++;
    } catch(error) {this.disposeGpuTiming();this.report(`GPU timing disabled: ${error.message}`);}
  }
  endGpuTimer() {
    if(!this.gpuTimerActive)return;
    try {
      this.gl.endQuery(this.gpuTimerExtension.TIME_ELAPSED_EXT);
      this.gpuTimerPending.push(this.gpuTimerActive);this.gpuTimerActive=undefined;
      this.gpuTimingStats.pending=this.gpuTimerPending.length;
    } catch(error) {this.disposeGpuTiming();this.report(`GPU timing disabled: ${error.message}`);}
  }
  checkedHeapSpan(ptr, length) {
    const heap = this.memory().HEAPU8;
    if (!Number.isInteger(ptr) || !Number.isSafeInteger(length) || ptr < 0 || length < 0 || ptr + length > heap.length) throw new Error('GPU bridge span outside WASM memory');
    return heap;
  }
  bytes(ptr, length) { const heap=this.checkedHeapSpan(ptr,length);return heap.subarray(ptr,ptr+length); }
  wordView(ptr,n) { const heap=this.checkedHeapSpan(ptr,n*4);return new Uint32Array(heap.buffer,heap.byteOffset+ptr,n); }
  floatView(ptr,n) { const heap=this.checkedHeapSpan(ptr,n*4);return new Float32Array(heap.buffer,heap.byteOffset+ptr,n); }
  words(ptr,n) { return new Uint32Array(this.bytes(ptr,n*4).slice().buffer); }
  floats(ptr,n) { return new Float32Array(this.bytes(ptr,n*4).slice().buffer); }
  uploadImagePixels(image, width, height, rgba) {
    const g = this.gl;
    if (image.allocated) {
      g.texSubImage2D(g.TEXTURE_2D, 0, 0, 0, width, height, g.RGBA, g.UNSIGNED_BYTE, rgba);
    } else {
      g.texImage2D(g.TEXTURE_2D, 0, g.RGBA8, width, height, 0, g.RGBA, g.UNSIGNED_BYTE, rgba);
      image.allocated = true;
    }
  }
  uploadOrdinaryImage(image, encoding, pointer, width, height, size) {
    const bytes = encoding === 1 ? this.bytes(pointer, size) : null;
    if (bytes && this.directTextureUploads !== false) {
      // The bridge pins these immutable bytes until the command batch returns.
      try {
        this.uploadImagePixels(image, width, height, bytes);
        return true;
      } catch (error) {
        if (!(error instanceof TypeError) || typeof SharedArrayBuffer === 'undefined' ||
            !(bytes.buffer instanceof SharedArrayBuffer)) throw error;
        this.directTextureUploads = false;
      }
    }
    if (size > this.imageUpload.length) {
      this.imageUpload = new Uint8Array(this.capacity(this.imageUpload.length, size, 32 * 1024 * 1024));
    }
    const rgba = this.imageUpload.subarray(0, size);
    if (bytes) rgba.set(bytes);
    else {
      const pixels = this.floatView(pointer, size);
      for (let j = 0; j < size; j++) rgba[j] = Math.round(Math.max(0, Math.min(1, pixels[j])) * 255);
    }
    this.uploadImagePixels(image, width, height, rgba);
    return false;
  }
  capacity(current,required,limit) {
    if(required>limit)throw new Error('GPU geometry budget exceeded');
    let size=Math.max(current,256);
    while(size<required)size=Math.min(limit,size*2);
    return size;
  }
  uploadGeometry(target, storage, source, allowReuse) {
    const g = this.gl, size = source.byteLength;
    g.bindBuffer(target, storage.buffer);
    if (!size) return 0;
    if (size > storage.capacity) {
      storage.capacity = this.capacity(storage.capacity, size, storage.limit);
      g.bufferData(target, storage.capacity, g.STREAM_DRAW);
      storage.validWords = 0;
    }
    if (!allowReuse || size < MIN_GEOMETRY_REUSE_BYTES || size > storage.shadowLimit || (source.byteOffset & 3)) {
      g.bufferSubData(target, 0, source);
      storage.validWords = 0;
      return size;
    }

    // The snapshot owns the exact bytes last sent at these GPU offsets.
    // Restrict comparison to the valid prefix after shrinking or reallocating.
    const words = new Uint32Array(source.buffer, source.byteOffset, size / 4);
    if (!storage.snapshot || storage.snapshot.length < words.length) {
      storage.snapshot = new Uint32Array(Math.min(storage.capacity, storage.shadowLimit) / 4);
      storage.validWords = 0;
    }
    const snapshot = storage.snapshot;
    const overlap = Math.min(storage.validWords, words.length);
    let first = 0;
    while (first < overlap && words[first] === snapshot[first]) ++first;
    let end = words.length;
    if (end <= storage.validWords) {
      while (end > first && words[end - 1] === snapshot[end - 1]) --end;
    }
    if (first < end) {
      const changed = words.subarray(first, end);
      g.bufferSubData(target, first * 4, changed);
      snapshot.set(changed, first);
    }
    storage.validWords = words.length;
    return (end - first) * 4;
  }
  rawLayout(stride,words,mask) {
    if(!stride||stride>252||(stride&3))throw new Error('Unsupported raw vertex stride');
    let active=0;const attributes=new Array(16).fill(null);
    for(let i=0;i<16;i++){
      const word=words[i]>>>0;if(!word)continue;
      active|=1<<i;
      const format=word&255,offset=word>>>8,info=RAW_VERTEX_FORMATS.get(format);
      if(!info)throw new Error(`Unsupported raw DXGI vertex format ${format}`);
      const bytes=info.components*info.componentBytes;
      if(offset+bytes>stride||offset%info.componentBytes)throw new Error('Raw vertex attribute exceeds stride or alignment');
      attributes[i]={...info,offset,bytes};
    }
    if(active!==(mask&0xffff))throw new Error('Raw vertex layout does not match register mask');
    return attributes;
  }
  bindGeometry(binding) {
    if(this.currentGeometryBinding===binding)return;
    const g=this.gl;
    const group=binding.group||binding;
    const mask=binding.kind==='converted'?0xffff:group.mask;
    const changed=this.enabledAttributes===undefined?0xffff:this.enabledAttributes^mask;
    if(binding.kind==='converted'){
      for(let a=0;a<16;a++){
        if(changed&(1<<a))g.enableVertexAttribArray(a);
        g.vertexAttribPointer(a,4,g.FLOAT,false,16,a*binding.totalVertices*16);
      }
    }else{
      for(let a=0;a<16;a++){
        const attr=group.attributes[a],enableChanged=changed&(1<<a);
        if(!attr){
          if(enableChanged){g.disableVertexAttribArray(a);g.vertexAttrib4f(a,0,0,0,1);}
          continue;
        }
        if(enableChanged)g.enableVertexAttribArray(a);
        if(attr.integer)g.vertexAttribIPointer(a,attr.components,g[attr.type],group.stride,group.baseByte+attr.offset);
        else g.vertexAttribPointer(a,attr.components,g[attr.type],attr.normalized,group.stride,group.baseByte+attr.offset);
      }
    }
    this.enabledAttributes=mask;
    this.currentGeometryBinding=binding;
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
  compileShader(kind, source) {
    const g=this.gl,start=performance.now();
    const shader=g.createShader(kind);
    if(!shader)throw new Error('Could not allocate WebGL shader');
    g.shaderSource(shader,source);
    g.compileShader(shader);
    this.shaderStats.compileMs+=performance.now()-start;
    return shader;
  }
  linkStages(vertexShader,fragmentShader) {
    const g=this.gl,p=g.createProgram();
    if(!p)throw new Error('Could not allocate WebGL program');
    try {
      g.attachShader(p,vertexShader);
      g.attachShader(p,fragmentShader);
      const start=performance.now();
      g.linkProgram(p);
      this.shaderStats.linkMs+=performance.now()-start;
      return p;
    } catch(error) {
      g.deleteProgram(p);
      throw error;
    }
  }
  beginFixedFallbacks() {
    const g = this.gl;
    const programs = [];
    let smoothFragment;
    try {
      for (const flat of [false, true]) {
        let vertex, fragment;
        try {
          vertex = this.compileShader(g.VERTEX_SHADER, fixedFallbackVertexGlsl(flat));
          fragment = this.compileShader(g.FRAGMENT_SHADER, fixedFallbackPixelGlsl(flat));
          programs.push(this.linkStages(vertex, fragment));
          if (!flat) {
            smoothFragment = fragment;
            fragment = undefined;
          }
        } finally {
          if (vertex) g.deleteShader(vertex);
          if (fragment) g.deleteShader(fragment);
        }
      }
      g.flush();
      this.pendingFixedFallbackPrograms = {programs, smoothFragment};
    } catch (error) {
      for (const program of programs) g.deleteProgram(program);
      if (smoothFragment) g.deleteShader(smoothFragment);
      this.report(`Fixed shader fallback is unavailable: ${error.message}`);
    }
  }
  finishFixedFallbacks() {
    const pending = this.pendingFixedFallbackPrograms;
    if (!pending) return;
    this.pendingFixedFallbackPrograms = undefined;
    const {programs, smoothFragment} = pending;
    const g = this.gl, started = performance.now();
    // Both smooth programs hold their shader attachment until validation.
    g.deleteShader(smoothFragment);
    try {
      for (const program of programs) {
        if (!g.getProgramParameter(program, g.LINK_STATUS)) throw new Error(g.getProgramInfoLog(program));
      }
      this.shaderStats.fixedFallbackStartupWaitMs = performance.now() - started;
      this.fixedFallbackTexture = g.createTexture();
      if (!this.fixedFallbackTexture) throw new Error('Could not allocate the fallback texture');
      this.bindTexture(0, this.fixedFallbackTexture);
      g.texStorage2D(g.TEXTURE_2D, 1, g.RGBA8, 1, 1);
      this.fixedFallbackPrograms = programs.map(program => this.cacheDrawUniforms(this.finishProgram(program)));
      this.shaderStats.fixedFallbackEnabled = true;
    } catch (error) {
      if (this.fixedFallbackTexture) g.deleteTexture(this.fixedFallbackTexture);
      this.fixedFallbackTexture = undefined;
      for (const program of programs) g.deleteProgram(program);
      this.report(`Fixed shader fallback is unavailable: ${error.message}`);
    }
    this.finishProgrammableFallback();
  }
  beginProgrammableFallback(pointer) {
    const fragment = this.pendingFixedFallbackPrograms?.smoothFragment;
    if (!pointer || !fragment) return;
    const g = this.gl;
    let vertex, program;
    try {
      vertex = this.compileShader(g.VERTEX_SHADER, programmableFallbackVertexGlsl(this.string(pointer)));
      program = this.linkStages(vertex, fragment);
      g.flush();
      this.pendingProgrammableFallback = program;
    } catch (error) {
      if (program) g.deleteProgram(program);
      this.report(`Programmable shader fallback is unavailable: ${error.message}`);
    } finally {
      if (vertex) g.deleteShader(vertex);
    }
  }
  finishProgrammableFallback() {
    const g = this.gl, program = this.pendingProgrammableFallback;
    if (!program) return;
    this.pendingProgrammableFallback = undefined;
    if (!this.fixedFallbackTexture) {
      g.deleteProgram(program);
      return;
    }
    const started = performance.now();
    let code;
    try {
      if (!g.getProgramParameter(program, g.LINK_STATUS)) throw new Error(g.getProgramInfoLog(program));
      const block = g.getUniformBlockIndex(program, 'FallbackVertexCode');
      if (block === g.INVALID_INDEX) throw new Error('Vertex instruction block did not link');
      code = g.createBuffer();
      if (!code) throw new Error('Could not allocate the vertex instruction buffer');
      g.bindBufferBase(g.UNIFORM_BUFFER, 0, code);
      g.bufferData(g.UNIFORM_BUFFER, FALLBACK_VERTEX_CODE_BYTES, g.DYNAMIC_DRAW);
      g.uniformBlockBinding(program, block, 0);
      this.programmableFallback = this.cacheDrawUniforms(this.finishProgram(program, {codeBuffer:code}));
      this.shaderStats.programmableFallbackEnabled = true;
    } catch (error) {
      if (code) g.deleteBuffer(code);
      g.deleteProgram(program);
      this.report(`Programmable shader fallback is unavailable: ${error.message}`);
    } finally {
      this.shaderStats.programmableFallbackStartupWaitMs = performance.now() - started;
    }
  }
  prepareWarmup(store) {
    this.shaderWarmup=store;
    this.warmPrograms=new Map();
    this.warmVariants=new Map();
    const g=this.gl;
    const vertexShaders=new Map(),fragmentShaders=new Map();
    const compileStage=(cache,kind,source)=>{
      let shader=cache.get(source);
      if(!shader){
        shader=this.compileShader(kind,source);
        cache.set(source,shader);
      }
      return shader;
    };
    try {
      for (const [key,{recipe,drawLayout}] of store.recipes) {
        try {
          const [hlsl, pixel, kinds, shadows, flat, regs] = recipe;
          const vertexSource = hlsl === null ? this.vertexStage(0, regs, flat).source : vertexGlsl(hlsl, regs, flat);
          const fragmentSource = pixelGlsl(pixel, kinds, shadows, flat);
          const programKey = JSON.stringify([vertexSource, fragmentSource]);
          if (!this.warmPrograms.has(programKey)) {
            const vertexShader=compileStage(vertexShaders,g.VERTEX_SHADER,vertexSource);
            const fragmentShader=compileStage(fragmentShaders,g.FRAGMENT_SHADER,fragmentSource);
            const p=this.linkStages(vertexShader,fragmentShader);
            this.warmPrograms.set(programKey,{p,vertexShader,fragmentShader,drawLayout});
            this.shaderStats.warmupLinks++;
          } else if (drawLayout) this.warmPrograms.get(programKey).drawLayout = drawLayout;
          this.warmVariants.set(key, programKey);
        } catch { /* The exact runtime sources still use the checked draw path. */ }
      }
    } finally {
      // Pending programs own attachments until startup reflects or deletes them.
      for (const shader of vertexShaders.values()) g.deleteShader(shader);
      for (const shader of fragmentShaders.values()) g.deleteShader(shader);
    }
    if(this.warmPrograms.size)g.flush();
  }
  async finishWarmup(cancelled) {
    if (!this.warmPrograms?.size) return;
    const g = this.gl;
    const start = performance.now();
    const deadline = start + WARMUP_STARTUP_BUDGET_MS;
    const pending = new Set(Array.from(this.warmPrograms.keys()).reverse());
    // Finish known links while the loader is visible. Only completed,
    // reflected programs may enter the gameplay cache.
    while (pending.size && !cancelled() && !this.contextLost && !g.isContextLost()) {
      const pollStart = performance.now();
      for (const key of pending) {
        if (performance.now() >= deadline) break;
        const {p, vertexShader, fragmentShader, drawLayout} = this.warmPrograms.get(key);
        if (!g.getProgramParameter(p, this.parallelShaderCompile.COMPLETION_STATUS_KHR)) continue;
        pending.delete(key);
        if (!g.getProgramParameter(p, g.LINK_STATUS)) {
          g.deleteProgram(p);
          this.warmPrograms.delete(key);
          this.shaderStats.warmupSkipped++;
          continue;
        }
        g.detachShader(p, vertexShader);
        g.detachShader(p, fragmentShader);
        this.warmPrograms.set(key, this.cacheDrawUniforms(this.finishProgram(p, {drawLayout})));
        this.shaderStats.warmupPrimed++;
      }
      this.shaderStats.warmupPrimeMs += performance.now() - pollStart;
      const remaining = deadline - performance.now();
      if (!pending.size || remaining <= 0) break;
      await new Promise(resolve => setTimeout(resolve, Math.min(WARMUP_POLL_INTERVAL_MS, remaining)));
    }
    for (const key of pending) {
      g.deleteProgram(this.warmPrograms.get(key).p);
      this.warmPrograms.delete(key);
      this.shaderStats.warmupSkipped++;
    }
    for (const [key, programKey] of this.warmVariants)
      if (!this.warmPrograms.has(programKey)) this.warmVariants.delete(key);
    this.shaderStats.warmupWaitMs = performance.now() - start;
    await this.warmStartupDraws(cancelled);
  }
  async warmStartupDraws(cancelled) {
    const programs = Array.from(this.warmPrograms.values()).reverse().filter(({drawLayout}) =>
      drawLayout?.slice(1).every(format => format === 0 || RAW_VERTEX_FORMATS.has(format)));
    if (!programs.length || cancelled() || this.contextLost) return;
    const g = this.gl, start = performance.now(), deadline = start + WARMUP_STARTUP_BUDGET_MS;
    this.checkGpuError();
    const vao = g.createVertexArray(), vertices = g.createBuffer(), indices = g.createBuffer();
    const fbo = g.createFramebuffer(), color = g.createRenderbuffer(), depth = g.createRenderbuffer();
    const texture = g.createTexture();
    let fence;
    try {
      if (!vao || !vertices || !indices || !fbo || !color || !depth || !texture)
        throw new Error('Could not allocate WebGL warmup resources');
      g.bindVertexArray(vao);
      g.bindBuffer(g.ARRAY_BUFFER, vertices);
      g.bufferData(g.ARRAY_BUFFER, 16 * 3 * 16, g.STATIC_DRAW);
      g.bindBuffer(g.ELEMENT_ARRAY_BUFFER, indices);
      g.bufferData(g.ELEMENT_ARRAY_BUFFER, Uint32Array.of(0, 1, 2), g.STATIC_DRAW);
      g.bindFramebuffer(g.FRAMEBUFFER, fbo);
      g.bindRenderbuffer(g.RENDERBUFFER, color);
      g.renderbufferStorage(g.RENDERBUFFER, g.RGBA8, 1, 1);
      g.bindRenderbuffer(g.RENDERBUFFER, depth);
      g.renderbufferStorage(g.RENDERBUFFER, g.DEPTH24_STENCIL8, 1, 1);
      for (let unit = 0; unit < 4; unit++) {
        g.activeTexture(g.TEXTURE0 + unit);
        g.bindTexture(g.TEXTURE_2D, texture);
      }
      g.texStorage2D(g.TEXTURE_2D, 1, g.RGBA8, 1, 1);

      // ANGLE can specialize for actual vertex and framebuffer layouts on
      // the first draw, after WebGL already reports link completion.
      let attachments = 0;
      for (let index = 0; index < programs.length;) {
        if (cancelled() || this.contextLost || performance.now() >= deadline) break;
        const end = Math.min(index + WARMUP_DRAW_BATCH, programs.length);
        for (; index < end; index++) {
          const {p, drawLayout, locations} = programs[index];
          if (attachments !== drawLayout[0]) {
            attachments = drawLayout[0];
            g.framebufferRenderbuffer(g.FRAMEBUFFER, g.COLOR_ATTACHMENT0, g.RENDERBUFFER, attachments & 1 ? color : null);
            g.framebufferRenderbuffer(g.FRAMEBUFFER, g.DEPTH_STENCIL_ATTACHMENT, g.RENDERBUFFER, attachments & 2 ? depth : null);
            const drawBuffer = attachments & 1 ? g.COLOR_ATTACHMENT0 : g.NONE;
            g.drawBuffers([drawBuffer]);
            g.readBuffer(drawBuffer);
          }
          for (let attribute = 0; attribute < 16; attribute++) {
            const format = RAW_VERTEX_FORMATS.get(drawLayout[attribute + 1]);
            if (!format) { g.disableVertexAttribArray(attribute); continue; }
            g.enableVertexAttribArray(attribute);
            const offset = attribute * 3 * 16;
            if (format.integer) g.vertexAttribIPointer(attribute, format.components, g[format.type], 16, offset);
            else g.vertexAttribPointer(attribute, format.components, g[format.type], format.normalized, 16, offset);
          }
          g.useProgram(p);
          for (let unit = 0; unit < 4; unit++) g.uniform1i(locations.get(`tex${unit}`), unit);
          g.drawElements(g.TRIANGLES, 3, g.UNSIGNED_INT, 0);
        }
        fence = g.fenceSync(g.SYNC_GPU_COMMANDS_COMPLETE, 0);
        if (!fence) throw new Error('Could not fence WebGL warmup draws');
        g.flush();
        // The budget limits new submissions. Drain each submitted batch so
        // its backend work cannot carry over into the first gameplay frame.
        for (;;) {
          if (cancelled() || this.contextLost || g.isContextLost()) return;
          const status = g.clientWaitSync(fence, 0, 0);
          if (status === g.WAIT_FAILED) throw new Error('WebGL warmup fence failed');
          if (status !== g.TIMEOUT_EXPIRED) break;
          await new Promise(resolve => setTimeout(resolve, WARMUP_POLL_INTERVAL_MS));
        }
        this.checkGpuError();
        g.deleteSync(fence);
        fence = undefined;
        this.shaderStats.warmupDraws = index;
      }
    } catch (error) {
      this.report(`WebGL draw warmup stopped: ${error.message}`);
    } finally {
      if (fence) g.deleteSync(fence);
      g.useProgram(null);
      g.bindVertexArray(null);
      g.bindBuffer(g.ARRAY_BUFFER, null);
      g.bindFramebuffer(g.FRAMEBUFFER, null);
      g.bindRenderbuffer(g.RENDERBUFFER, null);
      for (let unit = 0; unit < 4; unit++) {
        g.activeTexture(g.TEXTURE0 + unit);
        g.bindTexture(g.TEXTURE_2D, this.boundTextures[unit] ?? null);
      }
      g.activeTexture(g.TEXTURE0 + this.activeUnit);
      g.deleteVertexArray(vao);
      g.deleteBuffer(vertices);
      g.deleteBuffer(indices);
      g.deleteFramebuffer(fbo);
      g.deleteRenderbuffer(color);
      g.deleteRenderbuffer(depth);
      g.deleteTexture(texture);
      this.shaderStats.warmupDrawMs = performance.now() - start;
    }
  }
  takeWarmProgram(key) {
    const program = this.warmPrograms?.get(key);
    if (!program) return null;
    this.warmPrograms.delete(key);
    this.programs.set(key, program);
    this.shaderStats.warmupHits++;
    this.shaderStats.readyWithoutWait++;
    this.shaderStats.completedPrograms++;
    return program;
  }
  checkGpuError() {
    const error = this.gl.getError();
    if (error !== this.gl.NO_ERROR) throw new Error(`GPU command batch error 0x${error.toString(16)}`);
  }
  cancelFrameErrorCheck() {
    const check = this.pendingFrameErrorCheck;
    if (!check) return;
    this.pendingFrameErrorCheck = undefined;
    clearTimeout(check.timer);
    this.gl.deleteSync(check.fence);
  }
  finishFrameErrorCheck() {
    if (this.frameError) throw this.frameError;
    if (!this.pendingFrameErrorCheck) return;
    this.cancelFrameErrorCheck();
    this.checkGpuError();
  }
  scheduleFrameErrorCheck(onError) {
    const g = this.gl;
    const fence = g.fenceSync(g.SYNC_GPU_COMMANDS_COMPLETE, 0);
    if (!fence) {
      this.deferGpuErrors = false;
      this.cancelFrameErrorCheck();
      this.checkGpuError();
      return;
    }
    const pending = this.pendingFrameErrorCheck;
    if (pending) {
      // The newest fence covers all earlier commands in this context. Keep
      // the first poll and deadline so busy frames cannot postpone checks.
      g.deleteSync(pending.fence);
      pending.fence = fence;
      return;
    }
    const check = {fence, deadline: performance.now() + FRAME_ERROR_CHECK_DELAY_MS, timer: 0};
    const poll = () => {
      if (this.pendingFrameErrorCheck !== check) return;
      try {
        const status = g.clientWaitSync(check.fence, 0, 0);
        if (status === g.TIMEOUT_EXPIRED && performance.now() < check.deadline) {
          check.timer = setTimeout(poll, 4);
          return;
        }
        this.finishFrameErrorCheck();
        if (status === g.WAIT_FAILED) throw new Error('GPU frame validation fence failed');
        if (status !== g.TIMEOUT_EXPIRED) this.stepShaderPromotion();
      } catch (error) {
        this.frameError = error;
        this.cancelFrameErrorCheck();
        this.disposeGpuTiming();
        onError(error);
      }
    };
    this.pendingFrameErrorCheck = check;
    check.timer = setTimeout(poll, 4);
  }
  pipelinedFrame(ptr, count, id, onError) {
    const deferred = this.deferGpuErrors;
    if (count && !this.commands(ptr, count, !deferred)) return false;
    return this.present(id, deferred ? onError : undefined);
  }
  commands(ptr,count,checkErrors=true) {
    try {
    const started=performance.now();
    if(checkErrors)this.finishFrameErrorCheck();
    else if(this.frameError)throw this.frameError;
    if(count>65536)throw new Error('GPU command budget exceeded');
    const words=this.wordView(ptr,count*12);
    let prepassPacketReuses = 0;
    if(this.parallelShaderCompile){
      const prewarmStart=performance.now(),heapBytes=this.memory().HEAPU8,heapWords=new Uint32Array(heapBytes.buffer,heapBytes.byteOffset,heapBytes.byteLength>>>2);this.shaderPrewarmSeen.clear();this.shaderPrewarmPending.clear();let prewarmAttempts=0,pendingPairCount=0;
      // A previously discovered pair stays discovered until this scan ends.
      // State packet addresses are safe to reuse only within this batch.
      let discoveredSource = -1, discoveredState = -1;
      for(let i=0;i<count;i++){
        const b=i*12,opcode=words[b];if(opcode!==1&&opcode!==3)continue;
        try{
          const sourcePointer=words[b+8],statePointer=words[b+6];
          if (sourcePointer === discoveredSource && statePointer === discoveredState) {
            ++prepassPacketReuses;
            continue;
          }
          if((statePointer&3)||statePointer>heapBytes.length-160)continue;
          const stateOffset=statePointer>>>2,flat=!!heapWords[stateOffset+32],vertexPointer=heapWords[stateOffset+37],regs=heapWords[stateOffset+39]&0xffff,signature=vertexPointer*131072+(flat?65536:0)+regs;
          if(this.shaderPrewarmDiscovered.get(sourcePointer)?.has(signature)) {
            discoveredSource = sourcePointer;
            discoveredState = statePointer;
            continue;
          }
          let pending=this.shaderPrewarmPending.get(sourcePointer);
          if(!pending){if(this.shaderPrewarmPairCount+pendingPairCount>=2048)continue;pending=new Set();this.shaderPrewarmPending.set(sourcePointer,pending);}
          if(!pending.has(signature)){if(this.shaderPrewarmPairCount+pendingPairCount>=2048)continue;pending.add(signature);pendingPairCount++;}
          if(prewarmAttempts>=128)continue;
          const texturePointer=words[b+9];if((texturePointer&3)||texturePointer>heapBytes.length-224)continue;
          const textureOffset=texturePointer>>>2,t0=heapWords[textureOffset],t1=heapWords[textureOffset+14],t2=heapWords[textureOffset+28],t3=heapWords[textureOffset+42],s0=heapWords[textureOffset+1],s1=heapWords[textureOffset+15],s2=heapWords[textureOffset+29],s3=heapWords[textureOffset+43],identity=`${sourcePointer}:${vertexPointer}:${t0},${t1},${t2},${t3}:${s0},${s1},${s2},${s3}:${flat}:${regs}`;
          if(this.pointerPrograms?.has(identity)||this.shaderPrewarmSeen.has(identity))continue;
          this.shaderPrewarmSeen.add(identity);
          prewarmAttempts++;
          const source=this.string(sourcePointer),before=this.programs.size;
          const kinds=[t0,t1,t2,t3],shadows=[s0,s1,s2,s3],variant=this.programVariant(source,kinds,shadows,flat,vertexPointer,regs);
          this.pointerPrograms??=new Map();this.pointerPrograms.set(identity,variant);
          if(this.programs.size!==before)this.shaderStats.prepassPrograms++;
        }catch{/* Preserve the original error point at the command's draw. */}
      }
      for(const [sourcePointer,signatures] of this.shaderPrewarmPending){let discovered=this.shaderPrewarmDiscovered.get(sourcePointer);if(!discovered){discovered=new Set();this.shaderPrewarmDiscovered.set(sourcePointer,discovered);}for(const signature of signatures){if(this.shaderPrewarmPairCount>=2048)break;if(!discovered.has(signature)){discovered.add(signature);this.shaderPrewarmPairCount++;}}}
      this.shaderStats.prepassMs+=performance.now()-prewarmStart;
    }
    const packingStarted=this.measurePacing?performance.now():0;
    // Keep converted float planes first; append raw, interleaved layout groups.
    let convertedVertices=0,rawBytes=0,indexCount=0,rawStart=0,contiguousRaw=true;
    for(let i=0;i<count;i++){
      const b=i*12,opcode=words[b];
      if(opcode===1){convertedVertices+=words[b+2];indexCount+=words[b+5];}
      else if(opcode===3){
        const stride=words[b+3];if(!stride||stride>252||(stride&3))throw new Error('Unsupported raw vertex stride');
        if(!rawBytes)rawStart=words[b+1];
        if(words[b+1]!==rawStart+rawBytes)contiguousRaw=false;
        rawBytes+=words[b+2]*stride;indexCount+=words[b+5];
        if(rawBytes>256*1024*1024||indexCount>16*1024*1024)throw new Error('GPU geometry budget exceeded');
      }
      if(convertedVertices*256+rawBytes>256*1024*1024||indexCount>16*1024*1024)throw new Error('GPU geometry budget exceeded');
    }
    const convertedBytes=convertedVertices*256,vertexBytes=convertedBytes+rawBytes;
    if(vertexBytes>256*1024*1024||indexCount>16*1024*1024)throw new Error('GPU geometry budget exceeded');
    // A raw-only batch already owns the complete upload span until consumption ends.
    const directVertices=rawBytes>0&&!convertedBytes&&contiguousRaw;
    if(!directVertices&&(!this.vertexStaging||this.vertexStaging.length<vertexBytes))this.vertexStaging=new Uint8Array(this.capacity(this.vertexStaging?.length||0,vertexBytes,256*1024*1024));
    if(!this.indexStaging||this.indexStaging.length<indexCount)this.indexStaging=new Uint32Array(this.capacity(this.indexStaging?.length||0,indexCount,16*1024*1024));
    const vertices=directVertices?this.bytes(rawStart,rawBytes):this.vertexStaging.subarray(0,vertexBytes);
    const elements=this.indexStaging.subarray(0,indexCount);
    if(convertedBytes)vertices.fill(0,0,convertedBytes);
    const vertexWords=convertedBytes?new Uint32Array(vertices.buffer,vertices.byteOffset,vertices.byteLength/4):null;
    const bindings=this.drawBindings??=[];
    bindings.length=count;
    let convertedOffset=0,rawOffset=convertedBytes,io=0,rawGroup=null;
    let rawLayoutReuses=0;
    const convertedBinding={kind:'converted',totalVertices:convertedVertices};
    for(let i=0;i<count;i++){
      const b=i*12,opcode=words[b];
      if(opcode===1){
        rawGroup=null;
        const vertexCount=words[b+2],sourceStride=words[b+3],metadata=this.wordView(words[b+6],40),raw=!!metadata[37],mask=metadata[39];
        this.frameStats??={gpuVertexDraws:0,cpuVertexDraws:0,gpuVertices:0,cpuVertices:0,vertexBytes:0,rawStreamDraws:0,rawStreamBytes:0};
        this.frameStats.rawStreamDraws??=0;this.frameStats.rawStreamBytes??=0;
        this.frameStats[raw?'gpuVertexDraws':'cpuVertexDraws']++;
        this.frameStats[raw?'gpuVertices':'cpuVertices']+=vertexCount;this.frameStats.vertexBytes+=vertexCount*256;
        if(!raw&&sourceStride!==160)throw new Error('Unexpected SDK shaded vertex stride');
        let registers=0;for(let r=0;r<16;r++)if(mask&(1<<r))registers++;
        if(raw&&sourceStride!==registers*16)throw new Error('Unexpected raw vertex register span');
        const sourceBytes=this.bytes(words[b+1],vertexCount*sourceStride);
        const input=new Uint32Array(sourceBytes.buffer,sourceBytes.byteOffset,sourceBytes.length/4);
        let slot=0;
        for(let r=0;r<(raw?16:10);r++)if(!raw||(mask&(1<<r))){
          let from=slot++*4,to=(r*convertedVertices+convertedOffset/256)*4;
          for(let v=0;v<vertexCount;v++,from+=sourceStride/4,to+=4){vertexWords[to]=input[from];vertexWords[to+1]=input[from+1];vertexWords[to+2]=input[from+2];vertexWords[to+3]=input[from+3];}
        }
        const indexCountForDraw=words[b+5],order=this.wordView(words[b+4],indexCountForDraw),base=convertedOffset/256,flat=metadata[32];
        for(let j=0;j<order.length;j++){
          const lane=flat&&j-j%3+2<order.length?j-j%3+(j%3+1)%3:j;
          const index=order[lane];if(index>=vertexCount)throw new Error('GPU index exceeds vertex span');elements[io+j]=index+base;
        }
        bindings[i]=convertedBinding;
        convertedOffset+=vertexCount*256;io+=indexCountForDraw;
      }else if(opcode===3){
        const vertexCount=words[b+2],stride=words[b+3],layoutPtr=words[b+10],indexCountForDraw=words[b+5];if(!layoutPtr)throw new Error('Missing raw vertex layout');
        const state=this.wordView(words[b+6],40),mask=state[39]&0xffff;
        let same=!!rawGroup&&rawGroup.stride===stride;
        // Equal interned pointers refer to the same owned layout in this batch.
        const interned=same&&rawGroup.layoutPtr===layoutPtr;
        let layout;
        if(interned){
          ++rawLayoutReuses;
        }else{
          layout=this.wordView(layoutPtr,16);
          if(same)for(let r=0;r<16;r++)if(rawGroup.layout[r]!==layout[r]){same=false;break;}
        }
        if(same){
          if(rawGroup.mask!==mask)throw new Error('Raw vertex layout does not match register mask');
          if(!interned)rawGroup.layoutPtr=layoutPtr;
        }else{
          rawGroup={kind:'raw',baseByte:rawOffset,stride,mask,layoutPtr,layout:layout.slice(),
            attributes:this.rawLayout(stride,layout,mask),vertexCount:0};
        }
        const base=rawGroup.vertexCount,sourceSize=vertexCount*stride;
        if(!directVertices)vertices.set(this.bytes(words[b+1],sourceSize),rawOffset);
        const order=this.wordView(words[b+4],indexCountForDraw),flat=state[32];
        for(let j=0;j<order.length;j++){
          const lane=flat&&j-j%3+2<order.length?j-j%3+(j%3+1)%3:j;
          const index=order[lane];if(index>=vertexCount)throw new Error('GPU index exceeds raw vertex span');elements[io+j]=index+base;
        }
        this.frameStats??={gpuVertexDraws:0,cpuVertexDraws:0,gpuVertices:0,cpuVertices:0,vertexBytes:0,rawStreamDraws:0,rawStreamBytes:0};
        this.frameStats.rawStreamDraws??=0;this.frameStats.rawStreamBytes??=0;
        const drawCalls=1+words[b+11];
        this.frameStats.rawStreamDraws+=drawCalls;this.frameStats.rawStreamBytes+=sourceSize;
        this.frameStats.rawStreamRows=(this.frameStats.rawStreamRows??0)+1;
        this.frameStats.vertexBytes+=sourceSize;
        this.frameStats.gpuVertexDraws+=drawCalls;this.frameStats.gpuVertices+=vertexCount;
        rawGroup.vertexCount+=vertexCount;rawOffset+=sourceSize;
        bindings[i]=rawGroup;
        io+=indexCountForDraw;
      }else bindings[i]=null;
    }
    if(rawLayoutReuses)this.frameStats.rawLayoutReuses=(this.frameStats.rawLayoutReuses??0)+rawLayoutReuses;
    if(directVertices)this.frameStats.directVertexBytes=(this.frameStats.directVertexBytes??0)+rawBytes;
    if(this.measurePacing&&this.frameStats)this.frameStats.geometryPackingMs=(this.frameStats.geometryPackingMs??0)+performance.now()-packingStarted;
    if(this.gpuTimerExtension)this.beginGpuTimer();
    const g=this.gl;g.bindVertexArray(this.vao);
    const uploadStarted = this.measurePacing ? performance.now() : 0;
    const vertexUpload = this.uploadGeometry(g.ARRAY_BUFFER, this.vertexBuffer, vertices, directVertices);
    const indexUpload = this.uploadGeometry(g.ELEMENT_ARRAY_BUFFER, this.indexBuffer, elements, directVertices);
    if (this.measurePacing && this.frameStats) {
      const stats = this.frameStats;
      stats.gpuVertexUploadBytes = (stats.gpuVertexUploadBytes ?? 0) + vertexUpload;
      stats.gpuIndexUploadBytes = (stats.gpuIndexUploadBytes ?? 0) + indexUpload;
      stats.geometryUploadMs = (stats.geometryUploadMs ?? 0) + performance.now() - uploadStarted;
    }
    // The bridge owns immutable uniform packets until this batch completes.
    // Its arena can reuse the same addresses in the next batch.
    const uniformBatch = {};
    // Consecutive draws may share a bridge-owned state packet. Keep its view
    // local to this command batch because the arena can reuse its address.
    const rasterPacket = {pointer: -1, words: null, reuses: 0};
    // Ordinary images stay in the cache until presentation, outside this batch.
    const imageSlots = Array.from({length: 4}, () => ({
      address: 0, width: 0, height: 0, encoding: 0, image: null,
    }));
    let pendingRow=-1,pendingCount=0,pendingBinding,pendingOffset=0,indexOffset=0;
    const emit=()=>{
      if(pendingRow<0)return;
      const b=pendingRow,layout=words[b]===3?words[b+10]:0;
      if(!this.draw(pendingCount,words[b+6],words[b+7],words[b+8],words[b+9],layout,pendingBinding,pendingOffset,uniformBatch,imageSlots,rasterPacket))throw new Error('GPU draw rejected');
      pendingRow=-1;
    };
    for(let i=0;i<count;i++) {
      const b=i*12,opcode=words[b];
      if(opcode===1||opcode===3) {
        // Packing writes indices in this same row order. SubmitDraw interns
        // byte-identical consecutive state, uniform and texture packets.
        const binding=bindings[i],n=words[b+5],offset=indexOffset;
        indexOffset+=n*4;
        if(pendingRow>=0&&words[pendingRow]===opcode&&pendingBinding===binding&&words[pendingRow+8]===words[b+8]&&
           words[pendingRow+6]===words[b+6]&&words[pendingRow+7]===words[b+7]&&words[pendingRow+9]===words[b+9]) {
          pendingCount+=n;continue;
        }
        emit();pendingRow=b;pendingCount=n;pendingBinding=binding;pendingOffset=offset;continue;
      }
      emit();
      if(opcode===0)this.target(words[b+1],words[b+2],words[b+3],words[b+4],words[b+5],words[b+6],words[b+7],words[b+8],words[b+9],words[b+10],words[b+11]);
      else if(opcode===2){this.commandFloatBits[0]=words[b+7];this.clear(words[b+1],words[b+2],words[b+3],words[b+4],words[b+5],words[b+6],this.commandFloatValue[0],words[b+8],words[b+9]);}
      else throw new Error('Unknown GPU command');
    }
    emit();
    if (rasterPacket?.reuses && this.frameStats)
      this.frameStats.rasterPacketReuses = (this.frameStats.rasterPacketReuses ?? 0) + rasterPacket.reuses;
    if (prepassPacketReuses && this.frameStats)
      this.frameStats.prepassPacketReuses = (this.frameStats.prepassPacketReuses ?? 0) + prepassPacketReuses;
    if(checkErrors)this.checkGpuError();
    this.submitMs=(this.submitMs||0)+performance.now()-started;
    return true;
    } catch(error) {this.disposeGpuTiming();this.report(error.message);postMessage({type:'gpu-error',text:error.message});return false;}
  }
  vertexStage(pointer,regs,flat) {
    if(!pointer){
      if(!flat)return this.defaultVertexStage;
      return this.flatVertexStage??={source:this.vertexSource.replace(/out vec4 v([DB][01])/g,'flat out vec4 v$1')};
    }
    const key=`${pointer}:${flat}:${regs}`;
    let stage=this.vertexPrograms.get(key);
    if(!stage){
      if(this.vertexPrograms.size>=MAX_LINKED_PROGRAMS)throw new Error('GPU vertex shader cache budget exceeded');
      const hlsl=this.string(pointer);
      stage={hlsl};
      if (this.fixedFallbackPrograms) stage.fixedKey = fixedShaderKey(hlsl, 'vertex', 11);
      if (this.programmableFallback)
        stage.fallbackCode = decodeFallbackVertexCode(hlsl, regs, flat);
      this.vertexPrograms.set(key,stage);
    }
    return stage;
  }
  programVariant(source,kinds,shadows,flat,vertexPointer,regs) {
    const key=source+':'+kinds+':'+shadows+':'+flat+':'+vertexPointer+':'+regs;
    const cached = this.programVariants.get(key);
    if (cached) return cached;
    const programLimit = MAX_LINKED_PROGRAMS - (this.fixedFallbackPrograms?.length ?? 0)
      - Number(Boolean(this.programmableFallback));
    if (this.programVariants.size >= programLimit) throw new Error('GPU shader variant budget exceeded');
    const vertex=this.vertexStage(vertexPointer,regs,flat);
    const recipe = this.shaderWarmup && [vertex.hlsl ?? null, source, kinds, shadows, flat, regs];
    const recipeKey = this.warmVariants?.size ? JSON.stringify(recipe) : undefined;
    const warmKey = this.warmVariants?.get(recipeKey);
    let specialized;
    if (warmKey !== undefined) {
      const live = this.programs.get(warmKey);
      if (live?.drawUniformsReady) {
        specialized = live;
        this.shaderStats.programReuses++;
      } else if (!live) specialized = this.takeWarmProgram(warmKey);
    }
    let fallback;
    const fixedVertex = vertex.fixedKey && vertex.fixedKey[0] === regs && !vertex.fixedKey[8] && !vertex.fixedKey[9];
    if (!specialized && this.fixedFallbackTexture && (fixedVertex || vertex.fallbackCode) &&
        kinds.every(kind => kind <= 1) && shadows.every(shadow => !shadow)) {
      const pixelKey = fixedShaderKey(source, 'pixel', FIXED_PIXEL_KEY_WORDS);
      const textureMask = kinds.reduce((mask, kind, stage) => mask | kind << stage, 0);
      if (pixelKey?.[4] === textureMask) {
        fallback = {
          pipeline: fixedVertex ? this.fixedFallbackPrograms[Number(flat)] : this.programmableFallback,
          vertexKey: vertex.fixedKey, vertexCode: vertex.fallbackCode, pixelKey:prepareFixedFallbackPixelKey(pixelKey)
        };
      }
    }
    const request = !specialized && {vertex, source, kinds, shadows, vertexPointer, regs, flat, programLimit};
    if (fallback) this.shaderStats.residentVariants++;
    else if (request) specialized = this.translateProgram(request);
    const variant = {specialized, fallback, recipe, recipeKey};
    if (fallback && this.shaderPromotionCandidates)
      variant.promotion = {variant, request, frame:-2, indices:0};
    this.programVariants.set(key, variant);
    return variant;
  }
  translateProgram({vertex, source, kinds, shadows, vertexPointer, regs, flat, programLimit}, background = false) {
    const start = performance.now();
    const fragmentSource = pixelGlsl(source, kinds, shadows, flat);
    if (vertex.source === undefined) {
      const {hlsl} = vertex;
      try {
        vertex.source = vertexGlsl(hlsl, regs, flat);
      } catch (error) {
        if (!background) postMessage({type:'vertex-diagnostic',hlsl,regs,flat});
        throw error;
      }
    }
    if (!background) {
      this.shaderStats.inlineTranslations++;
      this.shaderStats.inlineTranslationMaxMs = Math.max(this.shaderStats.inlineTranslationMaxMs, performance.now() - start);
    }
    return this.specializedProgram(vertex, fragmentSource, {vertexPointer, regs, flat}, programLimit, background);
  }
  cancelShaderPromotion() {
    this.shaderPromotionCandidates = undefined;
    this.activeShaderPromotion = undefined;
  }
  stepShaderPromotion() {
    // Link completion alone does not keep uniform reflection from waiting
    // on earlier draws. Run one step after the latest frame fence signals.
    const candidates = this.shaderPromotionCandidates;
    if (!candidates || this.contextLost || this.frameError) return;
    if (this.frame < this.shaderPromotionResumeFrame) {
      if (this.activeShaderPromotion || candidates.size) this.shaderStats.promotionUploadDeferrals++;
      return;
    }
    let job = this.activeShaderPromotion;
    if (!job) {
      // Prefer the busiest resident shader in the last completed frame.
      // A material that went away can qualify again when its draws return.
      for (const candidate of candidates) {
        if (candidate.frame !== this.frame - 1) candidates.delete(candidate);
        else if (!job || candidate.indices > job.indices) job = candidate;
      }
      if (!job) return;
      candidates.delete(job);
      job.variant.promotion = undefined;
      this.activeShaderPromotion = job;
    }
    const start = performance.now(), g = this.gl;
    try {
      if (!job.program) {
        job.program = this.translateProgram(job.request, true);
        job.request = undefined;
        if (job.program.pending) {
          g.flush();
          return;
        }
      }
      const program = this.readyProgram(job.program, false);
      if (!program) return;
      if (!program.drawUniformsReady) this.cacheDrawUniforms(program);
      job.variant.specialized = program;
      job.variant.fallback = undefined;
      this.shaderStats.promotedVariants++;
    } catch {
      // The already drawing resident program remains usable.
      this.shaderStats.promotionFailures++;
    } finally {
      this.shaderStats.promotionMaxStepMs = Math.max(this.shaderStats.promotionMaxStepMs, performance.now() - start);
    }
    this.activeShaderPromotion = undefined;
  }
  specializedProgram(vertex, fragmentSource, diagnostic, programLimit, background) {
    const key = JSON.stringify([vertex.source, fragmentSource]);
    const cached = this.programs.get(key);
    if (cached) {
      this.shaderStats.programReuses++;
      return cached;
    }
    if (this.programs.size >= programLimit) throw new Error('GPU shader cache budget exceeded');
    const g=this.gl;
    const warm = this.takeWarmProgram(key);
    if (warm) return warm;
    if (this.warmPrograms && this.programs.size + this.warmPrograms.size >= programLimit) {
      const unused = this.warmPrograms.keys().next().value;
      g.deleteProgram(this.warmPrograms.get(unused).p);
      this.warmPrograms.delete(unused);
    }
    let fragmentShader = this.fragmentShaders.get(fragmentSource);
    if (fragmentShader) {
      this.shaderStats.fragmentReuses++;
    } else {
      if (this.fragmentShaders.size >= MAX_LINKED_PROGRAMS) throw new Error('GPU fragment shader cache budget exceeded');
      fragmentShader = this.compileShader(g.FRAGMENT_SHADER, fragmentSource);
      this.fragmentShaders.set(fragmentSource, fragmentShader);
      this.shaderStats.fragmentCompiles++;
    }
    const vertexShader = vertex.shader ??= this.compileShader(g.VERTEX_SHADER, vertex.source);
    const p = this.linkStages(vertexShader, fragmentShader);
    const program = {pending:true, p, vertex, vertexShader, fragmentShader, fragmentSource, ...diagnostic};
    this.programs.set(key, program);
    this.shaderStats[background ? 'backgroundLinks' : 'liveLinks']++;
    return program;
  }
  finishProgram(p,result={}) {
    const g=this.gl;
    const uniforms=new Float32Array(152),uniformWords=new Uint32Array(uniforms.buffer),uniformSpans=new Map();
    for(const [name,offset,count] of PIXEL_UNIFORMS)
      uniformSpans.set(name,new Float32Array(uniforms.buffer,offset*4,count));
    const vertexConstants=new Float32Array(772),vertexConstantWords=new Uint32Array(vertexConstants.buffer);
    const vertexConstantSpans={c:new Float32Array(vertexConstants.buffer,0,768),fogp:new Float32Array(vertexConstants.buffer,768*4,4)};
    const fixedVertexConstants=new Float32Array(772),fixedVertexConstantWords=new Uint32Array(fixedVertexConstants.buffer);
    const fixedVertexSpans={
      wvp:new Float32Array(fixedVertexConstants.buffer,0,16),wv:new Float32Array(fixedVertexConstants.buffer,16*4,16),
      wvn:new Float32Array(fixedVertexConstants.buffer,32*4,16),tm:new Float32Array(fixedVertexConstants.buffer,48*4,64),
      mat:new Float32Array(fixedVertexConstants.buffer,112*4,20),amb:new Float32Array(fixedVertexConstants.buffer,132*4,4),
      lt:new Float32Array(fixedVertexConstants.buffer,136*4,224),
    };
    Object.assign(result,{p,locations:new Map(),uniforms,uniformWords,uniformSpans,uniformsValid:false,
      uniformBatch:null,uniformPacket:null,vertexConstantPacket:null,vertexConstantMode:0,
      vertexConstants,vertexConstantWords,vertexConstantSpans,vertexConstantsValid:false,fixedVertexConstants,fixedVertexConstantWords,fixedVertexSpans,fixedVertexConstantsValid:false,fogConstantsValid:false,
      addresses:new Float32Array(16),addressesValid:false,
      borders:new Float32Array(16),borderColors:new Uint32Array(4),bordersValid:false});
    return result;
  }
  readyProgram(program, allowWait = true) {
    if(!program.pending)return program;
    if(program.error)throw program.error;
    const g=this.gl,start=performance.now(),extension=this.parallelShaderCompile;
    const completed = !!extension && g.getProgramParameter(program.p,extension.COMPLETION_STATUS_KHR);
    if(!completed && !allowWait)return null;
    const linked=g.getProgramParameter(program.p,g.LINK_STATUS),wait=performance.now()-start;
    this.shaderStats.waitMs+=wait;this.shaderStats.maxWaitMs=Math.max(this.shaderStats.maxWaitMs,wait);
    if(!completed)this.shaderStats.blockingPrograms++;
    if(!linked){
      try {
        const {p,vertex,vertexShader,fragmentShader,vertexPointer,regs,flat,fragmentSource}=program;
        if(!g.getShaderParameter(vertexShader,g.COMPILE_STATUS)){
          if(vertexPointer)postMessage({type:'vertex-diagnostic',hlsl:vertex.hlsl,regs,flat});
          throw this.shaderError(vertexShader,vertex.source);
        }
        if(!g.getShaderParameter(fragmentShader,g.COMPILE_STATUS))throw this.shaderError(fragmentShader,fragmentSource);
        throw new Error(`GPU program: ${g.getProgramInfoLog(p)}`);
      } catch(error) {
        program.error=error;
        throw error;
      } finally { g.deleteProgram(program.p); }
    }
    if(completed)this.shaderStats.readyWithoutWait++;
    g.detachShader(program.p,program.vertexShader);
    g.detachShader(program.p,program.fragmentShader);
    program.pending=false;const ready=this.finishProgram(program.p,program);this.shaderStats.completedPrograms++;
    program.vertex=program.vertexShader=program.fragmentShader=program.fragmentSource=undefined;
    return ready;
  }
  cacheDrawUniforms(program) {
    for (const name of DRAW_UNIFORMS) {
      if (!program.locations.has(name)) program.locations.set(name, this.gl.getUniformLocation(program.p, name));
    }
    program.drawUniformsReady = true;
    return program;
  }
  shaderError(shader,source) {
    const message=this.gl.getShaderInfoLog(shader),lines=source.split('\n');
    const locations=[...new Set([...message.matchAll(/ERROR:\s*\d+:(\d+)/g)].map(match=>Number(match[1])))];
    const context=locations.slice(0,10).map(line=>`${line}: ${lines[line-1]}`).join('\n');
    return new Error(`GPU shader: ${message}\n${context}`);
  }
  target(id,w,h,format,pitch,linear,depth,ptr,size,hashLo,hashHi) {
    if(!id || !w || !h || w>4096 || h>4096) throw new Error('Invalid GPU surface');
    const old=this.targets.get(id);
    if(!old&&this.targets.size>=256)throw new Error('GPU target budget exceeded');
    const sameAllocation=old&&old.w===w&&old.h===h&&old.format===format&&old.depth===!!depth;
    if(sameAllocation && old.hashLo===hashLo && old.hashHi===hashHi) {old.last=this.frame;return;}
    if(sameAllocation&&!depth) {
      const rgba = decodeColorTarget(this.bytes(ptr,size),w,h,format,pitch,linear);
      this.bindTexture(this.activeUnit,old.texture,true);
      this.gl.texSubImage2D(this.gl.TEXTURE_2D,0,0,0,w,h,this.gl.RGBA,this.gl.UNSIGNED_BYTE,rgba);
      old.hashLo=hashLo;old.hashHi=hashHi;old.last=this.frame;
      return;
    }
    if(old) this.invalidate(id, false);
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
      const rgba = decodeColorTarget(this.bytes(ptr,size),w,h,format,pitch,linear);
      g.texImage2D(g.TEXTURE_2D,0,g.RGBA8,w,h,0,g.RGBA,g.UNSIGNED_BYTE,rgba);
    }
    this.targets.set(id,t);
  }
  attach(color,depth) {
    const bound=this.boundAttachments;
    // Repeated draws to the current pair already use the most recent entry.
    if(bound&&bound.color===color&&bound.depth===depth)return;
    const g=this.gl;
    const key=`${color?.id??0}:${depth?.id??0}`;
    let entry=this.framebuffers.get(key);
    if(entry&&(entry.color!==color||entry.depth!==depth)){
      this.framebuffers.delete(key);
      if(bound===entry){g.bindFramebuffer(g.FRAMEBUFFER,null);this.boundAttachments=null;}
      g.deleteFramebuffer(entry.framebuffer);entry=undefined;
    }
    if(!entry){
      const target = color ?? depth;
      // Level-zero RGBA8 color and packed depth/stencil are required formats.
      // The batch still checks WebGL errors if an allocation or draw fails.
      const standard = target?.texture && target.w > 0 && target.h > 0 &&
        target.w <= this.maxTextureSize && target.h <= this.maxTextureSize &&
        !color?.depth && (!depth || (depth.depth && depth.texture)) &&
        (!color || !depth || (color.w === depth.w && color.h === depth.h));
      const layout = standard ? null :
        `c:${color?`${color.w}x${color.h}:RGBA8:${color.format}`:'none'};d:${depth?`${depth.w}x${depth.h}:DEPTH24_STENCIL8:${depth.format}`:'none'};draw:${color?'COLOR_ATTACHMENT0':'NONE'};read:${color?'COLOR_ATTACHMENT0':'NONE'}`;
      const validated = layout && this.validatedFramebufferLayouts.has(layout);
      if(validated){this.validatedFramebufferLayouts.delete(layout);this.validatedFramebufferLayouts.set(layout,true);}
      const framebuffer=g.createFramebuffer();
      if (!framebuffer) throw new Error('Could not allocate a GPU framebuffer');
      g.bindFramebuffer(g.FRAMEBUFFER,framebuffer);this.boundAttachments=null;
      g.framebufferTexture2D(g.FRAMEBUFFER,g.COLOR_ATTACHMENT0,g.TEXTURE_2D,color?.texture||null,0);
      g.framebufferTexture2D(g.FRAMEBUFFER,g.DEPTH_STENCIL_ATTACHMENT,g.TEXTURE_2D,depth?.texture||null,0);
      g.drawBuffers([color?g.COLOR_ATTACHMENT0:g.NONE]);g.readBuffer(color?g.COLOR_ATTACHMENT0:g.NONE);
      if(layout && !validated){
        if(g.checkFramebufferStatus(g.FRAMEBUFFER)!==g.FRAMEBUFFER_COMPLETE){
          g.deleteFramebuffer(framebuffer);
          throw new Error('GPU framebuffer is incomplete');
        }
        this.validatedFramebufferLayouts.set(layout,true);
        if(this.validatedFramebufferLayouts.size>512)this.validatedFramebufferLayouts.delete(this.validatedFramebufferLayouts.keys().next().value);
      }
      entry={framebuffer,color,depth};
      this.framebuffers.set(key,entry);
      if(this.framebuffers.size>512){
        const oldestKey=this.framebuffers.keys().next().value,oldest=this.framebuffers.get(oldestKey);
        this.framebuffers.delete(oldestKey);g.deleteFramebuffer(oldest.framebuffer);
      }
    } else {
      // Refresh insertion order so eviction is least-recently-used.
      this.framebuffers.delete(key);this.framebuffers.set(key,entry);
      g.bindFramebuffer(g.FRAMEBUFFER,entry.framebuffer);
    }
    this.boundAttachments=entry;
  }
  clear(rt,zs,n,rects,flags,color,z,stencil,viewport) {
    this.lastState=undefined;
    const g=this.gl,r=this.targets.get(rt),d=this.targets.get(zs),v=this.wordView(viewport,4);
    if((flags&0xf0)&&!r)throw new Error('Clear references an unregistered color target');
    if((flags&3)&&!d)throw new Error('Clear references an unregistered depth target');
    this.attach(flags&0xf0?r:null,flags&3?d:null);
    g.enable(g.SCISSOR_TEST);g.colorMask(!!(flags&0x10),!!(flags&0x20),!!(flags&0x40),!!(flags&0x80));
    g.depthMask(true);g.stencilMask(255);g.clearColor((color>>>16&255)/255,(color>>>8&255)/255,(color&255)/255,(color>>>24)/255);g.clearDepth(z);g.clearStencil(stencil);
    const list=n?this.wordView(rects,n*4):[v[0],v[1],v[0]+v[2],v[1]+v[3]];
    for(let i=0;i<(n||1);i++){const o=i*4,x=list[o],y=list[o+1],x1=list[o+2],y1=list[o+3];g.scissor(x,y,Math.max(0,x1-x),Math.max(0,y1-y));g.clear((flags&0xf0?g.COLOR_BUFFER_BIT:0)|(flags&1?g.DEPTH_BUFFER_BIT:0)|(flags&2?g.STENCIL_BUFFER_BIT:0));}
  }
  applyRasterState(s,zscale) {
    const g=this.gl,previous=this.lastState;
    const changed=(start,end=start+1)=>{
      if(!previous)return true;
      for(let i=start;i<end;i++)if(s[i]!==previous[i])return true;
      return false;
    };
    const enable=(cap,on)=>on?g.enable(cap):g.disable(cap);
    if(changed(2,4))g.viewport(0,0,s[2],s[3]);
    if(!previous)g.enable(g.SCISSOR_TEST);
    if(changed(22,31)){
      let left=s[22],top=s[23],right=left+s[24],bottom=top+s[25];
      if(s[26]){
        left=Math.max(left,s[27]);top=Math.max(top,s[28]);
        right=Math.min(right,s[29]);bottom=Math.min(bottom,s[30]);
      }
      g.scissor(left,top,Math.max(0,right-left),Math.max(0,bottom-top));
    }
    if(changed(4))enable(g.BLEND,s[4]);
    if(changed(5,7))g.blendFunc(s[5],s[6]);
    if(changed(7))g.blendEquation(s[7]||g.FUNC_ADD);
    if(changed(31))g.blendColor((s[31]>>>16&255)/255,(s[31]>>>8&255)/255,(s[31]&255)/255,(s[31]>>>24)/255);
    if(changed(8))g.colorMask(!!(s[8]&0x10000),!!(s[8]&0x100),!!(s[8]&1),!!(s[8]&0x1000000));
    if(changed(9))enable(g.DEPTH_TEST,s[9]);
    if(changed(10))g.depthMask(!!s[10]);
    if(changed(11))g.depthFunc(s[11]);
    if(changed(12))enable(g.STENCIL_TEST,s[12]);
    if(changed(13,16))g.stencilFunc(s[13],s[14],s[15]);
    if(changed(16))g.stencilMask(s[16]);
    if(changed(17,20))g.stencilOp(s[17],s[18],s[19]);
    if(changed(20))enable(g.CULL_FACE,s[20]===0x900||s[20]===0x901);
    // GPU coordinates have y down, matching the reference winding convention.
    if(changed(21))g.frontFace(s[21]===0x901?g.CW:g.CCW);
    if(changed(20,22))g.cullFace(s[20]===s[21]?g.FRONT:g.BACK);
    if(changed(34))enable(g.POLYGON_OFFSET_FILL,s[34]);
    if(changed(35,37)||zscale!==this.lastZscale){
      const f=new Float32Array(s.buffer,s.byteOffset,s.length);
      g.polygonOffset(f[35],f[36]*16777215*zscale);
    }
    this.stateSnapshot.set(s);
    this.lastState=this.stateSnapshot;
    this.lastZscale=zscale;
  }
  drawVariant(source,state,textures) {
    const vertex=state[37],flat=!!state[32],regs=state[39]&0xffff;
    const last=this.lastDrawVariant;
    if(last&&last.source===source&&last.vertex===vertex&&last.flat===flat&&last.regs===regs&&
       last.kinds[0]===textures[0]&&last.kinds[1]===textures[14]&&last.kinds[2]===textures[28]&&last.kinds[3]===textures[42]&&
       last.shadows[0]===textures[1]&&last.shadows[1]===textures[15]&&last.shadows[2]===textures[29]&&last.shadows[3]===textures[43])
      return last.variant;

    const kinds=[textures[0],textures[14],textures[28],textures[42]];
    const shadows=[textures[1],textures[15],textures[29],textures[43]];
    if(kinds.some(kind=>kind===2||kind===3))throw new Error('GPU 3D/cube texture admission is not implemented');
    const cache=this.pointerPrograms??=new Map();
    const key=`${source}:${vertex}:${kinds}:${shadows}:${flat}:${regs}`;
    let variant=cache.get(key);
    if(!variant){
      variant=this.programVariant(this.string(source),kinds,shadows,flat,vertex,regs);
      cache.set(key,variant);
    }
    this.lastDrawVariant={source,vertex,flat,regs,kinds,shadows,variant};
    return variant;
  }
  draw(n,state,uniforms,source,textures,layout,binding,indexOffset,uniformBatch,imageSlots,rasterPacket) {
    try {
      const samePacket = rasterPacket?.pointer === state;
      const s = samePacket ? rasterPacket.words : this.wordView(state,40);
      const g=this.gl,tx=this.wordView(textures,4*14),u=this.floatView(uniforms,152);
      if(!binding||((binding.kind==='raw')!==!!layout))throw new Error('GPU vertex binding does not match command layout');
      const variant=this.drawVariant(source,s,tx);
      const {specialized, fallback} = variant;
      const program = specialized ? this.readyProgram(specialized) : fallback.pipeline;
      this.attach(this.targets.get(s[0]&0x1fffffff),this.targets.get(s[1]&0x1fffffff));
      if(this.lastProgram!==program.p){g.useProgram(program.p);this.lastProgram=program.p;}
      const loc=name=>{if(!program.locations.has(name))program.locations.set(name,g.getUniformLocation(program.p,name));return program.locations.get(name);};
      if (fallback) {
        if (program.fixedKeys !== fallback) {
          if (fallback.vertexCode) {
            g.bindBuffer(g.UNIFORM_BUFFER, program.codeBuffer);
            g.bufferSubData(g.UNIFORM_BUFFER, 0, fallback.vertexCode);
            g.uniform1i(loc('fallbackInstructionCount'), fallback.vertexCode.length / 4);
          } else {
            g.uniform1uiv(loc('fallbackVertexKey[0]'), fallback.vertexKey);
          }
          g.uniform1uiv(loc('fallbackPixelKey[0]'), fallback.pixelKey.words);
          program.fixedKeys = fallback;
        }
        if (fallback.pixelKey.fast) this.shaderStats.fastCombinerDraws++;
        if (fallback.vertexCode) {
          this.shaderStats.programmableFallbackDraws++;
          this.usedProgrammableFallback = true;
        } else {
          this.shaderStats.fixedFallbackDraws++;
          this.usedFallbackThisFrame = true;
        }
      }
      if (!uniformBatch || program.uniformBatch !== uniformBatch) {
        program.uniformBatch = uniformBatch;
        program.uniformPacket = null;
        program.vertexConstantPacket = null;
      }
      if (program.uniformPacket !== uniforms) {
        const inputUniformWords=new Uint32Array(u.buffer,u.byteOffset,152),uniformWords=program.uniformWords,forceUniforms=!program.uniformsValid;
        const uniform=(name,offset,count)=>{
          const location=loc(name);if(location===null)return;
          let changed=forceUniforms;
          if(!changed)for(let i=0;i<count;i++)if(inputUniformWords[offset+i]!==uniformWords[offset+i]){changed=true;break;}
          if(changed){for(let i=0;i<count;i++)uniformWords[offset+i]=inputUniformWords[offset+i];g.uniform4fv(location,program.uniformSpans.get(name));}
        };
        uniform('surf',0,4);uniform('fogcolor',4,4);uniform('misc',8,4);uniform('pc',12,72);uniform('tfactor',84,4);uniform('texsize',88,16);uniform('bumpmat',104,16);uniform('bumplum',120,16);uniform('uvrect',136,16);
        program.uniformsValid=true;
        program.uniformPacket=uniforms;
      }
      const vertexConstantMode = s[39] & 0x10000;
      if(s[37] && (program.vertexConstantPacket !== s[38] || program.vertexConstantMode !== vertexConstantMode)) {
        const constants=this.floatView(s[38],772),constantWords=new Uint32Array(constants.buffer,constants.byteOffset,772);
        if(vertexConstantMode) {
          const fixedWords=program.fixedVertexConstantWords,forceFixed=!program.fixedVertexConstantsValid;
          const upload=(name,offset,count,matrix)=>{
            const location=loc(name);if(location===null)return;
            let changed=forceFixed;
            if(!changed)for(let i=0;i<count;i++)if(constantWords[offset+i]!==fixedWords[offset+i]){changed=true;break;}
            if(changed){for(let i=0;i<count;i++)fixedWords[offset+i]=constantWords[offset+i];
              if(matrix)g.uniformMatrix4fv(location,false,program.fixedVertexSpans[name]);
              else g.uniform4fv(location,program.fixedVertexSpans[name]);
            }
          };
          upload('wvp',0,16,true);upload('wv',16,16,true);upload('wvn',32,16,true);upload('tm',48,64,true);
          upload('mat',112,20,false);upload('amb',132,4,false);upload('lt',136,224,false);
          program.fixedVertexConstantsValid=true;
        } else {
          const location=loc('c'),snapshotWords=program.vertexConstantWords,forceConstants=!program.vertexConstantsValid;
          if(location!==null){
            let changed=forceConstants;
            if(!changed)for(let i=0;i<768;i++)if(constantWords[i]!==snapshotWords[i]){changed=true;break;}
            if(changed){for(let i=0;i<768;i++)snapshotWords[i]=constantWords[i];g.uniform4fv(location,program.vertexConstantSpans.c);}
          }
          program.vertexConstantsValid=true;
        }
        const fogLocation=loc('fogp'),snapshotWords=program.vertexConstantWords;
        if(fogLocation!==null){
          let changed=!program.fogConstantsValid;
          if(!changed)for(let i=0;i<4;i++)if(constantWords[768+i]!==snapshotWords[768+i]){changed=true;break;}
          if(changed){for(let i=0;i<4;i++)snapshotWords[768+i]=constantWords[768+i];g.uniform4fv(fogLocation,program.vertexConstantSpans.fogp);}
        }
        program.fogConstantsValid=true;
        program.vertexConstantPacket=s[38];
        program.vertexConstantMode=vertexConstantMode;
      }
      const addressLocation = loc('addresses');
      if (addressLocation !== null) {
        const addresses = program.addresses;
        let changed = !program.addressesValid;
        for (let unit = 0; unit < 4 && !changed; ++unit) {
          const base = unit * 14 + 7, offset = unit * 4;
          for (let axis = 0; axis < 4; ++axis) {
            const value = tx[base + axis], previous = addresses[offset + axis];
            // Most modes are small integers. Preserve the old float rounding
            // for any descriptor word outside the exact integer range.
            if (value !== previous && Math.fround(value) !== previous) {
              changed = true;
              break;
            }
          }
        }
        if (changed) {
          for (let unit = 0; unit < 4; ++unit) {
            const base = unit * 14 + 7, offset = unit * 4;
            for (let axis = 0; axis < 4; ++axis) addresses[offset + axis] = tx[base + axis];
          }
          g.uniform4fv(addressLocation, addresses);
          program.addressesValid = true;
        }
      }
      const borderLocation = loc('border');
      if (borderLocation !== null) {
        const colors = program.borderColors;
        if (!program.bordersValid || colors[0] !== tx[13] || colors[1] !== tx[27] ||
            colors[2] !== tx[41] || colors[3] !== tx[55]) {
          const borders = program.borders;
          for (let unit = 0; unit < 4; ++unit) {
            const color = tx[unit * 14 + 13], offset = unit * 4;
            colors[unit] = color;
            borders[offset] = (color >>> 16 & 255) / 255;
            borders[offset + 1] = (color >>> 8 & 255) / 255;
            borders[offset + 2] = (color & 255) / 255;
            borders[offset + 3] = (color >>> 24) / 255;
          }
          g.uniform4fv(borderLocation, borders);
          program.bordersValid = true;
        }
      }
      for(let t=0;t<4;t++) {
        const base=t*14;
        if(!tx[base]){
          if (fallback) {
            this.bindTexture(t,this.fixedFallbackTexture);
            if(!program.samplersSet)g.uniform1i(loc(`tex${t}`),t);
          }
          continue;
        }
        const address=tx[base+2];
        let image=this.targets.get(address&0x1fffffff);
        if(tx[base+1]&&!image)throw new Error('CPU depth-texture uploads are not admitted; depth sampling requires a GPU-owned surface');
        if(!image) {
          // Versioned SDK pixels have immutable owners until this batch is
          // consumed. Encoding is independent of the guest texture layout.
          const format=tx[base+5],encoding=format>>>8,width=tx[base+3],height=tx[base+4];
          if((format&255)!==1||encoding>1)throw new Error('Unsupported GPU image encoding');
          const slot = imageSlots?.[t];
          if (slot?.image && slot.address === address && slot.width === width &&
              slot.height === height && slot.encoding === encoding) {
            image = slot.image;
          } else {
            const key=`${address}:${width}:${height}:${encoding}`;
            image=this.images.get(key);
            if(!image){if(this.images.size>=4096)throw new Error('GPU texture cache budget exceeded');image={texture:g.createTexture(),frame:-1};this.images.set(key,image);}
            if (slot) {
              slot.address = address;
              slot.width = width;
              slot.height = height;
              slot.encoding = encoding;
              slot.image = image;
            }
          }
          this.bindTexture(t,image.texture);
          const hashLo=tx[base+11],hashHi=tx[base+12];
          if(image.hashLo!==hashLo || image.hashHi!==hashHi){
            this.bindTexture(t,image.texture,true);
            const size=width*height*4,pixelsPointer=tx[base+6];
            if(!width||!height||!Number.isSafeInteger(size)||size>32*1024*1024)throw new Error('GPU image staging budget exceeded');
            const uploadStarted = this.measurePacing ? performance.now() : 0;
            const direct = this.uploadOrdinaryImage(image, encoding, pixelsPointer, width, height, size);
            image.hashLo=hashLo;image.hashHi=hashHi;
            if(this.frameStats){
              this.frameStats.textureUploads=(this.frameStats.textureUploads||0)+1;
              this.frameStats.textureUploadBytes=(this.frameStats.textureUploadBytes||0)+size;
              if(encoding===1)this.frameStats.byteTextureUploads=(this.frameStats.byteTextureUploads||0)+1;
              if(this.measurePacing){
                this.frameStats.textureUploadMs=(this.frameStats.textureUploadMs??0)+performance.now()-uploadStarted;
                if(direct)this.frameStats.directTextureUploadBytes=(this.frameStats.directTextureUploadBytes??0)+size;
              }
            }
          }
          image.frame=this.frame;
        } else this.bindTexture(t,image.texture);
        if(image===this.targets.get(s[0]&0x1fffffff)||image===this.targets.get(s[1]&0x1fffffff))throw new Error('GPU feedback texture overlaps an active attachment');
        const sampler=this.samplers[t],wrapS=tx[base+7],wrapT=tx[base+8],min=tx[base+9],mag=tx[base+10];
        // Key the actual WebGL state, including the existing default mappings.
        const sMode = wrapS === 1 || wrapS === 2 ? wrapS : 0;
        const tMode = wrapT === 1 || wrapT === 2 ? wrapT : 0;
        const samplerKey = sMode | (tMode << 2) | (Number(min === 1) << 4) | (Number(mag === 1) << 5);
        if(this.samplerKeys[t]!==samplerKey){
          const wrap=mode=>mode===1?g.REPEAT:mode===2?g.MIRRORED_REPEAT:g.CLAMP_TO_EDGE;
          g.samplerParameteri(sampler,g.TEXTURE_WRAP_S,wrap(wrapS));g.samplerParameteri(sampler,g.TEXTURE_WRAP_T,wrap(wrapT));
          g.samplerParameteri(sampler,g.TEXTURE_MIN_FILTER,min===1?g.NEAREST:g.LINEAR);g.samplerParameteri(sampler,g.TEXTURE_MAG_FILTER,mag===1?g.NEAREST:g.LINEAR);
          this.samplerKeys[t]=samplerKey;
        }
        if(!program.samplersSet)g.uniform1i(loc(`tex${t}`),t);
      }
      program.samplersSet=true;
      let stateChanged=!this.lastState||u[2]!==this.lastZscale;
      if(!stateChanged&&!samePacket)for(let i=0;i<37;i++)if(s[i]!==this.lastState[i]){stateChanged=true;break;}
      if(stateChanged)this.applyRasterState(s,u[2]);
      if (rasterPacket) {
        if (samePacket) ++rasterPacket.reuses;
        else {
          rasterPacket.pointer = state;
          rasterPacket.words = s;
        }
      }
      this.bindGeometry(binding);
      g.drawElements(g.TRIANGLES,n,g.UNSIGNED_INT,indexOffset);
      const candidate = variant.promotion;
      if (candidate && this.shaderPromotionCandidates) {
        if (candidate.frame !== this.frame) {
          // Confirm demand in consecutive frames before spending a driver link.
          if (candidate.frame === this.frame - 1) this.shaderPromotionCandidates.add(candidate);
          else this.shaderPromotionCandidates.delete(candidate);
          candidate.frame = this.frame;
          candidate.indices = 0;
        }
        candidate.indices += n;
      }
      if (variant.recipe) {
        const {color, depth} = this.boundAttachments;
        const attachments = Number(Boolean(color)) | Number(Boolean(depth)) << 1;
        const formats = layout ? Array.from((binding.group || binding).layout, word => word & 255) : new Array(16).fill(2);
        this.shaderWarmup.observe(variant.recipe, variant.recipeKey, [attachments, ...formats]);
        variant.recipe = variant.recipeKey = undefined;
      }
      this.issuedDraws=(this.issuedDraws||0)+1;
      return true;
    } catch(error) {this.report(error.message);postMessage({type:'gpu-error',text:error.message});return false;}
  }
  present(id,onError) {
    if(!onError)this.finishFrameErrorCheck();
    else if(this.frameError)throw this.frameError;
    const t=this.targets.get(id);if(!t||t.depth){this.disposeGpuTiming();return false;}
    if(this.gpuTimerExtension)this.beginGpuTimer();
    const g=this.gl;if(this.canvas.width!==t.w||this.canvas.height!==t.h){this.canvas.width=t.w;this.canvas.height=t.h;}
    g.bindFramebuffer(g.READ_FRAMEBUFFER,this.readFbo);g.framebufferTexture2D(g.READ_FRAMEBUFFER,g.COLOR_ATTACHMENT0,g.TEXTURE_2D,t.texture,0);g.framebufferTexture2D(g.READ_FRAMEBUFFER,g.DEPTH_STENCIL_ATTACHMENT,g.TEXTURE_2D,null,0);g.readBuffer(g.COLOR_ATTACHMENT0);this.readFramebufferTarget=t;
    g.bindFramebuffer(g.DRAW_FRAMEBUFFER,null);g.disable(g.SCISSOR_TEST);g.blitFramebuffer(0,0,t.w,t.h,0,t.h,t.w,0,g.COLOR_BUFFER_BIT,g.NEAREST);
    this.endGpuTimer();
    this.boundAttachments=null;this.lastState=undefined;
    ++this.frame;
    if (this.usedFallbackThisFrame) {
      this.shaderStats.fixedFallbackFrames++;
      this.usedFallbackThisFrame = false;
    }
    if (this.usedProgrammableFallback) {
      this.shaderStats.programmableFallbackFrames++;
      this.usedProgrammableFallback = false;
    }
    this.lastSubmitMs=this.submitMs||0;this.lastIssuedDraws=this.issuedDraws||0;this.submitMs=0;this.issuedDraws=0;
    // Let the new scene use resident shaders while its first textures arrive.
    if (this.shaderPromotionCandidates && this.frameStats?.textureUploadBytes >= PROMOTION_UPLOAD_BURST_BYTES)
      this.shaderPromotionResumeFrame = this.frame + PROMOTION_SETTLE_FRAMES;
    this.lastFrameStats=this.frameStats;this.frameStats=undefined;
    for(const [key,image] of this.images)if(image.frame<this.frame-2){g.deleteTexture(image.texture);this.images.delete(key);}
    if(onError)this.scheduleFrameErrorCheck(onError);
    g.flush();
    // The page uses this signal to leave its startup state. The transferred
    // canvas presents subsequent frames without a separate page message.
    if(this.frame===1)postMessage({type:'gpu-frame',width:t.w,height:t.h});
    return true;
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
    if(this.frameError)throw this.frameError;
    const t=this.targets.get(id);if(!t)return false;
    if(t.depth)throw new Error('Guest depth readback is not yet admitted by the WebGL backend');
    const g=this.gl;this.attach(t,null);const pixels=new Uint8Array(t.w*t.h*4);g.readPixels(0,0,t.w,t.h,g.RGBA,g.UNSIGNED_BYTE,pixels);
    this.cancelFrameErrorCheck(); // The synchronous check covers all earlier commands too.
    const error=g.getError();if(error!==g.NO_ERROR)throw new Error(`GPU readback error 0x${error.toString(16)}`);
    this.bytes(ptr,pixels.length).set(pixels);return true;
  }
  invalidate(id,checkErrors=true) {
    if(checkErrors)this.finishFrameErrorCheck();
    const t=this.targets.get(id);if(!t)return;
    const g=this.gl;
    for(const [key,entry] of this.framebuffers){
      if(entry.color!==t&&entry.depth!==t)continue;
      if(this.boundAttachments===entry){g.bindFramebuffer(g.FRAMEBUFFER,null);this.boundAttachments=null;}
      g.deleteFramebuffer(entry.framebuffer);this.framebuffers.delete(key);
    }
    if(this.readFramebufferTarget===t){
      this.boundAttachments=null;
      g.bindFramebuffer(g.READ_FRAMEBUFFER,this.readFbo);
      g.framebufferTexture2D(g.READ_FRAMEBUFFER,g.COLOR_ATTACHMENT0,g.TEXTURE_2D,null,0);
      this.readFramebufferTarget=undefined;
    }
    g.deleteTexture(t.texture);this.targets.delete(id);
  }
}
