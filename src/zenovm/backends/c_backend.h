#ifndef C_BACKEND_H
#define C_BACKEND_H

#include "../zenovm.h"
#include "backend.h"

// C Backend for ZenoVM
// Generates ANSI C code from ZenoIR

typedef struct {
    StringBuilder* header_includes;
    StringBuilder* type_declarations;
    StringBuilder* function_declarations;
    StringBuilder* main_code;
    bool needs_string_lib;
    bool needs_stdio;
    bool needs_stdlib;
    bool needs_stdbool;
} CBackendData;

// Main backend interface
ZenoBackend* c_backend_create(void);
void c_backend_destroy(ZenoBackend* backend);

// Initialization and cleanup
int c_backend_init(ZenoBackend* backend);
void c_backend_cleanup(ZenoBackend* backend);

// Code generation functions
char* c_backend_generate_header(ZIRModule* module);
char* c_backend_generate_struct(ZIRNode* node);
char* c_backend_generate_trait(ZIRNode* node);
char* c_backend_generate_function(ZIRNode* node);
char* c_backend_generate_expression(ZIRNode* node);
char* c_backend_generate_statement(ZIRNode* node);
char* c_backend_generate_type(ZIRType* type);
char* c_backend_generate_footer(ZIRModule* module);

// Type mapping
char* c_backend_map_type(ZIRType* zir_type);
char* c_backend_map_operator(ZIRBinaryOp op);

// Specific generators
char* c_backend_generate_struct_decl(ZIRNode* node);
char* c_backend_generate_function_decl(ZIRNode* node);
char* c_backend_generate_let_binding(ZIRNode* node);
char* c_backend_generate_match_expr(ZIRNode* node);
char* c_backend_generate_pipe_expr(ZIRNode* node);
char* c_backend_generate_call_expr(ZIRNode* node);
char* c_backend_generate_if_expr(ZIRNode* node);
char* c_backend_generate_binary_op(ZIRNode* node);
char* c_backend_generate_identifier(ZIRNode* node);
char* c_backend_generate_literal(ZIRNode* node);
char* c_backend_generate_block(ZIRNode* node);

// Helper functions
char* c_backend_generate_includes(CBackendData* data);
char* c_backend_generate_main_function(ZIRModule* module);
char* c_backend_escape_string_c(const char* str);
char* c_backend_atom_to_enum(const char* atom);
bool c_backend_needs_malloc(ZIRType* type);

// Memory management helpers
char* c_backend_generate_allocation(ZIRType* type);
char* c_backend_generate_deallocation(ZIRType* type);

// Runtime integration
char* c_backend_generate_runtime_calls(ZenoRuntime* runtime);

#endif