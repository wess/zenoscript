# Bun Integration

Zenoscript is designed from the ground up to work seamlessly with Bun, the fast all-in-one JavaScript runtime. This integration provides exceptional performance, built-in bundling, and a superior developer experience.

## Why Bun?

Zenoscript chooses Bun as the primary runtime because:

- **Speed** - Bun is significantly faster than Node.js for most operations
- **Built-in bundler** - No need for separate build tools like Webpack or Vite
- **Native TypeScript** - First-class TypeScript support without compilation steps
- **Plugin system** - Perfect for Zenoscript's transpilation needs
- **Web APIs** - Modern web standards built-in (fetch, WebSocket, etc.)

## Installation and Setup

### Installing Bun

```bash
# Install Bun (if not already installed)
curl -fsSL https://bun.sh/install | bash

# Verify installation
bun --version
```

### Setting up Zenoscript with Bun

```bash
# Create a new project
zeno init my-zenoscript-project
cd my-zenoscript-project

# The init command automatically configures Bun integration
```

This creates:

```
my-zenoscript-project/
├── package.json          # Bun-optimized configuration
├── bunfig.toml           # Bun configuration with Zenoscript plugin
├── index.zs              # Main Zenoscript file
└── tsconfig.json         # TypeScript configuration
```

## Configuration

### bunfig.toml

The `bunfig.toml` file configures Bun to work with Zenoscript:

```toml
# Zenoscript plugin configuration
preload = ["zenoscript/preload"]

[plugins]
zenoscript = "zenoscript/plugin"

# Optional: configure transpiler options
[zenoscript]
target = "es2022"
strict = true
minify = false

# Bun runtime configuration
[run]
bun = true
hot = true

# Install configuration
[install]
registry = "https://registry.npmjs.org"
lockfile = true
```

### package.json

Optimized for Bun development:

```json
{
  "name": "my-zenoscript-project",
  "version": "1.0.0",
  "type": "module",
  "scripts": {
    "dev": "bun --hot index.zs",
    "build": "bun build index.zs --outdir dist",
    "start": "bun index.zs",
    "test": "bun test",
    "clean": "rm -rf dist"
  },
  "dependencies": {
    "zenoscript": "latest"
  },
  "devDependencies": {
    "@types/bun": "latest"
  }
}
```

## Development Workflow

### Hot Reloading

Bun provides instant hot reloading for Zenoscript files:

```bash
# Start development with hot reloading
bun --hot index.zs

# Or use the script
bun dev
```

Changes to `.zs` files are automatically:
1. Transpiled to TypeScript
2. Executed in Bun
3. Updated in real-time

### Running Zenoscript Files

```bash
# Direct execution
bun index.zs

# With arguments
bun index.zs --arg1 value1 --arg2 value2

# With environment variables
NODE_ENV=production bun index.zs
```

### Building for Production

```bash
# Build single file
bun build index.zs --outdir dist

# Build with minification
bun build index.zs --outdir dist --minify

# Build for specific target
bun build index.zs --outdir dist --target browser

# Build multiple entry points
bun build src/*.zs --outdir dist
```

## Plugin System

### How the Plugin Works

The Zenoscript plugin for Bun:

1. **File Detection** - Recognizes `.zs` files
2. **Transpilation** - Converts Zenoscript to TypeScript
3. **Type Checking** - Leverages TypeScript's type system
4. **Bundling** - Includes in Bun's bundle process

```typescript
// The plugin architecture
import { plugin } from "bun";

export const zenoscriptPlugin = plugin({
  name: "zenoscript",
  setup(build) {
    // Handle .zs files
    build.onLoad({ filter: /\.zs$/ }, async (args) => {
      const source = await Bun.file(args.path).text();
      const typescript = await transpileZenoscript(source);
      
      return {
        contents: typescript,
        loader: "ts"
      };
    });
  }
});
```

### Custom Plugin Configuration

```toml
# bunfig.toml - Advanced plugin configuration
[zenoscript]
# Transpiler options
target = "es2022"
strict = true
experimentalDecorators = true

# Output options
minify = false
sourceMaps = true
removeComments = false

# Path mappings
[zenoscript.paths]
"@/*" = ["src/*"]
"@components/*" = ["src/components/*"]
```

