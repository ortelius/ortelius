import * as Block from 'multiformats/block'
import * as codec from '@ipld/dag-cbor'
import { murmur3128 as hasher } from '@multiformats/murmur3'

async function run () {
  const value = { hello: 'world' }
  const block = await Block.encode({ value, hasher, codec })
  console.log(block.cid)
  // -> CID(bafyseebn7ksk6khsn4an2lzmae6wm4qk)
}

run().catch(console.error)
