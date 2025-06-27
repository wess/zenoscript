# Pattern Matching

Pattern matching is one of Zenoscript's most powerful features, allowing you to destructure data and control program flow in an expressive and safe way. It combines the best of switch statements, destructuring, and type checking.

## Basic Match Expressions

### Simple Value Matching

```zenoscript
let describe = (value: number) => match value {
  0 => "zero"
  1 => "one"
  2 => "two"
  _ => "many"
}

let result = describe(5) // "many"

// With atoms
let getStatusMessage = (status: symbol) => match status {
  :pending => "Please wait..."
  :success => "Operation completed!"
  :error => "Something went wrong"
  _ => "Unknown status"
}
```

### String Patterns

```zenoscript
let greetUser = (name: string) => match name {
  "admin" => "Welcome, Administrator!"
  "guest" => "Hello, Guest User"
  "alice" | "bob" | "charlie" => `Hi ${name}, team member!`
  _ => `Hello, ${name}!`
}

// Regular expressions in patterns
let classifyInput = (input: string) => match input {
  /^\d+$/ => "numeric"
  /^[a-zA-Z]+$/ => "alphabetic"
  /^\w+@\w+\.\w+$/ => "email"
  _ => "other"
}
```

## Destructuring Patterns

### Array Destructuring

```zenoscript
let processArray = (arr: number[]) => match arr {
  [] => "empty array"
  [single] => `single item: ${single}`
  [first, second] => `two items: ${first}, ${second}`
  [first, ...rest] => `first: ${first}, remaining: ${rest.length}`
}

// Nested array patterns
let processMatrix = (matrix: number[][]) => match matrix {
  [] => "empty matrix"
  [[]] => "single empty row"
  [[single]] => `single cell: ${single}`
  [[a, b], [c, d]] => `2x2 matrix: ${a}, ${b}, ${c}, ${d}`
  _ => "complex matrix"
}
```

### Object Destructuring

```zenoscript
interface User {
  name: string;
  age: number;
  role: string;
  active?: boolean;
}

let processUser = (user: User) => match user {
  { name: "admin" } => "Administrator user"
  { role: "guest", active: false } => "Inactive guest"
  { age, name } when age < 18 => `Minor user: ${name}`
  { role: "admin", active: true } => "Active administrator"
  { name, age } => `Regular user: ${name}, age ${age}`
}

// Nested destructuring
interface Address {
  street: string;
  city: string;
  country: string;
}

interface Person {
  name: string;
  address: Address;
}

let getLocation = (person: Person) => match person {
  { address: { city: "New York", country: "USA" } } => "NYC resident"
  { address: { country: "USA" } } => "US resident"
  { address: { country } } => `Lives in ${country}`
  _ => "Unknown location"
}
```

## Guards and Conditions

### When Clauses

```zenoscript
let categorizeNumber = (n: number) => match n {
  x when x < 0 => "negative"
  0 => "zero"
  x when x > 0 && x < 10 => "small positive"
  x when x >= 10 && x < 100 => "medium positive"
  x when x >= 100 => "large positive"
  _ => "unknown"
}

// Complex guards
let validateUser = (user: User) => match user {
  { age } when age < 0 => "Invalid age"
  { age } when age > 120 => "Unrealistic age"
  { name } when name.length === 0 => "Name required"
  { name } when name.length < 2 => "Name too short"
  { role } when !["admin", "user", "guest"].includes(role) => "Invalid role"
  _ => "Valid user"
}
```

### Multiple Conditions

```zenoscript
let processData = (data: any) => match data {
  { type: "user", verified: true, age } when age >= 18 => "Verified adult user"
  { type: "user", verified: false } => "Unverified user"
  { type: "admin", permissions } when permissions.includes("write") => "Admin with write access"
  { type: "guest" } => "Guest user"
  _ => "Unknown data type"
}
```

## Type-Based Patterns

### Discriminated Unions

