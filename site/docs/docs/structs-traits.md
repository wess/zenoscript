# Structs & Traits

Structs and traits are Zenoscript's primary tools for organizing data and behavior. Structs define data structures, while traits define interfaces and contracts that types can implement.

## Structs

### Basic Struct Declaration

```zenoscript
// Simple struct
struct User {
  id: string;
  name: string;
  email: string;
  age: number;
}

// Creating struct instances
let user: User = {
  id: "123",
  name: "Alice",
  email: "alice@example.com",
  age: 30
}

// Struct with optional fields
struct Config {
  host: string;
  port?: number;
  ssl?: boolean;
  timeout?: number;
}

let config: Config = {
  host: "localhost"
  // Optional fields can be omitted
}
```

### Generic Structs

```zenoscript
// Generic struct
struct Container<T> {
  value: T;
  timestamp: Date;
  metadata?: any;
}

// Usage with different types
let stringContainer: Container<string> = {
  value: "hello",
  timestamp: new Date()
}

let numberContainer: Container<number> = {
  value: 42,
  timestamp: new Date(),
  metadata: { source: "api" }
}

// Multiple type parameters
struct KeyValue<K, V> {
  key: K;
  value: V;
  created: Date;
}

let kvPair: KeyValue<string, number> = {
  key: "count",
  value: 100,
  created: new Date()
}
```

### Nested Structs

```zenoscript
struct Address {
  street: string;
  city: string;
  state: string;
  zipCode: string;
  country: string;
}

struct Person {
  name: string;
  age: number;
  address: Address;
  contacts: {
    email?: string;
    phone?: string;
  };
}

let person: Person = {
  name: "Bob",
  age: 25,
  address: {
    street: "123 Main St",
    city: "Springfield",
    state: "IL",
    zipCode: "62701",
    country: "USA"
  },
  contacts: {
    email: "bob@example.com"
  }
}
```

### Struct Inheritance and Composition

```zenoscript
// Base struct
struct Entity {
  id: string;
  createdAt: Date;
  updatedAt: Date;
}

// Extending with intersection types
struct User = Entity & {
  name: string;
  email: string;
  role: "admin" | "user" | "guest";
}

// Composition with embedded structs
struct BlogPost {
  title: string;
  content: string;
  author: User;
  tags: string[];
  metadata: Entity;
}
```

## Traits

### Basic Trait Declaration

```zenoscript
// Simple trait
trait Drawable {
  draw(): void;
  isVisible(): boolean;
}

// Trait with default methods
trait Logger {
  log(message: string): void;
  
  // Default implementation
  logError(error: string): void {
    this.log(`ERROR: ${error}`)
  }
  
  logInfo(info: string): void {
    this.log(`INFO: ${info}`)
  }
}

// Trait with properties
trait Identifiable {
  id: string;
  getName(): string;
}
```

### Generic Traits

```zenoscript
// Generic trait
trait Serializable<T> {
  serialize(): T;
  deserialize(data: T): this;
}

// Generic trait with constraints
trait Comparable<T extends Comparable<T>> {
  compareTo(other: T): number;
  equals(other: T): boolean;
}

// Generic trait with multiple parameters
trait Converter<From, To> {
  convert(input: From): To;
  canConvert(input: From): boolean;
}
```

### Trait Inheritance

```zenoscript
// Base trait
trait Shape {
  area(): number;
  perimeter(): number;
}

// Extending traits
trait ColoredShape extends Shape {
  color: string;
  setColor(color: string): void;
}

// Multiple trait inheritance
trait MovableShape extends Shape {
  x: number;
  y: number;
  move(dx: number, dy: number): void;
}

trait AnimatedShape extends ColoredShape, MovableShape {
  animate(): void;
}
```

## Implementation Blocks

### Basic Implementation

```zenoscript
struct Circle {
  radius: number;
  center: { x: number; y: number };
}

// Implementing methods for a struct
impl Circle {
  // Constructor-like static method
  static create(radius: number, x: number = 0, y: number = 0): Circle {
    return {
      radius,
      center: { x, y }
    }
  }
  
  // Instance methods
  area(): number {
    return Math.PI * this.radius * this.radius
  }
  
  perimeter(): number {
    return 2 * Math.PI * this.radius
  }
  
  move(dx: number, dy: number): void {
    this.center.x += dx
    this.center.y += dy
  }
}

// Usage
let circle = Circle.create(5, 10, 20)
let area = circle.area()
circle.move(5, -5)
```

