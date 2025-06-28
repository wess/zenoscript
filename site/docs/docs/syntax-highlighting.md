# Syntax Highlighting

Zenoscript supports syntax highlighting in various editors and documentation tools.

## VitePress (This Documentation)

This documentation site uses enhanced syntax highlighting for Zenoscript code blocks. Features include:

- **Atom highlighting** - `:symbol` syntax is highlighted in orange
- **Pipe operators** - `|>` operators are emphasized in purple
- **Zenoscript keywords** - `struct`, `trait`, `impl`, `match` are highlighted
- **Pattern matching** - Wildcard `_` patterns are highlighted

Example:

```zenoscript
struct User {
  name: string;
  status: symbol;
}

let user = { name: "Alice", status: :active }

let message = match user.status {
  :active => "User is active"
  :inactive => "User is inactive"
  _ => "Unknown status"
}

let result = users
  |> filter(u => u.status === :active)
  |> map(u => u.name)
```

## VS Code

The official Zenoscript VSCode extension provides comprehensive language support.

### Installation

#### From GitHub Releases (Recommended)

1. Download the latest `zenoscript-*.vsix` file from [GitHub releases](https://github.com/wess/zenoscript/releases)
2. Install via command line:
   ```bash
   code --install-extension zenoscript-*.vsix
   ```
3. Or through VSCode UI: Extensions → "..." → "Install from VSIX..."

#### From Marketplace (Coming Soon)

```bash
code --install-extension zenoscript.vscode-zenoscript
```

### Features

The VSCode extension includes:

- **Syntax Highlighting** - Complete highlighting for all Zenoscript constructs
- **Language Configuration** - Bracket matching, auto-closing pairs, comments
- **Document Formatting** - Basic code formatting and indentation
- **File Association** - Automatic `.zs` file recognition
- **Language Server Integration** - Error checking and IntelliSense (planned)

### Supported Syntax

The extension highlights:

- **Keywords**: `fn`, `let`, `const`, `struct`, `trait`, `impl`, `match`, `if`, `else`, etc.
- **Types**: `int`, `float`, `string`, `bool`, `void`, `any`, custom types
- **Operators**: `+`, `-`, `*`, `/`, `==`, `!=`, `|>`, `<|`, `->`, `=>`
- **Literals**: Numbers (decimal, hex, binary, octal), strings, booleans
- **Comments**: `//` line comments and `/* */` block comments
- **Functions**: Function names and calls
- **Symbols**: Atom syntax `:symbol` (planned)

## Other Editors

### Vim/Neovim

Add to your configuration:

```vim
au BufRead,BufNewFile *.zs set filetype=typescript
```

### Sublime Text

Use the TypeScript syntax as a base and add custom rules for Zenoscript-specific features.

### Emacs

Configure typescript-mode for `.zs` files:

```lisp
(add-to-list 'auto-mode-alist '("\\.zs\\'" . typescript-mode))
```

## Custom Themes

The Zenoscript language definition includes these token types:

- `atom` - For `:symbol` syntax
- `pipe-operator` - For `|>` operators  
- `pattern-wildcard` - For `_` in pattern matching
- `zenoscript-keyword` - For language-specific keywords

You can customize these in your editor's theme configuration.

## Contributing

Help improve Zenoscript syntax highlighting:

1. [VS Code Extension](https://github.com/wess/zenoscript/tree/main/vscode)
2. [Language Grammar](https://github.com/wess/zenoscript/tree/main/vscode/syntaxes)
3. [Main Repository](https://github.com/wess/zenoscript)

## Language Server

The Zenoscript Language Server provides advanced features:

- Real-time error checking
- Auto-completion
- Hover information
- Go to definition
- Refactoring tools

Install with:

```bash
bun install -g @zenoscript/language-server
```