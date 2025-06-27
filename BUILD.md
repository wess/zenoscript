# Building Zenoscript

Zenoscript can be compiled into standalone binaries for multiple platforms using Bun's built-in compilation feature.

## Build Scripts

### Individual Platform Builds

```bash
# Linux x64
bun run compile:linux-x64

# macOS x64 (Intel)
bun run compile:darwin-x64

# macOS ARM64 (Apple Silicon)
bun run compile:darwin-arm64

# Windows x64
bun run compile:windows-x64
```

### Build All Platforms

```bash
# Compile for all supported platforms
bun run compile
```

### Quick Build (Current Platform)

```bash
# Build for the current platform (darwin-arm64)
bun run build
```

## Output

Compiled binaries are placed in:
- `build/linux-x64/zeno`
- `build/darwin-x64/zeno`
- `build/darwin-arm64/zeno`
- `build/windows-x64/zeno.exe`

## Testing Compiled Binaries

```bash
# Test version
./build/darwin-arm64/zeno --version

# Test help
./build/darwin-arm64/zeno --help

# Test transpilation
echo 'let greeting = "Hello World"' | ./build/darwin-arm64/zeno /dev/stdin
```

## Clean Build Directory

```bash
bun run clean
```

## Distribution

The compiled binaries are self-contained and can be distributed without requiring Bun or Node.js to be installed on the target system. Each binary includes:

- The Zenoscript CLI
- The TypeScript transpiler
- All dependencies

Simply copy the appropriate binary for your target platform and run it directly.