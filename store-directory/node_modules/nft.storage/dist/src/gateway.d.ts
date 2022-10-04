export const GATEWAY: URL;
export function toGatewayURL(url: string | URL, options?: GatewayURLOptions | undefined): URL;
/**
 * Base URL of an IPFS Gateway e.g. https://dweb.link/ or https://ipfs.io/
 */
export type GatewayURL = string | URL;
export type GatewayURLOptions = {
    gateway?: GatewayURL;
};
//# sourceMappingURL=gateway.d.ts.map