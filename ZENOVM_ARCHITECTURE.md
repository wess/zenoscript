# ZenoVM: Multi-Backend Virtual Machine Architecture

## Dual Architecture Strategy

```
Fast Path (Current):     Zenoscript → TypeScript Transpiler → TS/JS
Multi-Target Path (New): Zenoscript → ZenoVM → Multiple Backends
```

## ZenoVM Architecture

### Core Components

```
┌─────────────────────────────────────────────────────────────┐
│                        ZenoVM                               │
├─────────────────────────────────────────────────────────────┤
│  Frontend: Zenoscript → Zeno IR (ZIR)                      │
├─────────────────────────────────────────────────────────────┤
│  Runtime: ZenoVM Runtime System                            │
├─────────────────────────────────────────────────────────────┤
│  Backends: ZIR → Target Languages                          │
│  ├─ C Backend                                              │
│  ├─ Go Backend                                             │
│  ├─ Rust Backend                                           │
│  ├─ Python Backend                                         │
│  ├─ WebAssembly Backend                                    │
│  └─ LLVM Backend (future)                                  │
└─────────────────────────────────────────────────────────────┘
```

## Implementation Strategy

### Phase 1: Foundation
Build ZenoVM using existing C transpiler infrastructure:

```c
// Enhanced from existing src/transpiler/
typedef struct {
    ZenoIR* ir;
    ZenoRuntime* runtime;
    BackendRegistry* backends;
} ZenoVM;

// Reuse existing parser/AST
ZenoVM* zenovm_create(void);
ZenoIR* zenovm_parse(ZenoVM* vm, const char* source);
char* zenovm_compile(ZenoVM* vm, ZenoIR* ir, const char* target);
```

### Phase 2: Backend System
```c
// Backend interface
typedef struct {
    char* name;
    char* (*generate_struct)(StructDecl* decl);
    char* (*generate_function)(FunctionDecl* decl);
    char* (*generate_expression)(Expression* expr);
} Backend;

// Register backends
zenovm_register_backend(vm, "c", &c_backend);
zenovm_register_backend(vm, "go", &go_backend);
zenovm_register_backend(vm, "rust", &rust_backend);
```

## CLI Integration

### Dual Mode CLI
```bash
# Fast path - TypeScript transpiler (current)
zeno compile input.zs                    # → TypeScript (default)
zeno compile --fast input.zs             # → TypeScript (explicit)

# ZenoVM path - Multi-target
zeno vm compile --target=c input.zs      # → C
zeno vm compile --target=go input.zs     # → Go  
zeno vm compile --target=rust input.zs   # → Rust
zeno vm compile --target=wasm input.zs   # → WebAssembly

# Runtime modes
zeno vm run input.zs                     # → JIT execution
zeno vm aot --target=native input.zs     # → AOT compilation
```

## ZenoVM Features

### 1. Intermediate Representation (ZIR)
```c
typedef struct {
    ZenoModule* modules;
    ZenoStruct* structs;
    ZenoTrait* traits;
    ZenoFunction* functions;
    ZenoExpression* expressions;
} ZenoIR;

// Enhanced from existing AST
typedef struct {
    char* name;
    ZenoType* fields;
    ZenoGeneric* generics;
    ZenoMetadata* metadata;  // For backend-specific info
} ZenoStruct;
```

### 2. Runtime System
```c
typedef struct {
    GarbageCollector* gc;      // For GC targets
    MemoryManager* mem;        // For manual memory targets  
    TypeSystem* types;         // Type checking/inference
    ErrorHandler* errors;      // Error reporting
} ZenoRuntime;
```

### 3. Backend Abstraction
```c
// Each backend implements this interface
typedef struct {
    // Code generation
    char* (*gen_header)(ZenoIR* ir);
    char* (*gen_struct)(ZenoStruct* s);
    char* (*gen_function)(ZenoFunction* f);
    char* (*gen_expression)(ZenoExpression* e);
    char* (*gen_footer)(ZenoIR* ir);
    
    // Target-specific transforms
    ZenoIR* (*optimize)(ZenoIR* ir);
    ZenoType* (*map_type)(ZenoType* type);
    
    // Runtime integration
    char* (*gen_runtime_calls)(ZenoRuntime* runtime);
} Backend;
```

## Backend Examples