## Performance Optimizations

### Bun vs Node.js Performance

```zenoscript
// benchmark.zs - Performance comparison
let benchmark = async (name: string, fn: () => void, iterations: number = 1000) => {
  let start = performance.now()
  
  for (let i = 0; i < iterations; i++) {
    await fn()
  }
  
  let end = performance.now()
  let avg = (end - start) / iterations
  
  console.log(`${name}: ${avg.toFixed(2)}ms average`)
}

// File I/O benchmark
await benchmark("File read", async () => {
  let content = await Bun.file("large-file.txt").text()
}, 100)

// JSON parsing benchmark  
await benchmark("JSON parse", () => {
  JSON.parse('{"data": "' + "x".repeat(10000) + '"}')
}, 1000)
```

Results typically show:
- **File I/O**: 2-3x faster than Node.js
- **JSON operations**: 1.5-2x faster
- **HTTP requests**: 2-4x faster
- **Startup time**: 3-4x faster

### Memory Usage

Bun's efficient memory management benefits Zenoscript:

```zenoscript
// Memory monitoring
let monitorMemory = () => {
  let usage = process.memoryUsage()
  
  console.log({
    heap: `${Math.round(usage.heapUsed / 1024 / 1024)}MB`,
    external: `${Math.round(usage.external / 1024 / 1024)}MB`,
    rss: `${Math.round(usage.rss / 1024 / 1024)}MB`
  })
}

// Bun typically uses 30-50% less memory than Node.js
```

## Web Development

### HTTP Server

```zenoscript
// server.zs - Web server with Bun
import { serve } from "bun"

struct ApiResponse<T> {
  success: boolean;
  data?: T;
  error?: string;
}

let createResponse = <T>(data: T): ApiResponse<T> => ({
  success: true,
  data
})

let createError = (error: string): ApiResponse<never> => ({
  success: false,
  error
})

serve({
  port: 3000,
  
  fetch(req) {
    let url = new URL(req.url)
    
    return match [req.method, url.pathname] {
      ["GET", "/api/health"] => 
        Response.json(createResponse({ status: "healthy" }))
      
      ["GET", "/api/users"] => 
        Response.json(createResponse([
          { id: 1, name: "Alice" },
          { id: 2, name: "Bob" }
        ]))
      
      ["POST", "/api/users"] => 
        req.json()
          |> (data => createResponse({ id: Date.now(), ...data }))
          |> Response.json
      
      _ => 
        Response.json(createError("Route not found"), { status: 404 })
    }
  },
  
  error(error) {
    return new Response(`Error: ${error.message}`, { status: 500 })
  }
})

console.log("Server running on http://localhost:3000")
```

### WebSocket Support

```zenoscript
// websocket.zs - Real-time communication
import { serve } from "bun"

type ClientMessage = 
  | { type: "join"; room: string }
  | { type: "message"; content: string }
  | { type: "leave" }

type ServerMessage = 
  | { type: "joined"; room: string }
  | { type: "message"; user: string; content: string }
  | { type: "error"; message: string }

let rooms = new Map<string, Set<WebSocket>>()

serve({
  port: 3000,
  
  websocket: {
    open(ws) {
      ws.data = { room: null, id: crypto.randomUUID() }
      console.log(`Client ${ws.data.id} connected`)
    },
    
    message(ws, message) {
      let data: ClientMessage = JSON.parse(message.toString())
      
      match data {
        { type: "join", room } => {
          // Leave current room
          if (ws.data.room) {
            rooms.get(ws.data.room)?.delete(ws)
          }
          
          // Join new room
          if (!rooms.has(room)) {
            rooms.set(room, new Set())
          }
          rooms.get(room)!.add(ws)
          ws.data.room = room
          
          ws.send(JSON.stringify({ type: "joined", room }))
        }
        
        { type: "message", content } => {
          if (!ws.data.room) return
          
          let message: ServerMessage = {
            type: "message",
            user: ws.data.id,
            content
          }
          
          // Broadcast to room
          rooms.get(ws.data.room)?.forEach(client => {
            if (client !== ws) {
              client.send(JSON.stringify(message))
            }
          })
        }
        
        { type: "leave" } => {
          if (ws.data.room) {
            rooms.get(ws.data.room)?.delete(ws)
            ws.data.room = null
          }
        }
      }
    },
    
    close(ws) {
      if (ws.data.room) {
        rooms.get(ws.data.room)?.delete(ws)
      }
      console.log(`Client ${ws.data.id} disconnected`)
    }
  }
})
```

