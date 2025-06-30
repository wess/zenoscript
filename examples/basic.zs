// Basic Zenoscript example showcasing core language features

// Struct declarations
struct User {
  name: string;
  email: string;
  age: number;
}

struct ApiResponse<T> {
  status: number;
  data: T;
  message: string;
}

// Trait declarations
trait Serializable {
  serialize(): string;
  deserialize(data: string): any;
}

trait Displayable {
  display(): string;
}

// Let bindings and values
let userName = "Alice"
let userEmail = "alice@example.com"
let userAge = 25

// Create user object
let user = { name: userName, email: userEmail, age: userAge }

// Atoms and pattern matching
let status = :loading

let statusMessage = match status {
  :idle => "Ready to start"
  :loading => "Please wait..."
  :success => "Operation completed"
  :error => "Something went wrong"
  _ => "Unknown status"
}

// Simple pipe operations
let greeting = "  Hello World  " |> trim |> toUpperCase
let result = greeting |> console.log

// More pipe examples
let data = "zenoscript"
let processed = data |> toUpperCase |> console.log

// Object creation and JSON
let apiResponse = {
  status: 200,
  data: user,
  message: "User retrieved successfully"
}

// Function calls with optional parentheses
console.log "Status message:"
console.log statusMessage

// Conditional logic with simplified if
if userAge >= 18 {
  console.log "User is an adult"
}

if status == :loading {
  console.log "Currently loading..."
}