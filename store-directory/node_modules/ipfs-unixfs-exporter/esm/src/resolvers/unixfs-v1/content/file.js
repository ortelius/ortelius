import extractDataFromBlock from '../../../utils/extract-data-from-block.js';
import validateOffsetAndLength from '../../../utils/validate-offset-and-length.js';
import { UnixFS } from 'ipfs-unixfs';
import errCode from 'err-code';
import * as dagPb from '@ipld/dag-pb';
import * as dagCbor from '@ipld/dag-cbor';
import * as raw from 'multiformats/codecs/raw';
async function* emitBytes(blockstore, node, start, end, streamPosition = 0, options) {
  if (node instanceof Uint8Array) {
    const buf = extractDataFromBlock(node, streamPosition, start, end);
    if (buf.length) {
      yield buf;
    }
    streamPosition += buf.length;
    return streamPosition;
  }
  if (node.Data == null) {
    throw errCode(new Error('no data in PBNode'), 'ERR_NOT_UNIXFS');
  }
  let file;
  try {
    file = UnixFS.unmarshal(node.Data);
  } catch (err) {
    throw errCode(err, 'ERR_NOT_UNIXFS');
  }
  if (file.data && file.data.length) {
    const buf = extractDataFromBlock(file.data, streamPosition, start, end);
    if (buf.length) {
      yield buf;
    }
    streamPosition += file.data.length;
  }
  let childStart = streamPosition;
  for (let i = 0; i < node.Links.length; i++) {
    const childLink = node.Links[i];
    const childEnd = streamPosition + file.blockSizes[i];
    if (start >= childStart && start < childEnd || end > childStart && end <= childEnd || start < childStart && end > childEnd) {
      const block = await blockstore.get(childLink.Hash, { signal: options.signal });
      let child;
      switch (childLink.Hash.code) {
      case dagPb.code:
        child = await dagPb.decode(block);
        break;
      case raw.code:
        child = block;
        break;
      case dagCbor.code:
        child = await dagCbor.decode(block);
        break;
      default:
        throw Error(`Unsupported codec: ${ childLink.Hash.code }`);
      }
      for await (const buf of emitBytes(blockstore, child, start, end, streamPosition, options)) {
        streamPosition += buf.length;
        yield buf;
      }
    }
    streamPosition = childEnd;
    childStart = childEnd + 1;
  }
}
const fileContent = (cid, node, unixfs, path, resolve, depth, blockstore) => {
  function yieldFileContent(options = {}) {
    const fileSize = unixfs.fileSize();
    if (fileSize === undefined) {
      throw new Error('File was a directory');
    }
    const {offset, length} = validateOffsetAndLength(fileSize, options.offset, options.length);
    const start = offset;
    const end = offset + length;
    return emitBytes(blockstore, node, start, end, 0, options);
  }
  return yieldFileContent;
};
export default fileContent;