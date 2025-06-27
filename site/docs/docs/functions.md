# Functions

Functions are first-class citizens in Zenoscript, designed with functional programming principles in mind. They support currying, partial application, composition, and other powerful functional patterns.

## Function Declaration

### Basic Syntax

```zenoscript
// Arrow function (preferred)
let add = (a: number, b: number) => a + b

// Multi-line function
let greet = (name: string) => {
  let message = `Hello, ${name}!`
  return message
}

// Function with type annotations
let multiply = (x: number, y: number): number => {
  return x * y
}

// No parameters
let getCurrentTime = () => new Date()

// Single parameter (parentheses optional)
let square = x => x * x
let double = (x) => x * 2
```

### Function Expressions

```zenoscript
// Function as expression
let operation = (a: number, b: number) => a + b

// Function in object
let math = {
  add: (a: number, b: number) => a + b,
  subtract: (a: number, b: number) => a - b,
  multiply: (a: number, b: number) => a * b
}

// Function as parameter
let applyOperation = (fn: (a: number, b: number) => number, x: number, y: number) => {
  return fn(x, y)
}
```

## Function Types

### Type Annotations

```zenoscript
// Function type aliases
type BinaryOperation = (a: number, b: number) => number
type Predicate<T> = (item: T) => boolean
type Mapper<T, U> = (input: T) => U
type Consumer<T> = (item: T) => void

// Using function types
let add: BinaryOperation = (a, b) => a + b
let isEven: Predicate<number> = n => n % 2 === 0
let toString: Mapper<number, string> = n => n.toString()
let log: Consumer<string> = msg => console.log(msg)
```

### Generic Functions

```zenoscript
// Generic function
let identity = <T>(value: T): T => value

// Generic with constraints
let getProperty = <T, K extends keyof T>(obj: T, key: K): T[K] => {
  return obj[key]
}

// Multiple type parameters
let combine = <T, U>(first: T, second: U): [T, U] => {
  return [first, second]
}

// Generic with default type
let createArray = <T = string>(item: T, length: number): T[] => {
  return new Array(length).fill(item)
}

// Usage
let nums = createArray(0, 5)        // number[]
let strs = createArray("hello", 3)  // string[]
let bools = createArray<boolean>(true, 2) // boolean[]
```

## Higher-Order Functions

### Functions that Return Functions

```zenoscript
// Currying
let add = (a: number) => (b: number) => a + b
let add5 = add(5)
let result = add5(3) // 8

// Function factory
let createValidator = (min: number, max: number) => {
  return (value: number) => value >= min && value <= max
}

let ageValidator = createValidator(0, 120)
let isValidAge = ageValidator(25) // true

// Configuration function
let createLogger = (prefix: string) => {
  return (message: string) => {
    console.log(`[${prefix}] ${message}`)
  }
}

let errorLogger = createLogger("ERROR")
let infoLogger = createLogger("INFO")
```

### Functions that Take Functions

```zenoscript
// Higher-order array operations
let map = <T, U>(array: T[], fn: (item: T) => U): U[] => {
  let result: U[] = []
  for (let item of array) {
    result.push(fn(item))
  }
  return result
}

let filter = <T>(array: T[], predicate: (item: T) => boolean): T[] => {
  let result: T[] = []
  for (let item of array) {
    if (predicate(item)) {
      result.push(item)
    }
  }
  return result
}

let reduce = <T, U>(array: T[], fn: (acc: U, item: T) => U, initial: U): U => {
  let result = initial
  for (let item of array) {
    result = fn(result, item)
  }
  return result
}

// Usage
let numbers = [1, 2, 3, 4, 5]
let doubled = map(numbers, x => x * 2)
let evens = filter(numbers, x => x % 2 === 0)
let sum = reduce(numbers, (acc, x) => acc + x, 0)
```

## Function Composition

### Basic Composition

```zenoscript
// Compose function
let compose = <A, B, C>(f: (b: B) => C, g: (a: A) => B) => {
  return (a: A) => f(g(a))
}

// Pipe function
let pipe = <A, B, C>(g: (a: A) => B, f: (b: B) => C) => {
  return (a: A) => f(g(a))
}

// Example functions
let addOne = (x: number) => x + 1
let double = (x: number) => x * 2
let toString = (x: number) => x.toString()

// Composition
let addOneThenDouble = compose(double, addOne)
let doubleThenAddOne = pipe(double, addOne)

let result1 = addOneThenDouble(5) // (5 + 1) * 2 = 12
let result2 = doubleThenAddOne(5) // (5 * 2) + 1 = 11
```

