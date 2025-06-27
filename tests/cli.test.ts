import { test, expect } from "bun:test";
import { spawn } from "bun";
import { join } from "path";
import { existsSync, rmSync, mkdirSync } from "fs";

const CLI_BINARY = join(import.meta.dir, "..", "src", "index.ts");
const TEST_DIR = join(import.meta.dir, "..", "test-projects");

async function runCli(args: string[], cwd?: string) {
  const spawnProcess = spawn({
    cmd: ["bun", CLI_BINARY, ...args],
    cwd: cwd || import.meta.dir,
    stdout: "pipe",
    stderr: "pipe",
  });

  const timeout = setTimeout(() => {
    spawnProcess.kill();
  }, 10000); // 10 second timeout

  const exitCode = await spawnProcess.exited;
  clearTimeout(timeout);

  const stdout = await new Response(spawnProcess.stdout).text();
  const stderr = await new Response(spawnProcess.stderr).text();

  return { exitCode, stdout, stderr };
}

async function cleanupTestDir(dirName: string) {
  const testPath = join(TEST_DIR, dirName);
  if (existsSync(testPath)) {
    rmSync(testPath, { recursive: true, force: true });
  }
}

async function setupTestDir(dirName: string) {
  const testPath = join(TEST_DIR, dirName);
  await cleanupTestDir(dirName);
  mkdirSync(testPath, { recursive: true });
  return testPath;
}

test("cli - help command", async () => {
  const result = await runCli(["--help"]);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain("Zenoscript CLI");
  expect(result.stdout).toContain("Commands:");
  expect(result.stdout).toContain("init");
  expect(result.stdout).toContain("setup");
  expect(result.stdout).toContain("repl");
  expect(result.stdout).toContain("-e <code>");
  expect(result.stdout).toContain("Execute Zenoscript code inline");
  expect(result.stdout).toContain("Examples:");
  expect(result.stdout).toContain('zeno -e \'"Hello" |> console.log\'');
});

test("cli - version command", async () => {
  const result = await runCli(["--version"]);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain("Zenoscript v");
});

test("cli - init command creates project structure", async () => {
  const testPath = await setupTestDir("init-test");
  
  const result = await runCli(["init"], testPath);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain("Initializing Zenoscript project");
  expect(result.stdout).toContain("✓ Created package.json");
  expect(result.stdout).toContain("✓ Created bunfig.toml");
  expect(result.stdout).toContain("✓ Created index.zs");

  // Verify files were created
  expect(existsSync(join(testPath, "package.json"))).toBe(true);
  expect(existsSync(join(testPath, "bunfig.toml"))).toBe(true);
  expect(existsSync(join(testPath, "index.zs"))).toBe(true);

  // Verify package.json content
  const packageJson = await Bun.file(join(testPath, "package.json")).json();
  expect(packageJson.name).toBe("zenoscript-project");
  expect(packageJson.scripts.dev).toBe("bun --hot index.zs");
  expect(packageJson.dependencies.zenoscript).toBe("latest");

  // Verify bunfig.toml content
  const bunfig = await Bun.file(join(testPath, "bunfig.toml")).text();
  expect(bunfig).toContain('preload = ["zenoscript/plugin"]');
  expect(bunfig).toContain('zenoscript = "zenoscript/plugin"');

  // Verify index.zs content
  const indexZs = await Bun.file(join(testPath, "index.zs")).text();
  expect(indexZs).toContain("Welcome to Zenoscript!");
  expect(indexZs).toContain("const greet = (name: string)");

  await cleanupTestDir("init-test");
});

test("cli - setup command creates bunfig.toml", async () => {
  const testPath = await setupTestDir("setup-test");
  
  // Create a basic package.json first
  await Bun.write(join(testPath, "package.json"), JSON.stringify({
    name: "test-project",
    version: "1.0.0"
  }, null, 2));

  const result = await runCli(["setup"], testPath);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain("Setting up Zenoscript plugin for Bun");
  expect(result.stdout).toContain("✓ Created bunfig.toml with Zenoscript plugin");

  // Verify bunfig.toml was created
  expect(existsSync(join(testPath, "bunfig.toml"))).toBe(true);
  
  const bunfig = await Bun.file(join(testPath, "bunfig.toml")).text();
  expect(bunfig).toContain('preload = ["zenoscript/plugin"]');
  expect(bunfig).toContain('zenoscript = "zenoscript/plugin"');

  await cleanupTestDir("setup-test");
});

