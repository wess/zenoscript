# Zenoscript VSCode Extension

Language support for Zenoscript in Visual Studio Code.

## Features

- Syntax highlighting for `.zs` files
- Basic language configuration (comments, brackets, auto-closing pairs)
- Document formatting support
- Language-aware editing features

## Installation

1. Open VSCode
2. Go to the Extensions view (Ctrl+Shift+X)
3. Search for "Zenoscript"
4. Install the extension

## Development

To build and test the extension locally:

```bash
cd vscode
bun install
bun run compile
```

Then press F5 to launch a new Extension Development Host with the extension loaded.

## Language Features

### Syntax Highlighting

The extension provides syntax highlighting for:
- Keywords: `fn`, `let`, `const`, `if`, `else`, `return`, etc.
- Comments: `//` and `/* */`
- Strings: single, double, and template literals
- Numbers: decimal, hex, binary, and octal
- Operators and punctuation

### File Association

Files with the `.zs` extension are automatically recognized as Zenoscript files.

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## License

This extension is part of the Zenoscript project.