### Trait Implementation

```zenoscript
// Implementing a trait for a struct
impl Drawable for Circle {
  draw(): void {
    console.log(`Drawing circle at (${this.center.x}, ${this.center.y}) with radius ${this.radius}`)
  }
  
  isVisible(): boolean {
    return this.radius > 0
  }
}

impl Serializable<string> for Circle {
  serialize(): string {
    return JSON.stringify({
      type: "circle",
      radius: this.radius,
      center: this.center
    })
  }
  
  deserialize(data: string): Circle {
    let parsed = JSON.parse(data)
    return {
      radius: parsed.radius,
      center: parsed.center
    }
  }
}

// Usage
let circle = Circle.create(5)
circle.draw()
let serialized = circle.serialize()
```

### Generic Implementation

```zenoscript
struct List<T> {
  items: T[];
}

impl<T> List<T> {
  static empty<T>(): List<T> {
    return { items: [] }
  }
  
  static from<T>(items: T[]): List<T> {
    return { items: [...items] }
  }
  
  push(item: T): List<T> {
    return { items: [...this.items, item] }
  }
  
  pop(): [T | undefined, List<T>] {
    if (this.items.length === 0) {
      return [undefined, this]
    }
    
    let last = this.items[this.items.length - 1]
    let rest = this.items.slice(0, -1)
    return [last, { items: rest }]
  }
  
  map<U>(fn: (item: T) => U): List<U> {
    return { items: this.items.map(fn) }
  }
  
  filter(predicate: (item: T) => boolean): List<T> {
    return { items: this.items.filter(predicate) }
  }
}

// Generic trait implementation
impl<T> Serializable<T[]> for List<T> {
  serialize(): T[] {
    return [...this.items]
  }
  
  deserialize(data: T[]): List<T> {
    return List.from(data)
  }
}
```

## Advanced Patterns

### Associated Types

```zenoscript
trait Iterator {
  type Item;
  
  next(): Option<this.Item>;
  hasNext(): boolean;
}

struct NumberIterator {
  current: number;
  end: number;
}

impl Iterator for NumberIterator {
  type Item = number;
  
  next(): Option<number> {
    if (this.current <= this.end) {
      let value = this.current
      this.current++
      return { type: :some, value }
    }
    return { type: :none }
  }
  
  hasNext(): boolean {
    return this.current <= this.end
  }
}
```

### Trait Objects

```zenoscript
// Using traits as types
let shapes: Drawable[] = [
  Circle.create(5),
  Rectangle.create(10, 20),
  Triangle.create(3, 4, 5)
]

// All shapes must implement Drawable
shapes.forEach(shape => shape.draw())

// Function accepting trait objects
let drawAll = (drawables: Drawable[]) => {
  drawables
    .filter(d => d.isVisible())
    .forEach(d => d.draw())
}
```

### Conditional Implementation

```zenoscript
// Implement trait only for certain types
impl<T> Comparable<List<T>> for List<T> where T: Comparable<T> {
  compareTo(other: List<T>): number {
    let minLength = Math.min(this.items.length, other.items.length)
    
    for (let i = 0; i < minLength; i++) {
      let cmp = this.items[i].compareTo(other.items[i])
      if (cmp !== 0) return cmp
    }
    
    return this.items.length - other.items.length
  }
  
  equals(other: List<T>): boolean {
    return this.compareTo(other) === 0
  }
}
```

## Design Patterns

### Builder Pattern

