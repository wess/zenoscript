import { test, expect } from "bun:test";
import { spawn } from "bun";
import { join } from "path";

const TRANSPILER_DIR = join(import.meta.dir, "..", "src", "transpiler");
const BUILD_DIR = join(import.meta.dir, "..", "build");
const TRANSPILER_BINARY = join(BUILD_DIR, "zeno");

async function buildIfNeeded() {
  const binaryExists = await Bun.file(TRANSPILER_BINARY).exists();
  
  if (!binaryExists) {
    console.log("Building transpiler for tests...");
    const makeProcess = spawn({
      cmd: ["make"],
      cwd: TRANSPILER_DIR,
      stdio: ["inherit", "inherit", "inherit"],
    });
    
    const exitCode = await makeProcess.exited;
    if (exitCode !== 0) {
      throw new Error("Failed to build transpiler");
    }
  }
}

async function testTokens(source: string) {
  await buildIfNeeded();
  
  // Write test source to temp file
  const tempFile = `/tmp/test_${Date.now()}.zs`;
  await Bun.write(tempFile, source);
  
  // Run transpiler with verbose flag to get more output
  const process = spawn({
    cmd: [TRANSPILER_BINARY, "--verbose", tempFile],
    stdout: "pipe",
    stderr: "pipe",
  });
  
  // Add timeout to prevent hanging
  const timeout = setTimeout(() => {
    process.kill();
  }, 3000); // 3 second timeout for parsing tests
  
  const result = await process.exited;
  clearTimeout(timeout);
  
  const stdout = await new Response(process.stdout).text();
  const stderr = await new Response(process.stderr).text();
  
  // Cleanup
  await Bun.$`rm -f ${tempFile}`;
  
  return { exitCode: result, stdout, stderr };
}

test("lexer - basic parsing", async () => {
  const source = `struct User {
    name: string;
    age: number;
  }`;
  
  const result = await testTokens(source);
  expect(result.exitCode).toBe(0);
  // Just check that it parses without error
  expect(result.stderr).not.toContain("error");
});

test("lexer - let binding parsing", async () => {
  const source = `let message = "Hello World"`;
  
  const result = await testTokens(source);
  expect(result.exitCode).toBe(0);
  expect(result.stderr).not.toContain("error");
});

test("lexer - pipe expression parsing", async () => {
  const source = `"hello" |> trim`;
  
  const result = await testTokens(source);
  expect(result.exitCode).toBe(0);
  expect(result.stderr).not.toContain("error");
});

test("lexer - atoms parsing", async () => {
  const source = `let status = :ok`;
  
  const result = await testTokens(source);
  expect(result.exitCode).toBe(0);
  expect(result.stderr).not.toContain("error");
});

test("lexer - match expression parsing", async () => {
  const source = `match status {
    :ok => "Success"
    _ => "Unknown"
  }`;
  
  const result = await testTokens(source);
  expect(result.exitCode).toBe(0);
  expect(result.stderr).not.toContain("error");
});

test("lexer - numbers parsing", async () => {
  const source = `let x = 42`;
  
  const result = await testTokens(source);
  expect(result.exitCode).toBe(0);
  expect(result.stderr).not.toContain("error");
});