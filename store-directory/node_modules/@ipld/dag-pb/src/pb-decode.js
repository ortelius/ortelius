const textDecoder = new TextDecoder()

/**
 * @typedef {import('./interface').RawPBLink} RawPBLink
 */

/**
 * @typedef {import('./interface').RawPBNode} RawPBNode
 */

/**
 * @param {Uint8Array} bytes
 * @param {number} offset
 * @returns {[number, number]}
 */
function decodeVarint (bytes, offset) {
  let v = 0

  for (let shift = 0; ; shift += 7) {
    /* c8 ignore next 3 */
    if (shift >= 64) {
      throw new Error('protobuf: varint overflow')
    }
    /* c8 ignore next 3 */
    if (offset >= bytes.length) {
      throw new Error('protobuf: unexpected end of data')
    }

    const b = bytes[offset++]
    v += shift < 28 ? (b & 0x7f) << shift : (b & 0x7f) * (2 ** shift)
    if (b < 0x80) {
      break
    }
  }
  return [v, offset]
}

/**
 * @param {Uint8Array} bytes
 * @param {number} offset
 * @returns {[Uint8Array, number]}
 */
function decodeBytes (bytes, offset) {
  let byteLen
  ;[byteLen, offset] = decodeVarint(bytes, offset)
  const postOffset = offset + byteLen

  /* c8 ignore next 3 */
  if (byteLen < 0 || postOffset < 0) {
    throw new Error('protobuf: invalid length')
  }
  /* c8 ignore next 3 */
  if (postOffset > bytes.length) {
    throw new Error('protobuf: unexpected end of data')
  }

  return [bytes.subarray(offset, postOffset), postOffset]
}

/**
 * @param {Uint8Array} bytes
 * @param {number} index
 * @returns {[number, number, number]}
 */
function decodeKey (bytes, index) {
  let wire
  ;[wire, index] = decodeVarint(bytes, index)
  // [wireType, fieldNum, newIndex]
  return [wire & 0x7, wire >> 3, index]
}

/**
 * @param {Uint8Array} bytes
 * @returns {RawPBLink}
 */
function decodeLink (bytes) {
  /** @type {RawPBLink} */
  const link = {}
  const l = bytes.length
  let index = 0

  while (index < l) {
    let wireType, fieldNum
    ;[wireType, fieldNum, index] = decodeKey(bytes, index)

    if (fieldNum === 1) {
      if (link.Hash) {
        throw new Error('protobuf: (PBLink) duplicate Hash section')
      }
      if (wireType !== 2) {
        throw new Error(`protobuf: (PBLink) wrong wireType (${wireType}) for Hash`)
      }
      if (link.Name !== undefined) {
        throw new Error('protobuf: (PBLink) invalid order, found Name before Hash')
      }
      if (link.Tsize !== undefined) {
        throw new Error('protobuf: (PBLink) invalid order, found Tsize before Hash')
      }

      ;[link.Hash, index] = decodeBytes(bytes, index)
    } else if (fieldNum === 2) {
      if (link.Name !== undefined) {
        throw new Error('protobuf: (PBLink) duplicate Name section')
      }
      if (wireType !== 2) {
        throw new Error(`protobuf: (PBLink) wrong wireType (${wireType}) for Name`)
      }
      if (link.Tsize !== undefined) {
        throw new Error('protobuf: (PBLink) invalid order, found Tsize before Name')
      }

      let byts
      ;[byts, index] = decodeBytes(bytes, index)
      link.Name = textDecoder.decode(byts)
    } else if (fieldNum === 3) {
      if (link.Tsize !== undefined) {
        throw new Error('protobuf: (PBLink) duplicate Tsize section')
      }
      if (wireType !== 0) {
        throw new Error(`protobuf: (PBLink) wrong wireType (${wireType}) for Tsize`)
      }

      ;[link.Tsize, index] = decodeVarint(bytes, index)
    } else {
      throw new Error(`protobuf: (PBLink) invalid fieldNumber, expected 1, 2 or 3, got ${fieldNum}`)
    }
  }

  /* c8 ignore next 3 */
  if (index > l) {
    throw new Error('protobuf: (PBLink) unexpected end of data')
  }

  return link
}

/**
 * @param {Uint8Array} bytes
 * @returns {RawPBNode}
 */
export function decodeNode (bytes) {
  const l = bytes.length
  let index = 0
  /** @type {RawPBLink[]|void} */
  let links
  let linksBeforeData = false
  /** @type {Uint8Array|void} */
  let data

  while (index < l) {
    let wireType, fieldNum
    ;[wireType, fieldNum, index] = decodeKey(bytes, index)

    if (wireType !== 2) {
      throw new Error(`protobuf: (PBNode) invalid wireType, expected 2, got ${wireType}`)
    }

    if (fieldNum === 1) {
      if (data) {
        throw new Error('protobuf: (PBNode) duplicate Data section')
      }

      ;[data, index] = decodeBytes(bytes, index)
      if (links) {
        linksBeforeData = true
      }
    } else if (fieldNum === 2) {
      if (linksBeforeData) { // interleaved Links/Dode/Links
        throw new Error('protobuf: (PBNode) duplicate Links section')
      } else if (!links) {
        links = []
      }
      let byts
      ;[byts, index] = decodeBytes(bytes, index)
      links.push(decodeLink(byts))
    } else {
      throw new Error(`protobuf: (PBNode) invalid fieldNumber, expected 1 or 2, got ${fieldNum}`)
    }
  }

  /* c8 ignore next 3 */
  if (index > l) {
    throw new Error('protobuf: (PBNode) unexpected end of data')
  }

  /** @type {RawPBNode} */
  const node = {}
  if (data) {
    node.Data = data
  }
  node.Links = links || []
  return node
}
