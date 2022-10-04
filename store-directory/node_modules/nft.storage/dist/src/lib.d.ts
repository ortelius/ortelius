/**
 * @typedef {import('./lib/interface.js').Service} Service
 * @typedef {import('./lib/interface.js').CIDString} CIDString
 * @typedef {import('./lib/interface.js').Deal} Deal
 * @typedef {import('./lib/interface.js').FileObject} FileObject
 * @typedef {import('./lib/interface.js').FilesSource} FilesSource
 * @typedef {import('./lib/interface.js').Pin} Pin
 * @typedef {import('./lib/interface.js').CarReader} CarReader
 * @typedef {import('ipfs-car/blockstore').Blockstore} BlockstoreI
 * @typedef {import('./lib/interface.js').RateLimiter} RateLimiter
 * @typedef {import('./lib/interface.js').RequestOptions} RequestOptions
 */
/**
 * @returns {RateLimiter}
 */
export function createRateLimiter(): RateLimiter;
/**
 * Cast an iterable to an asyncIterable
 * @template T
 * @param {Iterable<T>} iterable
 * @returns {AsyncIterable<T>}
 */
export function toAsyncIterable<T>(iterable: Iterable<T>): AsyncIterable<T>;
export type Service = import('./lib/interface.js').Service;
export type CIDString = import('./lib/interface.js').CIDString;
export type Deal = import('./lib/interface.js').Deal;
export type FileObject = import('./lib/interface.js').FileObject;
export type FilesSource = import('./lib/interface.js').FilesSource;
export type Pin = import('./lib/interface.js').Pin;
export type CarReader = import('./lib/interface.js').CarReader;
export type BlockstoreI = import('ipfs-car/blockstore').Blockstore;
export type RateLimiter = import('./lib/interface.js').RateLimiter;
export type RequestOptions = import('./lib/interface.js').RequestOptions;
export type TokenType<T extends import("./lib/interface.js").TokenInput> = import('./lib/interface.js').Token<T>;
/**
 * @template {import('./lib/interface.js').TokenInput} T
 * @typedef {import('./lib/interface.js').Token<T>} TokenType
 */
/**
 * @implements {Service}
 */
