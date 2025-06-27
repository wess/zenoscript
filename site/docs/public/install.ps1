# Zenoscript installer script for Windows PowerShell
# Usage: iwr https://zeno.run/install.ps1 | iex

param(
    [string]$Version = "latest",
    [string]$InstallDir = "$env:USERPROFILE\.zenoscript"
)

# Zenoscript ASCII Art
Write-Host @"
   ____                            _       _   
  |_  /___ _ __   ___  ___  ___ _ __(_)_ __ | |_ 
   / // _ \ '_ \ / _ \/ __|/ __| '__| | '_ \| __|
  / /|  __/ | | | (_) \__ \ (__| |  | | |_) | |_ 
 /____\___|_| |_|\___/|___/\___|_|  |_| .__/ \__|
                                     |_|        
"@ -ForegroundColor Magenta

Write-Host "Fast functional programming for TypeScript" -ForegroundColor Magenta
Write-Host ""

# Configuration
$ErrorActionPreference = "Stop"
$ProgressPreference = "SilentlyContinue"

$ZenoscriptRepo = "wess/zenoscript"
$BinDir = Join-Path $InstallDir "bin"
$Platform = "windows"

# Functions
function Write-Info {
    param($Message)
    Write-Host "info: $Message" -ForegroundColor Blue
}

function Write-Success {
    param($Message)
    Write-Host "success: $Message" -ForegroundColor Green
}

function Write-Warning {
    param($Message)
    Write-Host "warn: $Message" -ForegroundColor Yellow
}

function Write-Error {
    param($Message)
    Write-Host "error: $Message" -ForegroundColor Red
}

function Test-Command {
    param($CommandName)
    $null -ne (Get-Command $CommandName -ErrorAction SilentlyContinue)
}

function Get-LatestVersion {
    try {
        $response = Invoke-RestMethod -Uri "https://api.github.com/repos/$ZenoscriptRepo/releases/latest"
        return $response.tag_name
    }
    catch {
        Write-Error "Failed to get latest version: $_"
        exit 1
    }
}

function Install-ViaBun {
    if (Test-Command "bun") {
        Write-Info "Bun detected! Installing Zenoscript via Bun..."
        try {
            & bun install -g zenoscript
            return $true
        }
        catch {
            Write-Warning "Bun installation failed, falling back to manual installation"
            return $false
        }
    }
    return $false
}

function Install-Zenoscript {
    param($Version)
    
    $DownloadUrl = "https://github.com/$ZenoscriptRepo/releases/download/$Version/zenoscript-$Platform.zip"
    $TempDir = Join-Path $env:TEMP "zenoscript-install"
    $ArchivePath = Join-Path $TempDir "zenoscript.zip"
    
    Write-Info "Creating installation directory..."
    New-Item -ItemType Directory -Force -Path $InstallDir | Out-Null
    New-Item -ItemType Directory -Force -Path $BinDir | Out-Null
    New-Item -ItemType Directory -Force -Path $TempDir | Out-Null
    
    try {
        Write-Info "Downloading Zenoscript $Version for Windows..."
        Invoke-WebRequest -Uri $DownloadUrl -OutFile $ArchivePath
        
        if (-not (Test-Path $ArchivePath)) {
            throw "Failed to download Zenoscript"
        }
        
        Write-Info "Extracting archive..."
        Expand-Archive -Path $ArchivePath -DestinationPath $InstallDir -Force
        
        Write-Success "Zenoscript installed to $InstallDir"
    }
    catch {
        Write-Error "Installation failed: $_"
        exit 1
    }
    finally {
        if (Test-Path $TempDir) {
            Remove-Item -Path $TempDir -Recurse -Force
        }
    }
}

function Update-EnvironmentPath {
    $CurrentPath = [Environment]::GetEnvironmentVariable("PATH", [EnvironmentVariableTarget]::User)
    
    if ($CurrentPath -notlike "*$BinDir*") {
        Write-Info "Adding Zenoscript to PATH..."
        $NewPath = "$BinDir;$CurrentPath"
        [Environment]::SetEnvironmentVariable("PATH", $NewPath, [EnvironmentVariableTarget]::User)
        
        # Update current session PATH
        $env:PATH = "$BinDir;$env:PATH"
        
        Write-Success "Updated user PATH"
    }
    else {
        Write-Info "PATH already configured"
    }
}

function Test-Installation {
    $ZenoPath = Join-Path $BinDir "zeno.exe"
    
    if (Test-Path $ZenoPath) {
        try {
            $VersionOutput = & $ZenoPath --version 2>$null
            Write-Success "Installation verified: $VersionOutput"
            return $true
        }
        catch {
            Write-Error "Installation verification failed"
            return $false
        }
    }
    else {
        Write-Error "Zenoscript binary not found at $ZenoPath"
        return $false
    }
}

function Test-PathConfiguration {
    return ($env:PATH -like "*$BinDir*")
}

# Main installation logic
function Install-Main {
    Write-Host "Installing Zenoscript..." -ForegroundColor Cyan
    Write-Host ""
    
    # Check PowerShell version
    if ($PSVersionTable.PSVersion.Major -lt 5) {
        Write-Error "PowerShell 5.0 or later is required"
        exit 1
    }
    
    # Try Bun installation first
    if (Install-ViaBun) {
        Write-Success "Zenoscript installed via Bun!"
        Write-Host ""
        Write-Host "Quick start:" -ForegroundColor Green
        Write-Host "  zeno init my-project"
        Write-Host "  cd my-project"
        Write-Host "  bun dev"
        Write-Host ""
        Write-Host "Documentation: https://zeno.run/docs" -ForegroundColor Blue
        return
    }
    
    # Fallback to manual installation
    Write-Info "Installing from GitHub releases..."
    
    if ($Version -eq "latest") {
        $Version = Get-LatestVersion
        if (-not $Version) {
            Write-Error "Could not determine latest version"
            exit 1
        }
    }
    
    # Install Zenoscript
    Install-Zenoscript -Version $Version
    
    # Update PATH
    Update-EnvironmentPath
    
    # Verify installation
    if (Test-Installation) {
        Write-Host ""
        Write-Host "🎉 Zenoscript successfully installed!" -ForegroundColor Green
        Write-Host ""
        Write-Host "To get started:" -ForegroundColor Yellow
        
        if (-not (Test-PathConfiguration)) {
            Write-Host "  1. Restart your terminal or run:"
            Write-Host "     `$env:PATH = `"$BinDir;`$env:PATH`""
            Write-Host "  2. Create a new project:"
        }
        else {
            Write-Host "  1. Create a new project:"
        }
        
        Write-Host "     zeno init my-project"
        Write-Host "     cd my-project"
        Write-Host "     bun dev"
        Write-Host ""
        Write-Host "Documentation: https://zeno.run/docs" -ForegroundColor Blue
        Write-Host "Examples: https://zeno.run/examples" -ForegroundColor Blue
        Write-Host "Community: https://discord.gg/zenoscript" -ForegroundColor Blue
    }
    else {
        exit 1
    }
}

# Handle Ctrl+C
$null = Register-EngineEvent PowerShell.Exiting -Action {
    Write-Host ""
    Write-Host "Installation cancelled." -ForegroundColor Red
}

# Check if running as administrator (optional warning)
$IsAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)

if ($IsAdmin) {
    Write-Warning "Running as Administrator. Zenoscript will be installed for the current user only."
}

# Run main installation
try {
    Install-Main
}
catch {
    Write-Error "Installation failed: $_"
    exit 1
}