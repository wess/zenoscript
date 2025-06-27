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

async function transpileSource(source: string) {
  await buildIfNeeded();
  
  // Write test source to temp file
  const tempFile = `/tmp/test_${Date.now()}.zs`;
  await Bun.write(tempFile, source);
  
  // Run transpiler with timeout
  const process = spawn({
    cmd: [TRANSPILER_BINARY, tempFile],
    stdout: "pipe",
    stderr: "pipe",
  });
  
  // Add timeout to prevent hanging
  const timeout = setTimeout(() => {
    process.kill();
  }, 3000); // 3 second timeout
  
  const result = await process.exited;
  clearTimeout(timeout);
  
  const stdout = await new Response(process.stdout).text();
  const stderr = await new Response(process.stderr).text();
  
  // Cleanup
  await Bun.$`rm -f ${tempFile}`;
  
  return { exitCode: result, stdout, stderr };
}

test("transpiler - struct declaration", async () => {
  const source = `struct User {
    name: string;
    age: number;
  }`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain("type User = {");
  expect(result.stdout).toContain("name: string;");
  expect(result.stdout).toContain("age: number;");
  expect(result.stdout).toContain("};");
});

test("transpiler - generic struct", async () => {
  const source = `struct Container<T> {
    value: T;
  }`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain("type Container<T> = {");
  expect(result.stdout).toContain("value: T;");
});

test("transpiler - trait declaration", async () => {
  const source = `trait Display {
    show(): string;
  }`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain("interface Display {");
  expect(result.stdout).toContain("show(): string;");
  expect(result.stdout).toContain("}");
});

test("transpiler - let binding", async () => {
  const source = `let message = "Hello World"`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('const message = "Hello World";');
});

test("transpiler - pipe expression", async () => {
  const source = `"  hello  " |> trim |> toUpperCase`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('"  hello  ".trim().toUpperCase()');
});

test("transpiler - atoms", async () => {
  const source = `let status = :ok`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('const status = Symbol.for("ok");');
});

test("transpiler - match expression", async () => {
  const source = `match status {
    :ok => "Success"
    :error => "Failed"
    _ => "Unknown"
  }`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain("(() => {");
  // Be more flexible with whitespace in the match value assignment
  expect(result.stdout).toMatch(/const __match_value = \s*status/);
  expect(result.stdout).toContain('if (__match_value === Symbol.for("ok"))');
  expect(result.stdout).toContain('return "Success";');
  expect(result.stdout).toContain('} else if (__match_value === Symbol.for("error"))');
  expect(result.stdout).toContain('return "Failed";');
  expect(result.stdout).toContain("} else if (true)");
  expect(result.stdout).toContain('return "Unknown";');
});

test("transpiler - simple assignment", async () => {
  const source = `let x = 42`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain("const x = 42;");
});

test("transpiler - empty struct", async () => {
  const source = `struct Empty;`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain("type Empty = {};");
});

test("transpiler - string literal", async () => {
  const source = `let greeting = "Hello World"`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('const greeting = "Hello World";');
});