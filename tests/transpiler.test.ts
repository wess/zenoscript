import { test, expect } from "bun:test";
import { ZenoscriptTranspiler } from "../src/transpiler.ts";

async function transpileSource(source: string) {
  const transpiler = new ZenoscriptTranspiler({ verbose: false, debug: false });
  
  try {
    const result = transpiler.transpile(source);
    return { exitCode: 0, stdout: result, stderr: "" };
  } catch (error) {
    return { exitCode: 1, stdout: "", stderr: error.message };
  }
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

test("transpiler - pipe expression with method calls", async () => {
  const source = `"  hello  " |> trim |> toUpperCase`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('(("  hello  ").trim()).toUpperCase()');
});

test("transpiler - pipe expression with console.log", async () => {
  const source = `"Hello World" |> console.log`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('console.log("Hello World")');
});

test("transpiler - chained pipe with console.log", async () => {
  const source = `"  hello  " |> trim |> toUpperCase |> console.log`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('console.log(((\"  hello  \").trim()).toUpperCase())');
});

test("transpiler - pipe with variable", async () => {
  const source = `data |> console.log`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('console.log(data)');
});

test("transpiler - multiline with let and pipe", async () => {
  const source = `let data = [1,2,3]; data |> console.log`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('const data = [1,2,3];');
  expect(result.stdout).toContain('console.log(data)');
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
  expect(result.stdout).toContain("} else {");
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