'use strict'

const protocols = require('../protocols-table')

const { code: dnsaddrCode } = protocols('dnsaddr')

/**
 * @typedef {import('..').Multiaddr} Multiaddr
 */

/**
 * Resolver for dnsaddr addresses.
 *
 * @param {Multiaddr} addr
 * @returns {Promise<string[]>}
 */
async function dnsaddrResolver (addr) {
  const Resolver = require('./dns')
  const resolver = new Resolver()

  const peerId = addr.getPeerId()
  const [, hostname] = addr.stringTuples().find(([proto]) => proto === dnsaddrCode) || []

  const records = await resolver.resolveTxt(`_dnsaddr.${hostname}`)
  let addresses = records.flat().map((a) => a.split('=')[1])

  if (peerId) {
    addresses = addresses.filter((entry) => entry.includes(peerId))
  }

  return addresses
}

module.exports = {
  dnsaddrResolver
}
