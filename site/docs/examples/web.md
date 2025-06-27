# Web Development Examples

This page demonstrates how to use Zenoscript for web development, including DOM manipulation, event handling, and building interactive applications.

## DOM Manipulation

```zenoscript
// Simple DOM manipulation
let createCounter = () => {
  let count = 0
  let counterElement = document.createElement("div")
  counterElement.className = "counter"
  
  let display = document.createElement("span")
  display.textContent = count.toString()
  
  let incrementButton = document.createElement("button")
  incrementButton.textContent = "+"
  incrementButton.addEventListener("click", () => {
    count += 1
    display.textContent = count.toString()
  })
  
  let decrementButton = document.createElement("button")
  decrementButton.textContent = "-"
  decrementButton.addEventListener("click", () => {
    count -= 1
    display.textContent = count.toString()
  })
  
  counterElement.appendChild(decrementButton)
  counterElement.appendChild(display)
  counterElement.appendChild(incrementButton)
  
  return counterElement
}

// Usage
document.getElementById("app").appendChild(createCounter())
```

## Event Handling

```zenoscript
// Event delegation pattern
let setupTodoList = () => {
  let todoList = document.getElementById("todo-list")
  let todoInput = document.getElementById("todo-input")
  let todoForm = document.getElementById("todo-form")
  
  todoForm.addEventListener("submit", event => {
    event.preventDefault()
    let text = todoInput.value.trim()
    
    if (text) {
      let item = document.createElement("li")
      item.innerHTML = `
        <span class="todo-text">${text}</span>
        <button class="delete-btn">Delete</button>
        <button class="complete-btn">Complete</button>
      `
      todoList.appendChild(item)
      todoInput.value = ""
    }
  })
  
  // Event delegation
  todoList.addEventListener("click", event => {
    let target = event.target
    
    if (target.classList.contains("delete-btn")) {
      let listItem = target.closest("li")
      listItem.remove()
    } else if (target.classList.contains("complete-btn")) {
      let listItem = target.closest("li")
      let todoText = listItem.querySelector(".todo-text")
      todoText.classList.toggle("completed")
    }
  })
}
```

## Fetch API and Async Operations

```zenoscript
// Fetch data from an API
let fetchUsers = async () => {
  try {
    let response = await fetch("https://jsonplaceholder.typicode.com/users")
    
    if (!response.ok) {
      throw new Error(`HTTP error! Status: ${response.status}`)
    }
    
    let users = await response.json()
    return users
  } catch (error) {
    console.error("Error fetching users:", error)
    return []
  }
}

// Display users in the DOM
let displayUsers = async () => {
  let userList = document.getElementById("user-list")
  let users = await fetchUsers()
  
  users |> forEach(user => {
    let userElement = document.createElement("div")
    userElement.className = "user-card"
    userElement.innerHTML = `
      <h3>${user.name}</h3>
      <p>Email: ${user.email}</p>
      <p>Company: ${user.company.name}</p>
    `
    userList.appendChild(userElement)
  })
}
```

## Custom Components

```zenoscript
// Simple component system
struct Component {
  render: () => HTMLElement;
  mount: (HTMLElement) => void;
  unmount: () => void;
}

let createComponent = (renderFn): Component => {
  let element = null
  
  return {
    render: () => {
      element = renderFn()
      return element
    },
    mount: (container) => {
      if (!element) {
        element = renderFn()
      }
      container.appendChild(element)
    },
    unmount: () => {
      if (element && element.parentNode) {
        element.parentNode.removeChild(element)
      }
    }
  }
}

// Usage example
let userProfile = createComponent(() => {
  let container = document.createElement("div")
  container.className = "user-profile"
  
  container.innerHTML = `
    <img src="avatar.jpg" alt="User Avatar" />
    <h2>John Doe</h2>
    <p>Frontend Developer</p>
  `
  
  return container
})

// Mount the component
userProfile.mount(document.getElementById("app"))
```

## State Management

```zenoscript
// Simple state management
let createStore = <T>(initialState: T) => {
  let state = initialState
  let listeners = []
  
  return {
    getState: () => state,
    setState: (newState: T) => {
      state = newState
      listeners |> forEach(listener => listener(state))
    },
    subscribe: (listener) => {
      listeners.push(listener)
      return () => {
        listeners = listeners |> filter(l => l !== listener)
      }
    }
  }
}

// Usage example
let todoStore = createStore({
  todos: [],
  filter: "all"
})

let addTodo = (text) => {
  let state = todoStore.getState()
  todoStore.setState({
    ...state,
    todos: [...state.todos, { id: Date.now(), text, completed: false }]
  })
}

let toggleTodo = (id) => {
  let state = todoStore.getState()
  todoStore.setState({
    ...state,
    todos: state.todos |> map(todo => 
      todo.id === id ? { ...todo, completed: !todo.completed } : todo
    )
  })
}

// Subscribe to state changes
todoStore.subscribe(state => {
  renderTodoList(state.todos, state.filter)
})
```

## Next Steps

- Check out the [Basic Examples](/examples/basic)
- Explore [Advanced Examples](/examples/advanced)
- Read the [Language Reference](/docs/syntax)