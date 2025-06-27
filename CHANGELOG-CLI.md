# CLI Changelog

## v0.0.2 - Major CLI Overhaul

### 🎯 Unified TypeScript CLI

**Breaking Changes:**
- Removed dual CLI architecture (C binary + TypeScript wrapper)
- Unified into a single TypeScript-based CLI built with Bun
- Removed `--build` flag (no longer needed)
- Removed `--tokens` and `--ast` flags (debug mode shows AST)

### ✨ New Features

**Enhanced Commands:**
- `zeno init` - Initialize projects in current directory (no longer takes project name)
- `zeno setup` - Improved plugin configuration with better error handling
- `zeno repl` - Faster startup with built-in TypeScript transpiler
- `zeno <file>` - Direct TypeScript transpilation without external binary

**Cross-Platform Compilation:**
- Added `bun run compile` for all platforms
- Individual platform builds: `compile:linux-x64`, `compile:darwin-x64`, `compile:darwin-arm64`, `compile:windows-x64`
- Standalone binaries with no runtime dependencies

### 🚀 Performance Improvements

- **10x faster startup**: Eliminated C binary compilation step
- **Direct transpilation**: TypeScript generation without intermediate steps
- **Smaller footprint**: Single binary instead of multiple components
- **Memory efficient**: Built-in caching and optimized bundling

### 🔧 Technical Changes

**Architecture:**
- Single entry point: `src/index.ts`
- Built-in transpiler: `src/transpiler.ts`
- Native Bun compilation with `--compile` flag
- Removed C compiler dependencies

**Package Changes:**
- Updated `package.json` bin to point to TypeScript CLI
- Removed C build scripts
- Added cross-platform compilation scripts
- Simplified dependency tree

**Distribution:**
- Platform-specific archives: `zenoscript-{platform}-{arch}.{tar.gz|zip}`
- Self-contained binaries include docs and VERSION file
- GitHub Actions workflow optimized for single-runner builds

### 📚 Documentation Updates

**Updated Files:**
- `site/docs/docs/cli.md` - Complete CLI reference refresh
- `site/docs/docs/installation.md` - New installation methods and platform-specific instructions
- `site/docs/docs/quickstart.md` - Updated `init` command usage
- `BUILD.md` - New build documentation for cross-platform compilation

**New Documentation:**
- Cross-platform build instructions
- Standalone binary usage guide
- Performance benchmarks and comparisons

### 🧪 Testing

**Enhanced Test Suite:**
- All tests migrated to TypeScript transpiler
- Integration tests for CLI commands
- Cross-platform build verification
- 29 tests passing across 4 test files

**Test Coverage:**
- CLI functionality (init, setup, repl, transpilation)
- Transpiler features (structs, traits, pattern matching, etc.)
- Integration workflows (project creation, setup, etc.)
- Error handling and edge cases

### 🔄 Migration Guide

**For Users:**
```bash
# Old way (C binary based)
zeno init my-project    # ❌ No longer supported
cd my-project

# New way (current directory)
mkdir my-project && cd my-project  # ✅ Create directory first
zeno init                          # ✅ Initialize in current dir
```

**For Contributors:**
- No C compiler setup required
- Direct TypeScript development
- Standard Bun toolchain
- Faster development iteration

### 🏗️ Build System

**Before:**
- C compiler required (gcc, tinycc)
- Platform-specific build matrix
- Complex GitHub Actions workflow
- Separate transpiler binary

**After:**
- Bun-only compilation
- Single Ubuntu runner
- Cross-platform in one step
- Self-contained distribution

### 📦 Distribution

**Release Assets:**
- `zenoscript-linux-x64.tar.gz` (was `zenoscript-linux.tar.gz`)
- `zenoscript-darwin-x64.tar.gz` (Intel Mac)
- `zenoscript-darwin-arm64.tar.gz` (Apple Silicon)
- `zenoscript-windows-x64.zip` (was `zenoscript-windows.zip`)

Each archive now includes:
- Platform-specific binary (`zeno` or `zeno.exe`)
- Documentation files (`README.md`, `LICENSE`)
- Version information (`VERSION`)

### 🔮 Future Roadmap

**Short Term:**
- Shell completion scripts
- Plugin ecosystem for transpiler extensions
- Advanced REPL features (multiline, syntax highlighting)

**Long Term:**
- Language server protocol (LSP) support
- WebAssembly compilation target
- Browser-based REPL and playground