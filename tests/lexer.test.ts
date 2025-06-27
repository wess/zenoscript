import { test, expect } from "bun:test";
import { ZenoscriptTranspiler } from "../src/transpiler.ts";

async function testParsing(source: string) {
  const transpiler = new ZenoscriptTranspiler({ verbose: false, debug: false });
  
  try {
    const result = transpiler.transpile(source);
    return { exitCode: 0, stdout: result, stderr: "" };
  } catch (error) {
    return { exitCode: 1, stdout: "", stderr: error.message };
  }
}

test("lexer - basic parsing", async () => {
  const source = `struct User {
    name: string;
    age: number;
  }`;
  
  const result = await testParsing(source);
  expect(result.exitCode).toBe(0);
  // Just check that it parses without error
  expect(result.stderr).not.toContain("error");
});

test("lexer - let binding parsing", async () => {
  const source = `let message = "Hello World"`;
  
  const result = await testParsing(source);
  expect(result.exitCode).toBe(0);
  expect(result.stderr).not.toContain("error");
});

test("lexer - pipe expression parsing", async () => {
  const source = `"hello" |> trim`;
  
  const result = await testParsing(source);
  expect(result.exitCode).toBe(0);
  expect(result.stderr).not.toContain("error");
});

test("lexer - atoms parsing", async () => {
  const source = `let status = :ok`;
  
  const result = await testParsing(source);
  expect(result.exitCode).toBe(0);
  expect(result.stderr).not.toContain("error");
});

test("lexer - match expression parsing", async () => {
  const source = `match status {
    :ok => "Success"
    _ => "Unknown"
  }`;
  
  const result = await testParsing(source);
  expect(result.exitCode).toBe(0);
  expect(result.stderr).not.toContain("error");
});

test("lexer - numbers parsing", async () => {
  const source = `let x = 42`;
  
  const result = await testParsing(source);
  expect(result.exitCode).toBe(0);
  expect(result.stderr).not.toContain("error");
});