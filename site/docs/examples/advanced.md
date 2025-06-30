# Advanced Examples

Advanced patterns using Zenoscript's functional programming features. All examples are fully implemented and working in Zenoscript 0.1.5.

## Complex Pattern Matching

### State Management with Atoms

```zenoscript
// Define application states
let appState = :idle
let loadingState = :pending
let userRole = :admin

// Complex state transitions
let handleStateTransition = (currentState) => match currentState {
  :idle => match loadingState {
    :pending => :loading
    :complete => :ready
    _ => :idle
  }
  :loading => :processing
  :processing => :complete
  :complete => :idle
  _ => :error
}

// Multi-level state evaluation
let getPermissionLevel = (role, state) => match role {
  :admin => match state {
    :active => :full_access
    :pending => :limited_access
    _ => :no_access
  }
  :user => match state {
    :active => :read_only
    _ => :no_access
  }
  :guest => :public_only
  _ => :denied
}

let permissions = getPermissionLevel :admin :active
console.log permissions // :full_access
```

## Advanced Pipe Operations

### Data Transformation Pipelines

```zenoscript
// Complex string processing
let processUserInput = (input) => {
  return input 
    |> trim 
    |> toLowerCase 
    |> replace(" ", "_")
}

// Chained transformations
let formatMessage = (rawMessage) => {
  let processed = rawMessage |> trim |> toUpperCase
  let result = processed |> console.log
  return result
}

// Multi-step data processing
let userData = "  John Doe  "
let processedName = userData |> trim |> toUpperCase
let output = processedName |> console.log

// Nested pipe operations
let complexTransform = (data) => {
  let step1 = data |> toString |> trim
  let step2 = step1 |> toUpperCase
  let final = step2 |> console.log
  return final
}
```

## Struct-Based Data Modeling

### Complex Data Structures

```zenoscript
struct User {
  id: number;
  username: string;
  email: string;
  role: string;
  active: boolean;
}

struct Product {
  id: number;
  name: string;
  price: number;
  category: string;
  inStock: boolean;
}

struct Order {
  id: number;
  userId: number;
  productId: number;
  quantity: number;
  status: string;
}

// Create complex data
let user = {
  id: 1,
  username: "john_doe",
  email: "john@example.com", 
  role: "admin",
  active: true
}

let product = {
  id: 101,
  name: "laptop",
  price: 999.99,
  category: "electronics",
  inStock: true
}

let order = {
  id: 1001,
  userId: user.id,
  productId: product.id,
  quantity: 2,
  status: "pending"
}
```

## Advanced Conditional Logic

### Complex Business Rules

```zenoscript
// User access control
let checkUserAccess = (user, resource) => {
  if user.active {
    if user.role == "admin" {
      console.log "Full access granted"
    }
    if user.role == "user" {
      console.log "Limited access granted" 
    }
  }
  
  if !user.active {
    console.log "Access denied - inactive user"
  }
}

// Order processing logic
let processOrder = (order, product, user) => {
  if product.inStock {
    if user.active {
      let orderStatus = match order.status {
        "pending" => :processing
        "processing" => :shipped
        "shipped" => :delivered
        "delivered" => :complete
        _ => :error
      }
      
      console.log orderStatus
    }
  }
  
  if !product.inStock {
    console.log "Product out of stock"
  }
}

// Price calculation with business rules
let calculatePrice = (product, quantity, userRole) => {
  let basePrice = product.price * quantity
  
  let discount = match userRole {
    "admin" => 0.2
    "premium" => 0.15
    "standard" => 0.05
    _ => 0.0
  }
  
  let finalPrice = basePrice * (1 - discount)
  return finalPrice
}
```

## Validation and Error Handling

### Result Pattern Implementation

```zenoscript
// Result type using atoms
let validateUser = (userData) => {
  if userData.username == "" {
    return { type: :error, message: "Username required" }
  }
  
  if userData.email == "" {
    return { type: :error, message: "Email required" }
  }
  
  return { type: :success, data: userData }
}

// Chain validations
let validateAndProcess = (userData) => {
  let validationResult = validateUser userData
  
  let finalResult = match validationResult {
    { type: :success, data } => {
      console.log "User validated successfully"
      { type: :success, processedData: data }
    }
    { type: :error, message } => {
      console.log message
      validationResult
    }
  }
  
  return finalResult
}

// Usage
let testUser = { username: "john", email: "john@test.com" }
let result = validateAndProcess testUser
```

