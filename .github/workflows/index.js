

import { NFTStorage, File } from 'nft.storage'
const client = new NFTStorage({ token: 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiJkaWQ6ZXRocjoweDMzQWJjNTMzQkYwZTkzNzBFMjdlRDlENTFkRWU0N2U0NUVBQjVjNzgiLCJpc3MiOiJuZnQtc3RvcmFnZSIsImlhdCI6MTY2NDkwODM1OTI4MCwibmFtZSI6Im5mdCB0b2tlbiJ9.ocMRh96M6WZjZb8kuXrntv2KSDxFyw3jlCteJq-YlY0' })

async function main() {
  const metadata = await client.store({
    name: 'Pinpie',
    description: 'Pin is not delicious beef!',
    image: new File(
      [
        /* data */
      ],
      'pinpie.jpg',
      { type: 'image/jpg' }
    ),
  })
  console.log(metadata.url)

  // ipfs://bafyreib4pff766vhpbxbhjbqqnsh5emeznvujayjj4z2iu533cprgbz23m/metadata.json
}

main()