import { test, expect } from "bun:test";
import { spawn } from "bun";
import { join } from "path";
import { existsSync, rmSync, mkdirSync } from "fs";

const CLI_BINARY = join(import.meta.dir, "..", "src", "index.ts");
const TEST_DIR = join(import.meta.dir, "..", "test-integration");

async function runCli(args: string[], cwd: string) {
  const process = spawn({
    cmd: ["bun", CLI_BINARY, ...args],
    cwd,
    stdout: "pipe",
    stderr: "pipe",
  });

  const timeout = setTimeout(() => {
    process.kill();
  }, 15000); // 15 second timeout for integration tests

  const exitCode = await process.exited;
  clearTimeout(timeout);

  const stdout = await new Response(process.stdout).text();
  const stderr = await new Response(process.stderr).text();

  return { exitCode, stdout, stderr };
}

async function runBun(args: string[], cwd: string) {
  const process = spawn({
    cmd: ["bun", ...args],
    cwd,
    stdout: "pipe",
    stderr: "pipe",
  });

  const timeout = setTimeout(() => {
    process.kill();
  }, 10000);

  const exitCode = await process.exited;
  clearTimeout(timeout);

  const stdout = await new Response(process.stdout).text();
  const stderr = await new Response(process.stderr).text();

  return { exitCode, stdout, stderr };
}

async function setupIntegrationTest() {
  const testPath = join(TEST_DIR, `test-${Date.now()}`);
  if (existsSync(testPath)) {
    rmSync(testPath, { recursive: true, force: true });
  }
  mkdirSync(testPath, { recursive: true });
  return testPath;
}

async function cleanupIntegrationTest(testPath: string) {
  if (existsSync(testPath)) {
    rmSync(testPath, { recursive: true, force: true });
  }
}

test("integration - project initialization workflow", async () => {
  const testPath = await setupIntegrationTest();
  
  try {
    // Step 1: Initialize project
    console.log("Step 1: Initializing project...");
    const initResult = await runCli(["init"], testPath);
    expect(initResult.exitCode).toBe(0);
    expect(initResult.stdout).toContain("✓ Created package.json");

    // Step 2: Test that the project structure is correct
    console.log("Step 2: Verifying project structure...");
    expect(existsSync(join(testPath, "package.json"))).toBe(true);
    expect(existsSync(join(testPath, "bunfig.toml"))).toBe(true);
    expect(existsSync(join(testPath, "index.zs"))).toBe(true);

    // Step 3: Verify the generated index.zs content
    console.log("Step 3: Verifying generated content...");
    const indexContent = await Bun.file(join(testPath, "index.zs")).text();
    expect(indexContent).toContain("Welcome to Zenoscript!");
    expect(indexContent).toContain("const greet =");

    console.log("✓ Integration test completed successfully");
    
  } finally {
    await cleanupIntegrationTest(testPath);
  }
}, 20000);

test("integration - setup existing project workflow", async () => {
  const testPath = await setupIntegrationTest();
  
  try {
    // Step 1: Create a basic Node.js project
    console.log("Step 1: Creating basic project...");
    const packageJson = {
      name: "test-project",
      version: "1.0.0",
      type: "module",
      scripts: {
        dev: "bun --hot index.js"
      }
    };
    await Bun.write(join(testPath, "package.json"), JSON.stringify(packageJson, null, 2));

    // Step 2: Setup Zenoscript
    console.log("Step 2: Setting up Zenoscript...");
    const setupResult = await runCli(["setup"], testPath);
    expect(setupResult.exitCode).toBe(0);
    expect(setupResult.stdout).toContain("✓ Created bunfig.toml with Zenoscript plugin");

    // Step 3: Verify configuration files
    console.log("Step 3: Verifying configuration...");
    const bunfig = await Bun.file(join(testPath, "bunfig.toml")).text();
    expect(bunfig).toContain('zenoscript = "zenoscript/plugin"');

    console.log("✓ Setup integration test completed successfully");
    
  } finally {
    await cleanupIntegrationTest(testPath);
  }
}, 15000);

test("integration - error handling workflow", async () => {
  const testPath = await setupIntegrationTest();
  
  try {
    // Test: Missing file
    console.log("Test: Testing missing file...");
    const missingResult = await runCli(["nonexistent.zs"], testPath);
    expect(missingResult.exitCode).not.toBe(0);

    console.log("✓ Error handling test completed successfully");
    
  } finally {
    await cleanupIntegrationTest(testPath);
  }
}, 10000);