// Identity checks for bounded local blobs; does not establish ownership or provenance.
export async function sha256Blob(blob, expectedSize, beforeHash) {
  if (!(blob instanceof Blob) || blob.size !== expectedSize || !Number.isSafeInteger(expectedSize) || expectedSize < 0 || expectedSize > 16 * 1024 * 1024) throw new Error('Unsupported content size');
  const bytes=await blob.arrayBuffer();
  if(beforeHash) await beforeHash(bytes.byteLength);
  return [...new Uint8Array(await crypto.subtle.digest('SHA-256',bytes))].map(x=>x.toString(16).padStart(2,'0')).join('');
}
export async function requireBlobIdentity(blob, size, expectedHash, beforeHash) {
  if(!/^[a-f0-9]{64}$/.test(expectedHash) || await sha256Blob(blob,size,beforeHash)!==expectedHash) throw new Error('Unsupported or changed content');
}