## Database Integration

### Built-in SQLite

```zenoscript
// database.zs - SQLite with Bun
import { Database } from "bun:sqlite"

struct User {
  id?: number;
  name: string;
  email: string;
  createdAt?: string;
}

let db = new Database("users.db")

// Create table
db.run(`
  CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    email TEXT UNIQUE NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
  )
`)

// Repository pattern
struct UserRepository {
  private db: Database;
}

impl UserRepository {
  constructor(database: Database) {
    this.db = database
  }
  
  create(user: Omit<User, "id" | "createdAt">): User {
    let stmt = this.db.prepare(`
      INSERT INTO users (name, email) 
      VALUES (?, ?) 
      RETURNING *
    `)
    
    return stmt.get(user.name, user.email) as User
  }
  
  findById(id: number): User | null {
    let stmt = this.db.prepare("SELECT * FROM users WHERE id = ?")
    return stmt.get(id) as User | null
  }
  
  findByEmail(email: string): User | null {
    let stmt = this.db.prepare("SELECT * FROM users WHERE email = ?")
    return stmt.get(email) as User | null
  }
  
  findAll(): User[] {
    let stmt = this.db.prepare("SELECT * FROM users ORDER BY created_at DESC")
    return stmt.all() as User[]
  }
  
  update(id: number, updates: Partial<User>): User | null {
    let fields = Object.keys(updates)
      .map(key => `${key} = ?`)
      .join(", ")
    
    let values = Object.values(updates)
    
    let stmt = this.db.prepare(`
      UPDATE users 
      SET ${fields} 
      WHERE id = ? 
      RETURNING *
    `)
    
    return stmt.get(...values, id) as User | null
  }
  
  delete(id: number): boolean {
    let stmt = this.db.prepare("DELETE FROM users WHERE id = ?")
    let result = stmt.run(id)
    return result.changes > 0
  }
}

// Usage
let userRepo = new UserRepository(db)

let newUser = userRepo.create({
  name: "Alice",
  email: "alice@example.com"
})

console.log("Created user:", newUser)
```

## Testing with Bun

### Built-in Test Runner

```zenoscript
// user.test.zs - Testing with Bun's test runner
import { test, expect, describe, beforeEach } from "bun:test"

struct Calculator {
  private value: number = 0;
}

impl Calculator {
  add(n: number): Calculator {
    return { value: this.value + n }
  }
  
  subtract(n: number): Calculator {
    return { value: this.value - n }
  }
  
  multiply(n: number): Calculator {
    return { value: this.value * n }
  }
  
  divide(n: number): Calculator {
    if (n === 0) throw new Error("Division by zero")
    return { value: this.value / n }
  }
  
  getValue(): number {
    return this.value
  }
  
  reset(): Calculator {
    return { value: 0 }
  }
}

describe("Calculator", () => {
  let calc: Calculator
  
  beforeEach(() => {
    calc = { value: 0 }
  })
  
  test("should add numbers correctly", () => {
    let result = calc.add(5).add(3).getValue()
    expect(result).toBe(8)
  })
  
  test("should subtract numbers correctly", () => {
    let result = calc.add(10).subtract(3).getValue()
    expect(result).toBe(7)
  })
  
  test("should chain operations", () => {
    let result = calc
      .add(10)
      .multiply(2)
      .subtract(5)
      .divide(3)
      .getValue()
    
    expect(result).toBeCloseTo(5)
  })
  
  test("should throw on division by zero", () => {
    expect(() => calc.divide(0)).toThrow("Division by zero")
  })
})
```

Run tests:

```bash
# Run all tests
bun test

# Run specific test file
bun test user.test.zs

# Run with coverage
bun test --coverage

# Watch mode
bun test --watch
```

