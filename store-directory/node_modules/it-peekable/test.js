const peekableIt = require('./')
const test = require('ava')
const all = require('it-all')

test('Should peek at an iterable', async (t) => {
  const iterable = [0, 1, 2, 3]
  const peekable = peekableIt(iterable)
  const { value, done } = peekable.peek()

  t.is(value, 0)
  t.is(done, false)
})

test('Should peek at an async iterable', async (t) => {
  const content = [0, 1, 2, 3]
  const iterable = async function * () {
    for (let i = 0; i < content.length; i++) {
      yield content[i]
    }
  }

  const peekable = peekableIt(iterable())
  const { value, done } = await peekable.peek()

  t.is(value, 0)
  t.is(done, false)
})

test('Should push an iterable', async (t) => {
  const iterable = [0, 1, 2, 3]
  const peekable = peekableIt(iterable)
  const { value } = peekable.peek()

  peekable.push(value)

  t.deepEqual([...peekable], iterable)
})

test('Should push an async iterable', async (t) => {
  const content = [0, 1, 2, 3]
  const iterable = async function * () {
    for (let i = 0; i < content.length; i++) {
      yield content[i]
    }
  }

  const peekable = peekableIt(iterable())
  const { value } = await peekable.peek()

  peekable.push(value)

  t.deepEqual(await all(peekable), content)
})
