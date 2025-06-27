#!/bin/bash
# Zenoscript installer script for Unix systems (Linux, macOS)
# Usage: curl -fsSL https://zeno.run/install.sh | bash

set -euo pipefail

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m' # No Color

# Zenoscript ASCII Art
cat << 'EOF'
   ____                            _       _   
  |_  /___ _ __   ___  ___  ___ _ __(_)_ __ | |_ 
   / // _ \ '_ \ / _ \/ __|/ __| '__| | '_ \| __|
  / /|  __/ | | | (_) \__ \ (__| |  | | |_) | |_ 
 /____\___|_| |_|\___/|___/\___|_|  |_| .__/ \__|
                                     |_|        
EOF

echo -e "${PURPLE}Fast functional programming for TypeScript${NC}"
echo

# Configuration
ZENOSCRIPT_REPO="zenoscript/zenoscript"
INSTALL_DIR="$HOME/.zenoscript"
BIN_DIR="$HOME/.zenoscript/bin"
VERSION="latest"

# Platform detection
OS="$(uname -s)"
ARCH="$(uname -m)"

case $OS in
  Linux*)
    PLATFORM="linux"
    ;;
  Darwin*)
    PLATFORM="macos"
    ;;
  CYGWIN*|MINGW*|MSYS*)
    echo -e "${RED}Error: This script is for Unix systems. Use install.ps1 for Windows.${NC}"
    exit 1
    ;;
  *)
    echo -e "${RED}Error: Unsupported operating system: $OS${NC}"
    exit 1
    ;;
esac

case $ARCH in
  x86_64|amd64)
    ARCH="x64"
    ;;
  arm64|aarch64)
    ARCH="arm64"
    ;;
  *)
    echo -e "${RED}Error: Unsupported architecture: $ARCH${NC}"
    exit 1
    ;;
esac

# Functions
log_info() {
  echo -e "${BLUE}info:${NC} $1"
}

log_success() {
  echo -e "${GREEN}success:${NC} $1"
}

log_warn() {
  echo -e "${YELLOW}warn:${NC} $1"
}

log_error() {
  echo -e "${RED}error:${NC} $1"
}

# Check if command exists
command_exists() {
  command -v "$1" >/dev/null 2>&1
}

# Check for required tools
check_dependencies() {
  local missing_deps=()
  
  if ! command_exists curl && ! command_exists wget; then
    missing_deps+=("curl or wget")
  fi
  
  if ! command_exists tar; then
    missing_deps+=("tar")
  fi
  
  if [ ${#missing_deps[@]} -ne 0 ]; then
    log_error "Missing required dependencies: ${missing_deps[*]}"
    echo "Please install them and try again."
    exit 1
  fi
}

# Get latest version from GitHub
get_latest_version() {
  if command_exists curl; then
    curl -s "https://api.github.com/repos/$ZENOSCRIPT_REPO/releases/latest" | grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/'
  else
    wget -qO- "https://api.github.com/repos/$ZENOSCRIPT_REPO/releases/latest" | grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/'
  fi
}

# Download and extract Zenoscript
install_zenoscript() {
  local version="$1"
  local download_url="https://github.com/$ZENOSCRIPT_REPO/releases/download/$version/zenoscript-$PLATFORM.tar.gz"
  local temp_dir="$(mktemp -d)"
  local archive_path="$temp_dir/zenoscript.tar.gz"
  
  log_info "Creating installation directory..."
  mkdir -p "$INSTALL_DIR" "$BIN_DIR"
  
  log_info "Downloading Zenoscript $version for $PLATFORM-$ARCH..."
  if command_exists curl; then
    curl -fL "$download_url" -o "$archive_path"
  else
    wget -O "$archive_path" "$download_url"
  fi
  
  if [ ! -f "$archive_path" ]; then
    log_error "Failed to download Zenoscript"
    exit 1
  fi
  
  log_info "Extracting archive..."
  tar -xzf "$archive_path" -C "$INSTALL_DIR" --strip-components=1
  
  # Make binary executable
  chmod +x "$BIN_DIR/zeno"
  
  # Clean up
  rm -rf "$temp_dir"
  
  log_success "Zenoscript installed to $INSTALL_DIR"
}

# Check if Bun is available and install via Bun if possible
install_via_bun() {
  if command_exists bun; then
    log_info "Bun detected! Installing Zenoscript via Bun..."
    bun install -g zenoscript
    return 0
  fi
  return 1
}

# Update shell configuration
update_shell_config() {
  local shell_config=""
  local current_shell="$(basename "$SHELL")"
  
  case "$current_shell" in
    bash)
      if [ -f "$HOME/.bashrc" ]; then
        shell_config="$HOME/.bashrc"
      elif [ -f "$HOME/.bash_profile" ]; then
        shell_config="$HOME/.bash_profile"
      fi
      ;;
    zsh)
      shell_config="$HOME/.zshrc"
      ;;
    fish)
      shell_config="$HOME/.config/fish/config.fish"
      ;;
  esac
  
  if [ -n "$shell_config" ] && [ -f "$shell_config" ]; then
    if ! grep -q "zenoscript" "$shell_config"; then
      echo "" >> "$shell_config"
      echo "# Zenoscript" >> "$shell_config"
      echo "export PATH=\"\$HOME/.zenoscript/bin:\$PATH\"" >> "$shell_config"
      log_success "Updated $shell_config"
    else
      log_info "PATH already configured in $shell_config"
    fi
  else
    log_warn "Could not automatically update shell configuration"
    echo "Please add this to your shell configuration:"
    echo "  export PATH=\"\$HOME/.zenoscript/bin:\$PATH\""
  fi
}

