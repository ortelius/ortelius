export const isIP: {
    (string: string): boolean;
    v4(string: string): boolean;
    v6(string: string): boolean;
    version(string: string): 4 | 6 | undefined;
};
export const isV4: (string: string) => boolean;
export const isV6: (string: string) => boolean;
export function toBytes(ip: any, buff: any, offset: any): any;
export function toString(buff: any, offset: any, length: any): string | undefined;
//# sourceMappingURL=ip.d.ts.map