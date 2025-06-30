# Frequently Asked Questions

## General Questions

### What is Zenoscript?

Zenoscript is a fast, functional programming language that compiles to TypeScript. It combines the expressiveness of functional programming with the ecosystem and tooling of TypeScript/JavaScript.

### Why was Zenoscript created?

Zenoscript was created to bring functional programming concepts to the JavaScript ecosystem in a way that feels natural and integrates seamlessly with existing TypeScript codebases. It aims to improve developer productivity and code quality by providing powerful abstractions and a concise syntax.

### Is Zenoscript ready for production use?

Zenoscript is currently in early development (v0.1.5). While you can experiment with it and provide feedback, we don't recommend using it for production applications yet. Stay tuned for stable releases.

## Technical Questions

### How does Zenoscript compare to TypeScript?

Zenoscript compiles to TypeScript, so it inherits TypeScript's type system and ecosystem. However, Zenoscript adds:
- Pattern matching
- Pipeline operator
- Immutable data structures by default
- More concise syntax for functional programming
- First-class support for algebraic data types

### Can I use Zenoscript with my existing TypeScript/JavaScript codebase?

Yes! Zenoscript is designed to interoperate seamlessly with TypeScript and JavaScript. You can:
- Import TypeScript/JavaScript modules into Zenoscript files
- Import Zenoscript modules into TypeScript/JavaScript files
- Gradually adopt Zenoscript in parts of your codebase

### What runtime environments does Zenoscript support?

Zenoscript supports all environments where TypeScript/JavaScript runs:
- Bun (recommended)
- Browsers
- Deno
- Node.js (legacy support)
- And any other JavaScript runtime

### Does Zenoscript have its own package manager?

No, Zenoscript uses the existing Bun ecosystem for package management. You can use any package in your Zenoscript code.

## Language Features

### Does Zenoscript support object-oriented programming?

While Zenoscript emphasizes functional programming, it supports object-oriented concepts through its struct and trait system. However, the language encourages immutability and composition over inheritance.

### How does pattern matching work in Zenoscript?

Pattern matching in Zenoscript allows you to destructure and match against complex data structures. It's similar to pattern matching in languages like Rust, Haskell, or OCaml. See the [Pattern Matching](/docs/pattern-matching) documentation for details.

### Does Zenoscript support asynchronous programming?

Yes, Zenoscript fully supports async/await and Promises, just like TypeScript. It also provides functional utilities for working with asynchronous operations.

### How does error handling work in Zenoscript?

Zenoscript encourages using the Result type for error handling, similar to Rust or OCaml. This approach makes errors explicit in the type system and encourages proper error handling. See the [Advanced Examples](/examples/advanced) for patterns.

## Tooling and Ecosystem

### Is there an IDE plugin for Zenoscript?

We're working on IDE plugins for popular editors like VS Code. In the meantime, you can use the [Syntax Highlighting](/docs/syntax-highlighting) instructions to get basic support.

### How can I contribute to Zenoscript?

We welcome contributions! See our [Contributing Guide](/contributing) for details on how to get involved, whether through code contributions, documentation, or bug reports.

### Where can I get help with Zenoscript?

- Join our [Community Discord](https://discord.gg/zenoscript)
- Check the [documentation](/docs/)
- Ask questions on [GitHub Discussions](https://github.com/wess/zenoscript/discussions)

### Is there a REPL for Zenoscript?

Yes, you can use the Zenoscript REPL to experiment with the language. See the [REPL documentation](/docs/repl) for installation and usage instructions.