# Check if PATH is updated
check_path() {
  if echo "$PATH" | grep -q "$BIN_DIR"; then
    return 0
  else
    return 1
  fi
}

# Verify installation
verify_installation() {
  if [ -x "$BIN_DIR/zeno" ]; then
    local version_output="$("$BIN_DIR/zeno" --version 2>/dev/null || echo "unknown")"
    log_success "Installation verified: $version_output"
    return 0
  else
    log_error "Installation verification failed"
    return 1
  fi
}

# Main installation logic
main() {
  echo -e "${CYAN}Installing Zenoscript...${NC}"
  echo
  
  # Check dependencies
  check_dependencies
  
  # Try Bun installation first
  if install_via_bun; then
    log_success "Zenoscript installed via Bun!"
    echo
    echo -e "${GREEN}Quick start:${NC}"
    echo "  zeno init my-project"
    echo "  cd my-project"
    echo "  bun dev"
    echo
    echo -e "${BLUE}Documentation:${NC} https://zeno.run/docs"
    return 0
  fi
  
  # Fallback to manual installation
  log_info "Installing from GitHub releases..."
  
  if [ "$VERSION" = "latest" ]; then
    VERSION="$(get_latest_version)"
    if [ -z "$VERSION" ]; then
      log_error "Could not determine latest version"
      exit 1
    fi
  fi
  
  # Install Zenoscript
  install_zenoscript "$VERSION"
  
  # Update shell configuration
  update_shell_config
  
  # Verify installation
  if verify_installation; then
    echo
    echo -e "${GREEN}🎉 Zenoscript successfully installed!${NC}"
    echo
    echo -e "${YELLOW}To get started:${NC}"
    
    if ! check_path; then
      echo "  1. Restart your terminal or run:"
      echo "     export PATH=\"\$HOME/.zenoscript/bin:\$PATH\""
      echo "  2. Create a new project:"
    else
      echo "  1. Create a new project:"
    fi
    
    echo "     zeno init my-project"
    echo "     cd my-project"
    echo "     bun dev"
    echo
    echo -e "${BLUE}Documentation:${NC} https://zeno.run/docs"
    echo -e "${BLUE}Examples:${NC} https://zeno.run/examples"
    echo -e "${BLUE}Community:${NC} https://discord.gg/zenoscript"
  else
    exit 1
  fi
}

# Handle Ctrl+C
trap 'echo -e "\n${RED}Installation cancelled.${NC}"; exit 1' INT

# Run main function
main "$@"