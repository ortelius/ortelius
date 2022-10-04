import http from 'http';
import https from 'https';
export default url => {
  if (!url) {
    throw new Error('URL required');
  }
  return url.protocol.startsWith('https') ? https.Agent : http.Agent;
};