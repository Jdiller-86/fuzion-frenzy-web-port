import { pixelGlsl } from './pixel-glsl.mjs';
import { vertexGlsl } from './vertex-glsl.mjs';

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
  constructor(canvas, memory, report, gpuVertices=true, shaderPrewarm=true) {
    this.canvas = canvas; this.memory = memory; this.report = report;
    const gl = this.gl = canvas.getContext('webgl2', { alpha: false, antialias: false, depth: false, stencil: false, preserveDrawingBuffer: false, powerPreference: 'high-performance' });
    if (!gl) throw new Error('WebGL 2 is unavailable. Select Software reference or enable browser hardware acceleration.');
    this.gpuVertices=gpuVertices&&gl.getParameter(gl.MAX_VERTEX_ATTRIBS)>=16&&gl.getParameter(gl.MAX_VERTEX_UNIFORM_VECTORS)>=195;
    this.parallelShaderCompile=shaderPrewarm?gl.getExtension('KHR_parallel_shader_compile'):null;
    this.shaderStats={enabled:!!this.parallelShaderCompile,prepassMs:0,compileMs:0,linkMs:0,waitMs:0,prepassPrograms:0,completedPrograms:0,readyWithoutWait:0,blockingPrograms:0,maxWaitMs:0};
    this.shaderPrewarmSeen=new Set();this.shaderPrewarmPending=new Map();this.shaderPrewarmDiscovered=new Map();this.shaderPrewarmPairCount=0;
    this.vertexPrograms=new Map();
    this.targets = new Map(); this.images = new Map(); this.programs = new Map(); this.frame = 0;
    this.readFbo = gl.createFramebuffer(); this.framebuffers = new Map(); this.validatedFramebufferLayouts = new Map(); this.boundFramebuffer = null;
    this.vao = gl.createVertexArray(); this.vbo = gl.createBuffer(); this.ibo = gl.createBuffer();
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
    this.vertexShader = this.shader(gl.VERTEX_SHADER, this.vertexSource);
    canvas.addEventListener('webglcontextlost', e => {
      e.preventDefault();
      postMessage({type:'gpu-error',text:'The graphics context was lost. Start the game again to recreate its resources.'});
    });
    const info = gl.getExtension('WEBGL_debug_renderer_info');
    report(`WebGL 2 triangle renderer: ${info ? gl.getParameter(info.UNMASKED_RENDERER_WEBGL) : gl.getParameter(gl.RENDERER)}`);
    report(`Programmable vertices: ${this.gpuVertices?'GPU':'CPU reference'}`);
  }
  bytes(ptr, length) {
    const heap = this.memory().HEAPU8;
    if (!Number.isInteger(ptr) || !Number.isSafeInteger(length) || ptr < 0 || length < 0 || ptr + length > heap.length) throw new Error('GPU bridge span outside WASM memory');
    return heap.subarray(ptr,ptr+length);
  }
  wordView(ptr,n) { const span=this.bytes(ptr,n*4);return new Uint32Array(span.buffer,span.byteOffset,n); }
  floatView(ptr,n) { const span=this.bytes(ptr,n*4);return new Float32Array(span.buffer,span.byteOffset,n); }
  words(ptr,n) { return new Uint32Array(this.bytes(ptr,n*4).slice().buffer); }
  floats(ptr,n) { return new Float32Array(this.bytes(ptr,n*4).slice().buffer); }
  sameWords(a,b,count) {
    if(a===b)return true;
    const av=this.wordView(a,count),bv=this.wordView(b,count);
    for(let i=0;i<count;i++)if(av[i]!==bv[i])return false;
    return true;
  }
  capacity(current,required,limit) {
    if(required>limit)throw new Error('GPU geometry budget exceeded');
    let size=Math.max(current,256);
    while(size<required)size=Math.min(limit,size*2);
    return size;
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
    if(binding.kind==='converted'){
      for(let a=0;a<16;a++){g.enableVertexAttribArray(a);g.vertexAttribPointer(a,4,g.FLOAT,false,16,a*binding.totalVertices*16);}
    }else{
      const group=binding.group||binding;
      for(let a=0;a<16;a++){
        const attr=group.attributes[a];
        if(!attr){g.disableVertexAttribArray(a);g.vertexAttrib4f(a,0,0,0,1);continue;}
        g.enableVertexAttribArray(a);
        if(attr.integer)g.vertexAttribIPointer(a,attr.components,g[attr.type],group.stride,group.baseByte+attr.offset);
        else g.vertexAttribPointer(a,attr.components,g[attr.type],attr.normalized,group.stride,group.baseByte+attr.offset);
      }
    }
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
  shader(kind, source) {
    const g=this.gl,start=performance.now(),s=g.createShader(kind);g.shaderSource(s,source);g.compileShader(s);this.shaderStats.compileMs+=performance.now()-start;
    const waitStart=performance.now(),compiled=g.getShaderParameter(s,g.COMPILE_STATUS),wait=performance.now()-waitStart;
    this.shaderStats.waitMs+=wait;this.shaderStats.maxWaitMs=Math.max(this.shaderStats.maxWaitMs,wait);
    if(!compiled) {
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
    const words=this.wordView(ptr,count*12);
    if(this.parallelShaderCompile){
      const prewarmStart=performance.now(),heapBytes=this.memory().HEAPU8,heapWords=new Uint32Array(heapBytes.buffer,heapBytes.byteOffset,heapBytes.byteLength>>>2);this.shaderPrewarmSeen.clear();this.shaderPrewarmPending.clear();let prewarmAttempts=0,pendingPairCount=0;
      for(let i=0;i<count;i++){
        const b=i*12,opcode=words[b];if(opcode!==1&&opcode!==3)continue;
        try{
          const sourcePointer=words[b+8],statePointer=words[b+6];
          if((statePointer&3)||statePointer>heapBytes.length-160)continue;
          const stateOffset=statePointer>>>2,flat=!!heapWords[stateOffset+32],vertexPointer=heapWords[stateOffset+37],regs=heapWords[stateOffset+39]&0xffff,signature=vertexPointer*131072+(flat?65536:0)+regs;
          if(this.shaderPrewarmDiscovered.get(sourcePointer)?.has(signature))continue;
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
          const kinds=[t0,t1,t2,t3],shadows=[s0,s1,s2,s3],program=this.program(source,kinds,shadows,flat,vertexPointer,regs,true);
          this.pointerPrograms??=new Map();this.pointerPrograms.set(identity,program);
          if(this.programs.size!==before)this.shaderStats.prepassPrograms++;
        }catch{/* Preserve the original error point at the command's draw. */}
      }
      for(const [sourcePointer,signatures] of this.shaderPrewarmPending){let discovered=this.shaderPrewarmDiscovered.get(sourcePointer);if(!discovered){discovered=new Set();this.shaderPrewarmDiscovered.set(sourcePointer,discovered);}for(const signature of signatures){if(this.shaderPrewarmPairCount>=2048)break;if(!discovered.has(signature)){discovered.add(signature);this.shaderPrewarmPairCount++;}}}
      this.shaderStats.prepassMs+=performance.now()-prewarmStart;
    }
    // Keep converted float planes first; append raw, interleaved layout groups.
    let convertedVertices=0,rawBytes=0,indexCount=0;
    for(let i=0;i<count;i++){
      const b=i*12,opcode=words[b];
      if(opcode===1){convertedVertices+=words[b+2];indexCount+=words[b+5];}
      else if(opcode===3){
        const stride=words[b+3];if(!stride||stride>252||(stride&3))throw new Error('Unsupported raw vertex stride');
        rawBytes+=words[b+2]*stride;indexCount+=words[b+5];
        if(rawBytes>256*1024*1024||indexCount>16*1024*1024)throw new Error('GPU geometry budget exceeded');
      }
      if(convertedVertices*256+rawBytes>256*1024*1024||indexCount>16*1024*1024)throw new Error('GPU geometry budget exceeded');
    }
    const convertedBytes=convertedVertices*256,vertexBytes=convertedBytes+rawBytes;
    if(vertexBytes>256*1024*1024||indexCount>16*1024*1024)throw new Error('GPU geometry budget exceeded');
    if(!this.vertexStaging||this.vertexStaging.length<vertexBytes)this.vertexStaging=new Uint8Array(this.capacity(this.vertexStaging?.length||0,vertexBytes,256*1024*1024));
    if(!this.indexStaging||this.indexStaging.length<indexCount)this.indexStaging=new Uint32Array(this.capacity(this.indexStaging?.length||0,indexCount,16*1024*1024));
    const vertices=this.vertexStaging.subarray(0,vertexBytes),elements=this.indexStaging.subarray(0,indexCount);
    vertices.fill(0,0,convertedBytes);
    const vertexWords=new Uint32Array(vertices.buffer,vertices.byteOffset,vertices.byteLength/4);
    this.indexOffsets??=new Map();this.indexOffsets.clear();this.indexBindings??=new Map();this.indexBindings.clear();let convertedOffset=0,rawOffset=convertedBytes,io=0,rawGroup=null;
    const convertedBinding={kind:'converted',totalVertices:convertedVertices};
    for(let i=0;i<count;i++){
      const b=i*12,opcode=words[b],commandOffset=words.byteOffset+b*4;
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
        this.indexOffsets.set(commandOffset,io*4);this.indexBindings.set(commandOffset,convertedBinding);
        convertedOffset+=vertexCount*256;io+=indexCountForDraw;
      }else if(opcode===3){
        const vertexCount=words[b+2],stride=words[b+3],layoutPtr=words[b+10],indexCountForDraw=words[b+5];if(!layoutPtr)throw new Error('Missing raw vertex layout');
        const layout=this.wordView(layoutPtr,16),state=this.wordView(words[b+6],40),mask=state[39]&0xffff;
        let same=!!rawGroup&&rawGroup.stride===stride;
        if(same)for(let r=0;r<16;r++)if(rawGroup.layout[r]!==layout[r]){same=false;break;}
        if(same){let active=0;for(let r=0;r<16;r++)if(layout[r])active|=1<<r;if(active!==mask)throw new Error('Raw vertex layout does not match register mask');}
        else rawGroup={kind:'raw',baseByte:rawOffset,stride,layout:layout.slice(),attributes:this.rawLayout(stride,layout,mask),vertexCount:0};
        const base=rawGroup.vertexCount,sourceBytes=this.bytes(words[b+1],vertexCount*stride);
        vertices.set(sourceBytes,rawOffset);
        const order=this.wordView(words[b+4],indexCountForDraw),flat=state[32];
        for(let j=0;j<order.length;j++){
          const lane=flat&&j-j%3+2<order.length?j-j%3+(j%3+1)%3:j;
          const index=order[lane];if(index>=vertexCount)throw new Error('GPU index exceeds raw vertex span');elements[io+j]=index+base;
        }
        this.frameStats??={gpuVertexDraws:0,cpuVertexDraws:0,gpuVertices:0,cpuVertices:0,vertexBytes:0,rawStreamDraws:0,rawStreamBytes:0};
        this.frameStats.rawStreamDraws??=0;this.frameStats.rawStreamBytes??=0;
        this.frameStats.rawStreamDraws++;this.frameStats.rawStreamBytes+=sourceBytes.length;
        this.frameStats.vertexBytes+=sourceBytes.length;
        this.frameStats.gpuVertexDraws++;this.frameStats.gpuVertices+=vertexCount;
        rawGroup.vertexCount+=vertexCount;rawOffset+=sourceBytes.length;
        this.indexOffsets.set(commandOffset,io*4);this.indexBindings.set(commandOffset,rawGroup);
        io+=indexCountForDraw;
      }
    }
    const g=this.gl;g.bindVertexArray(this.vao);g.bindBuffer(g.ARRAY_BUFFER,this.vbo);
    if(vertexBytes>(this.vertexBufferCapacity||0)){this.vertexBufferCapacity=this.capacity(this.vertexBufferCapacity||0,vertexBytes,256*1024*1024);g.bufferData(g.ARRAY_BUFFER,this.vertexBufferCapacity,g.STREAM_DRAW);}
    if(vertexBytes)g.bufferSubData(g.ARRAY_BUFFER,0,vertices);
    g.bindBuffer(g.ELEMENT_ARRAY_BUFFER,this.ibo);
    if(indexCount>(this.indexBufferCapacity||0)){this.indexBufferCapacity=this.capacity(this.indexBufferCapacity||0,indexCount,16*1024*1024);g.bufferData(g.ELEMENT_ARRAY_BUFFER,this.indexBufferCapacity*4,g.STREAM_DRAW);}
    if(indexCount)g.bufferSubData(g.ELEMENT_ARRAY_BUFFER,0,elements);
    let pendingRow=-1,pendingCount=0,pendingBinding;
    const emit=()=>{if(pendingRow>=0){const b=pendingRow,raw=words[b]===3,commandOffset=words.byteOffset+b*4;
      if(!this.draw(words[b+1],words[b+2],words[b+3],words[b+4],pendingCount,words[b+6],words[b+7],words[b+8],words[b+9],raw?words[b+10]:0,pendingBinding,commandOffset))throw new Error('GPU draw rejected');pendingRow=-1;}};
    for(let i=0;i<count;i++) {
      const b=i*12,opcode=words[b],commandOffset=words.byteOffset+b*4;
      if(opcode===1||opcode===3) {
        const binding=this.indexBindings.get(commandOffset);
        if(pendingRow>=0&&words[pendingRow]===opcode&&pendingBinding===binding&&words[pendingRow+8]===words[b+8]&&
           this.indexOffsets.get(words.byteOffset+pendingRow*4)+pendingCount*4===this.indexOffsets.get(commandOffset)&&
           this.sameWords(words[pendingRow+6],words[b+6],40)&&this.sameWords(words[pendingRow+7],words[b+7],152)&&this.sameWords(words[pendingRow+9],words[b+9],56)) {
          pendingCount+=words[b+5];continue;
        }
        emit();pendingRow=b;pendingCount=words[b+5];pendingBinding=binding;continue;
      }
      emit();
      if(opcode===0)this.target(words[b+1],words[b+2],words[b+3],words[b+4],words[b+5],words[b+6],words[b+7],words[b+8],words[b+9],words[b+10],words[b+11]);
      else if(opcode===2){this.commandFloatBits[0]=words[b+7];this.clear(words[b+1],words[b+2],words[b+3],words[b+4],words[b+5],words[b+6],this.commandFloatValue[0],words[b+8],words[b+9]);}
      else throw new Error('Unknown GPU command');
    }
    emit();
    const error=this.gl.getError();
    if(error)throw new Error(`GPU command batch error 0x${error.toString(16)}`);
    this.submitMs=(this.submitMs||0)+performance.now()-started;
    return true;
    } catch(error) {this.report(error.message);postMessage({type:'gpu-error',text:error.message});return false;}
  }
  program(source,kinds,shadows,flat,vertexPointer=0,regs=0,prewarm=false) {
    const key=source+':'+kinds+':'+shadows+':'+flat+':'+vertexPointer+':'+regs;
    if(this.programs.has(key)) return this.programs.get(key);
    if(this.programs.size>=2048)throw new Error('GPU shader cache budget exceeded');
    const g=this.gl, parallel=prewarm&&!!this.parallelShaderCompile;
    const compile=(kind,text)=>{const start=performance.now(),s=g.createShader(kind);g.shaderSource(s,text);g.compileShader(s);this.shaderStats.compileMs+=performance.now()-start;return s;};
    const fragmentSource=pixelGlsl(source,kinds,shadows,flat),f=parallel?compile(g.FRAGMENT_SHADER,fragmentSource):this.shader(g.FRAGMENT_SHADER,fragmentSource),p=g.createProgram();
    let vertexSource,vertexShader;
    try{
      if(vertexPointer){
        const vk=`${vertexPointer}:${flat}:${regs}`;vertexShader=this.vertexPrograms.get(vk);
        if(!vertexShader){
          const hlsl=this.string(vertexPointer);
          try{vertexSource=vertexGlsl(hlsl,regs,flat);vertexShader=parallel?compile(g.VERTEX_SHADER,vertexSource):this.shader(g.VERTEX_SHADER,vertexSource);}
          catch(error){postMessage({type:'vertex-diagnostic',hlsl,regs,flat});throw error;}
          this.vertexPrograms.set(vk,vertexShader);
        }
      } else {
        if(flat&&!this.flatVertexShader){vertexSource=this.vertexSource.replace(/out vec4 v([DB][01])/g,'flat out vec4 v$1');this.flatVertexShader=parallel?compile(g.VERTEX_SHADER,vertexSource):this.shader(g.VERTEX_SHADER,vertexSource);}
        vertexShader=flat?this.flatVertexShader:this.vertexShader;
      }
      g.attachShader(p,vertexShader);g.attachShader(p,f);const linkStart=performance.now();g.linkProgram(p);this.shaderStats.linkMs+=performance.now()-linkStart;
    }catch(error){g.deleteShader(f);g.deleteProgram(p);throw error;}
    g.deleteShader(f);
    if(parallel){const result={p,pending:true,vertexShader,vertexPointer,regs,flat,fragmentShader:f,fragmentSource,vertexSource};this.programs.set(key,result);return result;}
    const waitStart=performance.now(),linked=g.getProgramParameter(p,g.LINK_STATUS),wait=performance.now()-waitStart;
    this.shaderStats.waitMs+=wait;this.shaderStats.maxWaitMs=Math.max(this.shaderStats.maxWaitMs,wait);this.shaderStats.blockingPrograms++;
    if(!linked){const error=new Error(`GPU program: ${g.getProgramInfoLog(p)}`);g.deleteProgram(p);throw error;}
    const result=this.finishProgram(p);this.shaderStats.completedPrograms++;this.programs.set(key,result);return result;
  }
  finishProgram(p,result={}) {
    const g=this.gl;
    const uniforms=new Float32Array(152),uniformWords=new Uint32Array(uniforms.buffer),uniformSpans=new Map();
    for(const [name,offset,count] of [['surf',0,4],['fogcolor',4,4],['misc',8,4],['pc',12,72],['tfactor',84,4],['texsize',88,16],['bumpmat',104,16],['bumplum',120,16],['uvrect',136,16]])
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
      vertexConstants,vertexConstantWords,vertexConstantSpans,vertexConstantsValid:false,fixedVertexConstants,fixedVertexConstantWords,fixedVertexSpans,fixedVertexConstantsValid:false,fogConstantsValid:false,
      addresses:new Float32Array(16),addressScratch:new Float32Array(16),addressesValid:false,
      borders:new Float32Array(16),borderScratch:new Float32Array(16),bordersValid:false});
    return result;
  }
  readyProgram(program) {
    if(!program.pending)return program;
    const g=this.gl,start=performance.now(),extension=this.parallelShaderCompile;
    const completed=g.getProgramParameter(program.p,extension.COMPLETION_STATUS_KHR),linked=g.getProgramParameter(program.p,g.LINK_STATUS),wait=performance.now()-start;
    this.shaderStats.waitMs+=wait;this.shaderStats.maxWaitMs=Math.max(this.shaderStats.maxWaitMs,wait);
    if(!completed)this.shaderStats.blockingPrograms++;
    let statusStart=performance.now(),vertexCompiled=g.getShaderParameter(program.vertexShader,g.COMPILE_STATUS),statusWait=performance.now()-statusStart;
    this.shaderStats.waitMs+=statusWait;this.shaderStats.maxWaitMs=Math.max(this.shaderStats.maxWaitMs,statusWait);
    if(!vertexCompiled){
      const source=program.vertexSource??(program.vertexPointer?vertexGlsl(this.string(program.vertexPointer),program.regs,program.flat):program.flat?this.vertexSource.replace(/out vec4 v([DB][01])/g,'flat out vec4 v$1'):this.vertexSource),e=g.getShaderInfoLog(program.vertexShader),lines=source.split('\n'),context=[...new Set([...e.matchAll(/ERROR:\s*\d+:(\d+)/g)].map(m=>Number(m[1])))].slice(0,10).map(n=>`${n}: ${lines[n-1]}`).join('\n');
      if(program.vertexPointer){const hlsl=this.string(program.vertexPointer);postMessage({type:'vertex-diagnostic',hlsl,regs:program.regs,flat:program.flat});}
      throw new Error(`GPU shader: ${e}\n${context}`);
    }
    statusStart=performance.now();const fragmentCompiled=g.getShaderParameter(program.fragmentShader,g.COMPILE_STATUS);statusWait=performance.now()-statusStart;
    this.shaderStats.waitMs+=statusWait;this.shaderStats.maxWaitMs=Math.max(this.shaderStats.maxWaitMs,statusWait);
    if(!fragmentCompiled){
      const e=g.getShaderInfoLog(program.fragmentShader),lines=program.fragmentSource.split('\n'),context=[...new Set([...e.matchAll(/ERROR:\s*\d+:(\d+)/g)].map(m=>Number(m[1])))].slice(0,10).map(n=>`${n}: ${lines[n-1]}`).join('\n');
      throw new Error(`GPU shader: ${e}\n${context}`);
    }
    if(!linked)throw new Error(`GPU program: ${g.getProgramInfoLog(program.p)}`);
    if(completed)this.shaderStats.readyWithoutWait++;
    program.pending=false;const ready=this.finishProgram(program.p,program);this.shaderStats.completedPrograms++;
    program.vertexShader=program.fragmentShader=program.vertexSource=program.fragmentSource=undefined;
    return ready;
  }
  target(id,w,h,format,pitch,linear,depth,ptr,size,hashLo,hashHi) {
    if(!id || !w || !h || w>4096 || h>4096) throw new Error('Invalid GPU surface');
    const old=this.targets.get(id);
    if(!old&&this.targets.size>=256)throw new Error('GPU target budget exceeded');
    const sameAllocation=old&&old.w===w&&old.h===h&&old.format===format&&old.depth===!!depth;
    if(sameAllocation && old.hashLo===hashLo && old.hashHi===hashHi) {old.last=this.frame;return;}
    if(sameAllocation&&!depth) {
      const input=this.bytes(ptr,size),rgba=new Uint8Array(w*h*4),bpp=[0x05,0x11].includes(format)?2:4;
      for(let y=0;y<h;y++) for(let x=0;x<w;x++) {
        let o=0,bit=0;
        if(linear) o=y*pitch+x*bpp;
        else {for(let m=1;m<w||m<h;m<<=1) {if(m<w) {if(x&m)o|=1<<bit;bit++;}if(m<h){if(y&m)o|=1<<bit;bit++;}}o*=bpp;}
        const q=(y*w+x)*4;if(o+bpp>input.length)throw new Error('Surface span mismatch');
        if(bpp===4) {rgba[q]=input[o+2];rgba[q+1]=input[o+1];rgba[q+2]=input[o];rgba[q+3]=[7,0x1e].includes(format)?255:input[o+3];}
        else {const v=input[o]|input[o+1]<<8;rgba[q]=(v>>11)*255/31;rgba[q+1]=(v>>5&63)*255/63;rgba[q+2]=(v&31)*255/31;rgba[q+3]=255;}
      }
      this.bindTexture(this.activeUnit,old.texture,true);
      this.gl.texSubImage2D(this.gl.TEXTURE_2D,0,0,0,w,h,this.gl.RGBA,this.gl.UNSIGNED_BYTE,rgba);
      old.hashLo=hashLo;old.hashHi=hashHi;old.last=this.frame;
      return;
    }
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
    const key=`${color?.id??0}:${depth?.id??0}`;
    let entry=this.framebuffers.get(key);
    if(entry&&(entry.color!==color||entry.depth!==depth)){
      this.framebuffers.delete(key);
      if(this.boundFramebuffer===entry.framebuffer){g.bindFramebuffer(g.FRAMEBUFFER,null);this.boundFramebuffer=null;}
      g.deleteFramebuffer(entry.framebuffer);entry=undefined;
    }
    if(!entry){
      const layout=`c:${color?`${color.w}x${color.h}:RGBA8:${color.format}`:'none'};d:${depth?`${depth.w}x${depth.h}:DEPTH24_STENCIL8:${depth.format}`:'none'};draw:${color?'COLOR_ATTACHMENT0':'NONE'};read:${color?'COLOR_ATTACHMENT0':'NONE'}`;
      let validated=this.validatedFramebufferLayouts.has(layout);
      if(validated){this.validatedFramebufferLayouts.delete(layout);this.validatedFramebufferLayouts.set(layout,true);}
      const framebuffer=g.createFramebuffer();
      g.bindFramebuffer(g.FRAMEBUFFER,framebuffer);this.boundFramebuffer=framebuffer;
      g.framebufferTexture2D(g.FRAMEBUFFER,g.COLOR_ATTACHMENT0,g.TEXTURE_2D,color?.texture||null,0);
      g.framebufferTexture2D(g.FRAMEBUFFER,g.DEPTH_STENCIL_ATTACHMENT,g.TEXTURE_2D,depth?.texture||null,0);
      g.drawBuffers([color?g.COLOR_ATTACHMENT0:g.NONE]);g.readBuffer(color?g.COLOR_ATTACHMENT0:g.NONE);
      if(!validated){
        if(g.checkFramebufferStatus(g.FRAMEBUFFER)!==g.FRAMEBUFFER_COMPLETE){
          g.deleteFramebuffer(framebuffer);this.boundFramebuffer=null;
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
      if(this.boundFramebuffer!==entry.framebuffer){g.bindFramebuffer(g.FRAMEBUFFER,entry.framebuffer);this.boundFramebuffer=entry.framebuffer;}
    }
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
  draw(v,count,stride,indices,n,state,uniforms,source,textures,layout=0,binding,commandOffset=0) {
    try {
      const g=this.gl,s=this.words(state,40),tx=this.wordView(textures,4*14),u=this.floatView(uniforms,152);
      if(!binding||((binding.kind==='raw')!==!!layout))throw new Error('GPU vertex binding does not match command layout');
      const kinds=[0,1,2,3].map(t=>tx[t*14]),shadows=[0,1,2,3].map(t=>tx[t*14+1]);
      if(kinds.some(k=>k===2||k===3))throw new Error('GPU 3D/cube texture admission is not implemented');
      this.pointerPrograms ??= new Map();
      const programKey=`${source}:${s[37]}:${kinds}:${shadows}:${!!s[32]}:${s[39]&0xffff}`;
      let program=this.pointerPrograms.get(programKey);
      if(!program){program=this.program(this.string(source),kinds,shadows,!!s[32],s[37],s[39]&0xffff);this.pointerPrograms.set(programKey,program);}
      program=this.readyProgram(program);
      this.attach(this.targets.get(s[0]&0x1fffffff),this.targets.get(s[1]&0x1fffffff));
      if(this.lastProgram!==program.p){g.useProgram(program.p);this.lastProgram=program.p;}
      const loc=name=>{if(!program.locations.has(name))program.locations.set(name,g.getUniformLocation(program.p,name));return program.locations.get(name);};
      const inputUniformWords=new Uint32Array(u.buffer,u.byteOffset,152),uniformWords=program.uniformWords,forceUniforms=!program.uniformsValid;
      const uniform=(name,offset,count)=>{
        const location=loc(name);if(location===null)return;
        let changed=forceUniforms;
        if(!changed)for(let i=0;i<count;i++)if(inputUniformWords[offset+i]!==uniformWords[offset+i]){changed=true;break;}
        if(changed){for(let i=0;i<count;i++)uniformWords[offset+i]=inputUniformWords[offset+i];g.uniform4fv(location,program.uniformSpans.get(name));}
      };
      uniform('surf',0,4);uniform('fogcolor',4,4);uniform('misc',8,4);uniform('pc',12,72);uniform('tfactor',84,4);uniform('texsize',88,16);uniform('bumpmat',104,16);uniform('bumplum',120,16);uniform('uvrect',136,16);
      program.uniformsValid=true;
      if(s[37]) {
        const constants=this.floatView(s[38],772),constantWords=new Uint32Array(constants.buffer,constants.byteOffset,772);
        if(s[39]&0x10000) {
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
      }
      const addressLocation=loc('addresses');
      if(addressLocation!==null){
        const candidate=program.addressScratch,previous=program.addresses;let changed=!program.addressesValid;
        for(let t=0;t<4;t++){
          const base=t*14,o=t*4;
          for(let c=0;c<4;c++){const value=tx[base+7+c];candidate[o+c]=value;if(!changed&&candidate[o+c]!==previous[o+c])changed=true;}
        }
        if(changed){g.uniform4fv(addressLocation,candidate);program.addresses=candidate;program.addressScratch=previous;program.addressesValid=true;}
      }
      const borderLocation=loc('border');
      if(borderLocation!==null){
        const candidate=program.borderScratch,previous=program.borders;let changed=!program.bordersValid;
        for(let t=0;t<4;t++){
          const base=t*14,o=t*4,c=tx[base+13];
          const r=Math.fround((c>>>16&255)/255),green=Math.fround((c>>>8&255)/255),b=Math.fround((c&255)/255),a=Math.fround((c>>>24)/255);
          candidate[o]=r;candidate[o+1]=green;candidate[o+2]=b;candidate[o+3]=a;
          if(!changed&&(r!==previous[o]||green!==previous[o+1]||b!==previous[o+2]||a!==previous[o+3]))changed=true;
        }
        if(changed){g.uniform4fv(borderLocation,candidate);program.borders=candidate;program.borderScratch=previous;program.bordersValid=true;}
      }
      for(let t=0;t<4;t++) {
        const q=tx.subarray(t*14,t*14+14); if(!q[0])continue;
        let image=this.targets.get(q[2]&0x1fffffff);
        if(shadows[t]&&!image)throw new Error('CPU depth-texture uploads are not admitted; depth sampling requires a GPU-owned surface');
        if(!image) {
          // Versioned SDK pixels have immutable owners until this batch is
          // consumed. Encoding is independent of the guest texture layout.
          const encoding=q[5]>>>8;
          if((q[5]&255)!==1||encoding>1)throw new Error('Unsupported GPU image encoding');
          const key=`${q[2]}:${q[3]}:${q[4]}:${encoding}`;
          image=this.images.get(key);
          if(!image){if(this.images.size>=4096)throw new Error('GPU texture cache budget exceeded');image={texture:g.createTexture(),frame:-1};this.images.set(key,image);}
          this.bindTexture(t,image.texture);
          if(image.hashLo!==q[11] || image.hashHi!==q[12]){
            this.bindTexture(t,image.texture,true);
            const size=q[3]*q[4]*4;
            if(!q[3]||!q[4]||!Number.isSafeInteger(size)||size>32*1024*1024)throw new Error('GPU image staging budget exceeded');
            if(size>this.imageUpload.length)this.imageUpload=new Uint8Array(this.capacity(this.imageUpload.length,size,32*1024*1024));
            const rgba=this.imageUpload.subarray(0,size);
            if(encoding===1)rgba.set(this.bytes(q[6],size));
            else {
              const pixels=this.floatView(q[6],size);
              for(let j=0;j<size;j++)rgba[j]=Math.round(Math.max(0,Math.min(1,pixels[j]))*255);
            }
            // An ordinary ArrayBuffer also works on WebGL implementations
            // that reject views backed by the shared Wasm memory.
            if(image.allocated)g.texSubImage2D(g.TEXTURE_2D,0,0,0,q[3],q[4],g.RGBA,g.UNSIGNED_BYTE,rgba);
            else {g.texImage2D(g.TEXTURE_2D,0,g.RGBA8,q[3],q[4],0,g.RGBA,g.UNSIGNED_BYTE,rgba);image.allocated=true;}
            image.hashLo=q[11];image.hashHi=q[12];
            if(this.frameStats){
              this.frameStats.textureUploads=(this.frameStats.textureUploads||0)+1;
              this.frameStats.textureUploadBytes=(this.frameStats.textureUploadBytes||0)+size;
              if(encoding===1)this.frameStats.byteTextureUploads=(this.frameStats.byteTextureUploads||0)+1;
            }
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
      this.bindGeometry(binding);
      g.drawElements(g.TRIANGLES,n,g.UNSIGNED_INT,this.indexOffsets.get(commandOffset));
      this.issuedDraws=(this.issuedDraws||0)+1;
      return true;
    } catch(error) {this.report(error.message);postMessage({type:'gpu-error',text:error.message});return false;}
  }
  present(id) {
    const t=this.targets.get(id);if(!t||t.depth)return false;
    const g=this.gl;if(this.canvas.width!==t.w||this.canvas.height!==t.h){this.canvas.width=t.w;this.canvas.height=t.h;}
    g.bindFramebuffer(g.READ_FRAMEBUFFER,this.readFbo);g.framebufferTexture2D(g.READ_FRAMEBUFFER,g.COLOR_ATTACHMENT0,g.TEXTURE_2D,t.texture,0);g.framebufferTexture2D(g.READ_FRAMEBUFFER,g.DEPTH_STENCIL_ATTACHMENT,g.TEXTURE_2D,null,0);g.readBuffer(g.COLOR_ATTACHMENT0);this.readFramebufferTarget=t;
    g.bindFramebuffer(g.DRAW_FRAMEBUFFER,null);g.disable(g.SCISSOR_TEST);g.blitFramebuffer(0,0,t.w,t.h,0,t.h,t.w,0,g.COLOR_BUFFER_BIT,g.NEAREST);g.flush();
    this.boundFramebuffer=null;this.lastState=undefined;
    ++this.frame;
    this.lastSubmitMs=this.submitMs||0;this.lastIssuedDraws=this.issuedDraws||0;this.submitMs=0;this.issuedDraws=0;
    this.lastFrameStats=this.frameStats;this.frameStats=undefined;
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
    const g=this.gl;this.attach(t,null);const pixels=new Uint8Array(t.w*t.h*4);g.readPixels(0,0,t.w,t.h,g.RGBA,g.UNSIGNED_BYTE,pixels);
    const error=g.getError();if(error!==g.NO_ERROR)throw new Error(`GPU readback error 0x${error.toString(16)}`);
    this.bytes(ptr,pixels.length).set(pixels);return true;
  }
  invalidate(id) {
    const t=this.targets.get(id);if(!t)return;
    const g=this.gl;
    for(const [key,entry] of this.framebuffers){
      if(entry.color!==t&&entry.depth!==t)continue;
      if(this.boundFramebuffer===entry.framebuffer){g.bindFramebuffer(g.FRAMEBUFFER,null);this.boundFramebuffer=null;}
      g.deleteFramebuffer(entry.framebuffer);this.framebuffers.delete(key);
    }
    if(this.readFramebufferTarget===t){
      this.boundFramebuffer=null;
      g.bindFramebuffer(g.READ_FRAMEBUFFER,this.readFbo);
      g.framebufferTexture2D(g.READ_FRAMEBUFFER,g.COLOR_ATTACHMENT0,g.TEXTURE_2D,null,0);
      this.readFramebufferTarget=undefined;
    }
    g.deleteTexture(t.texture);this.targets.delete(id);
  }
}
