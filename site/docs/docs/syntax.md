# Language Syntax

Complete reference for Zenoscript syntax and language features.

## Variables and Constants

### Let Bindings

```zenoscript
// Immutable bindings (recommended)
let name = "Alice"
let age = 30
let isActive = true

// Type annotations (optional)
let score: number = 95.5
let message: string = "Hello"
let items: string[] = ["a", "b", "c"]
```

### Variable Declarations  

```zenoscript
// Mutable variables (use sparingly)
var counter = 0
counter = counter + 1
```

## Data Types

### Basic Types

```zenoscript
// Numbers
let integer = 42
let float = 3.14
let negative = -10

// Strings
let greeting = "Hello, World!"
let template = `Hello, ${name}!`
let multiline = `
  This is a
  multiline string
`

// Booleans
let isTrue = true
let isFalse = false

// Arrays
let numbers = [1, 2, 3, 4, 5]
let mixed = [1, "hello", true]
let empty: number[] = []

// Objects
let user = {
  name: "Alice",
  age: 30,
  active: true
}
```

### Atoms

Type-safe constants using symbol syntax:

```zenoscript
// Atom declaration
let :success = Symbol.for("success")
let :error = Symbol.for("error")
let :pending = Symbol.for("pending")

// Using atoms
let status = :pending
let result = :success

// Atoms in pattern matching
let message = match status {
  :pending => "Loading..."
  :success => "Done!"
  :error => "Failed!"
  _ => "Unknown"
}
```

## Structs

Define structured data types:

```zenoscript
// Simple struct
struct User {
  id: number;
  name: string;
  email: string;
}

// Generic struct
struct Container<T> {
  value: T;
  timestamp: number;
}

// Struct with optional fields
struct Config {
  host: string;
  port?: number;
  ssl?: boolean;
}

// Using structs
let user: User = {
  id: 1,
  name: "Alice",
  email: "alice@example.com"
}

let config: Config = {
  host: "localhost",
  port: 8080
}
```

## Traits

Define interfaces and contracts:

```zenoscript
// Simple trait
trait Drawable {
  draw(): void;
}

// Generic trait
trait Serializable<T> {
  serialize(): T;
  deserialize(data: T): this;
}

// Trait with default methods
trait Logger {
  log(message: string): void;
  
  // Default implementation
  logError(error: string): void {
    this.log(`ERROR: ${error}`)
  }
}
```

## Functions

### Function Declarations

```zenoscript
// Arrow functions
let add = (a: number, b: number) => a + b
let greet = (name: string) => `Hello, ${name}!`

// Multi-line functions
let complexFunction = (x: number, y: number) => {
  let sum = x + y
  let product = x * y
  return { sum, product }
}

// Function with destructuring
let getName = ({ first, last }: { first: string, last: string }) => 
  `${first} ${last}`

// Higher-order functions
let map = <T, U>(array: T[], fn: (T) => U): U[] => 
  array.map(fn)

let filter = <T>(array: T[], predicate: (T) => boolean): T[] =>
  array.filter(predicate)
```

### Currying

```zenoscript
// Curried function
let multiply = (a: number) => (b: number) => a * b
let double = multiply(2)
let triple = multiply(3)

let result1 = double(5) // 10
let result2 = triple(4) // 12

// Partial application
let greetWith = (greeting: string) => (name: string) => 
  `${greeting}, ${name}!`

let sayHello = greetWith("Hello")
let sayGoodbye = greetWith("Goodbye")
```

## Pattern Matching

### Basic Matching

```zenoscript
// Match expressions
let describe = (value) => match value {
  0 => "zero"
  1 => "one"
  2 => "two"
  _ => "many"
}

// Match with guards
let classify = (n: number) => match n {
  x if x < 0 => "negative"
  0 => "zero"
  x if x > 0 && x < 10 => "small positive"
  _ => "large positive"
}
```

### Destructuring

```zenoscript
// Array destructuring
let [first, second, ...rest] = [1, 2, 3, 4, 5]

// Object destructuring
let { name, age } = user
let { name: userName, age: userAge } = user

// Nested destructuring
let { user: { name }, config: { port } } = response

// Pattern matching with destructuring
let processResult = (result) => match result {
  { type: "success", data } => `Success: ${data}`
  { type: "error", message, code } => `Error ${code}: ${message}`
  { type: "pending" } => "Processing..."
  _ => "Unknown result"
}
```

### List Patterns

```zenoscript
// Match list patterns
let processItems = (items) => match items {
  [] => "empty list"
  [single] => `one item: ${single}`
  [first, second] => `two items: ${first}, ${second}`
  [head, ...tail] => `${head} and ${tail.length} more`
}

// Recursive list processing
let sum = (numbers) => match numbers {
  [] => 0
  [head, ...tail] => head + sum(tail)
}
```

