#ifndef ZIR_H
#define ZIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ZenoIR - Zenoscript Intermediate Representation
// This is a language-agnostic representation that can be used by multiple backends

// Forward declarations
typedef struct ZIRNode ZIRNode;
typedef struct ZIRList ZIRList;
typedef struct ZIRModule ZIRModule;
typedef struct ZIRType ZIRType;

// ZIR Node Types - Enhanced from AST for multi-target support
typedef enum {
    ZIR_MODULE,
    ZIR_STRUCT_DECL,
    ZIR_TRAIT_DECL,
    ZIR_FUNCTION_DECL,
    ZIR_LET_BINDING,
    ZIR_MATCH_EXPR,
    ZIR_PIPE_EXPR,
    ZIR_CALL_EXPR,
    ZIR_IDENTIFIER,
    ZIR_LITERAL,
    ZIR_BLOCK,
    ZIR_IF_EXPR,
    ZIR_RETURN_STMT,
    ZIR_ASSIGNMENT,
    ZIR_MEMBER_ACCESS,
    ZIR_BINARY_OP,
    ZIR_UNARY_OP,
    ZIR_ARRAY_LITERAL,
    ZIR_OBJECT_LITERAL
} ZIRNodeType;

// ZIR Type System - For multi-target type mapping
typedef enum {
    ZIR_TYPE_VOID,
    ZIR_TYPE_BOOL,
    ZIR_TYPE_INT,
    ZIR_TYPE_FLOAT,
    ZIR_TYPE_STRING,
    ZIR_TYPE_ARRAY,
    ZIR_TYPE_OBJECT,
    ZIR_TYPE_FUNCTION,
    ZIR_TYPE_GENERIC,
    ZIR_TYPE_USER_DEFINED,
    ZIR_TYPE_ATOM
} ZIRTypeKind;

struct ZIRType {
    ZIRTypeKind kind;
    char* name;
    ZIRList* generic_args;  // For generic types like Array<T>
    ZIRType* element_type;  // For arrays, function return types
    ZIRList* param_types;   // For function types
    bool is_optional;       // For optional types
    bool is_mutable;        // For mutable bindings
};

// ZIR Literal Types
typedef enum {
    ZIR_LIT_BOOL,
    ZIR_LIT_INT,
    ZIR_LIT_FLOAT,
    ZIR_LIT_STRING,
    ZIR_LIT_ATOM,
    ZIR_LIT_NULL
} ZIRLiteralType;

// ZIR Binary Operators
typedef enum {
    ZIR_OP_ADD,
    ZIR_OP_SUB,
    ZIR_OP_MUL,
    ZIR_OP_DIV,
    ZIR_OP_MOD,
    ZIR_OP_EQ,
    ZIR_OP_NE,
    ZIR_OP_LT,
    ZIR_OP_LE,
    ZIR_OP_GT,
    ZIR_OP_GE,
    ZIR_OP_AND,
    ZIR_OP_OR,
    ZIR_OP_PIPE
} ZIRBinaryOp;

// Generic list structure for ZIR nodes
struct ZIRList {
    ZIRNode** nodes;
    int count;
    int capacity;
};

// Main ZIR node structure
struct ZIRNode {
    ZIRNodeType type;
    ZIRType* zir_type;    // Type information for backends
    int line;
    int column;
    char* source_file;    // For error reporting
    bool has_optional_return;  // For function transformation
    bool has_optional_parens;  // For call expression transformation
    
    union {
        struct {
            char* name;
            ZIRList* declarations;
            ZIRList* imports;
        } module;
        
        struct {
            char* name;
            ZIRList* generic_params;
            ZIRList* fields;
            bool is_empty;    // For empty structs like "struct Empty;"
        } struct_decl;
        
        struct {
            char* name;
            ZIRList* generic_params;
            ZIRList* methods;
        } trait_decl;
        
        struct {
            char* name;
            ZIRList* params;
            ZIRType* return_type;
            ZIRNode* body;
            bool is_async;
            bool is_generator;
            bool has_optional_return;  // Last expression is return value
        } function_decl;
        
        struct {
            char* name;
            ZIRNode* value;
            ZIRType* type_annotation;
            bool is_mutable;
        } let_binding;
        
        struct {
            ZIRNode* expr;
            ZIRList* arms;
            bool is_exhaustive;
        } match_expr;
        
