const textEncoder = new TextEncoder()
const maxInt32 = 2 ** 32
const maxUInt32 = 2 ** 31

/**
 * @typedef {import('./interface').RawPBLink} RawPBLink
 */

/**
 * @typedef {import('./interface').RawPBNode} RawPBNode
 */

// the encoders work backward from the end of the bytes array

/**
 * encodeLink() is passed a slice of the parent byte array that ends where this
 * link needs to end, so it packs to the right-most part of the passed `bytes`
 *
 * @param {RawPBLink} link
 * @param {Uint8Array} bytes
 * @returns {number}
 */
function encodeLink (link, bytes) {
  let i = bytes.length

  if (typeof link.Tsize === 'number') {
    if (link.Tsize < 0) {
      throw new Error('Tsize cannot be negative')
    }
    if (!Number.isSafeInteger(link.Tsize)) {
      throw new Error('Tsize too large for encoding')
    }
    i = encodeVarint(bytes, i, link.Tsize) - 1
    bytes[i] = 0x18
  }

  if (typeof link.Name === 'string') {
    const nameBytes = textEncoder.encode(link.Name)
    i -= nameBytes.length
    bytes.set(nameBytes, i)
    i = encodeVarint(bytes, i, nameBytes.length) - 1
    bytes[i] = 0x12
  }

  if (link.Hash) {
    i -= link.Hash.length
    bytes.set(link.Hash, i)
    i = encodeVarint(bytes, i, link.Hash.length) - 1
    bytes[i] = 0xa
  }

  return bytes.length - i
}

/**
 * Encodes a PBNode into a new byte array of precisely the correct size
 *
 * @param {RawPBNode} node
 * @returns {Uint8Array}
 */
export function encodeNode (node) {
  const size = sizeNode(node)
  const bytes = new Uint8Array(size)
  let i = size

  if (node.Data) {
    i -= node.Data.length
    bytes.set(node.Data, i)
    i = encodeVarint(bytes, i, node.Data.length) - 1
    bytes[i] = 0xa
  }

  if (node.Links) {
    for (let index = node.Links.length - 1; index >= 0; index--) {
      const size = encodeLink(node.Links[index], bytes.subarray(0, i))
      i -= size
      i = encodeVarint(bytes, i, size) - 1
      bytes[i] = 0x12
    }
  }

  return bytes
}

/**
 * work out exactly how many bytes this link takes up
 *
 * @param {RawPBLink} link
 * @returns
 */
function sizeLink (link) {
  let n = 0

  if (link.Hash) {
    const l = link.Hash.length
    n += 1 + l + sov(l)
  }

  if (typeof link.Name === 'string') {
    const l = textEncoder.encode(link.Name).length
    n += 1 + l + sov(l)
  }

  if (typeof link.Tsize === 'number') {
    n += 1 + sov(link.Tsize)
  }

  return n
}

/**
 * Work out exactly how many bytes this node takes up
 *
 * @param {RawPBNode} node
* @returns {number}
 */
function sizeNode (node) {
  let n = 0

  if (node.Data) {
    const l = node.Data.length
    n += 1 + l + sov(l)
  }

  if (node.Links) {
    for (const link of node.Links) {
      const l = sizeLink(link)
      n += 1 + l + sov(l)
    }
  }

  return n
}

/**
 * @param {Uint8Array} bytes
 * @param {number} offset
 * @param {number} v
 * @returns {number}
 */
function encodeVarint (bytes, offset, v) {
  offset -= sov(v)
  const base = offset

  while (v >= maxUInt32) {
    bytes[offset++] = (v & 0x7f) | 0x80
    v /= 128
  }

  while (v >= 128) {
    bytes[offset++] = (v & 0x7f) | 0x80
    v >>>= 7
  }

  bytes[offset] = v

  return base
}

/**
 * size of varint
 *
 * @param {number} x
 * @returns {number}
 */
function sov (x) {
  if (x % 2 === 0) {
    x++
  }
  return Math.floor((len64(x) + 6) / 7)
}

/**
 * golang math/bits, how many bits does it take to represent this integer?
 *
 * @param {number} x
 * @returns {number}
 */
function len64 (x) {
  let n = 0
  if (x >= maxInt32) {
    x = Math.floor(x / maxInt32)
    n = 32
  }
  if (x >= (1 << 16)) {
    x >>>= 16
    n += 16
  }
  if (x >= (1 << 8)) {
    x >>>= 8
    n += 8
  }
  return n + len8tab[x]
}

// golang math/bits
const len8tab = [
  0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
]
