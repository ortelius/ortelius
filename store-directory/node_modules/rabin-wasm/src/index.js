const Rabin = require('./rabin')
const getRabin = require('../dist/rabin-wasm.node.js')

const create = async (avg, min, max, windowSize, polynomial) => {
    const compiled = await getRabin()
    return new Rabin(compiled, avg, min, max, windowSize, polynomial)
}

module.exports = {
    Rabin,
    create
}
