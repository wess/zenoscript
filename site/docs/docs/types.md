# Types

Zenoscript has a rich type system that combines the flexibility of TypeScript with functional programming concepts. The type system is designed to catch errors at compile time while remaining expressive and easy to use.

## Primitive Types

### Basic Types

```zenoscript
// Numbers
let age: number = 25
let price: number = 99.99
let hex: number = 0xFF
let binary: number = 0b1010
let octal: number = 0o755

// Strings
let name: string = "Alice"
let template: string = `Hello, ${name}!`
let multiline: string = `
  This is a
  multiline string
`

// Booleans
let isActive: boolean = true
let isComplete: boolean = false

// Symbols (for atoms)
let status: symbol = Symbol.for("pending")
let color: symbol = Symbol.for("red")
```

### Special Types

```zenoscript
// Void - function that returns nothing
let logMessage = (msg: string): void => {
  console.log(msg)
}

// Never - function that never returns
let throwError = (message: string): never => {
  throw new Error(message)
}

// Unknown - top type, safer than any
let userInput: unknown = getValue()

// Any - escape hatch (use sparingly)
let legacy: any = getLegacyValue()
```

## Collection Types

### Arrays

```zenoscript
// Array syntax
let numbers: number[] = [1, 2, 3, 4, 5]
let names: Array<string> = ["Alice", "Bob", "Charlie"]

// Empty arrays need type annotation
let items: string[] = []
let data: Array<number> = []

// Mixed types
let mixed: (string | number)[] = ["hello", 42, "world"]

// Readonly arrays
let constants: readonly number[] = [1, 2, 3]
let immutable: ReadonlyArray<string> = ["a", "b", "c"]
```

### Tuples

```zenoscript
// Fixed-length arrays with known types
let point: [number, number] = [10, 20]
let person: [string, number, boolean] = ["Alice", 30, true]

// Optional tuple elements
let optional: [string, number?] = ["hello"]
let withOptional: [string, number?] = ["hello", 42]

// Rest elements in tuples
let variadic: [string, ...number[]] = ["prefix", 1, 2, 3]
```

### Objects

```zenoscript
// Object type annotation
let user: { name: string; age: number; email?: string } = {
  name: "Alice",
  age: 30
}

// Index signatures
let scores: { [subject: string]: number } = {
  math: 95,
  science: 87,
  english: 92
}

// Nested objects
let config: {
  database: { host: string; port: number };
  cache: { enabled: boolean; ttl?: number };
} = {
  database: { host: "localhost", port: 5432 },
  cache: { enabled: true }
}
```

## Union Types

```zenoscript
// Basic unions
let id: string | number = "user123"
id = 42 // Also valid

// Literal unions
let status: "pending" | "complete" | "failed" = "pending"
let direction: "north" | "south" | "east" | "west" = "north"

// Complex unions
let response: 
  | { success: true; data: any }
  | { success: false; error: string }

// Discriminated unions
type Shape = 
  | { type: "circle"; radius: number }
  | { type: "rectangle"; width: number; height: number }
  | { type: "square"; size: number }
```

## Intersection Types

```zenoscript
// Combining types
type Named = { name: string }
type Aged = { age: number }
type Person = Named & Aged

let person: Person = {
  name: "Alice",
  age: 30
}

// Mixins pattern
type Timestamps = {
  createdAt: Date;
  updatedAt: Date;
}

type User = {
  id: string;
  email: string;
} & Timestamps
```

## Type Aliases

```zenoscript
// Simple aliases
type UserId = string
type Score = number
type Status = "active" | "inactive" | "pending"

// Generic aliases
type Result<T, E = Error> = 
  | { success: true; data: T }
  | { success: false; error: E }

type Optional<T> = T | null | undefined

// Function type aliases
type EventHandler<T> = (event: T) => void
type Predicate<T> = (item: T) => boolean
type Mapper<T, U> = (item: T) => U
```

## Atom Types

Zenoscript introduces atoms as a special type for type-safe constants:

```zenoscript
// Atom declarations
let :pending = Symbol.for("pending")
let :complete = Symbol.for("complete")
let :failed = Symbol.for("failed")

// Atom types
type TaskStatus = :pending | :complete | :failed

let currentStatus: TaskStatus = :pending

// Using atoms in functions
let getStatusMessage = (status: TaskStatus): string => {
  return match status {
    :pending => "Task is in progress"
    :complete => "Task completed successfully"
    :failed => "Task failed"
  }
}

// Atom unions with values
type ApiResponse = 
  | { status: :success; data: any }
  | { status: :error; message: string }
  | { status: :loading }
```

## Generic Types

### Basic Generics

```zenoscript
// Generic functions
let identity = <T>(value: T): T => value
let first = <T>(items: T[]): T | undefined => items[0]

// Generic with constraints
let getProperty = <T, K extends keyof T>(obj: T, key: K): T[K] => {
  return obj[key]
}

// Multiple type parameters
let combine = <T, U>(first: T, second: U): [T, U] => [first, second]
```

