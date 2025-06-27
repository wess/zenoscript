# Zenoscript

A functional programming language that compiles to TypeScript, featuring Rust-inspired syntax, Elixir-style pattern matching, and seamless TypeScript interoperability.

## Features

- **Functional Programming**: Immutable by default with `let` bindings
- **Pipe Operations**: Data transformation with `|>` operator
- **Pattern Matching**: Powerful `match` expressions with guards
- **Atoms**: Symbolic constants like Elixir (`:ok`, `:error`)
- **Struct & Trait System**: Similar to Rust's type system
- **Zero Runtime Overhead**: Compiles to clean TypeScript
- **Mixed Syntax**: Use alongside existing TypeScript/JavaScript

## Installation

```bash
bun install
bun run build
```

## Usage

### CLI

```bash
# Transpile a file
zeno input.zs output.ts

# Output to stdout
zeno input.zs

# Show help
zeno --help

# Show tokens (debugging)
zeno --tokens input.zs

# Show AST (debugging)
zeno --ast input.zs
```

### Programmatic

```bash
# Using bun directly
bun src/index.ts input.zs output.ts
```

### Bun Plugin Integration

Zenoscript includes a Bun plugin for seamless `.zs` file integration:

```typescript
// Import .zs files directly in TypeScript!
import { User, createUser } from "./types.zs";
import { processData } from "./handlers.zs";

// The .zs files are automatically transpiled to TypeScript
const user = createUser("Alice", "alice@example.com", 25);
```

The plugin is automatically loaded via `bunfig.toml`. Just run:

```bash
bun run examples/usage.ts
```

## Language Syntax

### Struct Declarations

```zenoscript
struct User {
  name: string;
  email: string;
  age: number;
}

struct Container<T> {
  value: T;
}
```

Compiles to:
```typescript
type User = {
  name: string;
  email: string;
  age: number;
};

type Container<T> = {
  value: T;
};
```

### Trait Declarations

```zenoscript
trait Serializable {
  serialize(): string;
  deserialize(data: string): Self;
}
```

Compiles to:
```typescript
interface Serializable {
  serialize(): string;
  deserialize(data: string): Self;
}
```

### Implementation Blocks

```zenoscript
impl User {
  new(name: string, email: string, age: number) {
    return { name, email, age };
  }
}

impl Serializable for User {
  serialize() {
    return JSON.stringify(this);
  }
}
```

### Let Bindings

```zenoscript
let message = "Hello World"
let count = 42
```

Compiles to:
```typescript
const message = "Hello World";
const count = 42;
```

### Pipe Expressions

```zenoscript
"  hello  " |> trim |> toUpperCase |> console.log
data |> processData |> validateResult |> saveToDatabase
```

Compiles to:
```typescript
console.log("  hello  ".trim().toUpperCase());
saveToDatabase(validateResult(processData(data)));
```

### Atoms and Pattern Matching

```zenoscript
let status = :loading

let message = match status {
  :idle => "Ready to start"
  :loading => "Please wait..."
  :success => "Operation completed"
  :error when isRetryable => "Retrying..."
  :error => "Failed permanently"
  _ => "Unknown status"
}
```

## Examples

See the `examples/` directory for complete examples:
- `examples/basic.zs` - Basic language features
- `examples/advanced.zs` - Advanced functional programming patterns

## Development

### Building the Transpiler

The transpiler is written in C and compiled to `build/zeno`:

```bash
bun run build
# or manually:
cd src/transpiler && make
```

### Running Tests

```bash
bun test
```

### Scripts

```bash
bun run build    # Build the C transpiler
bun run clean    # Clean build artifacts
bun run test     # Run test suite
bun run dev      # Development mode with hot reload
```

## Architecture

The transpiler follows a traditional compilation pipeline:

1. **Lexer** (`lexer.c`) - Tokenizes Zenoscript source code
2. **Parser** (`parser.c`) - Builds Abstract Syntax Tree (AST)
3. **Code Generator** (`codegen.c`) - Emits TypeScript code
4. **CLI** (`cli.c`) - Command-line interface

The Bun wrapper (`src/index.ts`) provides a seamless interface that automatically builds and runs the C transpiler.

## Language Philosophy

- **Mixed syntax support**: Seamless integration with TypeScript/JavaScript
- **Functional patterns**: Inspired by Rust, Elixir, and functional programming
- **Type safety**: Leverages TypeScript's type system
- **Immutability by default**: Uses `let` for immutable bindings
- **Zero runtime overhead**: Compiles to native TypeScript constructs

## Contributing

1. Fork the repository
2. Create your feature branch
3. Add tests for new functionality
4. Ensure all tests pass: `bun test`
5. Submit a pull request

## License

MIT License - see LICENSE file for details.