        struct {
            ZIRNode* left;
            ZIRNode* right;
            ZIRType* result_type;
        } pipe_expr;
        
        struct {
            ZIRNode* function;
            ZIRList* args;
            bool has_optional_parens;  // Was called without parens
        } call_expr;
        
        struct {
            char* name;
            ZIRType* resolved_type;
        } identifier;
        
        struct {
            ZIRLiteralType lit_type;
            union {
                bool bool_value;
                long int_value;
                double float_value;
                char* string_value;
                char* atom_value;
            };
        } literal;
        
        struct {
            ZIRList* statements;
            ZIRNode* return_expr;  // Optional last expression
        } block;
        
        struct {
            ZIRNode* condition;
            ZIRNode* then_branch;
            ZIRNode* else_branch;
            bool has_optional_parens;  // Was written without parens
        } if_expr;
        
        struct {
            ZIRNode* value;
        } return_stmt;
        
        struct {
            char* target;
            ZIRNode* value;
        } assignment;
        
        struct {
            ZIRNode* object;
            char* member;
            ZIRType* member_type;
        } member_access;
        
        struct {
            ZIRBinaryOp op;
            ZIRNode* left;
            ZIRNode* right;
        } binary_op;
        
        struct {
            ZIRList* elements;
            ZIRType* element_type;
        } array_literal;
        
        struct {
            ZIRList* fields;  // Key-value pairs
        } object_literal;
    };
};

// ZIR Module - Top-level container
struct ZIRModule {
    char* name;
    char* source_file;
    ZIRList* declarations;
    ZIRList* imports;
    ZIRList* exports;
};

// Function prototypes for ZIR creation and manipulation
ZIRNode* zir_node_new(ZIRNodeType type);
void zir_node_free(ZIRNode* node);
ZIRList* zir_list_new(void);
void zir_list_free(ZIRList* list);
void zir_list_add(ZIRList* list, ZIRNode* node);
ZIRNode* zir_list_get(ZIRList* list, int index);

ZIRType* zir_type_new(ZIRTypeKind kind);
void zir_type_free(ZIRType* type);
ZIRType* zir_type_clone(ZIRType* type);
bool zir_type_equals(ZIRType* a, ZIRType* b);

ZIRModule* zir_module_new(char* name, char* source_file);
void zir_module_free(ZIRModule* module);

// ZIR creation helpers
ZIRNode* zir_create_module(char* name, ZIRList* declarations);
ZIRNode* zir_create_struct_decl(char* name, ZIRList* generic_params, ZIRList* fields);
ZIRNode* zir_create_trait_decl(char* name, ZIRList* generic_params, ZIRList* methods);
ZIRNode* zir_create_function_decl(char* name, ZIRList* params, ZIRType* return_type, ZIRNode* body);
ZIRNode* zir_create_let_binding(char* name, ZIRNode* value, ZIRType* type_annotation);
ZIRNode* zir_create_match_expr(ZIRNode* expr, ZIRList* arms);
ZIRNode* zir_create_pipe_expr(ZIRNode* left, ZIRNode* right);
ZIRNode* zir_create_call_expr(ZIRNode* function, ZIRList* args);
ZIRNode* zir_create_identifier(char* name);
ZIRNode* zir_create_string_literal(char* value);
ZIRNode* zir_create_int_literal(long value);
ZIRNode* zir_create_bool_literal(bool value);
ZIRNode* zir_create_atom_literal(char* value);
ZIRNode* zir_create_block(ZIRList* statements);
ZIRNode* zir_create_if_expr(ZIRNode* condition, ZIRNode* then_branch, ZIRNode* else_branch);
ZIRNode* zir_create_binary_op(ZIRBinaryOp op, ZIRNode* left, ZIRNode* right);

// ZIR analysis and transformation
void zir_analyze_types(ZIRModule* module);
void zir_resolve_symbols(ZIRModule* module);
void zir_optimize(ZIRModule* module);

// ZIR serialization (for caching)
char* zir_serialize(ZIRModule* module);
ZIRModule* zir_deserialize(char* data);

// Debugging and inspection
void zir_print(ZIRNode* node, int indent);
const char* zir_node_type_to_string(ZIRNodeType type);
const char* zir_type_to_string(ZIRType* type);

#endif