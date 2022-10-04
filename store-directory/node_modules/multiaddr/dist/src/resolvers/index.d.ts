export type Multiaddr = import('..').Multiaddr;
/**
 * @typedef {import('..').Multiaddr} Multiaddr
 */
/**
 * Resolver for dnsaddr addresses.
 *
 * @param {Multiaddr} addr
 * @returns {Promise<string[]>}
 */
export function dnsaddrResolver(addr: Multiaddr): Promise<string[]>;
//# sourceMappingURL=index.d.ts.map