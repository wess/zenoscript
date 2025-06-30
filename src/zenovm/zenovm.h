#ifndef ZENOVM_H
#define ZENOVM_H

#include "zir.h"
#include "../transpiler/lexer.h"
#include "../transpiler/parser.h"
#include <stdbool.h>

// Forward declarations
typedef struct ZenoVM ZenoVM;
typedef struct ZenoBackend ZenoBackend;
typedef struct ZenoBackendRegistry ZenoBackendRegistry;
typedef struct ZenoRuntime ZenoRuntime;
typedef struct ZenoCompileOptions ZenoCompileOptions;

// Compilation modes
typedef enum {
    ZENO_MODE_JIT,          // Just-in-time compilation
    ZENO_MODE_AOT,          // Ahead-of-time compilation
    ZENO_MODE_TRANSPILE     // Source-to-source transpilation
} ZenoCompileMode;

// Optimization levels
typedef enum {
    ZENO_OPT_NONE,     // No optimization (fast compilation)
    ZENO_OPT_SIZE,     // Optimize for size
    ZENO_OPT_SPEED,    // Optimize for speed
    ZENO_OPT_DEBUG     // Debug-friendly optimization
} ZenoOptLevel;

// Compilation options
struct ZenoCompileOptions {
    ZenoCompileMode mode;
    ZenoOptLevel opt_level;
    char* target_backend;
    bool debug_info;
    bool verbose;
    bool emit_ir;          // Emit intermediate representation
    char* output_file;
    char* source_file;
};

// Backend interface - Each target language implements this
struct ZenoBackend {
    char* name;
    char* version;
    char* description;
    
    // Initialization
    int (*init)(ZenoBackend* backend);
    void (*cleanup)(ZenoBackend* backend);
    
    // Code generation
    char* (*generate_header)(ZIRModule* module);
    char* (*generate_struct)(ZIRNode* node);
    char* (*generate_trait)(ZIRNode* node);
    char* (*generate_function)(ZIRNode* node);
    char* (*generate_expression)(ZIRNode* node);
    char* (*generate_statement)(ZIRNode* node);
    char* (*generate_type)(ZIRType* type);
    char* (*generate_footer)(ZIRModule* module);
    
    // Type mapping
    char* (*map_type)(ZIRType* zir_type);
    char* (*map_operator)(ZIRBinaryOp op);
    
    // Target-specific optimizations
    ZIRModule* (*optimize)(ZIRModule* module, ZenoOptLevel level);
    
    // Runtime integration
    char* (*generate_runtime_calls)(ZenoRuntime* runtime);
    
    // File extensions and conventions
    char* file_extension;
    char* header_extension;
    bool needs_header_file;
    bool supports_generics;
    bool supports_gc;
    bool supports_manual_memory;
    
    // Backend-specific data
    void* backend_data;
};

// Backend registry for managing multiple backends
struct ZenoBackendRegistry {
    ZenoBackend** backends;
    int count;
    int capacity;
};

// Runtime system for GC, memory management, etc.
struct ZenoRuntime {
    bool use_gc;
    bool use_manual_memory;
    size_t heap_size;
    char* runtime_lib_path;
    
    // Memory management callbacks
    void* (*allocate)(size_t size);
    void (*deallocate)(void* ptr);
    void (*gc_collect)(void);
    
    // Error handling
    void (*error_handler)(const char* message, int line, char* file);
};

// Main ZenoVM structure
struct ZenoVM {
    ZenoBackendRegistry* backends;
    ZenoRuntime* runtime;
    ZenoCompileOptions* options;
    
    // Compilation pipeline
    Lexer* lexer;
    Parser* parser;
    ZIRModule* current_module;
    
    // Error handling
    int error_count;
    char** error_messages;
    
    // Statistics
    double last_compile_time;
    size_t last_output_size;
    int total_compilations;
};

// ZenoVM creation and management
ZenoVM* zenovm_create(void);
void zenovm_free(ZenoVM* vm);

// Configuration
int zenovm_set_options(ZenoVM* vm, ZenoCompileOptions* options);
int zenovm_set_runtime(ZenoVM* vm, ZenoRuntime* runtime);

// Backend management
int zenovm_register_backend(ZenoVM* vm, ZenoBackend* backend);
ZenoBackend* zenovm_get_backend(ZenoVM* vm, const char* name);
char** zenovm_list_backends(ZenoVM* vm);

// Compilation pipeline
ZIRModule* zenovm_parse(ZenoVM* vm, const char* source, const char* filename);
ZIRModule* zenovm_analyze(ZenoVM* vm, ZIRModule* module);
ZIRModule* zenovm_optimize(ZenoVM* vm, ZIRModule* module);
char* zenovm_generate(ZenoVM* vm, ZIRModule* module, const char* backend_name);

// High-level compilation functions
char* zenovm_compile_string(ZenoVM* vm, const char* source, const char* backend_name);
int zenovm_compile_file(ZenoVM* vm, const char* input_file, const char* output_file, const char* backend_name);

// JIT execution (future feature)
int zenovm_execute(ZenoVM* vm, ZIRModule* module);

// Error handling
int zenovm_has_errors(ZenoVM* vm);
char** zenovm_get_errors(ZenoVM* vm);
void zenovm_clear_errors(ZenoVM* vm);

// Utilities
char* zenovm_get_version(void);
void zenovm_print_stats(ZenoVM* vm);
char* zenovm_serialize_ir(ZIRModule* module);
ZIRModule* zenovm_deserialize_ir(const char* data);

// Default options and runtime
ZenoCompileOptions* zeno_default_options(void);
ZenoRuntime* zeno_default_runtime(void);

#endif