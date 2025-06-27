# Examples

Explore real-world examples of Zenoscript code to see how functional programming concepts work in practice.

## Featured Examples

### 🚀 Quick Start
Basic Zenoscript syntax and concepts for newcomers.
[View Example →](/examples/basic)

### 🔄 Advanced Patterns
Advanced functional programming patterns and techniques.
[View Example →](/examples/advanced)

### 🌐 Web Development
Building web applications with Zenoscript and modern frameworks.
[View Example →](/examples/advanced#concurrent-programming)

## Code Snippets

### Data Processing Pipeline

```zenoscript
struct Product {
  id: number;
  name: string;
  price: number;
  category: string;
  inStock: boolean;
}

let products = [
  { id: 1, name: "Laptop", price: 999.99, category: "electronics", inStock: true },
  { id: 2, name: "Book", price: 19.99, category: "books", inStock: false },
  { id: 3, name: "Phone", price: 699.99, category: "electronics", inStock: true }
]

// Filter and transform products using pipes
let availableElectronics = products
  |> filter(p => p.inStock && p.category === "electronics")
  |> map(p => ({ ...p, discountPrice: p.price * 0.9 }))
  |> sortBy(p => p.price)

console.log(availableElectronics)
```

### HTTP API Handler

```zenoscript
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

let handleRequest = (req: Request) => {
  let response = match req.url {
    "/api/health" => createResponse({ status: "healthy" })
    "/api/users" => getUsersHandler(req)
    _ => createError("Route not found")
  }
  
  return new Response(JSON.stringify(response), {
    headers: { "Content-Type": "application/json" }
  })
}

serve({
  port: 3000,
  fetch: handleRequest
})
```

### State Machine

```zenoscript
// Define states as atoms
let :idle = Symbol.for("idle")
let :loading = Symbol.for("loading")  
let :success = Symbol.for("success")
let :error = Symbol.for("error")

struct AppState {
  status: symbol;
  data?: any;
  error?: string;
}

let transition = (state: AppState, action: string) => match [state.status, action] {
  [:idle, "FETCH"] => { ...state, status: :loading }
  [:loading, "SUCCESS"] => { status: :success, data: action.payload }
  [:loading, "ERROR"] => { status: :error, error: action.message }
  [_, "RESET"] => { status: :idle }
  _ => state  // No transition
}

let reducer = (state: AppState = { status: :idle }, action) => 
  transition(state, action)
```

### Form Validation

```zenoscript
struct ValidationError {
  field: string;
  message: string;
}

struct User {
  email: string;
  password: string;
  age: number;
}

let validateEmail = (email: string) => 
  email.includes("@") ? :valid : :invalid("Invalid email format")

let validatePassword = (password: string) =>
  password.length >= 8 ? :valid : :invalid("Password too short")

let validateAge = (age: number) =>
  age >= 18 ? :valid : :invalid("Must be 18 or older")

let validateUser = (user: User) => {
  let errors = []
  
  match validateEmail(user.email) {
    :invalid(msg) => errors.push({ field: "email", message: msg })
    _ => {}
  }
  
  match validatePassword(user.password) {
    :invalid(msg) => errors.push({ field: "password", message: msg })
    _ => {}
  }
  
  match validateAge(user.age) {
    :invalid(msg) => errors.push({ field: "age", message: msg })
    _ => {}
  }
  
  return errors.length === 0 ? :valid : :invalid(errors)
}
```

## Interactive Examples

Try these examples in the [REPL](/docs/repl):

### Pattern Matching with Lists

```zenoscript
let processList = (items) => match items {
  [] => "Empty list"
  [head] => `Single item: ${head}`
  [first, second] => `Two items: ${first}, ${second}`
  [first, ...rest] => `First: ${first}, ${rest.length} more`
}

// Try it:
// processList([])
// processList(["apple"])
// processList(["apple", "banana"])
// processList(["apple", "banana", "cherry", "date"])
```

### Functional Option Type

```zenoscript
// Option type implementation
let some = (value) => ({ type: :some, value })
let none = () => ({ type: :none })

let map = (option, fn) => match option {
  { type: :some, value } => some(fn(value))
  { type: :none } => none()
}

let flatMap = (option, fn) => match option {
  { type: :some, value } => fn(value)
  { type: :none } => none()
}

// Usage
let result = some(42)
  |> map(x => x * 2)
  |> map(x => x.toString())
  |> flatMap(s => s.length > 2 ? some(s) : none())
```

## Community Examples

Check out more examples from the community:

- [GitHub Repository Examples](https://github.com/zenoscript/zenoscript/tree/main/examples)
- [Community Showcase](https://github.com/zenoscript/awesome-zenoscript)
- [Tutorial Projects](https://github.com/zenoscript/learn-zenoscript)

## Contributing Examples

Have a great Zenoscript example? We'd love to feature it!

1. Fork the [repository](https://github.com/zenoscript/zenoscript)
2. Add your example to `examples/`
3. Include documentation and tests
4. Submit a pull request

See our [GitHub Repository](https://github.com/zenoscript/zenoscript) for details.