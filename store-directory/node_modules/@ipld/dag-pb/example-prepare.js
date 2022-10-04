import { CID } from 'multiformats/cid'
import { prepare } from '@ipld/dag-pb'

console.log(prepare({ Data: 'some data' }))
// ->{ Data: Uint8Array(9) [115, 111, 109, 101, 32, 100,  97, 116, 97] }
console.log(prepare({ Links: [CID.parse('bafkqabiaaebagba')] }))
// -> { Links: [ { Hash: CID(bafkqabiaaebagba) } ] }
