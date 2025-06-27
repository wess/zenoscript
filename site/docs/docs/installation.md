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

- **macOS**: `zenoscript-macos.tar.gz`
- **Linux**: `zenoscript-linux.tar.gz`  
- **Windows**: `zenoscript-windows.zip`

Extract and add the `bin` directory to your PATH.

## Verify Installation

Check that Zenoscript is installed correctly:

```bash
zeno --version
```

You should see output like:

```
Zenoscript v0.0.1
```

## Create Your First Project

Use the `init` command to create a new Zenoscript project:

```bash
# Create a new project
zeno init my-zenoscript-app
cd my-zenoscript-app

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

### VS Code

Install the Zenoscript extension for syntax highlighting and language support:

```bash
code --install-extension zenoscript.vscode-zenoscript
```

### Other Editors

For other editors, you can use the TypeScript language server since Zenoscript compiles to TypeScript.

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
preload = ["zenoscript/preload"]

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
- Read the [FAQ](/docs/faq)