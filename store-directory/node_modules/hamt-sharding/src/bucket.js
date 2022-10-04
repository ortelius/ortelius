'use strict'

// @ts-ignore
const SparseArray = require('sparse-array')
const { fromString: uint8ArrayFromString } = require('uint8arrays/from-string')

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
class Bucket {
  /**
   * @param {BucketOptions} options
   * @param {Bucket<T>} [parent]
   * @param {number} [posAtParent=0]
   */
  constructor (options, parent, posAtParent = 0) {
    this._options = options
    this._popCount = 0
    this._parent = parent
    this._posAtParent = posAtParent

    /** @type {SA<Bucket<T> | BucketChild<T>>} */
    this._children = new SparseArray()

    /** @type {string | null} */
    this.key = null
  }

  /**
   * @param {string} key
   * @param {T} value
   */
  async put (key, value) {
    const place = await this._findNewBucketAndPos(key)

    await place.bucket._putAt(place, key, value)
  }

  /**
   * @param {string} key
   */
  async get (key) {
    const child = await this._findChild(key)

    if (child) {
      return child.value
    }
  }

  /**
   * @param {string} key
   */
  async del (key) {
    const place = await this._findPlace(key)
    const child = place.bucket._at(place.pos)

    if (child && child.key === key) {
      place.bucket._delAt(place.pos)
    }
  }

  /**
   * @returns {number}
   */
  leafCount () {
    const children = this._children.compactArray()

    return children.reduce((acc, child) => {
      if (child instanceof Bucket) {
        return acc + child.leafCount()
      }

      return acc + 1
    }, 0)
  }

  childrenCount () {
    return this._children.length
  }

  onlyChild () {
    return this._children.get(0)
  }

  /**
   * @returns {Iterable<BucketChild<T>>}
   */
  * eachLeafSeries () {
    const children = this._children.compactArray()

    for (const child of children) {
      if (child instanceof Bucket) {
        yield * child.eachLeafSeries()
      } else {
        yield child
      }
    }

    // this is necessary because tsc requires a @return annotation as it
    // can't derive a return type due to the recursion, and eslint requires
    // a return statement when there is a @return annotation
    return []
  }

  /**
   * @param {(value: BucketChild<T>, index: number) => T} map
   * @param {(reduced: any) => any} reduce
   */
  serialize (map, reduce) {
    /** @type {T[]} */
    const acc = []
    // serialize to a custom non-sparse representation
    return reduce(this._children.reduce((acc, child, index) => {
      if (child) {
        if (child instanceof Bucket) {
          acc.push(child.serialize(map, reduce))
        } else {
          acc.push(map(child, index))
        }
      }
      return acc
    }, acc))
  }

  /**
   * @param {(value: BucketChild<T>) => Promise<T[]>} asyncMap
   * @param {(reduced: any) => Promise<any>} asyncReduce
   */
  asyncTransform (asyncMap, asyncReduce) {
    return asyncTransformBucket(this, asyncMap, asyncReduce)
  }

  toJSON () {
    return this.serialize(mapNode, reduceNodes)
  }

  prettyPrint () {
    return JSON.stringify(this.toJSON(), null, '  ')
  }

  tableSize () {
    return Math.pow(2, this._options.bits)
  }

  /**
   * @param {string} key
   * @returns {Promise<BucketChild<T> | undefined>}
   */
  async _findChild (key) {
    const result = await this._findPlace(key)
    const child = result.bucket._at(result.pos)

    if (child instanceof Bucket) {
      // should not be possible, this._findPlace should always
      // return a location for a child, not a bucket
      return undefined
    }

    if (child && child.key === key) {
      return child
    }
  }

  /**
   * @param {string | InfiniteHash} key
   * @returns {Promise<BucketPosition<T>>}
   */
  async _findPlace (key) {
    const hashValue = this._options.hash(typeof key === 'string' ? uint8ArrayFromString(key) : key)
    const index = await hashValue.take(this._options.bits)

    const child = this._children.get(index)

    if (child instanceof Bucket) {
      return child._findPlace(hashValue)
    }

    return {
      bucket: this,
      pos: index,
      hash: hashValue,
      existingChild: child
    }
  }

  /**
   * @param {string | InfiniteHash} key
   * @returns {Promise<BucketPosition<T>>}
   */
  async _findNewBucketAndPos (key) {
    const place = await this._findPlace(key)

    if (place.existingChild && place.existingChild.key !== key) {
      // conflict
      const bucket = new Bucket(this._options, place.bucket, place.pos)
      place.bucket._putObjectAt(place.pos, bucket)

      // put the previous value
      const newPlace = await bucket._findPlace(place.existingChild.hash)
      newPlace.bucket._putAt(newPlace, place.existingChild.key, place.existingChild.value)

      return bucket._findNewBucketAndPos(place.hash)
    }

    // no conflict, we found the place
    return place
  }

  /**
   * @param {BucketPosition<T>} place
   * @param {string} key
   * @param {T} value
   */
  _putAt (place, key, value) {
    this._putObjectAt(place.pos, {
      key: key,
      value: value,
      hash: place.hash
    })
  }

  /**
   * @param {number} pos
   * @param {Bucket<T> | BucketChild<T>} object
   */
  _putObjectAt (pos, object) {
    if (!this._children.get(pos)) {
      this._popCount++
    }
    this._children.set(pos, object)
  }

  /**
   * @param {number} pos
   */
  _delAt (pos) {
    if (pos === -1) {
      throw new Error('Invalid position')
    }

    if (this._children.get(pos)) {
      this._popCount--
    }
    this._children.unset(pos)
    this._level()
  }

  _level () {
    if (this._parent && this._popCount <= 1) {
      if (this._popCount === 1) {
        // remove myself from parent, replacing me with my only child
        const onlyChild = this._children.find(exists)

        if (onlyChild && !(onlyChild instanceof Bucket)) {
          const hash = onlyChild.hash
          hash.untake(this._options.bits)
          const place = {
            pos: this._posAtParent,
            hash: hash,
            bucket: this._parent
          }
          this._parent._putAt(place, onlyChild.key, onlyChild.value)
        }
      } else {
        this._parent._delAt(this._posAtParent)
      }
    }
  }

  /**
   * @param {number} index
   * @returns {BucketChild<T> | Bucket<T> | undefined}
   */
  _at (index) {
    return this._children.get(index)
  }
}

/**
 * @param {any} o
 */
function exists (o) {
  return Boolean(o)
}

/**
 *
 * @param {*} node
 * @param {number} index
 */
function mapNode (node, index) {
  return node.key
}

/**
 * @param {*} nodes
 */
function reduceNodes (nodes) {
  return nodes
}

/**
 * @template T
 *
 * @param {Bucket<T>} bucket
 * @param {(value: BucketChild<T>) => Promise<T[]>} asyncMap
 * @param {(reduced: any) => Promise<any>} asyncReduce
 */
async function asyncTransformBucket (bucket, asyncMap, asyncReduce) {
  const output = []

  for (const child of bucket._children.compactArray()) {
    if (child instanceof Bucket) {
      await asyncTransformBucket(child, asyncMap, asyncReduce)
    } else {
      const mappedChildren = await asyncMap(child)

      output.push({
        bitField: bucket._children.bitField(),
        children: mappedChildren
      })
    }
  }

  return asyncReduce(output)
}

module.exports = Bucket
