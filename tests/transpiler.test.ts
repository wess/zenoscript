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
  expect(result.stdout).toContain('"  hello  ".trim().toUpperCase()');
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
  expect(result.stdout).toContain('console.log("  hello  ".trim().toUpperCase())');
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

// Tests for Elixir-inspired features

test("transpiler - optional parentheses with single argument", async () => {
  const source = `console.log message`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('console.log(message)');
});

test("transpiler - optional parentheses with string argument", async () => {
  const source = `myFunction "hello world"`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('myFunction("hello world")');
});

test("transpiler - optional parentheses with number argument", async () => {
  const source = `processValue 42`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('processValue(42)');
});

test("transpiler - optional parentheses should not affect keywords", async () => {
  const source = `if x == 1 { return value }`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('if (x == 1)');
  expect(result.stdout).not.toContain('if(x)');
});

test("transpiler - optional parentheses with variable argument", async () => {
  const source = `doSomething myVariable`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('doSomething(myVariable)');
});

test("transpiler - optional return in function", async () => {
  const source = `function add(a, b) { a + b }`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('return a + b');
});

test("transpiler - optional return in arrow function", async () => {
  const source = `(x) => { x * 2 }`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('return x * 2');
});

test("transpiler - optional return should not affect explicit return", async () => {
  const source = `function getValue() { return 42 }`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('return 42');
  // Should not have double return
  expect(result.stdout).not.toContain('return return');
});

test("transpiler - optional return with multiple statements", async () => {
  const source = `function process(x) { let y = x * 2; y + 1 }`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('const y = x * 2');
  expect(result.stdout).toContain('return y + 1');
});

test("transpiler - simplified if statement", async () => {
  const source = `if x == 1 { console.log "one" }`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('if (x == 1) {');
});

test("transpiler - simplified if with complex condition", async () => {
  const source = `if x > 0 && y < 10 { return true }`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('if (x > 0 && y < 10) {');
});

test("transpiler - simplified if should not affect already parenthesized", async () => {
  const source = `if (x == 1) { return value }`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('if (x == 1) {');
  // Should not have double parentheses
  expect(result.stdout).not.toContain('if ((x == 1)) {');
});

test("transpiler - simplified if with string comparison", async () => {
  const source = `if name == "John" { greet name }`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('if (name == "John") {');
});

// Additional tests for complex scenarios
test("transpiler - comments are preserved", async () => {
  const source = `// This is a comment
let x = 42`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('// This is a comment');
  expect(result.stdout).toContain('const x = 42;');
});

test("transpiler - multiline comments are preserved", async () => {
  const source = `/* This is a 
     multiline comment */
let x = 42`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('/* This is a');
  expect(result.stdout).toContain('multiline comment */');
});

test("transpiler - complex pipe chain with method calls", async () => {
  const source = `let result = "hello world" |> trim |> toUpperCase |> split |> join`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('const result = "hello world".trim().toUpperCase().split().join();');
});

test("transpiler - nested match expressions", async () => {
  const source = `match status {
    :loading => match progress {
      :started => "Loading started"
      :complete => "Loading complete"
      _ => "Loading..."
    }
    :error => "Failed"
    _ => "Unknown"
  }`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('if (__match_value === Symbol.for("loading"))');
  expect(result.stdout).toContain('Symbol.for("started")');
  expect(result.stdout).toContain('Symbol.for("complete")');
});

test("transpiler - let binding with pipe in same line", async () => {
  const source = `let result = data |> transform |> validate`;
  
  const result = await transpileSource(source);
  expect(result.exitCode).toBe(0);
  expect(result.stdout).toContain('const result = data.transform().validate();');
});