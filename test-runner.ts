#!/usr/bin/env bun

import { spawn } from "bun";

const testProcess = spawn({
  cmd: ["bun", "test", "tests/simple.test.zs"],
  stdio: ["inherit", "inherit", "inherit"],
});

// Set a timeout
const timeout = setTimeout(() => {
  testProcess.kill();
  console.log("Test timed out after 10 seconds");
  process.exit(1);
}, 10000);

const exitCode = await testProcess.exited;
clearTimeout(timeout);

if (exitCode === 0) {
  console.log("Tests passed!");
} else {
  console.log(`Tests failed with exit code: ${exitCode}`);
}

process.exit(exitCode);