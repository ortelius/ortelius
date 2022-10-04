# throttled-queue

Throttles arbitrary code to execute a maximum number of times per interval. Best for making throttled API requests.

For example, making network calls to popular APIs such as Twitter is subject to rate limits.  By wrapping all of your API calls in a throttle, it will automatically adjust your requests to be within the acceptable rate limits.

Unlike the `throttle` functions of popular libraries like lodash and underscore, `throttled-queue` will not prevent any executions. Instead, every execution is placed into a queue, which will be drained at the desired rate limit.

## Installation
```shell
npm install throttled-queue
```

It can be used in a Node.js environment, or directly in the browser.

## Usage
1) `require` or `import` the factory function:
```javascript
const throttledQueue = require('throttled-queue');
```

```javascript
import throttledQueue from 'throttled-queue';
```

2) Create an instance of a throttled queue by specifying the maximum number of requests as the first parameter,
and the interval in milliseconds as the second:
```javascript
const throttle = throttledQueue(5, 1000); // at most 5 requests per second.
```

3) Use the `throttle` instance as a function to enqueue actions:
```javascript
throttle(() => {
    // perform some type of activity in here.
});
```

The `throttle` function will also return a promise with the result of your operation:
```javascript
const result = await throttle(() => {
    return Promise.resolve('hello!');
});
// result now equals "hello"
```

## Quick Examples
### Basic
Rapidly assigning network calls to be run, but they will be limited to 1 request per second.
```javascript
const throttledQueue = require('throttled-queue');
const throttle = throttledQueue(1, 1000); // at most make 1 request every second.

for (let x = 0; x < 100; x++) {
    throttle(() => {
        // make a network request.
        return fetch('https://api.github.com/search/users?q=shaunpersad');
    });
}
```
### Reusable
Wherever the `throttle` instance is used, your action will be placed into the same queue, 
and be subject to the same rate limits.
```javascript
const throttledQueue = require('throttled-queue');
const throttle = throttledQueue(1, 60 * 1000); // at most make 1 request every minute.

for (let x = 0; x < 50; x++) {
    throttle(() => {
        // make a network request.
        return fetch('https://api.github.com/search/users?q=shaunpersad');
    });
}
for (let y = 0; y < 50; y++) {
    throttle(() => {
        // make another type of network request.
        return fetch('https://api.github.com/search/repositories?q=throttled-queue+user:shaunpersad');
    });
}
```
### Bursts
By specifying a number higher than 1 as the first parameter, you can dequeue multiple actions within the given interval:
```javascript
const throttledQueue = require('throttled-queue');
const throttle = throttledQueue(10, 1000); // at most make 10 requests every second.

for (let x = 0; x < 100; x++) {
    throttle(() => {
        // This will fire at most 10 a second, as rapidly as possible.
        return fetch('https://api.github.com/search/users?q=shaunpersad');
    });
}
```
### Evenly spaced
You can space out your actions by specifying `true` as the third (optional) parameter:
```javascript
const throttledQueue = require('throttled-queue');
const throttle = throttledQueue(10, 1000, true); // at most make 10 requests every second, but evenly spaced.

for (var x = 0; x < 100; x++) {
    throttle(() => {
        // This will fire at most 10 requests a second, spacing them out instead of in a burst.
        return fetch('https://api.github.com/search/users?q=shaunpersad');
    });
}
```
### Promises
Starting in version `2.0.0`, you can wait for the results of your operation:
```javascript
const throttledQueue = require('throttled-queue');
const throttle = throttledQueue(10, 1000, true); // at most make 10 requests every second, but evenly spaced.

const usernames = ['shaunpersad', 'forward-motion'];
const profiles = await Promise.all(
    usernames.map((username) => throttle(() => {
        return fetch(`https://api.github.com/search/users?q=${username}`);
    }))
);

const justMe = await throttle(() => fetch('https://api.github.com/search/users?q=shaunpersad'));
```

## Typescript support
The package is written in Typescript and includes types by default. The `throttle` function is a generic,
and in most cases will automatically infer the right type for the result of the promise from the input.

However, you may also specify the return type of the promise when needed:
```typescript
import throttledQueue from 'throttled-queue';
const throttle = throttledQueue(1, 1000);
const result1 = await throttle<string>(() => '1');
const result2 = await throttle<boolean>(() => Promise.resolve(true));
```



