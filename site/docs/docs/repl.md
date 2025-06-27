# REPL

The Zenoscript REPL (Read-Eval-Print Loop) provides an interactive environment for experimenting with the language, testing code snippets, and learning Zenoscript features in real-time.

## Getting Started

### Starting the REPL

```bash
# Start the REPL
zeno repl

# Alternative ways
bun run zeno repl
npx zenoscript repl
```

You'll see the Zenoscript prompt:

```
   ____                            _       _   
  |_  /___ _ __   ___  ___  ___ _ __(_)_ __ | |_ 
   / // _ \ '_ \ / _ \/ __|/ __| '__| | '_ \| __|
  / /|  __/ | | | (_) \__ \ (__| |  | | |_) | |_ 
 /____\___|_| |_|\___/|___/\___|_|  |_| .__/ \__|
                                     |_|        

Fast functional programming for TypeScript

Type 'exit' or press Ctrl+C to quit

zs> 
```

### Basic Usage

```
zs> let x = 42
→ const x = 42;

zs> x * 2
→ x * 2
84

zs> let greeting = "Hello, World!"
→ const greeting = "Hello, World!";

zs> greeting.toUpperCase()
→ greeting.toUpperCase()
"HELLO, WORLD!"
```

## REPL Features

### Code Completion

The REPL provides intelligent code completion:

```
zs> let user = { name: "Alice", age: 30 }
→ const user = { name: "Alice", age: 30 };

zs> user. [TAB]
   name    age
   
zs> user.name
→ user.name
"Alice"
```

### Multi-line Input

For complex expressions, use multi-line input:

```
zs> let processUser = (user) => {
...   let message = `Hello, ${user.name}!`
...   return message.toUpperCase()
... }
→ const processUser = (user) => {
  const message = `Hello, ${user.name}!`;
  return message.toUpperCase();
};

zs> processUser({ name: "Bob" })
→ processUser({ name: "Bob" })
"HELLO, BOB!"
```

### Command History

Navigate through previous commands:

- **↑ (Up Arrow)** - Previous command
- **↓ (Down Arrow)** - Next command
- **Ctrl+R** - Search command history

```
zs> let x = 10
→ const x = 10;

zs> let y = 20
→ const y = 20;

zs> [Press ↑ twice to get back to "let x = 10"]
```

## Language Features in REPL

### Variables and Functions

```
zs> let add = (a, b) => a + b
→ const add = (a, b) => a + b;

zs> add(5, 3)
→ add(5, 3)
8

zs> let square = x => x * x
→ const square = x => x * x;

zs> [1, 2, 3, 4].map(square)
→ [1, 2, 3, 4].map(square)
[1, 4, 9, 16]
```

### Structs and Types

```
zs> struct User { name: string; age: number; }
→ type User = {
  name: string;
  age: number;
};

zs> let user: User = { name: "Alice", age: 30 }
→ const user: User = { name: "Alice", age: 30 };

zs> user
→ user
{ name: "Alice", age: 30 }
```

### Atoms and Pattern Matching

```
zs> let status = :pending
→ const status = Symbol.for("pending");

zs> match status { :pending => "Loading...", _ => "Done" }
→ (() => {
  const __match_value = status;
  if (__match_value === Symbol.for("pending")) {
    return "Loading...";
  } else {
    return "Done";
  }
})()
"Loading..."
```

### Pipe Operations

```
zs> let numbers = [1, 2, 3, 4, 5]
→ const numbers = [1, 2, 3, 4, 5];

zs> numbers |> filter(x => x % 2 === 0) |> map(x => x * 2)
→ numbers.filter(x => x % 2 === 0).map(x => x * 2)
[4, 8]

zs> "  hello world  " |> trim |> toUpperCase |> split(" ")
→ "  hello world  ".trim().toUpperCase().split(" ")
["HELLO", "WORLD"]
```

## Advanced REPL Usage

### Import Statements