### Pipeline Operations

```zenoscript
// Using Zenoscript's pipe operator
let processNumber = (x: number) => {
  return x
    |> addOne
    |> double
    |> toString
}

let result = processNumber(5) // "12"

// Complex pipeline
let processUsers = (users: User[]) => {
  return users
    |> filter(user => user.active)
    |> map(user => user.name)
    |> filter(name => name.length > 3)
    |> sort
}
```

## Partial Application

```zenoscript
// Partial application helper
let partial = <T extends any[], U extends any[], R>(
  fn: (...args: [...T, ...U]) => R,
  ...partialArgs: T
) => {
  return (...restArgs: U) => fn(...partialArgs, ...restArgs)
}

// Example usage
let multiply3 = (a: number, b: number, c: number) => a * b * c
let multiplyBy2And3 = partial(multiply3, 2, 3)
let result = multiplyBy2And3(4) // 2 * 3 * 4 = 24

// Practical example
let createUrl = (protocol: string, domain: string, path: string) => {
  return `${protocol}://${domain}${path}`
}

let createHttpsUrl = partial(createUrl, "https")
let createApiUrl = partial(createHttpsUrl, "api.example.com")

let userUrl = createApiUrl("/users") // "https://api.example.com/users"
```

## Closures and Lexical Scope

```zenoscript
// Closure example
let createCounter = () => {
  let count = 0
  return {
    increment: () => ++count,
    decrement: () => --count,
    getValue: () => count
  }
}

let counter = createCounter()
console.log(counter.increment()) // 1
console.log(counter.increment()) // 2
console.log(counter.getValue())  // 2

// Module pattern with closures
let createCalculator = () => {
  let history: number[] = []
  
  return {
    add: (a: number, b: number) => {
      let result = a + b
      history.push(result)
      return result
    },
    
    getHistory: () => [...history], // Return copy
    
    clearHistory: () => {
      history = []
    }
  }
}
```

## Recursion

### Basic Recursion

```zenoscript
// Factorial
let factorial = (n: number): number => {
  return n <= 1 ? 1 : n * factorial(n - 1)
}

// Fibonacci
let fibonacci = (n: number): number => {
  return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2)
}

// Sum of array
let sumArray = (arr: number[]): number => {
  return arr.length === 0 ? 0 : arr[0] + sumArray(arr.slice(1))
}
```

### Tail Recursion

```zenoscript
// Tail-recursive factorial
let factorialTail = (n: number, acc: number = 1): number => {
  return n <= 1 ? acc : factorialTail(n - 1, n * acc)
}

// Tail-recursive sum
let sumTail = (arr: number[], acc: number = 0): number => {
  return arr.length === 0 ? acc : sumTail(arr.slice(1), acc + arr[0])
}

// Tail-recursive list operations
let mapTail = <T, U>(
  arr: T[], 
  fn: (item: T) => U, 
  acc: U[] = []
): U[] => {
  return arr.length === 0 
    ? acc 
    : mapTail(arr.slice(1), fn, [...acc, fn(arr[0])])
}
```

## Memoization

```zenoscript
// Memoization helper
let memoize = <T extends any[], R>(fn: (...args: T) => R) => {
  let cache = new Map<string, R>()
  
  return (...args: T): R => {
    let key = JSON.stringify(args)
    
    if (cache.has(key)) {
      return cache.get(key)!
    }
    
    let result = fn(...args)
    cache.set(key, result)
    return result
  }
}

// Memoized fibonacci
let fibonacciMemo = memoize((n: number): number => {
  return n <= 1 ? n : fibonacciMemo(n - 1) + fibonacciMemo(n - 2)
})

// Memoized expensive computation
let expensiveComputation = memoize((data: any[]) => {
  // Simulate expensive operation
  return data.reduce((acc, item) => acc + item.value, 0)
})
```

## Async Functions

### Promises and Async/Await

```zenoscript
// Async function
let fetchUser = async (id: string): Promise<User> => {
  let response = await fetch(`/api/users/${id}`)
  return await response.json()
}

// Error handling
let fetchUserSafe = async (id: string): Promise<User | null> => {
  try {
    let user = await fetchUser(id)
    return user
  } catch (error) {
    console.error("Failed to fetch user:", error)
    return null
  }
}

// Async composition
let fetchAndProcessUser = async (id: string) => {
  return await fetchUser(id)
    |> (user => ({ ...user, processed: true }))
}
```

### Async Higher-Order Functions

```zenoscript
// Async map
let mapAsync = async <T, U>(
  array: T[], 
  fn: (item: T) => Promise<U>
): Promise<U[]> => {
  let results: U[] = []
  for (let item of array) {
    results.push(await fn(item))
  }
  return results
}

