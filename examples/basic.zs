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

// Trait declaration
trait Serializable {
  serialize(): string;
  deserialize(data: string): Self;
}

trait Displayable {
  display(): string;
}

// Implementation blocks
impl User {
  new(name: string, email: string, age: number) {
    return { name, email, age };
  }
  
  isAdult() {
    return this.age >= 18;
  }
}

impl Serializable for User {
  serialize() {
    return JSON.stringify(this);
  }
  
  deserialize(data: string) {
    return JSON.parse(data);
  }
}

impl Displayable for User {
  display() {
    return `${this.name} (${this.email})`;
  }
}

// Let bindings and pipe expressions
let user = User.new("Alice", "alice@example.com", 25)
let userJson = user |> serialize
let displayStr = user |> display

// Atoms and pattern matching
let status = :loading

let statusMessage = match status {
  :idle => "Ready to start"
  :loading => "Please wait..."
  :success => "Operation completed"
  :error => "Something went wrong"
  _ => "Unknown status"
}

// More complex pattern matching with guards
let userStatus = match user.age {
  age when age < 13 => :child
  age when age < 18 => :teenager
  age when age < 65 => :adult
  _ => :senior
}

// Chained pipe operations
let processedData = "  Hello World  " 
  |> trim 
  |> toUpperCase 
  |> console.log

// Generic usage
let apiResponse = {
  status: 200,
  data: user,
  message: "User retrieved successfully"
}

let response = apiResponse |> serialize