## Pipe Operations

Chain operations for readable data flow:

```zenoscript
// Basic piping
let result = value
  |> transform
  |> validate
  |> process

// With function calls
let processed = data
  |> filter(x => x > 0)
  |> map(x => x * 2)
  |> reduce((a, b) => a + b, 0)

// String processing
let cleaned = input
  |> trim
  |> toLowerCase
  |> replace(/\s+/g, "-")

// Async piping
let response = await url
  |> fetch
  |> (res => res.json())
  |> validateResponse
```

## Control Flow

### Conditionals

```zenoscript
// If expressions
let message = if (user.active) "Welcome!" else "Please activate"

// Multi-line if
let status = if (score >= 90) {
  "excellent"
} else if (score >= 70) {
  "good"
} else {
  "needs improvement"
}

// Ternary operator
let label = isActive ? "Active" : "Inactive"
```

### Loops

```zenoscript
// For loops (avoid in functional style)
for (let i = 0; i < items.length; i++) {
  console.log(items[i])
}

// For-of loops
for (let item of items) {
  console.log(item)
}

// Functional alternatives (preferred)
items |> forEach(item => console.log(item))
items |> map(item => item.toUpperCase())
items |> filter(item => item.length > 3)
```

## Error Handling

### Option Type Pattern

```zenoscript
// Option type
let findUser = (id: number) => 
  users.find(u => u.id === id) ? 
    { type: :some, value: users.find(u => u.id === id) } :
    { type: :none }

// Using option
let userResult = match findUser(123) {
  { type: :some, value } => `Found: ${value.name}`
  { type: :none } => "User not found"
}
```

### Result Type Pattern

```zenoscript
// Result type for operations that can fail
let parseJson = (text: string) => {
  try {
    let data = JSON.parse(text)
    return { type: :ok, value: data }
  } catch (error) {
    return { type: :error, message: error.message }
  }
}

// Using result
let handleJson = (text) => match parseJson(text) {
  { type: :ok, value } => processData(value)
  { type: :error, message } => logError(message)
}
```

## Modules and Imports

### Importing

```zenoscript
// ES module imports
import { readFile } from "fs/promises"
import express from "express"
import * as utils from "./utils"

// Zenoscript module imports
import { User, createUser } from "./models/user.zs"
import { DatabaseConfig } from "./config.zs"
```

### Exporting

```zenoscript
// Named exports
export let PI = 3.14159
export let square = (x: number) => x * x

export struct Point {
  x: number;
  y: number;
}

// Default export
let main = () => {
  console.log("Hello from Zenoscript!")
}

export default main
```

## Comments

```zenoscript
// Single line comment

/*
  Multi-line comment
  spanning multiple lines
*/

/**
 * Documentation comment
 * @param name The user's name
 * @returns A greeting message
 */
let greet = (name: string) => `Hello, ${name}!`
```

## Type Annotations

```zenoscript
// Variable annotations
let name: string = "Alice"
let age: number = 30
let active: boolean = true

// Function annotations
let add = (a: number, b: number): number => a + b
let greet = (name: string): string => `Hello, ${name}!`

// Generic annotations
let identity = <T>(value: T): T => value
let map = <T, U>(array: T[], fn: (T) => U): U[] => array.map(fn)

// Complex types
let user: { name: string; age: number } = { name: "Alice", age: 30 }
let callback: (error: Error | null, data?: any) => void = () => {}
```

## Operators

### Arithmetic

```zenoscript
let a = 10 + 5   // Addition
let b = 10 - 5   // Subtraction  
let c = 10 * 5   // Multiplication
let d = 10 / 5   // Division
let e = 10 % 3   // Modulo
let f = 2 ** 3   // Exponentiation
```

### Comparison

```zenoscript
let eq = a === b  // Strict equality
let neq = a !== b // Strict inequality
let lt = a < b    // Less than
let lte = a <= b  // Less than or equal
let gt = a > b    // Greater than
let gte = a >= b  // Greater than or equal
```

### Logical

```zenoscript
let and = a && b  // Logical AND
let or = a || b   // Logical OR
let not = !a      // Logical NOT
let nullish = a ?? b  // Nullish coalescing
```

### Assignment

```zenoscript
let x = 5       // Assignment
x += 3          // Add and assign
x -= 2          // Subtract and assign
x *= 4          // Multiply and assign
x /= 2          // Divide and assign
```

This covers the essential syntax of Zenoscript. For more advanced patterns, check out the [Advanced Examples](/examples/advanced).