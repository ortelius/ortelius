export = Bucket;
/**
 * @typedef {import('./consumable-hash').InfiniteHash} InfiniteHash
 * @typedef {import('../').UserBucketOptions} UserBucketOptions
 */
/**
 * @template V
 * @typedef {object} BucketChild<V>
 * @property {string} key
 * @property {V} value
 * @property {InfiniteHash} hash
 */
/**
 * @template B
 *
 * @typedef {object} SA<B>
 * @property {number} length
 * @property {() => B[]} compactArray
 * @property {(i: number) => B} get
 * @property {(i: number, value: B) => void} set
 * @property {<A> (fn: (acc: A, curr: B, index: number) => A, initial: A) => B} reduce
 * @property {(fn: (item: B) => boolean) => B | undefined} find
 * @property {() => number[]} bitField
 * @property {(i: number) => void} unset
 */
/**
 * @template T
 *
 * @typedef {object} BucketPosition<T>
 * @property {Bucket<T>} bucket
 * @property {number} pos
 * @property {InfiniteHash} hash
 * @property {BucketChild<T>} [existingChild]
 */
/**
 * @typedef {object} BucketOptions
 * @property {number} bits
 * @property {(value: Uint8Array | InfiniteHash) => InfiniteHash} hash
 */
/**
 * @template T
 */
declare class Bucket<T> {
    /**
     * @param {BucketOptions} options
     * @param {Bucket<T>} [parent]
     * @param {number} [posAtParent=0]
     */
    constructor(options: BucketOptions, parent?: Bucket<T> | undefined, posAtParent?: number | undefined);
    _options: BucketOptions;
    _popCount: number;
    _parent: Bucket<T> | undefined;
    _posAtParent: number;
    /** @type {SA<Bucket<T> | BucketChild<T>>} */
    _children: SA<Bucket<T> | BucketChild<T>>;
    /** @type {string | null} */
    key: string | null;
    /**
     * @param {string} key
     * @param {T} value
     */
    put(key: string, value: T): Promise<void>;
    /**
     * @param {string} key
     */
    get(key: string): Promise<T | undefined>;
    /**
     * @param {string} key
     */
    del(key: string): Promise<void>;
    /**
     * @returns {number}
     */
    leafCount(): number;
    childrenCount(): number;
    onlyChild(): Bucket<T> | BucketChild<T>;
    /**
     * @returns {Iterable<BucketChild<T>>}
     */
    eachLeafSeries(): Iterable<BucketChild<T>>;
    /**
     * @param {(value: BucketChild<T>, index: number) => T} map
     * @param {(reduced: any) => any} reduce
     */
    serialize(map: (value: BucketChild<T>, index: number) => T, reduce: (reduced: any) => any): any;
    /**
     * @param {(value: BucketChild<T>) => Promise<T[]>} asyncMap
     * @param {(reduced: any) => Promise<any>} asyncReduce
     */
    asyncTransform(asyncMap: (value: BucketChild<T>) => Promise<T[]>, asyncReduce: (reduced: any) => Promise<any>): Promise<any>;
    toJSON(): any;
    prettyPrint(): string;
    tableSize(): number;
    /**
     * @param {string} key
     * @returns {Promise<BucketChild<T> | undefined>}
     */
    _findChild(key: string): Promise<BucketChild<T> | undefined>;
    /**
     * @param {string | InfiniteHash} key
     * @returns {Promise<BucketPosition<T>>}
     */
    _findPlace(key: string | InfiniteHash): Promise<BucketPosition<T>>;
    /**
     * @param {string | InfiniteHash} key
     * @returns {Promise<BucketPosition<T>>}
     */
    _findNewBucketAndPos(key: string | InfiniteHash): Promise<BucketPosition<T>>;
    /**
     * @param {BucketPosition<T>} place
     * @param {string} key
     * @param {T} value
     */
    _putAt(place: BucketPosition<T>, key: string, value: T): void;
    /**
     * @param {number} pos
     * @param {Bucket<T> | BucketChild<T>} object
     */
    _putObjectAt(pos: number, object: Bucket<T> | BucketChild<T>): void;
    /**
     * @param {number} pos
     */
    _delAt(pos: number): void;
    _level(): void;
    /**
     * @param {number} index
     * @returns {BucketChild<T> | Bucket<T> | undefined}
     */
    _at(index: number): Bucket<T> | BucketChild<T> | undefined;
}
declare namespace Bucket {
    export { InfiniteHash, UserBucketOptions, BucketChild, SA, BucketPosition, BucketOptions };
}
type BucketOptions = {
    bits: number;
    hash: (value: Uint8Array | InfiniteHash) => InfiniteHash;
};
/**
 * <V>
 */
type BucketChild<V> = {
    key: string;
    value: V;
    hash: InfiniteHash;
};
/**
 * <B>
 */
type SA<B> = {
    length: number;
    compactArray: () => B[];
    get: (i: number) => B;
    set: (i: number, value: B) => void;
    reduce: <A>(fn: (acc: A, curr: B, index: number) => A, initial: A) => B;
    find: (fn: (item: B) => boolean) => B | undefined;
    bitField: () => number[];
    unset: (i: number) => void;
};
type InfiniteHash = import('./consumable-hash').InfiniteHash;
/**
 * <T>
 */
type BucketPosition<T> = {
    bucket: Bucket<T>;
    pos: number;
    hash: InfiniteHash;
    existingChild?: BucketChild<T> | undefined;
};
type UserBucketOptions = import('../').UserBucketOptions;
//# sourceMappingURL=bucket.d.ts.map