```
zs> import { readFileSync } from "fs"
→ import { readFileSync } from "fs";

zs> import _ from "lodash"
→ import _ from "lodash";

zs> _.range(5)
→ _.range(5)
[0, 1, 2, 3, 4]
```

### Async/Await

```
zs> let fetchData = async () => {
...   let response = await fetch("https://api.github.com/users/octocat")
...   return await response.json()
... }
→ const fetchData = async () => {
  const response = await fetch("https://api.github.com/users/octocat");
  return await response.json();
};

zs> await fetchData()
→ await fetchData()
{
  login: "octocat",
  id: 1,
  node_id: "MDQ6VXNlcjE=",
  ...
}
```

### Error Handling

```
zs> let safeDivide = (a, b) => {
...   if (b === 0) throw new Error("Division by zero")
...   return a / b
... }
→ const safeDivide = (a, b) => {
  if (b === 0) throw new Error("Division by zero");
  return a / b;
};

zs> safeDivide(10, 2)
→ safeDivide(10, 2)
5

zs> safeDivide(10, 0)
→ safeDivide(10, 0)
Error: Division by zero
    at safeDivide (REPL:2:26)
    at REPL:1:1
```

## REPL Commands

### Built-in Commands

```
zs> .help
Available commands:
  .help       Show this help message
  .clear      Clear the REPL context
  .reset      Reset the REPL session
  .save       Save session to file
  .load       Load session from file
  .exit       Exit the REPL

zs> .clear
Context cleared.

zs> .exit
Goodbye!
```

### Session Management

```
# Save current session
zs> .save my-session.zs
Session saved to my-session.zs

# Load a previous session
zs> .load my-session.zs
Session loaded from my-session.zs

# Reset everything
zs> .reset
REPL reset. All variables cleared.
```

## Customization

### Configuration File

Create a `.zenoscript-repl.json` file in your home directory:

```json
{
  "prompt": "zs> ",
  "historySize": 1000,
  "autoComplete": true,
  "syntaxHighlighting": true,
  "showCompilationOutput": true,
  "colorScheme": "dark",
  "prelude": [
    "import _ from 'lodash'",
    "let log = console.log"
  ]
}
```

### Custom Startup

Create a startup file that runs when the REPL starts:

```zenoscript
// ~/.zenoscript-repl-startup.zs

// Utility functions
let range = (n) => Array.from({ length: n }, (_, i) => i)
let times = (n, fn) => range(n).map(fn)
let log = console.log

// Helper for testing
let test = (name, fn) => {
  try {
    fn()
    log(`✓ ${name}`)
  } catch (error) {
    log(`✗ ${name}: ${error.message}`)
  }
}

log("Custom REPL environment loaded!")
```

Run REPL with startup file:

```bash
zeno repl --startup ~/.zenoscript-repl-startup.zs
```

## REPL vs Script Mode

### REPL Mode Features

- **Interactive evaluation** - Immediate feedback
- **Context persistence** - Variables persist between commands
- **Error recovery** - Errors don't stop the session
- **Exploration-friendly** - Perfect for learning and testing

```
zs> let x = 10
→ const x = 10;

zs> let y = x + 5  // x is still available
→ const y = x + 5;

zs> y
→ y
15
```

### Script Mode Differences

```bash
# Script mode - run once and exit
zeno my-script.zs

# REPL mode - interactive session
zeno repl
```

## Debugging with REPL

### Step-by-Step Debugging

```
zs> let buggyFunction = (arr) => {
...   let result = []
...   for (let i = 0; i <= arr.length; i++) {  // Bug: <= instead of <
...     result.push(arr[i] * 2)
...   }
...   return result
... }

zs> buggyFunction([1, 2, 3])
→ buggyFunction([1, 2, 3])
[2, 4, 6, NaN]  // NaN reveals the bug

zs> // Fix the function
zs> let fixedFunction = (arr) => {
...   let result = []
...   for (let i = 0; i < arr.length; i++) {  // Fixed: < instead of <=
...     result.push(arr[i] * 2)
...   }
...   return result
... }

zs> fixedFunction([1, 2, 3])
→ fixedFunction([1, 2, 3])
[2, 4, 6]  // Fixed!
```

