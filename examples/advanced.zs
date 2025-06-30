// Advanced Zenoscript features - functional programming patterns

// Generic container types
struct Option<T> {
  value: T;
  isSome: boolean;
}

struct Result<T, E> {
  value: T;
  error: E;
  isOk: boolean;
}

// Advanced traits  
trait Functor<T> {
  map(fn: any): any;
}

trait Monad<T> {
  flatMap(fn: any): any;
  unit(value: any): any;
}

// State management with atoms
let appState = :idle

let stateTransition = match appState {
  :idle => :loading
  :loading => :success
  :success => :idle
  :error => :idle
  _ => :error
}

// Complex data processing 
struct User {
  id: number;
  name: string;
  email: string;
  isActive: boolean;
}

struct ValidationError {
  field: string;
  message: string;
}

// Create sample users
let user1 = { id: 1, name: "Alice", email: "alice@example.com", isActive: true }
let user2 = { id: 2, name: "Bob", email: "bob@test.org", isActive: false }
let user3 = { id: 3, name: "", email: "invalid", isActive: true }

// Pattern matching for validation
let validateName = (name) => {
  return match name.length {
    0 => :invalid_empty
    1 => :invalid_short
    _ => :valid
  }
}

let validateEmail = (email) => {
  return match email.includes("@") {
    true => :valid
    false => :invalid_format
  }
}

// User categorization
let categorizeUser = (user) => {
  let nameStatus = validateName user.name
  let emailStatus = validateEmail user.email
  
  return match nameStatus {
    :valid => match emailStatus {
      :valid => :premium_user
      _ => :standard_user
    }
    _ => :invalid_user
  }
}

// Process multiple users
let user1Category = categorizeUser user1
let user2Category = categorizeUser user2  
let user3Category = categorizeUser user3

// Output results
console.log "User categories:"
console.log user1Category
console.log user2Category
console.log user3Category

// Chained data transformations
let data = "zenoscript programming"
let processed = data |> toUpperCase |> trim |> console.log

// State transitions
let currentState = :idle
let nextState = match currentState {
  :idle => :loading
  :loading => :processing  
  :processing => :complete
  :complete => :idle
  _ => :error
}

console.log "State transition:"
console.log nextState

// Complex matching with multiple atoms
let systemStatus = :healthy
let userLoad = :medium

let recommendation = match systemStatus {
  :healthy => match userLoad {
    :low => :scale_down
    :medium => :maintain
    :high => :scale_up
    _ => :monitor
  }
  :degraded => :investigate
  :critical => :emergency_scale
  _ => :unknown_action
}