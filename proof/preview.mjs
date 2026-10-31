// Experimental local geometry preview; not a fidelity-validated arena renderer.
export function renderGeometry(canvas, positions, indices, texture = null, uvs = null, batches = null, options = {}) {
  const gl = canvas.getContext('webgl2', { antialias: true });
  if (!gl) throw new Error('WebGL 2 is unavailable');
  const compile = (type, source) => { const s=gl.createShader(type); gl.shaderSource(s,source); gl.compileShader(s); if (!gl.getShaderParameter(s,gl.COMPILE_STATUS)) throw new Error('Shader compilation failed'); return s; };
  const vs=compile(gl.VERTEX_SHADER,`#version 300 es
    in vec3 position; in vec2 uv; in vec4 vertexColor; uniform float angle; uniform vec2 tilt; uniform float depthSign; out float height; out vec2 texcoord; out vec4 diffuse;
    void main() { float c=cos(angle), s=sin(angle); vec3 p=vec3(c*position.x+s*position.z,position.y,-s*position.x+c*position.z); p=vec3(p.x,tilt.x*p.y-tilt.y*p.z,tilt.y*p.y+tilt.x*p.z); height=position.y; texcoord=uv; diffuse=vertexColor; gl_Position=vec4(p.x,p.y,p.z*0.4*depthSign,1.0); }`);
  const fs=compile(gl.FRAGMENT_SHADER,`#version 300 es
    precision mediump float; in float height; in vec2 texcoord; in vec4 diffuse; uniform sampler2D image; uniform bool textured; uniform bool vertexModulate; out vec4 color;
    void main() { color=textured?texture(image,texcoord):vec4(0.30+height*0.08,0.66+height*0.08,0.71+height*0.08,1.0); if(vertexModulate)color=clamp(color*diffuse*2.0,0.0,1.0); if(color.a<(vertexModulate?0.001:0.5)) discard; }`);
  const program=gl.createProgram(); gl.attachShader(program,vs); gl.attachShader(program,fs); gl.linkProgram(program);
  if(!gl.getProgramParameter(program,gl.LINK_STATUS)) throw new Error('Shader linking failed');
  let min=[Infinity,Infinity,Infinity], max=[-Infinity,-Infinity,-Infinity];
  const frameLength=options.frameVertices===undefined?positions.length:options.frameVertices*3;
  if(!Number.isSafeInteger(frameLength)||frameLength<=0||frameLength>positions.length)throw new Error('Invalid framing range');
  for(let i=0;i<frameLength;i++) { const k=i%3; min[k]=Math.min(min[k],positions[i]); max[k]=Math.max(max[k],positions[i]); }
  const span=Math.max(...max.map((x,i)=>x-min[i])); if(!Number.isFinite(span) || span<=0) throw new Error('Invalid geometry bounds');
  const normalized=positions.map((x,i)=>(x-(min[i%3]+max[i%3])/2)/span*1.5);
  gl.useProgram(program); gl.uniform1f(gl.getUniformLocation(program,'depthSign'),options.reverseDepth?-1:1); let buffer=gl.createBuffer(); gl.bindBuffer(gl.ARRAY_BUFFER,buffer); gl.bufferData(gl.ARRAY_BUFFER,normalized,gl.STATIC_DRAW);
  const a=gl.getAttribLocation(program,'position'); gl.enableVertexAttribArray(a); gl.vertexAttribPointer(a,3,gl.FLOAT,false,0,0);
  const glTextures=new Map(), extraBuffers=[];
  gl.uniform1i(gl.getUniformLocation(program,'vertexModulate'),options.colors?1:0);
  if(options.colors){if(options.colors.length!==positions.length/3*4)throw new Error('Invalid vertex color dimensions');const cb=gl.createBuffer();extraBuffers.push(cb);gl.bindBuffer(gl.ARRAY_BUFFER,cb);gl.bufferData(gl.ARRAY_BUFFER,options.colors,gl.STATIC_DRAW);const location=gl.getAttribLocation(program,'vertexColor');gl.enableVertexAttribArray(location);gl.vertexAttribPointer(location,4,gl.UNSIGNED_BYTE,true,0,0);}
  if(texture && uvs) {
    const tb=gl.createBuffer(); extraBuffers.push(tb); gl.bindBuffer(gl.ARRAY_BUFFER,tb); gl.bufferData(gl.ARRAY_BUFFER,uvs,gl.STATIC_DRAW); const location=gl.getAttribLocation(program,'uv'); gl.enableVertexAttribArray(location); gl.vertexAttribPointer(location,2,gl.FLOAT,false,0,0);
    for(const item of (Array.isArray(texture)?texture:[texture])) { const t=gl.createTexture(); gl.bindTexture(gl.TEXTURE_2D,t); gl.texParameteri(gl.TEXTURE_2D,gl.TEXTURE_MIN_FILTER,gl.LINEAR); gl.texParameteri(gl.TEXTURE_2D,gl.TEXTURE_MAG_FILTER,gl.LINEAR); gl.texParameteri(gl.TEXTURE_2D,gl.TEXTURE_WRAP_S,gl.REPEAT); gl.texParameteri(gl.TEXTURE_2D,gl.TEXTURE_WRAP_T,gl.REPEAT); gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,item.width,item.height,0,gl.RGBA,gl.UNSIGNED_BYTE,item.rgba); glTextures.set(item.hash??0,t); }
  }
  gl.uniform1i(gl.getUniformLocation(program,'textured'),texture&&uvs?1:0);
  let ib=gl.createBuffer(); gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,ib); gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,indices,gl.STATIC_DRAW);
  gl.viewport(0,0,canvas.width,canvas.height); gl.clearColor(0.04,0.07,0.1,1); gl.enable(gl.DEPTH_TEST);
  gl.uniform2f(gl.getUniformLocation(program,'tilt'),options.levelDirection?Math.cos(0.4):0.78,options.levelDirection?Math.sin(0.4):0.62);
  if(options.cull==='back'||options.cull==='front'){gl.enable(gl.CULL_FACE);gl.cullFace(options.cull==='back'?gl.BACK:gl.FRONT);}
  if(options.colors&&batches?.some(b=>b.pass===1))throw new Error('Unsupported middle draw pass');
  const drawBatches=options.colors&&batches?[...batches.filter(b=>b.pass!==2),...batches.filter(b=>b.pass===2)]:batches;
  function draw(next) {
    if(next.length!==positions.length) throw new Error('Geometry size changed');
    const adjusted=next.map((x,i)=>(x-(min[i%3]+max[i%3])/2)/span*1.5);
    gl.bindBuffer(gl.ARRAY_BUFFER,buffer); gl.bufferSubData(gl.ARRAY_BUFFER,0,adjusted);
    gl.depthMask(true); gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT); gl.uniform1f(gl.getUniformLocation(program,'angle'),options.levelDirection?-Math.PI/2:0.65); if(drawBatches) { for(const batch of drawBatches) { const blend=!!options.colors&&(batch.pass===2); if(blend){gl.enable(gl.BLEND);gl.blendFunc(gl.SRC_ALPHA,gl.ONE_MINUS_SRC_ALPHA);}else gl.disable(gl.BLEND);gl.depthMask(!blend); const t=glTextures.get(batch.hash); if(!t)throw new Error('Missing preview material'); gl.bindTexture(gl.TEXTURE_2D,t); gl.drawElements(gl.TRIANGLES,batch.count,gl.UNSIGNED_INT,batch.start*4); } } else { if(texture)gl.bindTexture(gl.TEXTURE_2D,glTextures.get(0)); gl.drawElements(gl.TRIANGLES,indices.length,gl.UNSIGNED_INT,0); }
    if(gl.getError()!==gl.NO_ERROR) throw new Error('WebGL draw failed');
  }
  draw.dispose=()=>{ for(const t of glTextures.values())gl.deleteTexture(t);for(const b of extraBuffers)gl.deleteBuffer(b);gl.deleteBuffer(buffer);gl.deleteBuffer(ib);gl.deleteProgram(program);gl.deleteShader(vs);gl.deleteShader(fs);gl.getExtension('WEBGL_lose_context')?.loseContext(); };
  draw(positions); return draw;
}
