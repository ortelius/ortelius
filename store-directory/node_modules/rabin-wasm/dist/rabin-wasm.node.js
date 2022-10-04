
const { instantiateSync } = require("@assemblyscript/loader");
const fs = require('fs')

loadWebAssembly.supported = typeof WebAssembly !== 'undefined'

async function loadWebAssembly (imp = {}) {
  if (!loadWebAssembly.supported) return null
  
  return instantiateSync(fs.readFileSync(__dirname + "/../dist/rabin.wasm"), imp);
}
module.exports = loadWebAssembly
