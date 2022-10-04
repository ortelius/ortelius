# @ipld/dag-pb

An implementation of the [DAG-PB spec](https://github.com/ipld/specs/blob/master/block-layer/codecs/dag-pb.md) for JavaScript designed for use with [multiformats](https://github.com/multiformats/js-multiformats) or via the higher-level `Block` abstraction in [@ipld/block](https://github.com/ipld/js-block).

## Example

```js
import { CID } from 'multiformats/cid'
import { sha256 } from 'multiformats/hashes/sha2'
import * as dagPB from '@ipld/dag-pb'

async function run () {
  const bytes = dagPB.encode({
    Data: new TextEncoder().encode('Some data as a string'),
    Links: []
  })

  // also possible if you `import dagPB, { prepare } from '@ipld/dag-pb'`
  // const bytes = dagPB.encode(prepare('Some data as a string'))
  // const bytes = dagPB.encode(prepare(new TextEncoder().encode('Some data as a string')))

  const hash = await sha256.digest(bytes)
  const cid = CID.create(1, dagPB.code, hash)

  console.log(cid, '=>', Buffer.from(bytes).toString('hex'))

  const decoded = dagPB.decode(bytes)

  console.log(decoded)
  console.log(`decoded "Data": ${new TextDecoder().decode(decoded.Data)}`)
}

run().catch((err) => {
  console.error(err)
  process.exit(1)
})
```

## Usage

`@ipld/dag-pb` is designed to be used within multiformats but can be used separately. `encode()`, `decode()`, `validate()` and `prepare()` functions are available if you pass in a `multiformats` object to the default export function. Each of these can operate independently as required.

### `prepare()`

The DAG-PB encoding is very strict about the Data Model forms that are passed in. The objects _must_ exactly resemble what they would if they were to undergo a round-trip of encode & decode. Therefore, extraneous or mistyped properties are not acceptable and will be rejected. See the [DAG-PB spec](https://github.com/ipld/specs/blob/master/block-layer/codecs/dag-pb.md) for full details of the acceptable schema and additional constraints.

Due to this strictness, a `prepare()` function is made available which simplifies construction and allows for more flexible input forms. Prior to encoding objects, call `prepare()` to receive a new object that strictly conforms to the schema.

```js
import { CID } from 'multiformats/cid'
import { prepare } from '@ipld/dag-pb'

console.log(prepare({ Data: 'some data' }))
// ->{ Data: Uint8Array(9) [115, 111, 109, 101, 32, 100,  97, 116, 97] }
console.log(prepare({ Links: [CID.parse('bafkqabiaaebagba')] }))
// -> { Links: [ { Hash: CID(bafkqabiaaebagba) } ] }
```

Some features of `prepare()`:

* Extraneous properties are omitted
* String values for `Data` are converted
* Strings are converted to `{ Data: bytes }` (as are `Uint8Array`s)
* Multiple ways of finding CIDs in the `Links` array are attempted, including interpreting the whole link element as a CID, reading a `Uint8Array` as a CID
* Ensuring that properties are of the correct type (link `Name` is a `string` and `Tsize` is a `number`)
* `Links` array is always present, even if empty
* `Links` array is properly sorted

## `createNode()` & `createLink()`

These utility exports are available to make transition from the older [ipld-dag-pb](https://github.com/ipld/js-ipld-dag-pb) library which used `DAGNode` and `DAGLink` objects with constructors. `createNode()` mirrors the `new DAGNode()` API while `createLink()` mirrors `new DAGLink()` API.

* `createNode(data: Uint8Array, links: PBLink[]|void): PBNode`: create a correctly formed `PBNode` object from a `Uint8Array` and an optional array of correctly formed `PBLink` objects. The returned object will be suitable for passing to `encode()` and using `prepare()` on it should result in a noop.
* `createLink(name: string, size: number, cid: CID): PBLink`: create a correctly formed `PBLink` object from a name, size and CID. The returned object will be suitable for attaching to a `PBNode`'s `Links` array, or in an array for the second argument to `createNode()`.

```js
import { CID, bytes } from 'multiformats'
import * as Block from 'multiformats/block'
import { sha256 as hasher } from 'multiformats/hashes/sha2'
import * as codec from '@ipld/dag-pb'

const { createLink, createNode } = codec

async function run () {
  const cid1 = CID.parse('QmWDtUQj38YLW8v3q4A6LwPn4vYKEbuKWpgSm6bjKW6Xfe')
  const cid2 = CID.parse('bafyreifepiu23okq5zuyvyhsoiazv2icw2van3s7ko6d3ixl5jx2yj2yhu')

  const links = [createLink('link1', 100, cid1), createLink('link2', 200, cid2)]
  const value = createNode(Uint8Array.from([0, 1, 2, 3, 4]), links)
  console.log(value)

  const block = await Block.encode({ value, codec, hasher })
  console.log(block.cid)
  console.log(`Encoded: ${bytes.toHex(block.bytes).replace(/(.{80})/g, '$1\n         ')}`)
}

run().catch((err) => console.error(err))
```

Results in:

```
{
  Data: Uint8Array(5) [ 0, 1, 2, 3, 4 ],
  Links: [
    {
      Hash: CID(QmWDtUQj38YLW8v3q4A6LwPn4vYKEbuKWpgSm6bjKW6Xfe),
      Name: 'link1',
      Tsize: 100
    },
    {
      Hash: CID(bafyreifepiu23okq5zuyvyhsoiazv2icw2van3s7ko6d3ixl5jx2yj2yhu),
      Name: 'link2',
      Tsize: 200
    }
  ]
}
CID(bafybeihsp53wkzsaif76mjv564cawzqyjwianosamlvf6sht2m25ttyxiy)
Encoded: 122d0a2212207521fe19c374a97759226dc5c0c8e674e73950e81b211f7dd3b6b30883a08a511205
         6c696e6b31186412300a2401711220a47a29adb950ee698ae0f272019ae902b6aa06ee5f53bc3da2
         ebea6fac27583d12056c696e6b3218c8010a050001020304
```

## License

Licensed under either of

 * Apache 2.0, ([LICENSE-APACHE](LICENSE-APACHE) / http://www.apache.org/licenses/LICENSE-2.0)
 * MIT ([LICENSE-MIT](LICENSE-MIT) / http://opensource.org/licenses/MIT)

### Contribution

Unless you explicitly state otherwise, any contribution intentionally submitted for inclusion in the work by you, as defined in the Apache-2.0 license, shall be dual licensed as above, without any additional terms or conditions.