### Generic Interfaces

```zenoscript
interface Container<T> {
  value: T;
  isEmpty(): boolean;
  map<U>(fn: (value: T) => U): Container<U>;
}

interface Repository<T> {
  findById(id: string): Promise<T | null>;
  save(entity: T): Promise<T>;
  delete(id: string): Promise<void>;
}
```

### Conditional Types

```zenoscript
// Basic conditional types
type IsArray<T> = T extends any[] ? true : false

type Example1 = IsArray<string[]> // true
type Example2 = IsArray<string>   // false

// Distributive conditional types
type ToArray<T> = T extends any ? T[] : never

type StringOrNumberArray = ToArray<string | number> // string[] | number[]

// Utility conditional types
type NonNullable<T> = T extends null | undefined ? never : T
type ReturnType<T> = T extends (...args: any[]) => infer R ? R : never
```

## Mapped Types

```zenoscript
// Make all properties optional
type Partial<T> = {
  [P in keyof T]?: T[P]
}

// Make all properties required
type Required<T> = {
  [P in keyof T]-?: T[P]
}

// Make all properties readonly
type Readonly<T> = {
  readonly [P in keyof T]: T[P]
}

// Pick specific properties
type Pick<T, K extends keyof T> = {
  [P in K]: T[P]
}

// Example usage
interface User {
  id: string;
  name: string;
  email: string;
  age?: number;
}

type PartialUser = Partial<User>     // All properties optional
type UserEmail = Pick<User, "email"> // Only email property
type ReadonlyUser = Readonly<User>   // All properties readonly
```

## Type Guards

```zenoscript
// Built-in type guards
let isString = (value: any): value is string => {
  return typeof value === "string"
}

let isNumber = (value: any): value is number => {
  return typeof value === "number"
}

// Custom type guards
interface Dog {
  breed: string;
  bark(): void;
}

interface Cat {
  breed: string;
  meow(): void;
}

let isDog = (animal: Dog | Cat): animal is Dog => {
  return "bark" in animal
}

// Usage with type narrowing
let handleAnimal = (animal: Dog | Cat) => {
  if (isDog(animal)) {
    animal.bark() // TypeScript knows this is a Dog
  } else {
    animal.meow() // TypeScript knows this is a Cat
  }
}
```

## Advanced Type Patterns

### Option Type Pattern

```zenoscript
type Option<T> = 
  | { type: :some; value: T }
  | { type: :none }

let some = <T>(value: T): Option<T> => ({ type: :some, value })
let none = <T>(): Option<T> => ({ type: :none })

let safeDivide = (a: number, b: number): Option<number> => {
  return b === 0 ? none() : some(a / b)
}
```

### Result Type Pattern

```zenoscript
type Result<T, E = Error> = 
  | { type: :ok; value: T }
  | { type: :error; error: E }

let ok = <T, E>(value: T): Result<T, E> => ({ type: :ok, value })
let error = <T, E>(err: E): Result<T, E> => ({ type: :error, error: err })

let parseNumber = (str: string): Result<number, string> => {
  let num = parseFloat(str)
  return isNaN(num) ? error("Invalid number") : ok(num)
}
```

## Type Inference

Zenoscript has powerful type inference that reduces the need for explicit type annotations:

```zenoscript
// Type is inferred as number
let count = 42

// Type is inferred as string[]
let names = ["Alice", "Bob", "Charlie"]

// Return type is inferred
let add = (a: number, b: number) => a + b // Returns number

// Generic type is inferred
let items = [1, 2, 3] // T is inferred as number
let doubled = items.map(x => x * 2) // Returns number[]

// Contextual typing
let handler: (event: string) => void = event => {
  // event is inferred as string
  console.log(event.toUpperCase())
}
```

## Best Practices

### 1. Prefer Type Inference

```zenoscript
// Good - let TypeScript infer
let users = fetchUsers()

// Avoid - unnecessary annotation
let users: User[] = fetchUsers()
```

### 2. Use Strict Types

```zenoscript
// Good - specific types
type Status = "idle" | "loading" | "success" | "error"

// Avoid - too general
type Status = string
```

### 3. Leverage Union Types

```zenoscript
// Good - use discriminated unions
type ApiResponse = 
  | { status: "success"; data: any }
  | { status: "error"; message: string }

// Avoid - loose object
type ApiResponse = {
  status: string;
  data?: any;
  message?: string;
}
```

### 4. Use Type Guards

```zenoscript
// Good - type guard for narrowing
if (isError(response)) {
  console.error(response.message)
} else {
  console.log(response.data)
}

// Avoid - type assertions
console.log((response as ErrorResponse).message)
```

The Zenoscript type system is designed to make your code more reliable and maintainable while preserving the expressiveness needed for functional programming patterns.