```zenoscript
type Shape = 
  | { type: "circle"; radius: number }
  | { type: "rectangle"; width: number; height: number }
  | { type: "triangle"; base: number; height: number }

let calculateArea = (shape: Shape) => match shape {
  { type: "circle", radius } => Math.PI * radius * radius
  { type: "rectangle", width, height } => width * height
  { type: "triangle", base, height } => (base * height) / 2
}

// Result type pattern
type Result<T, E> = 
  | { type: "success"; data: T }
  | { type: "error"; error: E }

let handleResult = <T, E>(result: Result<T, E>) => match result {
  { type: "success", data } => {
    console.log("Success:", data)
    return data
  }
  { type: "error", error } => {
    console.error("Error:", error)
    throw error
  }
}
```

### Optional Types

```zenoscript
type Option<T> = 
  | { type: "some"; value: T }
  | { type: "none" }

let processOption = <T>(option: Option<T>) => match option {
  { type: "some", value } => `Got value: ${value}`
  { type: "none" } => "No value"
}

// Nested options
let processNestedOption = (option: Option<Option<string>>) => match option {
  { type: "some", value: { type: "some", value } } => `Nested value: ${value}`
  { type: "some", value: { type: "none" } } => "Outer some, inner none"
  { type: "none" } => "Outer none"
}
```

## Advanced Patterns

### Recursive Data Structures

```zenoscript
type Tree<T> = 
  | { type: "leaf"; value: T }
  | { type: "node"; left: Tree<T>; right: Tree<T> }

let traverseTree = <T>(tree: Tree<T>): T[] => match tree {
  { type: "leaf", value } => [value]
  { type: "node", left, right } => [
    ...traverseTree(left),
    ...traverseTree(right)
  ]
}

// List pattern
type List<T> = 
  | { type: "empty" }
  | { type: "cons"; head: T; tail: List<T> }

let sumList = (list: List<number>): number => match list {
  { type: "empty" } => 0
  { type: "cons", head, tail } => head + sumList(tail)
}
```

### Range Patterns

```zenoscript
// Custom range matching
let classifyScore = (score: number) => match score {
  n when n >= 90 => "A"
  n when n >= 80 => "B"
  n when n >= 70 => "C"
  n when n >= 60 => "D"
  _ => "F"
}

// HTTP status code patterns
let handleHttpStatus = (status: number) => match status {
  200 => "OK"
  201 => "Created"
  n when n >= 200 && n < 300 => "Success"
  n when n >= 400 && n < 500 => "Client Error"
  n when n >= 500 => "Server Error"
  _ => "Unknown Status"
}
```

## Pattern Matching with Functions

### Function Pattern Matching

```zenoscript
// Pattern matching in function parameters
let processResponse = (response: { status: number; data?: any; error?: string }) => {
  return match response {
    { status: 200, data } => ({ success: true, result: data })
    { status, error } when status >= 400 => ({ success: false, error })
    { status } => ({ success: false, error: `Unexpected status: ${status}` })
  }
}

// Multiple parameter patterns
let combineResults = (a: Result<number, string>, b: Result<number, string>) => {
  return match [a, b] {
    [{ type: "success", data: x }, { type: "success", data: y }] => 
      ({ type: "success", data: x + y })
    [{ type: "error", error }, _] => 
      ({ type: "error", error })
    [_, { type: "error", error }] => 
      ({ type: "error", error })
  }
}
```

### Partial Pattern Matching

```zenoscript
// Extract specific properties
let getUserInfo = (user: User) => match user {
  { name, email, ...rest } => {
    console.log(`User: ${name} (${email})`)
    return rest
  }
}

// Pattern matching with spread
let mergeConfigs = (config: any) => match config {
  { database, ...appConfig } => {
    return {
      app: appConfig,
      db: database
    }
  }
}
```

## Error Handling with Patterns

### Exception Patterns

