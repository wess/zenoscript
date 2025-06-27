# CLI Reference

The Zenoscript CLI (`zeno`) provides everything you need to develop, build, and manage Zenoscript projects.

## Installation

```bash
# Install globally
bun install -g zenoscript

# Verify installation
zeno --version
```

## Commands

### `zeno init`

Initialize a new Zenoscript project with recommended structure and configuration.

```bash
zeno init my-project
```

**Options:**
- Creates `package.json` with Zenoscript dependencies
- Sets up `bunfig.toml` with plugin configuration
- Creates sample `index.zs` file
- Automatically runs `zeno setup`

**Example:**
```bash
zeno init my-web-app
cd my-web-app
bun dev
```

### `zeno setup`

Configure Zenoscript plugin for an existing project.

```bash
zeno setup
```

**What it does:**
- Updates `bunfig.toml` with Zenoscript plugin configuration
- Installs Zenoscript dependency if missing
- Validates Bun compatibility

**Manual bunfig.toml setup:**
```toml
preload = ["zenoscript/preload"]

[plugins]
zenoscript = "zenoscript/plugin"
```

### `zeno repl`

Start an interactive Zenoscript REPL for experimentation and learning.

```bash
zeno repl
```

**Features:**
- Real-time compilation and execution
- Syntax highlighting
- Error display
- Command history

**Example session:**
```
zs> let x = 42
→ const x = 42;

zs> match :success { :success => "It worked!" }
→ (() => {
  const __match_value = Symbol.for("success");
  if (__match_value === Symbol.for("success")) {
    return "It worked!";
  }
})()

zs> exit
Goodbye!
```

### `zeno <file>`

Transpile a Zenoscript file to TypeScript.

```bash
# Transpile to stdout
zeno input.zs

# Transpile to specific file
zeno input.zs output.ts

# Transpile multiple files
zeno src/*.zs
```

**Example:**
```bash
# Create a Zenoscript file
echo 'let greeting = "Hello, World!"' > hello.zs

# Transpile it
zeno hello.zs
# Output: const greeting = "Hello, World!";
```

## Global Options

### `--help, -h`

Show help information for any command.

```bash
zeno --help
zeno init --help
```

### `--version, -v`

Display the current Zenoscript version.

```bash
zeno --version
# Output: Zenoscript v0.0.1
```

### `--verbose, -V`

Enable verbose output for debugging.

```bash
zeno --verbose input.zs
```

### `--debug, -d`

Enable debug mode with additional diagnostics.

```bash
zeno --debug input.zs
```

### `--build`

Force rebuild of the transpiler from source.

```bash
zeno --build
```

## Integration with Bun

Zenoscript is designed to work seamlessly with Bun's build system.

### Development

```bash
# Hot reloading during development
bun --hot index.zs

# Run with Zenoscript files directly
bun run dev  # if configured in package.json
```

### Building

```bash
# Build for production
bun build index.zs --outdir dist

# Build with specific target
bun build index.zs --target browser --outdir public
```

### Project Scripts

Add these to your `package.json`:

```json
{
  "scripts": {
    "dev": "bun --hot index.zs",
    "build": "bun build index.zs --outdir dist",
    "start": "bun dist/index.js",
    "type-check": "bun tsc --noEmit"
  }
}
```

## Configuration Files

### `bunfig.toml`

Configure the Bun plugin for automatic `.zs` file handling:

```toml
# Enable Zenoscript plugin
preload = ["zenoscript/preload"]

[plugins]
zenoscript = "zenoscript/plugin"

# Optional: configure transpiler options
[zenoscript]
target = "es2022"
strict = true
```

### `tsconfig.json`

Zenoscript respects TypeScript configuration:

```json
{
  "compilerOptions": {
    "target": "ES2022",
    "module": "ESNext",
    "moduleResolution": "bundler",
    "strict": true,
    "types": ["bun-types"]
  },
  "include": ["**/*.ts", "**/*.zs"],
  "exclude": ["node_modules", "dist"]
}
```

## Troubleshooting

### Common Issues

**Command not found:**
```bash
# Check PATH includes Bun's bin directory
echo $PATH | grep -o '[^:]*bun[^:]*'

# Reinstall if needed
bun install -g zenoscript
```

**Plugin not working:**
```bash
# Verify Bun is being used
which bun

# Re-setup the plugin
zeno setup

# Check bunfig.toml configuration
cat bunfig.toml
```

**Transpilation errors:**
```bash
# Use verbose mode for details
zeno --verbose --debug problematic.zs

# Check for syntax errors
zeno --help  # shows available options
```

### Getting Help

- Use `--help` with any command for detailed usage
- Check [GitHub Issues](https://github.com/zenoscript/zenoscript/issues) for known problems
- Join our [Discord](https://discord.gg/zenoscript) for community support

## Environment Variables

### `ZENOSCRIPT_DEBUG`

Enable debug mode globally:

```bash
export ZENOSCRIPT_DEBUG=1
zeno input.zs  # Will show debug output
```

### `ZENOSCRIPT_TARGET`

Set default compilation target:

```bash
export ZENOSCRIPT_TARGET=es2020
zeno input.zs
```

## Shell Completion

### Bash

Add to your `.bashrc`:

```bash
eval "$(zeno completion bash)"
```

### Zsh

Add to your `.zshrc`:

```bash
eval "$(zeno completion zsh)"
```

### Fish

```fish
zeno completion fish | source
```