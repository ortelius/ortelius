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
