import { NFTStorage } from 'nft.storage'
import { filesFromPath } from 'files-from-path'
import path from 'path'




//NFT.Storage supports file directory uploads, using the files-from-path package.

//npm install nft.storage files-from-path
//Next create a similar script to the previous one, but instead use the filesFromPath() function to process each file within the target directory. Once complete, store the directory with the storeDirectory() function.

//Here is a directory upload script:
const token = 'REPLACE_WITH_API_KEY'

async function main() {
  // you'll probably want more sophisticated argument parsing in a real app
  if (process.argv.length !== 3) {
    console.error(`usage: ${process.argv[0]} ${process.argv[1]} <directory-path>`)
  }
  const directoryPath = process.argv[2]
  const files = filesFromPath(directoryPath, {
    pathPrefix: path.resolve(directoryPath), // see the note about pathPrefix below
    hidden: true, // use the default of false if you want to ignore files that start with '.'
  })

  const storage = new NFTStorage({ token })

  console.log(`storing file(s) from ${path}`)
  const cid = await storage.storeDirectory(files)
  console.log({ cid })

  const status = await storage.status(cid)
  console.log(status)
}



//NFT.Storage supports custom metadata, using a traditional JSON data scheme.The standard metadata field values are image, name, & description. An additional field, properties, can be added to provide additional, custom fields specific to the upload.Within such schemes, images are stored either as File or Blob objects, which should contain image data in either PNG or JPEG formats.

//Here is a custom metadata upload:
import { NFTStorage } from `nft.storage`

const API_KEY = `REPLACE_WITH_API_KEY`

//fetching 
async function getImage() {

	const imageURL = "https://image-url.com"
	const r = await fetch(imageURL)

	if (!r.ok) {
		throw new Error(`error fetching image: [${r.statusCode}]: ${r.status}`)
	} return r.blob()
}

//storing
async function storeNFT() {

	const image = await getImage()

	const nft = {
		image, 
		name: "...",
		description: "...",
		properties: {
			type: "...",
			author: "...",
			content: "..."
		}
	}

	const client = new NFTStorage({ token: API_KEY})
	const metadata = await client.store(nft)
	
	console.log(`NFT data stored`)
	console.log(`Metadata URI: `, metadata.url)

}

storeNFT()
main()