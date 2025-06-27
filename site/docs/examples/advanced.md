# Advanced Examples

Explore sophisticated functional programming patterns and advanced Zenoscript features.

## Monadic Patterns

### Option Type Implementation

```zenoscript
// Option type with full monadic interface
struct Option<T> {
  type: symbol;
  value?: T;
}

let some = <T>(value: T): Option<T> => ({ type: :some, value })
let none = <T>(): Option<T> => ({ type: :none })

// Functor (map)
let map = <T, U>(option: Option<T>, fn: (T) => U): Option<U> => match option {
  { type: :some, value } => some(fn(value))
  { type: :none } => none()
}

// Applicative (apply)
let apply = <T, U>(optionFn: Option<(T) => U>, optionValue: Option<T>): Option<U> => 
  match [optionFn, optionValue] {
    [{ type: :some, value: fn }, { type: :some, value }] => some(fn(value))
    _ => none()
  }

// Monad (flatMap/bind)
let flatMap = <T, U>(option: Option<T>, fn: (T) => Option<U>): Option<U> => match option {
  { type: :some, value } => fn(value)
  { type: :none } => none()
}

// Utility functions
let isSome = <T>(option: Option<T>): boolean => option.type === :some
let isNone = <T>(option: Option<T>): boolean => option.type === :none
let getOrElse = <T>(option: Option<T>, defaultValue: T): T => match option {
  { type: :some, value } => value
  { type: :none } => defaultValue
}

// Usage example
let safeDivide = (a: number, b: number): Option<number> =>
  b === 0 ? none() : some(a / b)

let compute = (x: number, y: number, z: number) =>
  safeDivide(x, y)
    |> flatMap(result => safeDivide(result, z))
    |> map(final => final * 2)
    |> getOrElse(0)
```

### Result Type for Error Handling

```zenoscript
struct Result<T, E> {
  type: symbol;
  value?: T;
  error?: E;
}

let ok = <T, E>(value: T): Result<T, E> => ({ type: :ok, value })
let err = <T, E>(error: E): Result<T, E> => ({ type: :error, error })

let mapResult = <T, U, E>(result: Result<T, E>, fn: (T) => U): Result<U, E> => match result {
  { type: :ok, value } => ok(fn(value))
  { type: :error, error } => err(error)
}

let mapError = <T, E, F>(result: Result<T, E>, fn: (E) => F): Result<T, F> => match result {
  { type: :ok, value } => ok(value)
  { type: :error, error } => err(fn(error))
}

let flatMapResult = <T, U, E>(result: Result<T, E>, fn: (T) => Result<U, E>): Result<U, E> => match result {
  { type: :ok, value } => fn(value)
  { type: :error, error } => err(error)
}

// Async operations with Result
let fetchUser = async (id: number): Promise<Result<User, string>> => {
  try {
    let response = await fetch(`/api/users/${id}`)
    if (!response.ok) {
      return err(`HTTP ${response.status}: ${response.statusText}`)
    }
    let user = await response.json()
    return ok(user)
  } catch (error) {
    return err(error.message)
  }
}
```

## State Management

### Immutable State with Lenses

```zenoscript
// Lens implementation for immutable updates
struct Lens<S, A> {
  get: (S) => A;
  set: (A, S) => S;
}

let lens = <S, A>(get: (S) => A, set: (A, S) => S): Lens<S, A> => ({ get, set })

let view = <S, A>(lens: Lens<S, A>, state: S): A => lens.get(state)
let set = <S, A>(lens: Lens<S, A>, value: A, state: S): S => lens.set(value, state)
let over = <S, A>(lens: Lens<S, A>, fn: (A) => A, state: S): S => 
  lens.set(fn(lens.get(state)), state)

// Property lenses
let prop = <S, K extends keyof S>(key: K): Lens<S, S[K]> =>
  lens(
    s => s[key],
    (value, s) => ({ ...s, [key]: value })
  )

// Compose lenses
let compose = <S, A, B>(lensA: Lens<S, A>, lensB: Lens<A, B>): Lens<S, B> =>
  lens(
    s => lensB.get(lensA.get(s)),
    (value, s) => lensA.set(lensB.set(value, lensA.get(s)), s)
  )

// Example usage
struct Address {
  street: string;
  city: string;
}

struct User {
  name: string;
  address: Address;
}

let userAddressStreet = compose(prop("address"), prop("street"))

let user = {
  name: "Alice",
  address: { street: "123 Main St", city: "NYC" }
}

let updatedUser = over(userAddressStreet, street => street.toUpperCase(), user)
```