## Deployment

### Building for Production

```bash
# Build optimized bundle
bun build src/index.zs --outdir dist --minify --target bun

# Build for Node.js compatibility
bun build src/index.zs --outdir dist --target node

# Build for browser
bun build src/index.zs --outdir dist --target browser
```

### Docker Deployment

```dockerfile
# Dockerfile
FROM oven/bun:1 as builder

WORKDIR /app
COPY package.json bunfig.toml ./
RUN bun install --frozen-lockfile

COPY . .
RUN bun build src/index.zs --outdir dist --minify

FROM oven/bun:1-slim
WORKDIR /app
COPY --from=builder /app/dist ./dist
COPY --from=builder /app/package.json ./

EXPOSE 3000
CMD ["bun", "dist/index.js"]
```

### Environment Configuration

```zenoscript
// config.zs - Environment-aware configuration
struct Config {
  port: number;
  database: {
    host: string;
    port: number;
    name: string;
  };
  jwt: {
    secret: string;
    expiresIn: string;
  };
  cors: {
    origins: string[];
  };
}

let loadConfig = (): Config => {
  let env = process.env.NODE_ENV || "development"
  
  return match env {
    "production" => ({
      port: parseInt(process.env.PORT || "3000"),
      database: {
        host: process.env.DB_HOST || "localhost",
        port: parseInt(process.env.DB_PORT || "5432"),
        name: process.env.DB_NAME || "production_db"
      },
      jwt: {
        secret: process.env.JWT_SECRET!,
        expiresIn: "24h"
      },
      cors: {
        origins: process.env.CORS_ORIGINS?.split(",") || []
      }
    })
    
    "test" => ({
      port: 0, // Random port
      database: {
        host: "localhost",
        port: 5432,
        name: "test_db"
      },
      jwt: {
        secret: "test-secret",
        expiresIn: "1h"
      },
      cors: {
        origins: ["http://localhost:3000"]
      }
    })
    
    _ => ({ // development
      port: 3000,
      database: {
        host: "localhost",
        port: 5432,
        name: "dev_db"
      },
      jwt: {
        secret: "dev-secret",
        expiresIn: "7d"
      },
      cors: {
        origins: ["http://localhost:3000", "http://localhost:5173"]
      }
    })
  }
}

export let config = loadConfig()
```

## Best Practices

### 1. Use Bun APIs When Available

```zenoscript
// Good - use Bun's optimized file API
let content = await Bun.file("data.json").json()

// Avoid - slower fs module
import { readFileSync } from "fs"
let content = JSON.parse(readFileSync("data.json", "utf8"))
```

### 2. Leverage Built-in Performance

```zenoscript
// Good - use Bun's fast HTTP client
let response = await fetch("https://api.example.com/data")
let data = await response.json()

// Avoid - additional dependencies
import axios from "axios"
let { data } = await axios.get("https://api.example.com/data")
```

### 3. Optimize Bundle Size

```bash
# Analyze bundle
bun build --analyze src/index.zs

# Tree shake unused code
bun build --minify --dead-code-elimination src/index.zs
```

### 4. Use TypeScript Integration

```zenoscript
// Good - leverage TypeScript types
interface ApiResponse<T> {
  data: T;
  status: number;
}

let fetchApi = async <T>(url: string): Promise<ApiResponse<T>> => {
  let response = await fetch(url)
  return await response.json()
}

// Full type safety with Zenoscript + TypeScript + Bun
```

## Troubleshooting

### Common Issues

**Plugin not loading:**
```bash
# Check bunfig.toml configuration
cat bunfig.toml

# Reinstall Zenoscript
bun remove zenoscript && bun add zenoscript

# Clear Bun cache
rm -rf ~/.bun/install/cache
```

**Slow startup:**
```bash
# Check for large dependencies
bun pm ls

# Use Bun's faster package manager
rm package-lock.json yarn.lock
bun install
```

**Type errors:**
```bash
# Update TypeScript definitions
bun add -d @types/bun

# Check tsconfig.json
cat tsconfig.json
```

The integration between Zenoscript and Bun provides a modern, fast, and developer-friendly environment for functional programming in the JavaScript ecosystem.