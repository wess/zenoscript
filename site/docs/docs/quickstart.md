# Quick Start

Get up and running with Zenoscript in just a few minutes! This guide will walk you through creating your first Zenoscript program.

## Create a New Project

Start by creating a new Zenoscript project:

```bash
mkdir hello-zenoscript && cd hello-zenoscript
zeno init
```

This creates a basic project with:

```zenoscript
// index.zs - Notice the clean syntax!
const greet = (name: string) => { `Hello, ${name}!` }

const main = () => {
  console.log greet("World")  // Optional parentheses
  greet("World") |> console.log  // Using pipe operator
}

main()
```

**Key features shown:**
- **Automatic returns**: Functions return their last expression
- **Optional parentheses**: Clean function calls without parentheses
- **Pipe operations**: Chain function calls elegantly

## Run Your Program

```bash
# Development mode with hot reloading
bun dev

# Or compile and run
bun build index.zs --outdir dist
bun dist/index.js
```

## Your First Zenoscript Program

Let's explore the basic features by building a simple user management system:

```zenoscript
// user.zs
struct User {
  id: number;
  name: string;
  email: string;
  status: UserStatus;
}

// Using atoms for type-safe constants
let :active = Symbol.for("active")
let :inactive = Symbol.for("inactive")
let :pending = Symbol.for("pending")

// Create some users
let users = [
  { id: 1, name: "Alice", email: "alice@example.com", status: :active },
  { id: 2, name: "Bob", email: "bob@example.com", status: :inactive },
  { id: 3, name: "Charlie", email: "charlie@example.com", status: :pending }
]

// Filter active users using pipe operations and optional parentheses
let activeUsers = users
  |> filter(user => user.status === :active)
  |> map(user => user.name)

console.log "Active users:", activeUsers  // Optional parentheses

// Pattern matching with automatic return
let getStatusMessage = (status) => match status {
  :active => "User is active"
  :inactive => "User is inactive"
  :pending => "User registration pending"
  _ => "Unknown status"
}

// Display status for each user - showcasing clean syntax
users |> forEach(user => {
  let message = getStatusMessage user.status  // Optional parentheses
  console.log `${user.name}: ${message}`     // Optional parentheses
})

// Functions with automatic return
let isActiveUser = (user) => { user.status === :active }
let getUserName = (user) => { user.name }
let formatUserInfo = (user) => {
  let status = getStatusMessage user.status
  let activity = isActiveUser(user) ? "✓" : "✗"
  `${activity} ${user.name} (${status})`  // Automatically returned
}
```

## Key Concepts Demonstrated

### 1. Optional Parentheses
Clean, readable function calls:

```zenoscript
console.log "Hello"        // vs console.log("Hello")
processValue 42            // vs processValue(42)
validateInput userData     // vs validateInput(userData)
```

### 2. Automatic Returns
Functions return their last expression:

```zenoscript
let add = (a, b) => { a + b }  // No return needed
let greet = (name) => {
  let message = `Hello, ${name}!`
  message  // Automatically returned
}
```

### 3. Structs
Define structured data types:

```zenoscript
struct User {
  id: number;
  name: string;
  email: string;
}
```

### 4. Atoms
Type-safe constants using the `:` syntax:

```zenoscript
let status = :active  // Creates Symbol.for("active")
```

### 3. Pipe Operations
Chain operations for readable data flow:

```zenoscript
let result = data
  |> filter(predicate)
  |> map(transform)
  |> sort
```

### 4. Pattern Matching
Powerful control flow with destructuring:

```zenoscript
let message = match status {
  :success => "It worked!"
  :error => "Something went wrong"
  _ => "Unknown result"
}
```

## Compiling to TypeScript

When you run your Zenoscript code, it compiles to clean TypeScript:

```typescript
// Compiled output
type User = {
  id: number;
  name: string;
  email: string;
  status: symbol;
};

const active = Symbol.for("active");
const inactive = Symbol.for("inactive");
const pending = Symbol.for("pending");

const users = [
  { id: 1, name: "Alice", email: "alice@example.com", status: active },
  { id: 2, name: "Bob", email: "bob@example.com", status: inactive },
  { id: 3, name: "Charlie", email: "charlie@example.com", status: pending }
];

const activeUsers = users
  .filter(user => user.status === active)
  .map(user => user.name);

// ... rest compiles similarly
```

## Working with Existing TypeScript

Zenoscript works seamlessly with existing TypeScript libraries:

```zenoscript
// Import any TypeScript/JavaScript library
import express from "express"
import { readFile } from "fs/promises"

// Use TypeScript types
let app = express()

// Zenoscript syntax with TypeScript interop
let handler = (req, res) => {
  let response = match req.method {
    "GET" => handleGet(req)
    "POST" => handlePost(req)
    _ => { status: 405, message: "Method not allowed" }
  }
  
  res.json(response)
}

app.use("/api", handler)
```

## Interactive Development

Use the REPL for experimentation:

```bash
zeno repl
```

```
zs> let x = 42
→ const x = 42;

zs> x |> double |> toString
→ x.double().toString()

zs> match :success { :success => "Yes!" }
→ (() => {
  const __match_value = Symbol.for("success");
  if (__match_value === Symbol.for("success")) {
    return "Yes!";
  }
})()
```

## Next Steps

Now that you've got the basics down:

- Learn about [Language Syntax](/docs/syntax) in detail
- Explore [Pattern Matching](/docs/pattern-matching) capabilities  
- Check out [Examples](/examples/) for real-world usage
- Set up your [development environment](/docs/cli) with the CLI tools

## Need Help?

- Check the [Language Guide](/docs/syntax) for detailed syntax reference
- Browse [Examples](/examples/) for inspiration
- Join our community on [Discord](https://discord.gg/zenoscript)