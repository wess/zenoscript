# Zenoscript 0.1.5: Critical Fixes and Enhanced Reliability

*Published: June 30, 2025*

We're excited to announce Zenoscript 0.1.5, a critical stability release that addresses several important issues with the transpiler and significantly improves the overall developer experience.

## 🔧 Critical Fixes

### Pipe Expression Overhaul
The biggest improvement in 0.1.5 is a complete rewrite of how pipe expressions are handled. Previously, complex pipe chains could produce malformed TypeScript output. Now they generate clean, readable code:

```zenoscript
// Before (0.1.4) - Broken output
"hello world" |> trim |> toUpperCase |> console.log
// Generated: ().trim().toUpperCase()

// After (0.1.5) - Clean output  
"hello world" |> trim |> toUpperCase |> console.log
// Generated: console.log("hello world".trim().toUpperCase())
```

### Comment Preservation
Optional parentheses were incorrectly transforming content inside comments, breaking code documentation. This has been completely fixed:

```zenoscript
// This comment used to get mangled: example code
// Now stays perfectly intact!
let result = myFunction "argument"
```

### String Processing Improvements
Fixed issues where string arguments in function calls could be corrupted during the transformation pipeline, especially in complex expressions.

## 🧹 Example Cleanup

We've updated all examples to only showcase **implemented** language features. Removed experimental syntax like:
- `impl` blocks (not yet implemented)
- Pattern matching guards with `when` clauses
- Generic bounds with `extends`

This ensures that every example you see will actually work when you try it!

## 📊 Enhanced Testing

Added comprehensive test coverage for:
- Complex pipe chains with multiple method calls
- Nested match expressions
- Comment preservation across all transformation types
- Edge cases in string and argument processing

## ⚡ Performance & Reliability

The transpiler transformation pipeline has been reorganized for better reliability and performance. Each transformation phase now has clear responsibilities and better isolation from others.

## 🎯 What This Means for You

If you've been experiencing issues with:
- Pipe expressions not working correctly
- Comments getting mangled in your code
- Examples from our documentation not working

**Upgrade to 0.1.5 immediately** - these are all fixed!

## 🔮 Looking Ahead

With 0.1.5's stability improvements, we're now positioned to work on exciting new features for future releases:
- Implementation blocks (`impl`) for methods on structs
- Pattern matching guards with `when` clauses  
- Enhanced error reporting and debugging tools
- Performance optimizations

## 📦 Get 0.1.5

```bash
# Install or upgrade
bun add zenoscript@latest

# Or use the install script
curl -fsSL https://zenoscript.dev/install.sh | bash
```

## 🙏 Thank You

Thanks to everyone who reported issues and helped identify these critical problems. Zenoscript is getting more stable and reliable with each release!

---

*Have feedback or found a bug? Open an issue on [GitHub](https://github.com/wess/zenoscript) or join our community discussions.*