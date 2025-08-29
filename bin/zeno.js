#!/usr/bin/env node

import { spawn } from 'child_process';
import { fileURLToPath } from 'url';
import { dirname, join } from 'path';
import { existsSync } from 'fs';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

// Determine the platform and architecture
const platform = process.platform;
const arch = process.arch;

// Map to binary paths
const getBinaryPath = () => {
  let binaryPath;
  
  if (platform === 'darwin') {
    if (arch === 'arm64') {
      binaryPath = join(__dirname, '..', 'build', 'darwin-arm64', 'zeno');
    } else {
      binaryPath = join(__dirname, '..', 'build', 'darwin-x64', 'zeno');
    }
  } else if (platform === 'linux') {
    binaryPath = join(__dirname, '..', 'build', 'linux-x64', 'zeno');
  } else if (platform === 'win32') {
    binaryPath = join(__dirname, '..', 'build', 'windows-x64', 'zeno.exe');
  } else {
    console.error(`Unsupported platform: ${platform}-${arch}`);
    process.exit(1);
  }

  if (!existsSync(binaryPath)) {
    console.error(`Binary not found at ${binaryPath}`);
    console.error(`Your platform (${platform}-${arch}) may not be supported.`);
    process.exit(1);
  }

  return binaryPath;
};

// Get the binary path and spawn the process
const binaryPath = getBinaryPath();
const args = process.argv.slice(2);

const child = spawn(binaryPath, args, {
  stdio: 'inherit'
});

child.on('exit', (code) => {
  process.exit(code);
});

child.on('error', (err) => {
  console.error('Failed to start zeno:', err.message);
  process.exit(1);
});