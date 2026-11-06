// Identity checks for bounded local blobs; does not establish ownership or provenance.
export async function sha256Blob(blob, expectedSize, beforeHash, maxBytes=16*1024*1024) {
  if (!Number.isSafeInteger(maxBytes)||maxBytes<0||maxBytes>32*1024*1024||!(blob instanceof Blob) || blob.size !== expectedSize || !Number.isSafeInteger(expectedSize) || expectedSize < 0 || expectedSize > maxBytes) throw new Error('Unsupported content size');
  const bytes=await blob.arrayBuffer();
  if(beforeHash) await beforeHash(bytes.byteLength);
  return [...new Uint8Array(await crypto.subtle.digest('SHA-256',bytes))].map(x=>x.toString(16).padStart(2,'0')).join('');
}
export async function requireBlobIdentity(blob, size, expectedHash, beforeHash, maxBytes) {
  if(!/^[a-f0-9]{64}$/.test(expectedHash) || await sha256Blob(blob,size,beforeHash,maxBytes)!==expectedHash) throw new Error('Unsupported or changed content');
}
