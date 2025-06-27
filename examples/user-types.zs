// Zenoscript file that could be imported directly in TypeScript
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

trait Validatable {
  validate(): boolean;
}

impl Validatable for User {
  validate() {
    return this.name.length > 0 && this.email.includes("@") && this.age >= 0;
  }
}

let createUser = (name: string, email: string, age: number) => {
  return { name, email, age };
}

let isAdult = (user: User) => user.age >= 18