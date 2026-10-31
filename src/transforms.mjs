// Source-owned matrix math in the convention corroborated by the local renderer.
// Serialized quaternions rotate in the opposite direction to the usual column-vector convention.
export function multiply(a,b){const c=new Float64Array(16);for(let i=0;i<4;i++)for(let j=0;j<4;j++)for(let k=0;k<4;k++)c[i*4+j]+=a[i*4+k]*b[k*4+j];return c;}
export function matrix(t,q,s){let [x,y,z,w]=q; x=-x;y=-y;z=-z;let n=Math.hypot(x,y,z,w);if(n<1e-8)throw new Error('Invalid quaternion');x/=n;y/=n;z/=n;w/=n;return new Float64Array([(1-2*(y*y+z*z))*s[0],(2*(x*y-z*w))*s[1],(2*(x*z+y*w))*s[2],t[0],(2*(x*y+z*w))*s[0],(1-2*(x*x+z*z))*s[1],(2*(y*z-x*w))*s[2],t[1],(2*(x*z-y*w))*s[0],(2*(y*z+x*w))*s[1],(1-2*(x*x+y*y))*s[2],t[2],0,0,0,1]);}