### Variable Inspection

```
zs> let data = { users: [{ name: "Alice" }, { name: "Bob" }] }
→ const data = { users: [{ name: "Alice" }, { name: "Bob" }] };

zs> data.users
→ data.users
[{ name: "Alice" }, { name: "Bob" }]

zs> data.users[0]
→ data.users[0]
{ name: "Alice" }

zs> typeof data
→ typeof data
"object"
```

## Performance Profiling

### Timing Operations

```
zs> let timeIt = (fn) => {
...   let start = performance.now()
...   let result = fn()
...   let end = performance.now()
...   console.log(`Execution time: ${end - start}ms`)
...   return result
... }

zs> timeIt(() => Array.from({ length: 1000000 }, (_, i) => i * 2))
Execution time: 45.2ms
→ timeIt(() => Array.from({ length: 1000000 }, (_, i) => i * 2))
[0, 2, 4, 6, ...]
```

### Memory Usage

```
zs> let memoryUsage = () => {
...   if (typeof process !== 'undefined') {
...     let usage = process.memoryUsage()
...     console.log(`Memory: ${Math.round(usage.heapUsed / 1024 / 1024)}MB`)
...   }
... }

zs> memoryUsage()
Memory: 15MB
```

## Tips and Tricks

### 1. Use the REPL for Learning

```
zs> // Experiment with new language features
zs> let pipeline = [1, 2, 3, 4, 5]
...   |> filter(x => x % 2 === 0)
...   |> map(x => x * x)
...   |> reduce((sum, x) => sum + x, 0)

zs> pipeline
→ [1, 2, 3, 4, 5].filter(x => x % 2 === 0).map(x => x * x).reduce((sum, x) => sum + x, 0)
20
```

### 2. Quick Function Testing

```
zs> let isPalindrome = (str) => {
...   let cleaned = str.toLowerCase().replace(/[^a-z]/g, '')
...   return cleaned === cleaned.split('').reverse().join('')
... }

zs> isPalindrome("A man a plan a canal Panama")
→ isPalindrome("A man a plan a canal Panama")
true

zs> isPalindrome("race a car")
→ isPalindrome("race a car")
false
```

### 3. Interactive Documentation

```
zs> // The REPL shows you exactly what your code compiles to
zs> let example = match :status { :ok => "good", _ => "bad" }
→ const example = (() => {
  const __match_value = Symbol.for("status");
  if (__match_value === Symbol.for("ok")) {
    return "good";
  } else {
    return "bad";
  }
})();
```

### 4. Rapid Prototyping

```
zs> // Quickly test API designs
zs> let api = {
...   users: {
...     get: (id) => ({ id, name: `User ${id}` }),
...     list: () => [1, 2, 3].map(id => api.users.get(id))
...   }
... }

zs> api.users.list()
→ api.users.list()
[
  { id: 1, name: "User 1" },
  { id: 2, name: "User 2" },
  { id: 3, name: "User 3" }
]
```

## Integration with Development Workflow

### Testing Ideas Quickly

Use the REPL to test concepts before writing full implementations:

```bash
# Test an algorithm in REPL first
zeno repl

# Then implement in a file
zeno my-algorithm.zs

# Back to REPL for refinement
zeno repl
```

### Learning from Examples

Copy and paste examples from documentation directly into the REPL:

```
zs> // From the docs - test immediately
zs> struct Point { x: number; y: number; }
zs> let origin = { x: 0, y: 0 }
zs> let distance = (p1, p2) => Math.sqrt((p1.x - p2.x) ** 2 + (p1.y - p2.y) ** 2)
zs> distance(origin, { x: 3, y: 4 })
```

The Zenoscript REPL is designed to make functional programming exploration intuitive and productive, providing immediate feedback and encouraging experimentation with the language's features.