import { UnixFS } from 'ipfs-unixfs';
import persist from '../../utils/persist.js';
import * as dagPb from '@ipld/dag-pb';
import * as raw from 'multiformats/codecs/raw';
async function* bufferImporter(file, block, options) {
  for await (let buffer of file.content) {
    yield async () => {
      options.progress(buffer.length, file.path);
      let unixfs;
      const opts = {
        codec: dagPb,
        cidVersion: options.cidVersion,
        hasher: options.hasher,
        onlyHash: options.onlyHash
      };
      if (options.rawLeaves) {
        opts.codec = raw;
        opts.cidVersion = 1;
      } else {
        unixfs = new UnixFS({
          type: options.leafType,
          data: buffer
        });
        buffer = dagPb.encode({
          Data: unixfs.marshal(),
          Links: []
        });
      }
      return {
        cid: await persist(buffer, block, opts),
        unixfs,
        size: buffer.length
      };
    };
  }
}
export default bufferImporter;