# Changelog

All notable changes to Zenoscript will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.4] - 2024-12-30

### Added
- Enhanced optional parentheses for function calls with smart keyword detection
- Refined optional return statements for cleaner function definitions
- Biome linting integration and configuration
- Comprehensive test coverage for optional syntax features
- Better pattern recognition for edge cases and complex expressions

### Changed
- **BREAKING INTERNAL**: Replaced `eval()` usage in REPL and inline execution with safer temporary file execution
- Improved transpiler robustness and edge case handling
- Enhanced development tooling and error reporting
- Updated all documentation to reflect version 0.1.4

### Fixed
- Security vulnerabilities associated with `eval()` usage
- All style violations and unused parameter warnings
- Code consistency and maintainability issues

### Security
- Eliminated unsafe `eval()` calls from CLI tools
- Implemented secure temporary file execution model with automatic cleanup
- Enhanced security-conscious development practices

## [0.1.3] - 2024-12-29

### Added
- Core language feature implementations
- Pattern matching stabilization
- Enhanced transpiler test coverage

### Fixed
- Critical transpiler bugs and edge cases
- Type system improvements
- Performance optimizations

## [0.1.2] - 2024-12-27

### Added
- Enhanced VSCode extension with improved language support
- Better Bun integration documentation
- Improved installation scripts with multi-platform support

### Changed
- Updated documentation to prioritize Bun over Node.js/npm
- Improved CLI performance and error handling
- Enhanced REPL experience with better completion

### Fixed
- Fixed pattern matching edge cases
- Resolved type inference issues in certain scenarios
- Fixed plugin loading issues in development environments

## [0.1.1] - 2024-12-20

### Added
- Initial VSCode extension support
- Basic REPL functionality
- Core pattern matching features

### Changed
- Improved transpilation performance
- Better error messages

### Fixed
- Fixed critical compilation bugs
- Resolved dependency issues

## [0.1.0] - 2024-12-15

### Added
- Initial public release
- Basic language features
- TypeScript compilation target
- Bun plugin system
- Core CLI functionality

### Features
- Pattern matching with destructuring
- Pipe operator support
- Struct and trait system
- Atom (symbol) literals
- Function composition
- Type inference

## [0.0.2] - 2024-12-10

### Added
- Enhanced type system
- Better error reporting
- Initial documentation

### Fixed
- Performance improvements
- Bug fixes in parser

## [0.0.1] - 2024-12-05

### Added
- Initial development release
- Basic parser and transpiler
- Core language syntax
- Proof of concept implementation

---

For the complete list of changes, see the [commit history](https://github.com/wess/zenoscript/commits/main) on GitHub.