### Redux-style State Management

```zenoscript
// Action types
let :increment = Symbol.for("increment")
let :decrement = Symbol.for("decrement")
let :reset = Symbol.for("reset")
let :setCount = Symbol.for("setCount")

struct CounterState {
  count: number;
  history: number[];
}

struct Action<T> {
  type: symbol;
  payload?: T;
}

// Reducer
let counterReducer = (state: CounterState, action: Action<any>): CounterState => match action {
  { type: :increment } => {
    let newCount = state.count + 1
    return {
      count: newCount,
      history: [...state.history, newCount]
    }
  }
  { type: :decrement } => {
    let newCount = state.count - 1
    return {
      count: newCount,
      history: [...state.history, newCount]
    }
  }
  { type: :setCount, payload } => {
    return {
      count: payload,
      history: [...state.history, payload]
    }
  }
  { type: :reset } => {
    return { count: 0, history: [0] }
  }
  _ => state
}

// Store implementation
let createStore = <S>(reducer: (S, Action<any>) => S, initialState: S) => {
  let state = initialState
  let listeners = []
  
  return {
    getState: () => state,
    dispatch: (action: Action<any>) => {
      state = reducer(state, action)
      listeners |> forEach(listener => listener(state))
    },
    subscribe: (listener: (S) => void) => {
      listeners.push(listener)
      return () => {
        let index = listeners.indexOf(listener)
        if (index >= 0) listeners.splice(index, 1)
      }
    }
  }
}
```

## Functional Data Structures

### Persistent List

```zenoscript
// Persistent linked list implementation
struct List<T> {
  type: symbol;
  head?: T;
  tail?: List<T>;
}

let empty = <T>(): List<T> => ({ type: :empty })
let cons = <T>(head: T, tail: List<T>): List<T> => ({ type: :cons, head, tail })

let isEmpty = <T>(list: List<T>): boolean => list.type === :empty

let head = <T>(list: List<T>): Option<T> => match list {
  { type: :empty } => none()
  { type: :cons, head } => some(head)
}

let tail = <T>(list: List<T>): Option<List<T>> => match list {
  { type: :empty } => none()
  { type: :cons, tail } => some(tail)
}

let length = <T>(list: List<T>): number => match list {
  { type: :empty } => 0
  { type: :cons, tail } => 1 + length(tail)
}

let append = <T>(list1: List<T>, list2: List<T>): List<T> => match list1 {
  { type: :empty } => list2
  { type: :cons, head, tail } => cons(head, append(tail, list2))
}

let map = <T, U>(list: List<T>, fn: (T) => U): List<U> => match list {
  { type: :empty } => empty()
  { type: :cons, head, tail } => cons(fn(head), map(tail, fn))
}

let filter = <T>(list: List<T>, predicate: (T) => boolean): List<T> => match list {
  { type: :empty } => empty()
  { type: :cons, head, tail } => 
    predicate(head) ? cons(head, filter(tail, predicate)) : filter(tail, predicate)
}

let fold = <T, U>(list: List<T>, fn: (U, T) => U, initial: U): U => match list {
  { type: :empty } => initial
  { type: :cons, head, tail } => fold(tail, fn, fn(initial, head))
}
```

### Binary Tree

```zenoscript
struct Tree<T> {
  type: symbol;
  value?: T;
  left?: Tree<T>;
  right?: Tree<T>;
}

let leaf = <T>(): Tree<T> => ({ type: :leaf })
let node = <T>(value: T, left: Tree<T>, right: Tree<T>): Tree<T> => 
  ({ type: :node, value, left, right })

let insert = <T>(tree: Tree<T>, value: T, compare: (T, T) => number): Tree<T> => match tree {
  { type: :leaf } => node(value, leaf(), leaf())
  { type: :node, value: nodeValue, left, right } => {
    let cmp = compare(value, nodeValue)
    if (cmp < 0) {
      return node(nodeValue, insert(left, value, compare), right)
    } else if (cmp > 0) {
      return node(nodeValue, left, insert(right, value, compare))
    } else {
      return tree // Value already exists
    }
  }
}

let inOrder = <T>(tree: Tree<T>): T[] => match tree {
  { type: :leaf } => []
  { type: :node, value, left, right } => [
    ...inOrder(left),
    value,
    ...inOrder(right)
  ]
}

let search = <T>(tree: Tree<T>, value: T, compare: (T, T) => number): boolean => match tree {
  { type: :leaf } => false
  { type: :node, value: nodeValue, left, right } => {
    let cmp = compare(value, nodeValue)
    if (cmp === 0) return true
    if (cmp < 0) return search(left, value, compare)
    return search(right, value, compare)
  }
}
```