```zenoscript
struct HttpRequest {
  url: string;
  method: "GET" | "POST" | "PUT" | "DELETE";
  headers: Record<string, string>;
  body?: string;
}

impl HttpRequest {
  static builder(): HttpRequestBuilder {
    return new HttpRequestBuilder()
  }
}

struct HttpRequestBuilder {
  private url?: string;
  private method: "GET" | "POST" | "PUT" | "DELETE" = "GET";
  private headers: Record<string, string> = {};
  private body?: string;
}

impl HttpRequestBuilder {
  setUrl(url: string): this {
    this.url = url
    return this
  }
  
  setMethod(method: "GET" | "POST" | "PUT" | "DELETE"): this {
    this.method = method
    return this
  }
  
  addHeader(key: string, value: string): this {
    this.headers[key] = value
    return this
  }
  
  setBody(body: string): this {
    this.body = body
    return this
  }
  
  build(): HttpRequest {
    if (!this.url) {
      throw new Error("URL is required")
    }
    
    return {
      url: this.url,
      method: this.method,
      headers: { ...this.headers },
      body: this.body
    }
  }
}

// Usage
let request = HttpRequest.builder()
  .setUrl("https://api.example.com/users")
  .setMethod("POST")
  .addHeader("Content-Type", "application/json")
  .setBody(JSON.stringify({ name: "Alice" }))
  .build()
```

### Visitor Pattern

```zenoscript
trait Visitor<T> {
  visitNumber(value: number): T;
  visitString(value: string): T;
  visitArray(value: any[]): T;
  visitObject(value: object): T;
}

trait Visitable {
  accept<T>(visitor: Visitor<T>): T;
}

struct JsonValue {
  value: any;
}

impl Visitable for JsonValue {
  accept<T>(visitor: Visitor<T>): T {
    return match this.value {
      v when typeof v === "number" => visitor.visitNumber(v)
      v when typeof v === "string" => visitor.visitString(v)
      v when Array.isArray(v) => visitor.visitArray(v)
      v when typeof v === "object" => visitor.visitObject(v)
      _ => throw new Error("Unknown type")
    }
  }
}

// Concrete visitor
struct JsonPrinter {
  private indent: number = 0;
}

impl Visitor<string> for JsonPrinter {
  visitNumber(value: number): string {
    return value.toString()
  }
  
  visitString(value: string): string {
    return `"${value}"`
  }
  
  visitArray(value: any[]): string {
    let items = value.map(item => 
      JsonValue({ value: item }).accept(this)
    )
    return `[${items.join(", ")}]`
  }
  
  visitObject(value: object): string {
    let entries = Object.entries(value).map(([k, v]) =>
      `"${k}": ${JsonValue({ value: v }).accept(this)}`
    )
    return `{${entries.join(", ")}}`
  }
}
```

## Best Practices

### 1. Prefer Composition over Inheritance

```zenoscript
// Good - composition
struct User {
  profile: UserProfile;
  permissions: Permissions;
  settings: UserSettings;
}

// Avoid - deep inheritance
struct AdminUser extends SuperUser extends User {
  // Complex inheritance hierarchy
}
```

### 2. Keep Traits Focused

```zenoscript
// Good - single responsibility
trait Readable {
  read(): string;
}

trait Writable {
  write(data: string): void;
}

// Can combine when needed
trait ReadWritable extends Readable, Writable {}

// Avoid - too many responsibilities
trait FileOperations {
  read(): string;
  write(data: string): void;
  delete(): void;
  copy(dest: string): void;
  move(dest: string): void;
  compress(): void;
  encrypt(): void;
}
```

### 3. Use Default Implementations

```zenoscript
// Good - provide sensible defaults
trait Cache<K, V> {
  get(key: K): Option<V>;
  set(key: K, value: V): void;
  
  // Default implementations
  getOrDefault(key: K, defaultValue: V): V {
    return match this.get(key) {
      { type: :some, value } => value
      { type: :none } => defaultValue
    }
  }
  
  has(key: K): boolean {
    return match this.get(key) {
      { type: :some } => true
      { type: :none } => false
    }
  }
}
```

### 4. Design for Immutability

```zenoscript
// Good - immutable operations
struct Point {
  x: number;
  y: number;
}

impl Point {
  move(dx: number, dy: number): Point {
    return {
      x: this.x + dx,
      y: this.y + dy
    }
  }
  
  scale(factor: number): Point {
    return {
      x: this.x * factor,
      y: this.y * factor
    }
  }
}

// Avoid - mutating operations
impl Point {
  moveInPlace(dx: number, dy: number): void {
    this.x += dx  // Mutation
    this.y += dy  // Mutation
  }
}
```

Structs and traits in Zenoscript provide a powerful foundation for organizing code in a functional programming style while maintaining the flexibility and expressiveness needed for complex applications.