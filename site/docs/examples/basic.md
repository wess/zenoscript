# Basic Examples

Learn the fundamentals of Zenoscript with these beginner-friendly examples.

## Hello World

The simplest Zenoscript program:

```zenoscript
let message = "Hello, Zenoscript!"
console.log(message)
```

**Compiles to:**
```typescript
const message = "Hello, Zenoscript!";
console.log(message);
```

## Variables and Types

```zenoscript
// Basic types
let name = "Alice"
let age = 30
let isActive = true
let score = 95.5

// Arrays
let numbers = [1, 2, 3, 4, 5]
let names = ["Alice", "Bob", "Charlie"]

// Objects
let user = {
  id: 1,
  name: "Alice",
  email: "alice@example.com"
}
```

## Functions

```zenoscript
// Simple function
let greet = (name: string) => `Hello, ${name}!`

// Function with multiple parameters
let add = (a: number, b: number) => a + b

// Function with destructuring
let getFullName = ({ first, last }) => `${first} ${last}`

// Higher-order function
let twice = (fn, x) => fn(fn(x))
let double = (x) => x * 2

let result = twice(double, 5) // 20
```

## Structs

Define structured data types:

```zenoscript
struct Person {
  name: string;
  age: number;
  email: string;
}

struct Address {
  street: string;
  city: string;
  country: string;
}

// Using structs
let person: Person = {
  name: "Alice",
  age: 30,
  email: "alice@example.com"
}

let address: Address = {
  street: "123 Main St",
  city: "New York",
  country: "USA"
}
```

## Atoms

Type-safe constants using the atom syntax:

```zenoscript
// Status atoms
let :pending = Symbol.for("pending")
let :complete = Symbol.for("complete") 
let :failed = Symbol.for("failed")

// Color atoms
let :red = Symbol.for("red")
let :green = Symbol.for("green")
let :blue = Symbol.for("blue")

// Using atoms
let status = :pending
let primaryColor = :blue

// Atoms are great for enums
let getStatusMessage = (status) => {
  if (status === :pending) return "In progress..."
  if (status === :complete) return "Done!"
  if (status === :failed) return "Something went wrong"
  return "Unknown status"
}
```

## Pattern Matching

Control flow with pattern matching:

```zenoscript
// Simple pattern matching
let describeNumber = (n) => match n {
  0 => "zero"
  1 => "one"  
  2 => "two"
  _ => "many"
}

// Pattern matching with atoms
let handleStatus = (status) => match status {
  :loading => "Please wait..."
  :success => "Operation completed!"
  :error => "Something went wrong"
  _ => "Unknown status"
}

// Pattern matching with values
let processResult = (result) => match result {
  { type: :success, data } => `Success: ${data}`
  { type: :error, message } => `Error: ${message}`
  _ => "Invalid result"
}
```

## Pipe Operations

Chain operations for readable data processing:

```zenoscript
let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

// Traditional approach
let traditionalResult = numbers
  .filter(n => n % 2 === 0)
  .map(n => n * n)
  .reduce((sum, n) => sum + n, 0)

// Zenoscript pipe approach
let pipeResult = numbers
  |> filter(n => n % 2 === 0)    // [2, 4, 6, 8, 10]
  |> map(n => n * n)             // [4, 16, 36, 64, 100]
  |> reduce((sum, n) => sum + n, 0) // 220

// String processing
let text = "  hello world  "
let processed = text
  |> trim
  |> toUpperCase
  |> split(" ")
  |> join("-")  // "HELLO-WORLD"
```

## Working with Arrays

```zenoscript
let fruits = ["apple", "banana", "cherry", "date"]

// Filter fruits starting with 'a'
let aFruits = fruits |> filter(fruit => fruit.startsWith("a"))

// Get fruit lengths
let lengths = fruits |> map(fruit => fruit.length)

// Find the longest fruit
let longest = fruits |> reduce((a, b) => a.length > b.length ? a : b)

// Check if any fruit is longer than 5 characters
let hasLong = fruits |> some(fruit => fruit.length > 5)

// Check if all fruits are strings
let allStrings = fruits |> every(fruit => typeof fruit === "string")
```

## Working with Objects

```zenoscript
struct User {
  id: number;
  name: string;
  age: number;
  active: boolean;
}

let users = [
  { id: 1, name: "Alice", age: 30, active: true },
  { id: 2, name: "Bob", age: 25, active: false },
  { id: 3, name: "Charlie", age: 35, active: true }
]

// Get active users
let activeUsers = users |> filter(user => user.active)

// Get user names
let userNames = users |> map(user => user.name)

// Find user by ID
let findUser = (id) => users |> find(user => user.id === id)

// Calculate average age
let averageAge = users
  |> map(user => user.age)
  |> reduce((sum, age) => sum + age, 0)
  |> (total => total / users.length)
```

## Error Handling

```zenoscript
// Result type pattern
let parseNumber = (str) => {
  let num = parseInt(str)
  return isNaN(num) 
    ? { type: :error, message: "Not a valid number" }
    : { type: :success, value: num }
}

// Using the result
let handleInput = (input) => match parseNumber(input) {
  { type: :success, value } => `Parsed: ${value}`
  { type: :error, message } => `Error: ${message}`
}

// Option type pattern
let safeDivide = (a, b) => 
  b === 0 ? { type: :none } : { type: :some, value: a / b }

let divisionResult = match safeDivide(10, 2) {
  { type: :some, value } => `Result: ${value}`
  { type: :none } => "Cannot divide by zero"
}
```

## Recursive Functions

```zenoscript
// Factorial
let factorial = (n) => match n {
  0 => 1
  1 => 1
  _ => n * factorial(n - 1)
}

// Fibonacci
let fibonacci = (n) => match n {
  0 => 0
  1 => 1
  _ => fibonacci(n - 1) + fibonacci(n - 2)
}

// List operations
let sum = (list) => match list {
  [] => 0
  [head, ...tail] => head + sum(tail)
}

let length = (list) => match list {
  [] => 0
  [_, ...tail] => 1 + length(tail)
}
```

## Next Steps

Ready for more advanced examples? Check out:

- [Advanced Examples](/examples/advanced) - Complex patterns and techniques
- [Concurrent Programming](/examples/advanced#concurrent-programming) - Async patterns
- [Language Guide](/docs/syntax) - Complete syntax reference

Try these examples in the [REPL](/docs/repl) to see them in action!