---
layout: home

hero:
  name: "Zenoscript"
  text: "Fast functional programming for TypeScript"
  tagline: "A modern language that brings functional programming paradigms to the TypeScript ecosystem with zero runtime overhead"
  image:
    src: /zenoscript_mascot.svg
    alt: Zenoscript Mascot
  actions:
    - theme: brand
      text: Get Started
      link: /docs/
    - theme: alt
      text: View on GitHub
      link: https://github.com/wess/zenoscript

features:
  - icon: ⚡
    title: Blazing Fast
    details: Compiles directly to optimized TypeScript with zero runtime overhead. Built with performance in mind.
  
  - icon: 🔧
    title: Functional First
    details: Pattern matching, immutable data structures, and functional programming patterns built into the language.
  
  - icon: 🛠️
    title: TypeScript Native
    details: Seamless integration with existing TypeScript projects. Use any TypeScript library without hassle.
  
  - icon: 🎯
    title: Type Safe
    details: Strong static typing with inference. Catch errors at compile time, not runtime.
  
  - icon: 📦
    title: Bun Ready
    details: First-class support for Bun with built-in plugin integration for fast builds and hot reloading.
  
  - icon: 🚀
    title: Developer Experience
    details: Interactive REPL, comprehensive CLI tools, and excellent error messages make development a joy.
---

<div class="vp-doc">

## Quick Install

<InstallTabs />

## Why Zenoscript?

Zenoscript bridges the gap between functional programming and the TypeScript ecosystem. Write expressive, maintainable code that compiles to clean TypeScript.

### Quick Example

```zenoscript
// Define a struct with methods
struct User {
  name: string;
  age: number;
}

// Pattern matching with atoms
let status = :loading

let message = match status {
  :loading => "Please wait..."
  :success => "Data loaded!"
  :error => "Something went wrong"
  _ => "Unknown status"
}

// Pipe operations for clean data flow
let result = users
  |> filter(user => user.age >= 18)
  |> map(user => user.name)
  |> sort
```

### Compiles to Clean TypeScript

```typescript
type User = {
  name: string;
  age: number;
};

const status = Symbol.for("loading");

const message = (() => {
  const __match_value = status;
  if (__match_value === Symbol.for("loading")) {
    return "Please wait...";
  } else if (__match_value === Symbol.for("success")) {
    return "Data loaded!";
  } else if (__match_value === Symbol.for("error")) {
    return "Something went wrong";
  } else {
    return "Unknown status";
  }
})();

const result = users
  .filter(user => user.age >= 18)
  .map(user => user.name)
  .sort();
```

## Installation

### Quick Install

<InstallTabs />

### Get Started

```bash
# Create a new project
zeno init my-project
cd my-project

# Start development
bun dev
```

## What's Next?

<div class="tip custom-block" style="padding-top: 8px">

Just getting started? Check out the [Introduction](/docs/) and [Quick Start Guide](/docs/quickstart).

</div>

</div>

<style>
:root {
  --vp-home-hero-name-color: transparent;
  --vp-home-hero-name-background: -webkit-linear-gradient(120deg, #bd34fe 30%, #41d1ff);

  --vp-home-hero-image-background-image: linear-gradient(-45deg, #bd34fe 50%, #47caff 50%);
  --vp-home-hero-image-filter: blur(44px);
}

@media (min-width: 640px) {
  :root {
    --vp-home-hero-image-filter: blur(56px);
  }
}

@media (min-width: 960px) {
  :root {
    --vp-home-hero-image-filter: blur(68px);
  }
}

/* Match the feature grid's responsive padding */
.vp-doc {
  max-width: 1152px;
  margin: 0 auto;
  padding: 0 24px;
}

@media (min-width: 640px) {
  .vp-doc {
    padding: 0 48px;
  }
}

@media (min-width: 960px) {
  .vp-doc {
    padding: 0 64px;
  }
}
</style>