### C Backend
```c
// Leverage existing codegen.c
char* c_backend_gen_struct(ZenoStruct* s) {
    StringBuilder* sb = sb_create();
    sb_append(sb, "typedef struct {\n");
    
    for (int i = 0; i < s->field_count; i++) {
        char* c_type = map_zeno_type_to_c(s->fields[i].type);
        sb_appendf(sb, "  %s %s;\n", c_type, s->fields[i].name);
    }
    
    sb_append(sb, "} ");
    sb_append(sb, s->name);
    sb_append(sb, ";\n");
    
    return sb_to_string(sb);
}

char* map_zeno_type_to_c(ZenoType* type) {
    switch (type->kind) {
        case ZENO_TYPE_STRING:  return "char*";
        case ZENO_TYPE_NUMBER:  return "int";
        case ZENO_TYPE_BOOLEAN: return "bool";
        case ZENO_TYPE_ARRAY:   return "Array*";  // Custom array type
        default: return type->name;  // User-defined types
    }
}
```

### Go Backend
```c
char* go_backend_gen_struct(ZenoStruct* s) {
    StringBuilder* sb = sb_create();
    sb_appendf(sb, "type %s struct {\n", s->name);
    
    for (int i = 0; i < s->field_count; i++) {
        char* go_type = map_zeno_type_to_go(s->fields[i].type);
        char* field_name = capitalize(s->fields[i].name);  // Go convention
        sb_appendf(sb, "  %s %s\n", field_name, go_type);
    }
    
    sb_append(sb, "}\n");
    return sb_to_string(sb);
}
```

### WebAssembly Backend
```c
char* wasm_backend_gen_function(ZenoFunction* f) {
    // Generate WAT (WebAssembly Text) format
    StringBuilder* sb = sb_create();
    sb_appendf(sb, "(func $%s ", f->name);
    
    // Parameters
    for (int i = 0; i < f->param_count; i++) {
        sb_appendf(sb, "(param $%s %s) ", 
                  f->params[i].name, 
                  map_zeno_type_to_wasm(f->params[i].type));
    }
    
    // Return type
    if (f->return_type) {
        sb_appendf(sb, "(result %s) ", map_zeno_type_to_wasm(f->return_type));
    }
    
    // Body
    sb_append(sb, wasm_gen_function_body(f->body));
    sb_append(sb, ")\n");
    
    return sb_to_string(sb);
}
```

## Integration with Existing Codebase

### Reuse Current Infrastructure
```c
// Build on existing src/transpiler/
// ✅ lexer.c/lexer.h - Reuse for tokenization
// ✅ parser.c/parser.h - Reuse for AST generation  
// ✅ ast.c/ast.h - Enhance for ZIR
// ✅ codegen.c/codegen.h - Refactor into backend system
// ✅ Makefile - Extend for ZenoVM build

// New additions:
// 📁 src/zenovm/
//   ├── zenovm.c/zenovm.h - Core VM
//   ├── zir.c/zir.h - Intermediate representation  
//   ├── runtime.c/runtime.h - Runtime system
//   ├── backends/
//   │   ├── c_backend.c
//   │   ├── go_backend.c  
//   │   ├── rust_backend.c
//   │   └── wasm_backend.c
//   └── cli.c - ZenoVM CLI interface
```

### TypeScript Transpiler Integration
```typescript
// Keep existing src/transpiler.ts for fast path
// Add ZenoVM bridge for when more power is needed

export class ZenoscriptCompiler {
  // Fast path (current)
  transpileToTypeScript(source: string): string {
    return new ZenoscriptTranspiler().transpile(source);
  }
  
  // ZenoVM path (new)
  async compileWithZenoVM(source: string, target: string): Promise<string> {
    const zenovm = await import('./zenovm-bridge');
    return zenovm.compile(source, target);
  }
  
  // Auto-select best approach
  async compile(source: string, target: string = 'typescript'): Promise<string> {
    if (target === 'typescript' || target === 'javascript') {
      return this.transpileToTypeScript(source);  // Fast path
    } else {
      return this.compileWithZenoVM(source, target);  // Powerful path
    }
  }
}
```

## Benefits of ZenoVM Architecture

### ✅ Best of Both Worlds
- **TypeScript transpiler**: Lightning fast for web development
- **ZenoVM**: Powerful multi-target compilation for systems programming

### ✅ Clear Separation of Concerns
- **Frontend**: Parse Zenoscript → ZIR (reuse existing C code)
- **Runtime**: Memory management, GC, type system
- **Backends**: Target-specific code generation

### ✅ Extensibility
- Add new targets by implementing Backend interface
- JIT/AOT compilation modes
- Runtime optimizations
- Plugin system for custom backends

### ✅ Performance Options
- Development: Fast TypeScript transpiler
- Production: Optimized ZenoVM compilation
- Runtime: JIT for development, AOT for deployment

This architecture gives Zenoscript the flexibility of Dart while maintaining the speed advantage of the current TypeScript transpiler for common use cases.