```zenoscript
let safeParseNumber = (input: string): Result<number, string> => {
  try {
    let num = parseFloat(input)
    return match num {
      n when isNaN(n) => ({ type: "error", error: "Not a number" })
      n when !isFinite(n) => ({ type: "error", error: "Number not finite" })
      n => ({ type: "success", data: n })
    }
  } catch (error) {
    return { type: "error", error: error.message }
  }
}

// Async error handling
let fetchUserSafely = async (id: string) => {
  try {
    let response = await fetch(`/api/users/${id}`)
    return match response.status {
      200 => { type: "success", data: await response.json() }
      404 => { type: "error", error: "User not found" }
      status when status >= 500 => { type: "error", error: "Server error" }
      _ => { type: "error", error: "Unknown error" }
    }
  } catch (error) {
    return { type: "error", error: error.message }
  }
}
```

## Pattern Matching Utilities

### Custom Pattern Helpers

```zenoscript
// Range helper
let inRange = (min: number, max: number) => (value: number) => 
  value >= min && value <= max

let categorizeAge = (age: number) => match age {
  n when inRange(0, 12)(n) => "child"
  n when inRange(13, 19)(n) => "teenager"
  n when inRange(20, 64)(n) => "adult"
  n when inRange(65, 120)(n) => "senior"
  _ => "invalid age"
}

// Type guard patterns
let isString = (value: any): value is string => typeof value === "string"
let isNumber = (value: any): value is number => typeof value === "number"

let processValue = (value: any) => match value {
  v when isString(v) => `String: ${v.toUpperCase()}`
  v when isNumber(v) => `Number: ${v.toFixed(2)}`
  _ => "Unknown type"
}
```

### Pattern Composition

```zenoscript
// Compose patterns
let isPositive = (n: number) => n > 0
let isEven = (n: number) => n % 2 === 0

let classifyNumber = (n: number) => match n {
  0 => "zero"
  x when isPositive(x) && isEven(x) => "positive even"
  x when isPositive(x) => "positive odd"
  x when isEven(x) => "negative even"
  _ => "negative odd"
}
```

## Best Practices

### 1. Exhaustive Patterns

```zenoscript
// Good - handle all cases
type Status = "pending" | "success" | "error"

let handleStatus = (status: Status) => match status {
  "pending" => showLoader()
  "success" => showSuccess()
  "error" => showError()
  // No need for _ case - all cases covered
}

// Compiler will warn if cases are missing
```

### 2. Order Patterns by Specificity

```zenoscript
// Good - specific patterns first
let processUser = (user: User) => match user {
  { role: "admin", active: true } => "Active admin"     // Most specific
  { role: "admin" } => "Admin user"                     // Less specific
  { active: true } => "Active user"                     // General
  _ => "User"                                           // Catch-all
}

// Avoid - general patterns first (unreachable code)
let processUserBad = (user: User) => match user {
  _ => "User"                                           // Catches everything!
  { role: "admin" } => "Admin user"                     // Never reached
}
```

### 3. Use Guards for Complex Conditions

```zenoscript
// Good - use guards for complex logic
let validateInput = (input: string) => match input {
  s when s.length > 0 && s.trim().length > 0 => "valid"
  s when s.length === 0 => "empty"
  _ => "whitespace only"
}

// Avoid - complex patterns without guards
let validateInputBad = (input: string) => match input {
  "" => "empty"
  // Hard to express "non-empty but only whitespace" without guards
}
```

### 4. Extract Complex Patterns

```zenoscript
// Good - extract complex patterns to functions
let isValidUser = (user: User) => 
  user.name.length > 0 && user.age >= 0 && user.email.includes("@")

let processUser = (user: User) => match user {
  u when isValidUser(u) => "Valid user"
  _ => "Invalid user"
}

// Avoid - inline complex conditions
let processUserBad = (user: User) => match user {
  { name, age, email } when name.length > 0 && age >= 0 && email.includes("@") => "Valid"
  _ => "Invalid"
}
```

Pattern matching in Zenoscript provides a powerful and expressive way to handle complex data structures and control flow while maintaining type safety and readability.