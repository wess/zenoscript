# Introduction

Zenoscript is a modern functional programming language that compiles to TypeScript. It brings the expressiveness of functional programming to the JavaScript ecosystem while maintaining full interoperability with existing TypeScript codebases.

## What is Zenoscript?

Zenoscript is designed to make functional programming accessible and practical for everyday development. It combines:

- **Pattern Matching** - Powerful destructuring and control flow
- **Immutable Data** - Built-in immutable data structures
- **Type Safety** - Strong static typing with inference
- **Zero Runtime** - Compiles to clean TypeScript with no runtime dependencies

## Key Features

### Optional Parentheses

Clean, readable function calls inspired by functional languages:

```zenoscript
// Function calls without parentheses
console.log "Hello, World!"
processValue 42
validateInput userData

// Compiles to clean TypeScript
console.log("Hello, World!");
processValue(42);
validateInput(userData);
```

### Optional Return Statements

Functions automatically return their last expression:

```zenoscript
// Concise function definitions
let add = (a, b) => { a + b }
let multiply = (x, y) => { x * y }

// Multi-statement functions
let processUser = (user) => {
  let validated = validateUser(user)
  let normalized = normalizeData(validated)
  normalized  // Automatically returned
}
```

### Pattern Matching

```zenoscript
let handleResponse = match response {
  :success(data) => processData(data)
  :error(msg) => logError(msg)
  :loading => showSpinner()
  _ => defaultHandler()
}
```

### Pipe Operations

```zenoscript
let result = data
  |> filter(isValid)
  |> map(transform)
  |> reduce(combine, initialValue)
```

### Structural Types

```zenoscript
struct User {
  name: string;
  email: string;
  age: number;
}

trait Serializable {
  serialize(): string;
}
```

### Atoms for Constants

```zenoscript
let status = :pending
let color = :red
let direction = :north
```

## Philosophy

Zenoscript follows these core principles:

1. **Functional First** - Immutability and pure functions by default
2. **TypeScript Native** - Seamless integration with existing TS projects
3. **Zero Runtime** - No runtime dependencies or performance overhead
4. **Developer Joy** - Great tooling and error messages

## Getting Started

Ready to dive in? Check out the [Installation Guide](/docs/installation) to get Zenoscript set up on your machine.

::: tip
If you're new to functional programming, don't worry! Zenoscript is designed to be approachable while still being powerful.
:::