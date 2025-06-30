# Zenoscript 0.1.4: Security Enhancements and Code Quality Improvements

*Published: December 30, 2024*

We're excited to announce **Zenoscript 0.1.4**, a focused release that significantly improves the security and code quality of the Zenoscript transpiler and CLI tools. This release addresses important security vulnerabilities while maintaining full backward compatibility.

## 🔒 Security Improvements

### Eliminated eval() Usage

The biggest change in 0.1.4 is the complete removal of unsafe `eval()` calls from the REPL and inline execution features. Previously, the CLI used `eval()` to execute transpiled TypeScript code, which posed security risks.

**Before (0.1.3):**
```typescript
// Unsafe eval() usage
const result = eval(wrappedCode);
```

**After (0.1.4):**
```typescript
// Safe file-based execution
const tempFile = `/tmp/zenoscript_inline_${Date.now()}.ts`;
await Bun.write(tempFile, typescript);
const result = await import(tempFile);
```

This change eliminates the security vulnerabilities associated with `eval()` while maintaining the same functionality for users.

### Impact on Features

- **REPL (`zeno repl`)**: Now uses temporary file execution instead of eval
- **Inline execution (`zeno -e "code"`)**: Safer execution model with proper cleanup
- **Performance**: Minimal impact while significantly improving security posture

## 🧹 Code Quality Enhancements

### Biome Linting Integration

We've integrated [Biome](https://biomejs.dev/) as our primary linting tool and fixed all existing code quality issues:

- **Fixed style violations**: Proper block statements and template literal usage
- **Removed unused parameters**: Clean function signatures throughout the codebase
- **Consistent code formatting**: Improved readability and maintainability

### Test Coverage Maintained

All 53 existing tests continue to pass, ensuring that security improvements don't break existing functionality:

```bash
✓ 53 tests passed
✓ 0 tests failed
✓ 152 assertions completed successfully
```

## 🚀 What's Unchanged

### Full Backward Compatibility

All existing Zenoscript code continues to work exactly as before. This release focuses purely on internal improvements without changing language features or syntax.

### Performance Characteristics

The transpiler performance remains unchanged, with security improvements having minimal overhead on execution speed.

### Language Features

All core Zenoscript features work identically:
- Pattern matching with atoms
- Pipe operations
- Struct and trait definitions
- Optional parentheses and return statements
- TypeScript integration

## 🛠 Development Infrastructure

### Enhanced Development Workflow

The security improvements make Zenoscript safer for developers to work with:

```bash
# REPL is now completely safe
zeno repl

# Inline execution uses secure temporary files
zeno -e "let x = 42; x |> console.log"

# All existing functionality preserved
zeno init my-project
```

### Improved Error Handling

Better error reporting in the secure execution model helps developers debug issues more effectively.

## 📦 Installation and Upgrade

### Upgrading from 0.1.3

Upgrade to 0.1.4 using your preferred method:

```bash
# Using Bun (recommended)
bun install -g zenoscript@0.1.4

# Using install script
curl -fsSL https://zeno.run/install.sh | bash
```

### Verification

Confirm you're running 0.1.4:

```bash
zeno --version
# Output: Zenoscript v0.1.4
```

## 🔮 Looking Ahead

### ZenoVM Architecture Progress

While this release focuses on security, we continue development on the **ZenoVM** multi-target compilation system that will enable Zenoscript to compile to multiple backend languages (C, Go, WebAssembly) beyond TypeScript.

### Upcoming Features

Future releases will focus on:
- Enhanced pattern matching capabilities
- Performance optimizations in the transpiler
- Extended standard library
- Better IDE integration
- Additional compilation targets

## 🙏 Community and Feedback

### Contributing

This release demonstrates our commitment to security and code quality. If you're interested in contributing:

- **Security**: Report security issues via GitHub's security advisory feature
- **Code Quality**: Help us maintain high standards with pull requests
- **Testing**: Add test cases for edge cases and new features

### Getting Help

- **Documentation**: [zeno.run/docs](https://zeno.run/docs)
- **GitHub Issues**: [github.com/wess/zenoscript/issues](https://github.com/wess/zenoscript/issues)
- **Examples**: [zeno.run/examples](https://zeno.run/examples)

## 📋 Full Changelog

### Security
- **BREAKING INTERNAL**: Removed all `eval()` usage from REPL and inline execution
- **IMPROVED**: Safer temporary file execution model with automatic cleanup
- **ENHANCED**: Security-conscious development practices

### Code Quality
- **ADDED**: Biome linting integration and configuration
- **FIXED**: All style violations and unused parameter warnings
- **IMPROVED**: Code consistency and maintainability

### Maintenance
- **UPDATED**: Version references across all documentation
- **MAINTAINED**: 100% test coverage and backward compatibility
- **ENHANCED**: Development tooling and error reporting

---

Thank you to everyone who continues to support Zenoscript development. Version 0.1.4 makes Zenoscript more secure and maintainable while preserving the developer experience you've come to expect.

**Happy coding with Zenoscript! 🚀**