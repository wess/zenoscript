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

Install the Zenoscript extension for full language support:

```bash
code --install-extension zenoscript.vscode-zenoscript
```

Features:
- Syntax highlighting
- Error checking
- Auto-completion
- Code snippets
- Go to definition

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

```elisp
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

1. [VS Code Extension](https://github.com/zenoscript/vscode-zenoscript)
2. [Language Grammar](https://github.com/zenoscript/zenoscript-tmgrammar)
3. [Editor Plugins](https://github.com/zenoscript/editor-plugins)

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