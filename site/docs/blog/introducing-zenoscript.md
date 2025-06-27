# Introducing Zenoscript

*Published: 06/27/2025*

We're excited to introduce **Zenoscript**, a fast, functional programming language that compiles to TypeScript. Built with modern development workflows in mind, Zenoscript brings the power of functional programming to the TypeScript ecosystem while maintaining excellent performance and developer experience.

## What is Zenoscript?

Zenoscript is designed to be the functional programming language you've been waiting for. It combines:

- **Fast compilation** - Built for speed from the ground up
- **Functional-first approach** - Immutability, pattern matching, and pure functions by default  
- **TypeScript interoperability** - Seamlessly integrates with existing TypeScript codebases
- **Modern tooling** - First-class support for popular development tools

## Key Features

### Pattern Matching
Write expressive, safe code with powerful pattern matching:

```zenoscript
match value {
  Some(x) => x * 2,
  None => 0
}
```

### Immutable by Default
All data structures are immutable by default, helping you write more predictable code:

```zenoscript
let numbers = [1, 2, 3]
let doubled = numbers.map(x => x * 2)
// numbers is unchanged, doubled is [2, 4, 6]
```

### Type Safety
Leverage TypeScript's type system while enjoying Zenoscript's functional features:

```zenoscript
type User = {
  name: string,
  age: number
}

let createUser = (name: string, age: number): User => {
  { name, age }
}
```

## Getting Started

Getting started with Zenoscript is simple. Install it via your favorite package manager:

```bash
# Using bun (recommended)
bun install -g zenoscript

# Using npm
npm install -g zenoscript

# Using yarn
yarn global add zenoscript
```

Then create your first Zenoscript file:

```zenoscript
// hello.zs
let greet = (name: string) => `Hello, ${name}!`

console.log(greet("World"))
```

Compile and run it:

```bash
zenoscript compile hello.zs
bun hello.ts
```

## Why Zenoscript?

### Performance First
Zenoscript is built with performance in mind. The compiler is fast, and the generated TypeScript code is optimized for runtime performance.

### Developer Experience
We believe great tools make great developers. Zenoscript includes:
- Interactive REPL for quick experimentation
- Excellent error messages that help you fix issues quickly
- IDE support with syntax highlighting and autocompletion
- Seamless integration with existing TypeScript projects

### Functional Programming Done Right
Zenoscript brings functional programming concepts to TypeScript without the complexity. You get the benefits of immutability, pattern matching, and functional composition while still being able to leverage the vast TypeScript ecosystem.

## What's Next?

This is just the beginning. We have exciting plans for Zenoscript's future:

- Enhanced pattern matching capabilities
- More powerful type inference
- Better IDE integrations
- Expanded standard library
- Performance optimizations

## Get Involved

Zenoscript is open source and we'd love your contributions! Check out our [GitHub repository](https://github.com/zenoscript/zenoscript) to:

- Report bugs or request features
- Contribute code
- Improve documentation
- Share your Zenoscript projects

Join our community and help shape the future of functional programming on TypeScript!

---

Ready to try Zenoscript? Head over to our [Quick Start guide](/docs/quickstart) to get up and running in minutes.