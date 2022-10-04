import { Multiaddr } from 'multiaddr';
import multiAddrToUri from 'multiaddr-to-uri';
export function toUrlString(url) {
  try {
    url = multiAddrToUri(new Multiaddr(url));
  } catch (err) {
  }
  url = url.toString();
  return url;
}