export class NFTStorage implements Service {
    /**
     * @hidden
     * @param {object} options
     * @param {string} options.token
     * @param {string} [options.did]
     */
    static auth({ token, did }: {
        token: string;
        did?: string | undefined;
    }): {
        'x-agent-did'?: string | undefined;
        Authorization: string;
        'X-Client': string;
    };
    /**
     * Stores a single file and returns its CID.
     *
     * @param {Service} service
     * @param {Blob} blob
     * @param {RequestOptions} [options]
     * @returns {Promise<CIDString>}
     */
    static storeBlob(service: Service, blob: Blob, options?: import("./lib/interface.js").RequestOptions | undefined): Promise<CIDString>;
    /**
     * Stores a CAR file and returns its root CID.
     *
     * @param {Service} service
     * @param {Blob|CarReader} car
     * @param {import('./lib/interface.js').CarStorerOptions} [options]
     * @returns {Promise<CIDString>}
     */
    static storeCar({ endpoint, rateLimiter, ...token }: Service, car: Blob | CarReader, { onStoredChunk, maxRetries, decoders, signal }?: import("./lib/interface.js").CarStorerOptions | undefined): Promise<CIDString>;
    /**
     * Stores a directory of files and returns a CID. Provided files **MUST**
     * be within the same directory, otherwise error is raised e.g. `foo/bar.png`,
     * `foo/bla/baz.json` is ok but `foo/bar.png`, `bla/baz.json` is not.
     *
     * @param {Service} service
     * @param {FilesSource} filesSource
     * @param {RequestOptions} [options]
     * @returns {Promise<CIDString>}
     */
    static storeDirectory(service: Service, filesSource: FilesSource, options?: import("./lib/interface.js").RequestOptions | undefined): Promise<CIDString>;
    /**
     * Stores the given token and all resources it references (in the form of a
     * File or a Blob) along with a metadata JSON as specificed in ERC-1155. The
     * `token.image` must be either a `File` or a `Blob` instance, which will be
     * stored and the corresponding content address URL will be saved in the
     * metadata JSON file under `image` field.
     *
     * If `token.properties` contains properties with `File` or `Blob` values,
     * those also get stored and their URLs will be saved in the metadata JSON
     * file in their place.
     *
     * Note: URLs for `File` objects will retain file names e.g. in case of
     * `new File([bytes], 'cat.png', { type: 'image/png' })` will be transformed
     * into a URL that looks like `ipfs://bafy...hash/image/cat.png`. For `Blob`
     * objects, the URL will not have a file name name or mime type, instead it
     * will be transformed into a URL that looks like
     * `ipfs://bafy...hash/image/blob`.
     *
     * @template {import('./lib/interface.js').TokenInput} T
     * @param {Service} service
     * @param {T} metadata
     * @param {RequestOptions} [options]
     * @returns {Promise<TokenType<T>>}
     */
    static store<T_1 extends import("./lib/interface.js").TokenInput>(service: Service, metadata: T_1, options?: import("./lib/interface.js").RequestOptions | undefined): Promise<import("./lib/interface.js").Token<T_1>>;
    /**
     * Returns current status of the stored NFT by its CID. Note the NFT must
     * have previously been stored by this account.
     *
     * @param {Service} service
     * @param {string} cid
     * @param {RequestOptions} [options]
     * @returns {Promise<import('./lib/interface.js').StatusResult>}
     */
    static status({ endpoint, rateLimiter, ...token }: Service, cid: string, options?: import("./lib/interface.js").RequestOptions | undefined): Promise<import('./lib/interface.js').StatusResult>;
    /**
     * Check if a CID of an NFT is being stored by NFT.Storage.
     *
     * @param {import('./lib/interface.js').PublicService} service
     * @param {string} cid
     * @param {RequestOptions} [options]
     * @returns {Promise<import('./lib/interface.js').CheckResult>}
     */
    static check({ endpoint, rateLimiter }: import('./lib/interface.js').PublicService, cid: string, options?: import("./lib/interface.js").RequestOptions | undefined): Promise<import('./lib/interface.js').CheckResult>;
    /**
     * Removes stored content by its CID from this account. Please note that
     * even if content is removed from the service other nodes that have
     * replicated it might still continue providing it.
     *
     * @param {Service} service
     * @param {string} cid
     * @param {RequestOptions} [options]
     * @returns {Promise<void>}
     */
    static delete({ endpoint, rateLimiter, ...token }: Service, cid: string, options?: import("./lib/interface.js").RequestOptions | undefined): Promise<void>;
    /**
     * Encodes the given token and all resources it references (in the form of a
     * File or a Blob) along with a metadata JSON as specificed in ERC-1155 to a
     * CAR file. The `token.image` must be either a `File` or a `Blob` instance,
     * which will be stored and the corresponding content address URL will be
     * saved in the metadata JSON file under `image` field.
     *
     * If `token.properties` contains properties with `File` or `Blob` values,
     * those also get stored and their URLs will be saved in the metadata JSON
     * file in their place.
     *
     * Note: URLs for `File` objects will retain file names e.g. in case of
     * `new File([bytes], 'cat.png', { type: 'image/png' })` will be transformed
     * into a URL that looks like `ipfs://bafy...hash/image/cat.png`. For `Blob`
     * objects, the URL will not have a file name name or mime type, instead it
     * will be transformed into a URL that looks like
     * `ipfs://bafy...hash/image/blob`.
     *
     * @example
     * ```js
     * const { token, car } = await NFTStorage.encodeNFT({
     *   name: 'nft.storage store test',
     *   description: 'Test ERC-1155 compatible metadata.',
     *   image: new File(['<DATA>'], 'pinpie.jpg', { type: 'image/jpg' }),
     *   properties: {
     *     custom: 'Custom data can appear here, files are auto uploaded.',
     *     file: new File(['<DATA>'], 'README.md', { type: 'text/plain' }),
     *   }
     * })
     *
     * console.log('IPFS URL for the metadata:', token.url)
     * console.log('metadata.json contents:\n', token.data)
     * console.log('metadata.json with IPFS gateway URLs:\n', token.embed())
     *
     * // Now store the CAR file on NFT.Storage
     * await client.storeCar(car)
     * ```
     *
     * @template {import('./lib/interface.js').TokenInput} T
     * @param {T} input
     * @returns {Promise<{ cid: CID, token: TokenType<T>, car: CarReader }>}
     */
    static encodeNFT<T_2 extends import("./lib/interface.js").TokenInput>(input: T_2): Promise<{
        cid: CID;
        token: import("./lib/interface.js").Token<T_2>;
        car: CarReader;
    }>;
    /**
     * Encodes a single file to a CAR file and also returns its root CID.
     *
     * @example
     * ```js
     * const content = new Blob(['hello world'])
     * const { cid, car } = await NFTStorage.encodeBlob(content)
     *
     * // Root CID of the file
     * console.log(cid.toString())
     *
     * // Now store the CAR file on NFT.Storage
     * await client.storeCar(car)
     * ```
     *
     * @param {Blob} blob
     * @param {object} [options]
     * @param {BlockstoreI} [options.blockstore]
     * @returns {Promise<{ cid: CID, car: CarReader }>}
     */
    static encodeBlob(blob: Blob, { blockstore }?: {
        blockstore?: import("ipfs-car/blockstore").Blockstore | undefined;
    } | undefined): Promise<{
        cid: CID;
        car: CarReader;
    }>;
    /**
     * Encodes a directory of files to a CAR file and also returns the root CID.
     * Provided files **MUST** be within the same directory, otherwise error is
     * raised e.g. `foo/bar.png`, `foo/bla/baz.json` is ok but `foo/bar.png`,
     * `bla/baz.json` is not.
     *
     * @example
     * ```js
     * const { cid, car } = await NFTStorage.encodeDirectory([
     *   new File(['hello world'], 'hello.txt'),
     *   new File([JSON.stringify({'from': 'incognito'}, null, 2)], 'metadata.json')
     * ])
     *
     * // Root CID of the directory
     * console.log(cid.toString())
     *
     * // Now store the CAR file on NFT.Storage
     * await client.storeCar(car)
     * ```
     *
     * @param {FilesSource} files
     * @param {object} [options]
     * @param {BlockstoreI} [options.blockstore]
     * @returns {Promise<{ cid: CID, car: CarReader }>}
     */
    static encodeDirectory(files: FilesSource, { blockstore }?: {
        blockstore?: import("ipfs-car/blockstore").Blockstore | undefined;
    } | undefined): Promise<{
        cid: CID;
        car: CarReader;
    }>;
    /**
     * Constructs a client bound to the given `options.token` and
     * `options.endpoint`.
     *
     * @example
     * ```js
     * import { NFTStorage, File, Blob } from "nft.storage"
     * const client = new NFTStorage({ token: API_TOKEN })
     *
     * const cid = await client.storeBlob(new Blob(['hello world']))
     * ```
     * Optionally you could pass an alternative API endpoint (e.g. for testing)
     * @example
     * ```js
     * import { NFTStorage } from "nft.storage"
     * const client = new NFTStorage({
     *   token: API_TOKEN
     *   endpoint: new URL('http://localhost:8080/')
     * })
     * ```
     *
     * @param {{token: string, endpoint?: URL, rateLimiter?: RateLimiter, did?: string}} options
     */
    constructor({ token, did, endpoint, rateLimiter, }: {
        token: string;
        endpoint?: URL;
        rateLimiter?: RateLimiter;
        did?: string;
    });
    /**
     * Authorization token.
     *
     * @readonly
     */
    readonly token: string;
    /**
     * Service API endpoint `URL`.
     * @readonly
     */
    readonly endpoint: URL;
    /**
     * @readonly
     */
    readonly rateLimiter: import("./lib/interface.js").RateLimiter;
    /**
     * @readonly
     */
    readonly did: string | undefined;
    /**
     * Stores a single file and returns the corresponding Content Identifier (CID).
     * Takes a [Blob](https://developer.mozilla.org/en-US/docs/Web/API/Blob/Blob)
     * or a [File](https://developer.mozilla.org/en-US/docs/Web/API/File). Note
     * that no file name or file metadata is retained.
     *
     * @example
     * ```js
     * const content = new Blob(['hello world'])
     * const cid = await client.storeBlob(content)
     * cid //> 'zdj7Wn9FQAURCP6MbwcWuzi7u65kAsXCdjNTkhbJcoaXBusq9'
     * ```
     *
     * @param {Blob} blob
     * @param {RequestOptions} [options]
     */
    storeBlob(blob: Blob, options?: import("./lib/interface.js").RequestOptions | undefined): Promise<import("./lib/interface.js").CIDString>;
    /**
     * Stores files encoded as a single [Content Addressed Archive
     * (CAR)](https://github.com/ipld/specs/blob/master/block-layer/content-addressable-archives.md).
     *
     * Takes a [Blob](https://developer.mozilla.org/en-US/docs/Web/API/Blob/Blob)
     * or a [File](https://developer.mozilla.org/en-US/docs/Web/API/File).
     *
     * Returns the corresponding Content Identifier (CID).
     *
     * See the [`ipfs-car` docs](https://www.npmjs.com/package/ipfs-car) for more
     * details on packing a CAR file.
     *
     * @example
     * ```js
     * import { pack } from 'ipfs-car/pack'
     * import { CarReader } from '@ipld/car'
     * const { out, root } = await pack({
     *  input: fs.createReadStream('pinpie.pdf')
     * })
     * const expectedCid = root.toString()
     * const carReader = await CarReader.fromIterable(out)
     * const cid = await storage.storeCar(carReader)
     * console.assert(cid === expectedCid)
     * ```
     *
     * @example
     * ```
     * import { packToBlob } from 'ipfs-car/pack/blob'
     * const data = 'Hello world'
     * const { root, car } = await packToBlob({ input: [new TextEncoder().encode(data)] })
     * const expectedCid = root.toString()
     * const cid = await client.storeCar(car)
     * console.assert(cid === expectedCid)
     * ```
     * @param {Blob|CarReader} car
     * @param {import('./lib/interface.js').CarStorerOptions} [options]
     */
    storeCar(car: Blob | CarReader, options?: import("./lib/interface.js").CarStorerOptions | undefined): Promise<import("./lib/interface.js").CIDString>;
    /**
     * Stores a directory of files and returns a CID for the directory.
     *
     * @example
     * ```js
     * const cid = await client.storeDirectory([
     *   new File(['hello world'], 'hello.txt'),
     *   new File([JSON.stringify({'from': 'incognito'}, null, 2)], 'metadata.json')
     * ])
     * cid //>
     * ```
     *
     * Argument can be a [FileList](https://developer.mozilla.org/en-US/docs/Web/API/FileList)
     * instance as well, in which case directory structure will be retained.
     *
     * @param {FilesSource} files
     * @param {RequestOptions} [options]
     */
    storeDirectory(files: FilesSource, options?: import("./lib/interface.js").RequestOptions | undefined): Promise<import("./lib/interface.js").CIDString>;
    /**
     * Returns current status of the stored NFT by its CID. Note the NFT must
     * have previously been stored by this account.
     *
     * @example
     * ```js
     * const status = await client.status('zdj7Wn9FQAURCP6MbwcWuzi7u65kAsXCdjNTkhbJcoaXBusq9')
     * ```
     *
     * @param {string} cid
     * @param {RequestOptions} [options]
     */
    status(cid: string, options?: import("./lib/interface.js").RequestOptions | undefined): Promise<import("./lib/interface.js").StatusResult>;
    /**
     * Removes stored content by its CID from the service.
     *
     * > Please note that even if content is removed from the service other nodes
     * that have replicated it might still continue providing it.
     *
     * @example
     * ```js
     * await client.delete('zdj7Wn9FQAURCP6MbwcWuzi7u65kAsXCdjNTkhbJcoaXBusq9')
     * ```
     *
     * @param {string} cid
     * @param {RequestOptions} [options]
     */
    delete(cid: string, options?: import("./lib/interface.js").RequestOptions | undefined): Promise<void>;
    /**
     * Check if a CID of an NFT is being stored by nft.storage. Throws if the NFT
     * was not found.
     *
     * @example
     * ```js
     * const status = await client.check('zdj7Wn9FQAURCP6MbwcWuzi7u65kAsXCdjNTkhbJcoaXBusq9')
     * ```
     *
     * @param {string} cid
     * @param {RequestOptions} [options]
     */
    check(cid: string, options?: import("./lib/interface.js").RequestOptions | undefined): Promise<import("./lib/interface.js").CheckResult>;
    /**
     * Stores the given token and all resources it references (in the form of a
     * File or a Blob) along with a metadata JSON as specificed in
     * [ERC-1155](https://eips.ethereum.org/EIPS/eip-1155#metadata). The
     * `token.image` must be either a `File` or a `Blob` instance, which will be
     * stored and the corresponding content address URL will be saved in the
     * metadata JSON file under `image` field.
     *
     * If `token.properties` contains properties with `File` or `Blob` values,
     * those also get stored and their URLs will be saved in the metadata JSON
     * file in their place.
     *
     * Note: URLs for `File` objects will retain file names e.g. in case of
     * `new File([bytes], 'cat.png', { type: 'image/png' })` will be transformed
     * into a URL that looks like `ipfs://bafy...hash/image/cat.png`. For `Blob`
     * objects, the URL will not have a file name name or mime type, instead it
     * will be transformed into a URL that looks like
     * `ipfs://bafy...hash/image/blob`.
     *
     * @example
     * ```js
     * const metadata = await client.store({
     *   name: 'nft.storage store test',
     *   description: 'Test ERC-1155 compatible metadata.',
     *   image: new File(['<DATA>'], 'pinpie.jpg', { type: 'image/jpg' }),
     *   properties: {
     *     custom: 'Custom data can appear here, files are auto uploaded.',
     *     file: new File(['<DATA>'], 'README.md', { type: 'text/plain' }),
     *   }
     * })
     *
     * console.log('IPFS URL for the metadata:', metadata.url)
     * console.log('metadata.json contents:\n', metadata.data)
     * console.log('metadata.json with IPFS gateway URLs:\n', metadata.embed())
     * ```
     *
     * @template {import('./lib/interface.js').TokenInput} T
     * @param {T} token
     * @param {RequestOptions} [options]
     */
    store<T extends import("./lib/interface.js").TokenInput>(token: T, options?: import("./lib/interface.js").RequestOptions | undefined): Promise<import("./lib/interface.js").Token<T>>;
}
import { File } from "./platform.js";
import { Blob } from "./platform.js";
import { FormData } from "./platform.js";
import { toGatewayURL } from "./gateway.js";
import * as Token from "./token.js";
import { CID } from "multiformats/cid";
export { File, Blob, FormData, toGatewayURL, Token };
//# sourceMappingURL=lib.d.ts.map