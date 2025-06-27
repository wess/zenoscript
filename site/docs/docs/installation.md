# Installation

Zenoscript can be installed globally using Bun or npm, and works seamlessly with existing TypeScript projects.

## Prerequisites

- [Bun](https://bun.sh) (recommended) or Node.js 16+
- Basic familiarity with command line

## Install Zenoscript

### Quick Install (Recommended)

The fastest way to install Zenoscript is using our installation scripts:

<InstallTabs />

These scripts will:
- Detect your system and architecture automatically
- Try Bun installation first (if available) for fastest setup
- Fall back to GitHub releases for manual installation
- Download and extract the appropriate binary for your platform
- Set up your PATH environment variable automatically
- Update your shell configuration (`.bashrc`, `.zshrc`, etc.)
- Verify the installation works correctly
- Provide next steps and helpful links

**Features:**
- ✅ **Smart detection** - Automatically detects macOS, Linux, Windows
- ✅ **Multi-architecture** - Supports x64 and ARM64
- ✅ **Bun integration** - Uses Bun package manager when available
- ✅ **Safe installation** - Creates user-local installation (no sudo required)
- ✅ **Shell integration** - Updates your shell's PATH automatically
- ✅ **Error handling** - Provides clear error messages and troubleshooting

### Alternative Installation Methods

#### Using Bun

```bash
bun install -g zenoscript
```

#### Using npm

```bash
npm install -g zenoscript
```

#### Manual Installation

Download the appropriate binary for your platform from the [GitHub releases page](https://github.com/zenoscript/zenoscript/releases):

- **Linux x64**: `zenoscript-linux-x64.tar.gz`
- **macOS x64 (Intel)**: `zenoscript-darwin-x64.tar.gz`
- **macOS ARM64 (Apple Silicon)**: `zenoscript-darwin-arm64.tar.gz`
- **Windows x64**: `zenoscript-windows-x64.zip`

Extract and add the binary to your PATH:

```bash
# Linux/macOS
tar -xzf zenoscript-*.tar.gz
sudo mv */zeno /usr/local/bin/

# Windows
# Extract zip and add zeno.exe to your PATH
```

## Verify Installation

Check that Zenoscript is installed correctly:

```bash
zeno --version
```

You should see output like:

```
Zenoscript v0.0.2
```

## Create Your First Project

Use the `init` command to create a new Zenoscript project:

```bash
# Create a new project directory
mkdir my-zenoscript-app && cd my-zenoscript-app

# Initialize the project
zeno init

# Install dependencies
bun install

# Start development server
bun dev
```

This creates a project structure like:

```
my-zenoscript-app/
├── package.json
├── bunfig.toml
├── index.zs
└── README.md
```

## IDE Setup

### VS Code Extension

Zenoscript provides a dedicated VSCode extension with full language support. You can install it in several ways:

#### Method 1: From GitHub Releases (Recommended)

1. Go to the [GitHub releases page](https://github.com/wess/zenocode/releases)
2. Download the latest `zenoscript-*.vsix` file
3. Install using VSCode command line:
   ```bash
   code --install-extension zenoscript-*.vsix
   ```
4. Or install through VSCode UI:
   - Open VSCode
   - Go to Extensions (Ctrl+Shift+X)
   - Click the "..." menu → "Install from VSIX..."
   - Select the downloaded `.vsix` file

#### Method 2: VSCode Marketplace (Coming Soon)

```bash
code --install-extension zenoscript.vscode-zenoscript
```

#### Extension Features

The VSCode extension provides:

- **Syntax Highlighting** - Full syntax highlighting for `.zs` files
- **Language Configuration** - Bracket matching, auto-closing pairs, comments
- **Document Formatting** - Basic code formatting support
- **File Association** - Automatic recognition of `.zs` files
- **Error Detection** - Integration with Zenoscript transpiler

#### Configuration

Once installed, the extension automatically:
- Associates `.zs` files with Zenoscript language
- Provides syntax highlighting for Zenoscript keywords, operators, and constructs
- Enables proper commenting with `//` and `/* */`
- Sets up bracket matching and auto-closing pairs

### Other Editors

For other editors, you can use the TypeScript language server since Zenoscript compiles to TypeScript. Basic syntax highlighting can be achieved by associating `.zs` files with TypeScript syntax.

## Project Integration

### Adding to Existing Projects

To add Zenoscript to an existing TypeScript project:

```bash
# Install Zenoscript
bun add zenoscript

# Setup the plugin
zeno setup
```

This will:
- Add Zenoscript to your `package.json`
- Configure `bunfig.toml` with the Zenoscript plugin
- Allow you to import `.zs` files directly

### Manual Setup

If you prefer manual setup, add this to your `bunfig.toml`:

```toml
preload = ["zenoscript/plugin"]

[plugins]
zenoscript = "zenoscript/plugin"
```

## Next Steps

Now that you have Zenoscript installed, check out the [Quick Start Guide](/docs/quickstart) to learn the basics.

## Troubleshooting

### Command not found

If you get a "command not found" error:

1. Make sure Bun or npm's global bin directory is in your PATH
2. Try reinstalling Zenoscript
3. For Bun users, ensure `~/.bun/bin` is in your PATH

### Plugin not working

If the Bun plugin isn't working:

1. Make sure you're using Bun (not Node.js)
2. Check that `bunfig.toml` is properly configured
3. Try running `zeno setup` again

### Getting Help

- Check the [GitHub Issues](https://github.com/zenoscript/zenoscript/issues)
- Join our [Discord community](https://discord.gg/zenoscript)
- Read the [Documentation](/docs/)