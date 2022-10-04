import { from } from 'multiformats/hashes/hasher';
import { bytes } from 'multiformats';
import mur from 'murmurhash3js-revisited';
function fromNumberTo32BitBuf(number) {
  const bytes = new Array(4);
  for (let i = 0; i < 4; i++) {
    bytes[i] = number & 255;
    number = number >> 8;
  }
  return new Uint8Array(bytes);
}
export const murmur332 = from({
  name: 'murmur3-32',
  code: 35,
  encode: input => fromNumberTo32BitBuf(mur.x86.hash32(input))
});
export const murmur3128 = from({
  name: 'murmur3-128',
  code: 34,
  encode: input => bytes.fromHex(mur.x64.hash128(input))
});