'use strict'

module.exports = {
  // in React Native: global === window === self
  supportsFileReader: typeof self !== 'undefined' && 'FileReader' in self,
  supportsWebRTC: 'RTCPeerConnection' in globalThis &&
  (typeof navigator !== 'undefined' && typeof navigator.mediaDevices !== 'undefined' && 'getUserMedia' in navigator.mediaDevices),
  supportsWebRTCDataChannels: 'RTCPeerConnection' in globalThis
}
