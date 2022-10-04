# @multiformats/murmur3

Murmur3-32 (x86 32-bit) and Murmur3-128 (x64 128-bit) multihash hashers for [multiformats](https://github.com/multiformats/js-multiformats).

`MultihashHashers`s are exported from this library, they produce `MultihashDigest`s. Details about these can be found in the [multiformats multihash interface definitions](https://github.com/multiformats/js-multiformats/blob/master/src/hashes/interface.ts).

```js
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
```

## Usage

The `@multiformats/murmur3` package exports `murmur332` and `murmur3128` `MultihashHasher`s. The Multicodecs [table](https://github.com/multiformats/multicodec/blob/master/table.csv) defines these multihashes.

The `murmur3-32`, multicodec code `0x23`, may be imported as:

```js
import { murmur332 } from '@multiformats/murmur3'
```

The `murmur3-128`, multicodec code `0x22`, may be imported as:

```js
import { murmur3128 } from '@multiformats/murmur3'
```

## License

Licensed under either of

 * Apache 2.0, ([LICENSE-APACHE](LICENSE-APACHE) / http://www.apache.org/licenses/LICENSE-2.0)
 * MIT ([LICENSE-MIT](LICENSE-MIT) / http://opensource.org/licenses/MIT)

### Contribution

Unless you explicitly state otherwise, any contribution intentionally submitted for inclusion in the work by you, as defined in the Apache-2.0 license, shall be dual licensed as above, without any additional terms or conditions.