// Async filter
let filterAsync = async <T>(
  array: T[], 
  predicate: (item: T) => Promise<boolean>
): Promise<T[]> => {
  let results: T[] = []
  for (let item of array) {
    if (await predicate(item)) {
      results.push(item)
    }
  }
  return results
}

// Usage
let userIds = ["1", "2", "3"]
let users = await mapAsync(userIds, fetchUser)
let activeUsers = await filterAsync(users, async user => {
  return await checkUserStatus(user.id) === "active"
})
```

## Pure Functions

### Characteristics of Pure Functions

```zenoscript
// Pure function - same input, same output, no side effects
let add = (a: number, b: number) => a + b
let multiply = (x: number, y: number) => x * y

// Pure function with objects (returns new object)
let updateUser = (user: User, updates: Partial<User>): User => {
  return { ...user, ...updates }
}

// Pure array operations
let addItem = <T>(array: T[], item: T): T[] => {
  return [...array, item]
}

let removeItem = <T>(array: T[], index: number): T[] => {
  return array.filter((_, i) => i !== index)
}
```

### Avoiding Side Effects

```zenoscript
// Impure - modifies external state
let count = 0
let impureIncrement = () => {
  count++ // Side effect
  return count
}

// Pure - returns new state
let pureIncrement = (count: number) => count + 1

// Impure - modifies input
let impureSort = (array: number[]) => {
  array.sort() // Mutates input
  return array
}

// Pure - returns new array
let pureSort = (array: number[]) => {
  return [...array].sort()
}
```

## Function Utilities

### Common Utility Functions

```zenoscript
// Debounce
let debounce = <T extends any[]>(
  fn: (...args: T) => void, 
  delay: number
) => {
  let timeoutId: number | undefined
  
  return (...args: T) => {
    clearTimeout(timeoutId)
    timeoutId = setTimeout(() => fn(...args), delay)
  }
}

// Throttle
let throttle = <T extends any[]>(
  fn: (...args: T) => void, 
  limit: number
) => {
  let inThrottle = false
  
  return (...args: T) => {
    if (!inThrottle) {
      fn(...args)
      inThrottle = true
      setTimeout(() => inThrottle = false, limit)
    }
  }
}

// Once
let once = <T extends any[], R>(fn: (...args: T) => R) => {
  let called = false
  let result: R
  
  return (...args: T): R => {
    if (!called) {
      called = true
      result = fn(...args)
    }
    return result
  }
}
```

## Best Practices

### 1. Prefer Pure Functions

```zenoscript
// Good - pure function
let calculateTotal = (items: Item[]) => {
  return items.reduce((sum, item) => sum + item.price, 0)
}

// Avoid - impure function
let total = 0
let calculateTotalImpure = (items: Item[]) => {
  total = items.reduce((sum, item) => sum + item.price, 0)
}
```

### 2. Use Function Composition

```zenoscript
// Good - compose small functions
let processData = (data: any[]) => {
  return data
    |> filter(isValid)
    |> map(normalize)
    |> sort(byPriority)
}

// Avoid - large monolithic function
let processDataMonolithic = (data: any[]) => {
  let result = []
  for (let item of data) {
    if (item && item.value > 0) {
      let normalized = {
        id: item.id,
        value: item.value / 100,
        priority: item.priority || 0
      }
      result.push(normalized)
    }
  }
  return result.sort((a, b) => b.priority - a.priority)
}
```

### 3. Use Meaningful Names

```zenoscript
// Good - descriptive names
let isValidEmail = (email: string) => /\S+@\S+\.\S+/.test(email)
let formatCurrency = (amount: number) => `$${amount.toFixed(2)}`

// Avoid - generic names
let check = (input: string) => /\S+@\S+\.\S+/.test(input)
let format = (num: number) => `$${num.toFixed(2)}`
```

### 4. Keep Functions Small

```zenoscript
// Good - small, focused functions
let validateUser = (user: User) => {
  return isValidEmail(user.email) && 
         isValidAge(user.age) && 
         isValidName(user.name)
}

let isValidEmail = (email: string) => /\S+@\S+\.\S+/.test(email)
let isValidAge = (age: number) => age >= 0 && age <= 120
let isValidName = (name: string) => name.length >= 2
```

Functions in Zenoscript are designed to be expressive, composable, and safe, making it easy to build complex functionality from simple, reusable pieces.