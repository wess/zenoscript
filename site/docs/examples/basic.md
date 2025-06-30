# Basic Examples

Learn the fundamentals of Zenoscript with these beginner-friendly examples. All examples shown here are fully implemented and working in Zenoscript 0.1.5.

## Hello World

The simplest Zenoscript program:

```zenoscript
let message = "Hello, Zenoscript!"
console.log message
```

**Compiles to:**
```typescript
const message = "Hello, Zenoscript!";
console.log(message);
```

## Variables and Data

```zenoscript
// Let bindings (immutable by default)
let name = "Alice"
let age = 30
let isActive = true
let score = 95.5

// Arrays and objects work as expected
let numbers = [1, 2, 3, 4, 5]
let names = ["Alice", "Bob", "Charlie"]

// Object literals
let user = {
  id: 1,
  name: "Alice",
  email: "alice@example.com"
}
```

## Optional Parentheses

Function calls can omit parentheses for cleaner syntax:

```zenoscript
// Traditional function calls
console.log("Hello")
myFunction("argument")

// Optional parentheses (Zenoscript style)
console.log "Hello"
myFunction "argument"
processValue 42
greet user.name
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

// Create objects that match the struct types
let person = {
  name: "Alice",
  age: 30,
  email: "alice@example.com"
}

let address = {
  street: "123 Main St",
  city: "New York",
  country: "USA"
}
```

**Compiles to:**
```typescript
type Person = {
  name: string;
  age: number;
  email: string;
};

type Address = {
  street: string;
  city: string;
  country: string;
};
```

## Traits (Interfaces)

Define behavior contracts:

```zenoscript
trait Serializable {
  serialize(): string;
  deserialize(data: string): any;
}

trait Displayable {
  display(): string;
}
```

**Compiles to:**
```typescript
interface Serializable {
  serialize(): string;
  deserialize(data: string): any;
}

interface Displayable {
  display(): string;
}
```

## Atoms

Type-safe symbolic constants:

```zenoscript
// Define atoms (compile to symbols)
let status = :pending
let result = :success
let color = :blue

// Use in conditionals
if status == :pending {
  console.log "Still processing..."
}

if result == :success {
  console.log "Operation completed!"
}
```

**Compiles to:**
```typescript
const status = Symbol.for("pending");
const result = Symbol.for("success");
const color = Symbol.for("blue");

if (status == Symbol.for("pending")) {
  console.log("Still processing...");
}

if (result == Symbol.for("success")) {
  console.log("Operation completed!");
}
```

## Pattern Matching

Pattern matching for control flow:

```zenoscript
// Simple pattern matching
let handleStatus = match :loading {
  :idle => "Ready to start"
  :loading => "Please wait..."
  :success => "Operation completed!"
  :error => "Something went wrong"
  _ => "Unknown status"
}

// Pattern matching in variables
let status = :error
let message = match status {
  :pending => "Processing..."
  :success => "Done!"
  :error => "Failed!"
  _ => "Unknown"
}
```

**Compiles to:**
```typescript
const handleStatus = (() => {
  const __match_value = Symbol.for("loading");
  if (__match_value === Symbol.for("idle")) {
    return "Ready to start";
  } else if (__match_value === Symbol.for("loading")) {
    return "Please wait...";
  } else if (__match_value === Symbol.for("success")) {
    return "Operation completed!";
  } else if (__match_value === Symbol.for("error")) {
    return "Something went wrong";
  } else {
    return "Unknown status";
  }
})()
```

## Pipe Operations

Chain operations for readable data transformation:

```zenoscript
// String processing with pipes
let text = "  hello world  "
let processed = text |> trim |> toUpperCase

console.log processed // "HELLO WORLD"

// Chain multiple transformations
let greeting = "zenoscript" |> toUpperCase |> console.log

// Data processing
let data = [1, 2, 3]
let result = data |> toString |> console.log
```

**Compiles to:**
```typescript
const text = "  hello world  ";
const processed = text.trim().toUpperCase();

console.log(processed);

const greeting = console.log("zenoscript".toUpperCase());

const data = [1, 2, 3];
const result = console.log(data.toString());
```

## Simplified If Statements

Zenoscript allows omitting parentheses around if conditions:

```zenoscript
let age = 25

// Traditional if statement
if (age >= 18) {
  console.log("Adult")
}

// Simplified if (Zenoscript style)
if age >= 18 {
  console.log "Adult"
}

// With atoms
let status = :active
if status == :active {
  console.log "User is active"
}
```

**Compiles to:**
```typescript
const age = 25;

if (age >= 18) {
  console.log("Adult");
}

if (age >= 18) {
  console.log("Adult");
}

const status = Symbol.for("active");
if (status == Symbol.for("active")) {
  console.log("User is active");
}
```

## Working with Objects

```zenoscript
struct User {
  id: number;
  name: string;
  age: number;
  active: boolean;
}

// Create user objects
let user1 = { id: 1, name: "Alice", age: 30, active: true }
let user2 = { id: 2, name: "Bob", age: 25, active: false }

// Process user data
let userName = user1.name |> toUpperCase |> console.log
let userId = user1.id |> toString |> console.log

// Conditional processing
if user1.active {
  console.log "User is active"
} 

if user2.age >= 21 {
  console.log "User is old enough"
}
```

## Combining Features

Real-world example combining multiple Zenoscript features:

```zenoscript
struct Product {
  id: number;
  name: string;
  price: number;
  category: string;
}

// Sample data
let product = { 
  id: 1, 
  name: "laptop", 
  price: 999.99, 
  category: "electronics" 
}

// Process product information
let productInfo = product.name |> toUpperCase
console.log productInfo

// Check category
let categoryStatus = match product.category {
  "electronics" => :tech
  "books" => :education  
  "clothing" => :fashion
  _ => :other
}

// Price evaluation
if product.price > 500 {
  console.log "Premium product"
}

// Status messages
let statusMessage = match categoryStatus {
  :tech => "High-tech item"
  :education => "Educational material"
  :fashion => "Fashion item"
  _ => "General product"
}

console.log statusMessage
```

## Next Steps

Now that you understand the basics, explore:

- [Advanced Examples](/examples/advanced) - Complex patterns and state management
- [Language Guide](/docs/syntax) - Complete syntax reference
- [REPL](/docs/repl) - Try examples interactively

All the examples above are fully functional in Zenoscript 0.1.5!