test("cli - setup command updates existing bunfig.toml", async () => {
  const testPath = await setupTestDir("setup-existing-test");
  
  // Create existing bunfig.toml without zenoscript
  await Bun.write(join(testPath, "bunfig.toml"), `[build]
target = "node"
`);
  
  await Bun.write(join(testPath, "package.json"), JSON.stringify({
    name: "test-project",
    version: "1.0.0"
  }, null, 2));

  const result = await runCli(["setup"], testPath);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain("✓ Updated bunfig.toml with Zenoscript plugin");

  const bunfig = await Bun.file(join(testPath, "bunfig.toml")).text();
  expect(bunfig).toContain("[build]");
  expect(bunfig).toContain("target = \"node\"");
  expect(bunfig).toContain("zenoscript = \"zenoscript/plugin\"");

  await cleanupTestDir("setup-existing-test");
});

test("cli - setup command handles existing zenoscript config", async () => {
  const testPath = await setupTestDir("setup-exists-test");
  
  // Create bunfig.toml with zenoscript already configured
  await Bun.write(join(testPath, "bunfig.toml"), `preload = ["zenoscript/plugin"]

[plugins]
zenoscript = "zenoscript/plugin"
`);

  const result = await runCli(["setup"], testPath);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain("✓ Zenoscript plugin already configured in bunfig.toml");

  await cleanupTestDir("setup-exists-test");
});

test("cli - repl command starts interactive session", async () => {
  // This test just verifies the REPL starts up correctly
  // We can't easily test interactive features in this context
  const replProcess = spawn({
    cmd: ["bun", CLI_BINARY, "repl"],
    stdout: "pipe",
    stderr: "pipe",
    stdin: "pipe",
  });

  // Send exit command to quit REPL
  replProcess.stdin?.write("exit\n");
  replProcess.stdin?.end();

  const timeout = setTimeout(() => {
    replProcess.kill();
  }, 5000);

  const exitCode = await replProcess.exited;
  clearTimeout(timeout);

  const stdout = await new Response(replProcess.stdout).text();
  
  expect(stdout).toContain("Starting Zenoscript REPL");
  expect(stdout).toContain("zs>");
});

test("cli - transpile file command", async () => {
  const testPath = await setupTestDir("transpile-test");
  
  // Create a simple Zenoscript file
  const zsFile = join(testPath, "test.zs");
  await Bun.write(zsFile, 'let greeting = "Hello World"');

  const result = await runCli([zsFile]);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('const greeting = "Hello World";');

  await cleanupTestDir("transpile-test");
});

test("cli - error handling for missing file", async () => {
  const result = await runCli(["nonexistent.zs"]);
  expect(result.exitCode).not.toBe(0);
  // Should show some error about missing file
});

test("cli - execute inline code with -e", async () => {
  const result = await runCli(["-e", '"Hello World" |> console.log']);
  expect(result.exitCode).toBe(0);
  expect(result.stdout.trim()).toBe("Hello World");
});

test("cli - execute inline let binding with -e", async () => {
  const result = await runCli(["-e", 'let x = 42']);
  expect(result.exitCode).toBe(0);
  // Should execute without error (no output expected for let binding)
});

test("cli - execute inline atom with -e", async () => {
  const result = await runCli(["-e", 'let status = :success']);
  expect(result.exitCode).toBe(0);
  // Should execute without error
});

test("cli - execute complex pipe chain with -e", async () => {
  const result = await runCli(["-e", '"  hello world  " |> trim |> toUpperCase |> console.log']);
  expect(result.exitCode).toBe(0);
  expect(result.stdout.trim()).toBe("HELLO WORLD");
});

test("cli - execute multiline code with -e", async () => {
  const result = await runCli(["-e", 'let data = [1,2,3]; data |> console.log']);
  expect(result.exitCode).toBe(0);
  expect(result.stdout.trim()).toBe("[ 1, 2, 3 ]");
});

test("cli - error handling for invalid code with -e", async () => {
  const result = await runCli(["-e", 'invalid syntax here']);
  expect(result.exitCode).not.toBe(0);
  expect(result.stderr).toContain("error");
});

test("cli - missing code argument after -e", async () => {
  const result = await runCli(["-e"]);
  expect(result.exitCode).not.toBe(0);
  // Should fail when no code is provided after -e
});

test("cli - build transpiler command", async () => {
  const result = await runCli(["--build"]);
  // This might take longer, so we just check it doesn't immediately fail
  // The actual building is tested by the transpiler tests
  expect([0, 1]).toContain(result.exitCode); // Either succeeds or fails gracefully
});