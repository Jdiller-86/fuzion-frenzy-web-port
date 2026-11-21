export const GAME_AUDIO_RATE = 48000;
export const GAME_AUDIO_PROCESSOR = 'fuzion-stereo-pcm';

const capacity = 4096;
const mask = capacity - 1;
const header = 4 * Int32Array.BYTES_PER_ELEMENT;
const bytes = header + capacity * 2 * Float32Array.BYTES_PER_ELEMENT;
const READ = 0, WRITE = 1, STARVED = 2, DROPPED = 3;

// One producer publishes the completed write position and one consumer returns
// finished slots. Unsigned distance keeps the counters valid across wrap.
export class StereoPcmRing {
  constructor(buffer = new SharedArrayBuffer(bytes)) {
    if (!(buffer instanceof SharedArrayBuffer) || buffer.byteLength !== bytes)
      throw new TypeError('Invalid stereo PCM ring');
    this.buffer = buffer;
    this.state = new Int32Array(buffer, 0, 4);
    this.samples = new Float32Array(buffer, header);
  }

  available() {
    return (Atomics.load(this.state, WRITE) - Atomics.load(this.state, READ)) >>> 0;
  }

  write(samples, offset, frames) {
    const position = Atomics.load(this.state, WRITE);
    const queued = (position - Atomics.load(this.state, READ)) >>> 0;
    if (frames > capacity - queued) {
      Atomics.add(this.state, DROPPED, frames);
      return;
    }
    const start = position & mask;
    const first = Math.min(frames, capacity - start) * 2;
    this.samples.set(samples.subarray(offset, offset + first), start * 2);
    if (first !== frames * 2) this.samples.set(samples.subarray(offset + first, offset + frames * 2));
    Atomics.store(this.state, WRITE, position + frames);
  }

  read(left, right) {
    const position = Atomics.load(this.state, READ);
    const available = (Atomics.load(this.state, WRITE) - position) >>> 0;
    const frames = Math.min(available, left.length);
    for (let i = 0; i < frames; i++) {
      const source = ((position + i) & mask) * 2;
      left[i] = this.samples[source];
      right[i] = this.samples[source + 1];
    }
    Atomics.store(this.state, READ, position + frames);
    return frames;
  }

  stats() {
    return {
      read: Atomics.load(this.state, READ) >>> 0,
      written: Atomics.load(this.state, WRITE) >>> 0,
      starved: Atomics.load(this.state, STARVED) >>> 0,
      dropped: Atomics.load(this.state, DROPPED) >>> 0,
    };
  }
}

if (typeof registerProcessor === 'function') {
  class GameAudioProcessor extends AudioWorkletProcessor {
    constructor({processorOptions}) {
      super();
      this.ring = new StereoPcmRing(processorOptions.pcm);
      this.priming = true;
      this.started = false;
    }

    process(_inputs, outputs) {
      const [left, right] = outputs[0];
      if (!right) return true;
      // Refill to the same 30 ms lead as the scheduled-buffer path.
      if (this.priming && this.ring.available() < GAME_AUDIO_RATE * 0.03) {
        if (this.started) Atomics.add(this.ring.state, STARVED, left.length);
        return true;
      }
      const frames = this.ring.read(left, right);
      this.started = true;
      this.priming = frames < left.length;
      if (this.priming) Atomics.add(this.ring.state, STARVED, left.length - frames);
      return true;
    }
  }
  registerProcessor(GAME_AUDIO_PROCESSOR, GameAudioProcessor);
}