## State Machines with Pattern Matching

### Application State Management

```zenoscript
struct AppContext {
  currentState: string;
  userId: number;
  data: any;
}

// State machine implementation
let handleEvent = (context, event) => {
  let newState = match context.currentState {
    "idle" => match event {
      "start" => "loading"
      "reset" => "idle"
      _ => "idle"
    }
    "loading" => match event {
      "success" => "loaded"
      "error" => "error"
      "cancel" => "idle"
      _ => "loading"
    }
    "loaded" => match event {
      "refresh" => "loading"
      "clear" => "idle"
      _ => "loaded"
    }
    "error" => match event {
      "retry" => "loading"
      "reset" => "idle"
      _ => "error"
    }
    _ => "idle"
  }
  
  return { ...context, currentState: newState }
}

// Usage example
let initialContext = { 
  currentState: "idle", 
  userId: 1, 
  data: null 
}

let context1 = handleEvent initialContext "start"
console.log context1.currentState // "loading"

let context2 = handleEvent context1 "success" 
console.log context2.currentState // "loaded"
```

## Functional Composition Patterns

### Higher-Order Processing

```zenoscript
// Function composition using pipes
let sanitizeString = (str) => str |> trim |> toLowerCase

let validateString = (str) => {
  if str == "" {
    return { valid: false, error: "Empty string" }
  }
  return { valid: true, value: str }
}

let processInput = (input) => {
  let cleaned = sanitizeString input
  let validated = validateString cleaned
  
  let result = match validated {
    { valid: true, value } => { 
      status: :success, 
      data: value |> toUpperCase 
    }
    { valid: false, error } => { 
      status: :error, 
      message: error 
    }
  }
  
  return result
}

// Pipeline processing
let processingPipeline = (rawData) => {
  let step1 = rawData |> toString |> trim
  
  let step2 = match step1 {
    "" => { error: "No data" }
    data => { success: data |> toUpperCase }
  }
  
  let final = match step2 {
    { success: data } => data |> console.log
    { error: msg } => msg |> console.log
  }
  
  return final
}
```

## Real-World Application Example

### E-commerce Order Processing

```zenoscript
struct Customer {
  id: number;
  name: string;
  email: string;
  tier: string;
}

struct Item {
  id: number;
  name: string;
  price: number;
  category: string;
}

struct OrderRequest {
  customerId: number;
  itemId: number;
  quantity: number;
}

// Order processing system
let processOrderRequest = (request, customer, item) => {
  // Validate customer
  let customerStatus = match customer.tier {
    "premium" => :priority
    "standard" => :normal
    "trial" => :limited
    _ => :invalid
  }
  
  // Calculate pricing
  let basePrice = item.price * request.quantity
  let discount = match customer.tier {
    "premium" => 0.15
    "standard" => 0.05
    _ => 0.0
  }
  
  let finalPrice = basePrice * (1 - discount)
  
  // Determine processing priority
  let priority = match customerStatus {
    :priority => match item.category {
      "electronics" => :high
      "books" => :medium
      _ => :normal
    }
    :normal => :normal
    :limited => :low
    _ => :rejected
  }
  
  // Generate order result
  let orderResult = match priority {
    :rejected => {
      status: :rejected,
      reason: "Invalid customer tier"
    }
    _ => {
      status: :accepted,
      priority: priority,
      price: finalPrice,
      estimatedDelivery: match priority {
        :high => "1-2 days"
        :medium => "3-5 days"
        :normal => "5-7 days"
        :low => "7-10 days"
      }
    }
  }
  
  return orderResult
}

// Usage
let customer = { 
  id: 1, 
  name: "John Doe", 
  email: "john@example.com", 
  tier: "premium" 
}

let item = { 
  id: 101, 
  name: "Laptop", 
  price: 999.99, 
  category: "electronics" 
}

let request = { 
  customerId: 1, 
  itemId: 101, 
  quantity: 1 
}

let result = processOrderRequest request customer item
console.log result.status
console.log result.priority
```

These advanced examples demonstrate sophisticated patterns using only the core features available in Zenoscript 0.1.5: structs, traits, atoms, pattern matching, pipe operations, and optional syntax features.