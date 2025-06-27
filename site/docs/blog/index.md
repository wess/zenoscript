# Introducing Zenoscript

*Published: 06/27/2025*

**Zenoscript** is a functional programming language that compiles to TypeScript. It provides functional programming features while maintaining interoperability with existing TypeScript codebases.

## What is Zenoscript?

Zenoscript combines functional programming concepts with TypeScript's type system and ecosystem. It targets developers who want to use functional programming patterns without losing access to existing JavaScript libraries.

Key characteristics include:

- **Functional-first approach** - Immutability, pattern matching, and pure functions by default
- **Zero runtime overhead** - Compiles to clean TypeScript with no runtime dependencies  
- **TypeScript native** - Seamless integration with existing TypeScript projects
- **Modern tooling** - Built-in CLI, REPL, and VS Code extension

## Core Features

### Pattern Matching with Atoms
Pattern matching with symbolic constants:

```zenoscript
let handleResponse = match response {
  :success(data) => processData(data)
  :error(msg) => logError(msg)
  :loading => showSpinner()
  _ => defaultHandler()
}
```

### Pipe Operations
Chain operations using the pipe operator:

```zenoscript
let activeUsers = users
  |> filter(user => user.status === :active)
  |> map(user => user.name)
  |> sort
```

### Structural Types
Define data structures with structs and traits:

```zenoscript
struct User {
  id: number;
  name: string;
  email: string;
  status: UserStatus;
}

trait Serializable {
  serialize(): string;
  deserialize(data: string): Self;
}
```

### Immutable by Default
All bindings are immutable using `let`:

```zenoscript
let numbers = [1, 2, 3]
let doubled = numbers |> map(x => x * 2)
// numbers is unchanged, doubled is [2, 4, 6]
```

## Getting Started

### Installation

The fastest way to install Zenoscript is using our installation script:

```bash
# Unix/Linux/macOS
curl -fsSL https://zeno.run/install.sh | bash

# Windows PowerShell
iwr https://zeno.run/install.ps1 | iex
```

Alternatively, if you have Bun installed:

```bash
bun install -g zenoscript
```

### Your First Project

Create a new Zenoscript project:

```bash
# Create and initialize a new project
mkdir my-app && cd my-app
zeno init

# Start development
bun dev
```

This creates a simple project structure with hot reloading enabled.

### Basic Example

Here's an example of Zenoscript code:

```zenoscript
// user.zs
struct User {
  name: string;
  email: string;
  status: Symbol;
}

let users = [
  { name: "Alice", email: "alice@example.com", status: :active },
  { name: "Bob", email: "bob@example.com", status: :inactive }
]

let getActiveUsers = () => users
  |> filter(user => user.status === :active)
  |> map(user => user.name)

let statusMessage = (status) => match status {
  :active => "User is currently active"
  :inactive => "User is inactive"
  _ => "Unknown status"
}

console.log("Active users:", getActiveUsers())
```

This compiles to TypeScript that runs in any JavaScript environment.

## Why Zenoscript?

### Zero Runtime Overhead
Zenoscript compiles directly to TypeScript, meaning there's no runtime library or performance penalty. Your compiled code is as fast as hand-written TypeScript.

### Seamless TypeScript Integration
Import and use any TypeScript library directly in Zenoscript. The Bun plugin allows you to import `.zs` files directly into TypeScript projects:

```typescript
// Import Zenoscript files directly!
import { User, createUser } from "./types.zs";
import { processData } from "./handlers.zs";
```

### Developer Experience
- **Interactive REPL** for experimentation (`zeno repl`)
- **VS Code extension** with syntax highlighting and language support
- **Error messages** to help debug issues
- **Hot reloading** for development iteration

### Built for Bun
Zenoscript integrates with Bun, providing:
- Fast transpilation
- Built-in test runner
- Package management
- Web server capabilities

## What's Next?

Future development includes:

- Enhanced pattern matching with guards and nested destructuring
- More powerful type inference and compile-time optimizations
- Extended standard library with functional data structures
- Better IDE integrations and debugging tools
- Performance optimizations and WebAssembly targets

## Get Involved

Zenoscript is open source. You can contribute by:

- **GitHub**: [wess/zenoscript](https://github.com/wess/zenoscript) - Report bugs, request features, or contribute code
- **VS Code Extension**: Available for download from our releases page
- **Documentation**: Help us improve our guides and examples
- **Community**: Share your projects and get help

## Getting Started

To try Zenoscript:

1. **Install**: `curl -fsSL https://zeno.run/install.sh | bash`
2. **Create**: `zeno init my-project && cd my-project`
3. **Develop**: `bun dev`

Check out our [Quick Start Guide](/docs/quickstart) for a tutorial, or explore our [Examples](/examples/) to see more Zenoscript code.