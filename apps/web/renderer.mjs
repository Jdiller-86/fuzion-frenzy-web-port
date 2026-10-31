import {seatAction,visibleHazards} from '../../packages/simulation/twisted-system.mjs';
const COLORS=[[.3,1,.42],[1,.3,.28],[.38,.65,1],[1,.5,.85]];
// Camera, track anchor and duck pose remain presentation approximations (TS-R1).
export function createRenderer(canvas,assets){
  const gl=canvas.getContext('webgl2',{antialias:true});if(!gl)throw new Error('WebGL 2 is required');
  const resources=[];
  function shader(type,source){const s=gl.createShader(type);resources.push(['Shader',s]);gl.shaderSource(s,source);gl.compileShader(s);if(!gl.getShaderParameter(s,gl.COMPILE_STATUS))throw new Error(gl.getShaderInfoLog(s));return s;}
  const program=gl.createProgram();resources.push(['Program',program]);
  gl.attachShader(program,shader(gl.VERTEX_SHADER,`#version 300 es
    in vec3 position; in vec2 uv; in vec4 diffuse; uniform mat4 view; uniform vec3 offset; uniform float angle; uniform float squash;
    out vec2 texcoord; out vec4 vertexColor;
    void main(){float c=cos(angle),s=sin(angle);vec3 p=position*.01;p.y*=squash;p=vec3(c*p.x+s*p.z,p.y,-s*p.x+c*p.z)+offset;gl_Position=view*vec4(p,1);texcoord=uv;vertexColor=diffuse;}`));
  gl.attachShader(program,shader(gl.FRAGMENT_SHADER,`#version 300 es
    precision mediump float;in vec2 texcoord;in vec4 vertexColor;uniform sampler2D image;uniform bool textured;uniform bool modulate;uniform vec3 tint;out vec4 color;
    void main(){color=textured?texture(image,texcoord):vec4(tint,1);if(modulate)color=clamp(color*vertexColor*2.,0.,1.);if(color.a<.05)discard;}`));
  gl.linkProgram(program);if(!gl.getProgramParameter(program,gl.LINK_STATUS))throw new Error('Cannot link renderer');gl.useProgram(program);
  const uniform=name=>gl.getUniformLocation(program,name),textures=new Map();
  function texture(t){const out=gl.createTexture();resources.push(['Texture',out]);gl.bindTexture(gl.TEXTURE_2D,out);gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,t.width,t.height,0,gl.RGBA,gl.UNSIGNED_BYTE,t.rgba);for(const [name,value]of [[gl.TEXTURE_MIN_FILTER,gl.LINEAR],[gl.TEXTURE_MAG_FILTER,gl.LINEAR],[gl.TEXTURE_WRAP_S,gl.REPEAT],[gl.TEXTURE_WRAP_T,gl.REPEAT]])gl.texParameteri(gl.TEXTURE_2D,name,value);return out;}
  for(const t of assets.textures)textures.set(t.hash,texture(t));const characterTexture=texture(assets.characterTexture);
  function mesh(m){
    const vao=gl.createVertexArray();resources.push(['VertexArray',vao]);gl.bindVertexArray(vao);
    const attribute=(name,array,width,type=gl.FLOAT,normalized=false)=>{const b=gl.createBuffer();resources.push(['Buffer',b]);gl.bindBuffer(gl.ARRAY_BUFFER,b);gl.bufferData(gl.ARRAY_BUFFER,array,gl.DYNAMIC_DRAW);const a=gl.getAttribLocation(program,name);gl.enableVertexAttribArray(a);gl.vertexAttribPointer(a,width,type,normalized,0,0);return b;};
    const position=attribute('position',m.positions,3);attribute('uv',m.uvs??new Float32Array(m.positions.length/3*2),2);
    attribute('diffuse',m.colors??new Uint8Array(m.positions.length/3*4).fill(128),4,gl.UNSIGNED_BYTE,true);
    const index=gl.createBuffer();resources.push(['Buffer',index]);gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,index);gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,m.indices,gl.STATIC_DRAW);
    return {vao,position,count:m.indices.length,batches:m.batches};
  }
  const meshes=Object.fromEntries(Object.entries(assets.meshes).map(([key,m])=>[key,mesh(m)]));
  const characters=Array.from({length:4},()=>mesh(assets.character));
  // Independently authored seat markers, not replacement game geometry.
  const markerPositions=[],markerIndices=[];for(let i=0;i<48;i++){const a=i/48*Math.PI*2;markerPositions.push(Math.cos(a)*75,0,Math.sin(a)*75,Math.cos(a)*92,0,Math.sin(a)*92);const j=i*2,k=(i*2+2)%96;markerIndices.push(j,j+1,k,j+1,k+1,k);}
  const marker=mesh({positions:new Float32Array(markerPositions),indices:new Uint32Array(markerIndices)});
  const normalized=a=>{const n=Math.hypot(...a);return a.map(v=>v/n);},cross=(a,b)=>[a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]],dot=(a,b)=>a.reduce((n,v,i)=>n+v*b[i],0);
  function camera(aspect){const eye=[-24,10,24],target=[-7,-2,0],z=normalized(eye.map((v,i)=>v-target[i])),x=normalized(cross([0,1,0],z)),y=cross(z,x),v=[...x,-dot(x,eye),...y,-dot(y,eye),...z,-dot(z,eye),0,0,0,1];const f=1/Math.tan(.65/2),near=.1,far=500,p=[f/aspect,0,0,0,0,f,0,0,0,0,(far+near)/(near-far),2*far*near/(near-far),0,0,-1,0],out=new Float32Array(16);for(let i=0;i<4;i++)for(let j=0;j<4;j++)for(let k=0;k<4;k++)out[j*4+i]+=p[i*4+k]*v[k*4+j];return out;}
  function draw(m,offset=[0,0,0],angle=0,squash=1,tint=[1,1,1],singleTexture=null){
    gl.bindVertexArray(m.vao);gl.uniform3fv(uniform('offset'),offset);gl.uniform1f(uniform('angle'),angle);gl.uniform1f(uniform('squash'),squash);gl.uniform3fv(uniform('tint'),tint);gl.uniform1i(uniform('modulate'),m.batches?1:0);
    for(const b of m.batches??[{hash:0,start:0,count:m.count,pass:0}]){if(!b.count)continue;const t=singleTexture??textures.get(b.hash);if(b.hash!==0&&!t)throw new Error('Missing renderer texture');gl.uniform1i(uniform('textured'),t?1:0);if(t)gl.bindTexture(gl.TEXTURE_2D,t);const blend=b.pass===2;gl.depthMask(!blend);if(blend){gl.enable(gl.BLEND);gl.blendFunc(gl.SRC_ALPHA,gl.ONE_MINUS_SRC_ALPHA);}else gl.disable(gl.BLEND);gl.drawElements(gl.TRIANGLES,b.count,gl.UNSIGNED_INT,b.start*4);}
  }
  let disposed=false;
  return {
    updatePoses(poses){if(disposed)return;poses.forEach((p,i)=>{gl.bindBuffer(gl.ARRAY_BUFFER,characters[i].position);gl.bufferSubData(gl.ARRAY_BUFFER,0,p);});},
    render(state){
      if(disposed)return;
      const width=Math.round(canvas.clientWidth*Math.min(devicePixelRatio,1.5)),height=Math.round(canvas.clientHeight*Math.min(devicePixelRatio,1.5));if(canvas.width!==width||canvas.height!==height){canvas.width=width;canvas.height=height;}
      gl.viewport(0,0,width,height);gl.useProgram(program);gl.uniformMatrix4fv(uniform('view'),false,camera(width/height));gl.enable(gl.DEPTH_TEST);gl.depthMask(true);gl.clearColor(.025,.035,.065,1);gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT);
      draw(meshes.universe);draw(meshes.world);draw(meshes.drill);
      const spacing=Math.PI/4,anchor=-2.98,pitch=11.83125/(2*Math.PI);
      for(const h of visibleHazards(state,5)){const a=h.distance*spacing;if(a>Math.PI*1.35||a< -2.4)continue;draw(meshes[h.kind],[0,anchor+pitch*a+(h.kind==='high'?1.7:.65),0],a);}
      state.players.forEach((p,i)=>{
        const a=-p.lag*spacing,r=6.4+i*1.85,action=seatAction(state,p),fall=p.eliminated?Math.min(12,(state.tick-p.eliminatedTick)/18):0;
        const jump=action.jumping?Math.sin(Math.PI*action.jumpAge/state.rules.jumpTicks)*1.7:0;
        const pos=[-r*Math.cos(a),anchor+pitch*a,-(-r)*Math.sin(a)];draw(marker,[pos[0],pos[1]+.04,pos[2]],0,1,COLORS[i]);
        draw(characters[i],[pos[0],pos[1]+jump-fall,pos[2]],a+Math.PI/2,action.ducking?.48:1,[1,1,1],characterTexture);
      });
      if(gl.getError()!==gl.NO_ERROR)throw new Error('WebGL draw failed');
    },
    dispose(){if(disposed)return;disposed=true;for(const [kind,r]of resources)gl['delete'+kind](r);gl.depthMask(true);gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT);}
  };
}
