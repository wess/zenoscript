# CLI Reference

The Zenoscript CLI (`zeno`) provides everything you need to develop, build, and manage Zenoscript projects. Built with Bun and TypeScript, it offers fast transpilation and seamless project management.

## Installation

```bash
# Install globally with Bun (recommended)
bun install -g zenoscript

# Or use our install script
curl -fsSL https://zenoscript.dev/install.sh | sh

# Verify installation
zeno --version
```

## Commands

### `zeno init`

Initialize a new Zenoscript project with recommended structure and configuration.

```bash
zeno init
```

**What it creates:**
- `package.json` with Zenoscript dependencies and scripts
- `bunfig.toml` with plugin configuration  
- Sample `index.zs` file with example code
- Automatically runs `zeno setup`

**Example:**
```bash
mkdir my-web-app && cd my-web-app
zeno init
bun install
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
preload = ["zenoscript/plugin"]

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
- TypeScript output preview
- Error display with helpful messages
- Command history
- Built-in TypeScript transpiler

**Example session:**
```
zs> let x = 42
→ const x = 42;
42

zs> match :success { :success => "It worked!", _ => "Failed" }
→ (() => {
  const __match_value = Symbol.for("success");
  if (__match_value === Symbol.for("success")) {
    return "It worked!";
  } else {
    return "Failed";
  }
})()
"It worked!"

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

# With options
zeno --verbose input.zs
zeno --debug input.zs
```

**Example:**
```bash
# Create a Zenoscript file
echo 'let greeting = "Hello, World!"' > hello.zs

# Transpile it
zeno hello.zs
# Output: const greeting = "Hello, World!";

# With TypeScript output
zeno hello.zs hello.ts
# Creates hello.ts with transpiled code
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
# Output: Zenoscript v0.1.3
```

### `--verbose, -V`

Enable verbose output for debugging.

```bash
zeno --verbose input.zs
```

### `--debug, -d`

Enable debug mode with additional diagnostics and AST output.

```bash
zeno --debug input.zs
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
preload = ["zenoscript/plugin"]

[plugins]
zenoscript = "zenoscript/plugin"
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
  "exclude": ["node_modules", "dist"]  # Standard excludes
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
- Check [GitHub Issues](https://github.com/wess/zenoscript/issues) for known problems
- Join our [Discord](https://discord.gg/zenoscript) for community support

## Performance

The Zenoscript CLI is built with Bun and TypeScript for optimal performance:

- **Fast startup**: Sub-100ms command execution
- **Efficient transpilation**: Direct TypeScript generation without intermediate steps  
- **Built-in caching**: Automatic dependency resolution
- **Cross-platform**: Runs natively on Linux, macOS, and Windows

## Distribution

Zenoscript CLI is distributed as:

- **Bun packages**: `zenoscript` package for global installation
- **Standalone binaries**: Platform-specific executables requiring no runtime
- **Source builds**: Direct compilation from TypeScript source