## Advanced Type Patterns

### Generic Constraints

```zenoscript
// Phantom types for compile-time safety
struct UserId { _brand: "UserId"; value: number }
struct ProductId { _brand: "ProductId"; value: number }

let userId = (value: number): UserId => ({ _brand: "UserId", value })
let productId = (value: number): ProductId => ({ _brand: "ProductId", value })

// Type-safe database operations
let findUser = (id: UserId) => { /* implementation */ }
let findProduct = (id: ProductId) => { /* implementation */ }

// Compile-time error prevention
// findUser(productId(123)) // Type error!
```

### Builder Pattern

```zenoscript
struct QueryBuilder<T> {
  table: string;
  conditions: string[];
  joins: string[];
  orderBy?: string;
  limit?: number;
}

let query = <T>(table: string): QueryBuilder<T> => ({
  table,
  conditions: [],
  joins: [],
})

let where = <T>(builder: QueryBuilder<T>, condition: string): QueryBuilder<T> => ({
  ...builder,
  conditions: [...builder.conditions, condition]
})

let join = <T>(builder: QueryBuilder<T>, joinClause: string): QueryBuilder<T> => ({
  ...builder,
  joins: [...builder.joins, joinClause]
})

let orderBy = <T>(builder: QueryBuilder<T>, column: string): QueryBuilder<T> => ({
  ...builder,
  orderBy: column
})

let limit = <T>(builder: QueryBuilder<T>, count: number): QueryBuilder<T> => ({
  ...builder,
  limit: count
})

let build = <T>(builder: QueryBuilder<T>): string => {
  let sql = `SELECT * FROM ${builder.table}`
  
  if (builder.joins.length > 0) {
    sql += ` ${builder.joins.join(" ")}`
  }
  
  if (builder.conditions.length > 0) {
    sql += ` WHERE ${builder.conditions.join(" AND ")}`
  }
  
  if (builder.orderBy) {
    sql += ` ORDER BY ${builder.orderBy}`
  }
  
  if (builder.limit) {
    sql += ` LIMIT ${builder.limit}`
  }
  
  return sql
}

// Usage
let userQuery = query("users")
  |> where("age > 18")
  |> where("active = true")
  |> join("LEFT JOIN profiles ON users.id = profiles.user_id")
  |> orderBy("created_at DESC")
  |> limit(10)
  |> build
```

## Concurrent Programming

### Async/Await with Result

```zenoscript
let asyncResult = async <T, E>(fn: () => Promise<T>): Promise<Result<T, E>> => {
  try {
    let value = await fn()
    return ok(value)
  } catch (error) {
    return err(error)
  }
}

let sequence = async <T, E>(results: Promise<Result<T, E>>[]): Promise<Result<T[], E>> => {
  let resolved = await Promise.all(results)
  let values = []
  
  for (let result of resolved) {
    match result {
      { type: :ok, value } => values.push(value)
      { type: :error, error } => return err(error)
    }
  }
  
  return ok(values)
}

// Parallel processing
let fetchUsers = async (ids: number[]) => {
  let promises = ids |> map(id => asyncResult(() => fetchUser(id)))
  return await sequence(promises)
}
```

### Channel-like Communication

```zenoscript
struct Channel<T> {
  buffer: T[];
  capacity: number;
  waiting: { resolve: (T) => void; reject: (Error) => void }[];
}

let createChannel = <T>(capacity = 0): Channel<T> => ({
  buffer: [],
  capacity,
  waiting: []
})

let send = async <T>(channel: Channel<T>, value: T): Promise<void> => {
  if (channel.buffer.length < channel.capacity) {
    channel.buffer.push(value)
    
    if (channel.waiting.length > 0) {
      let { resolve } = channel.waiting.shift()
      resolve(channel.buffer.shift())
    }
  } else {
    throw new Error("Channel buffer full")
  }
}

let receive = async <T>(channel: Channel<T>): Promise<T> => {
  if (channel.buffer.length > 0) {
    return channel.buffer.shift()
  }
  
  return new Promise((resolve, reject) => {
    channel.waiting.push({ resolve, reject })
  })
}
```

These advanced patterns demonstrate Zenoscript's power for building complex, maintainable applications with functional programming principles.

## Next Steps

- Read the [Language Reference](/docs/syntax)
- Check out [Basic Examples](/examples/basic)
- Join the [Community Discord](https://discord.gg/zenoscript)