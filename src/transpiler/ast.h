#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations
typedef struct ASTNode ASTNode;
typedef struct ASTList ASTList;

// AST Node types
typedef enum {
    AST_PROGRAM,
    AST_STRUCT_DECL,
    AST_TRAIT_DECL,
    AST_IMPL_BLOCK,
    AST_LET_BINDING,
    AST_MATCH_EXPR,
    AST_PIPE_EXPR,
    AST_FUNCTION_DECL,
    AST_IDENTIFIER,
    AST_NUMBER_LITERAL,
    AST_STRING_LITERAL,
    AST_ATOM_LITERAL,
    AST_BLOCK,
    AST_FIELD_DECL,
    AST_METHOD_DECL,
    AST_PARAM_DECL,
    AST_TYPE_ANNOTATION,
    AST_GENERIC_PARAMS,
    AST_MATCH_ARM,
    AST_PATTERN,
    AST_EXPRESSION,
    AST_CALL_EXPR,
    AST_MEMBER_ACCESS,
    AST_ASSIGNMENT
} ASTNodeType;

// Generic list structure for AST nodes
struct ASTList {
    ASTNode** nodes;
    int count;
    int capacity;
};

// Main AST node structure
struct ASTNode {
    ASTNodeType type;
    int line;
    int column;
    
    union {
        struct {
            ASTList* declarations;
        } program;
        
        struct {
            char* name;
            ASTList* generic_params;
            ASTList* fields;
        } struct_decl;
        
        struct {
            char* name;
            ASTList* generic_params;
            ASTList* methods;
        } trait_decl;
        
        struct {
            char* trait_name;
            char* type_name;
            ASTList* generic_params;
            ASTList* methods;
        } impl_block;
        
        struct {
            char* name;
            ASTNode* value;
            ASTNode* type_annotation;
        } let_binding;
        
        struct {
            ASTNode* expr;
            ASTList* arms;
        } match_expr;
        
        struct {
            ASTNode* left;
            ASTNode* right;
        } pipe_expr;
        
        struct {
            char* name;
            ASTList* params;
            ASTNode* return_type;
            ASTNode* body;
        } function_decl;
        
        struct {
            char* name;
        } identifier;
        
        struct {
            char* value;
        } number_literal;
        
        struct {
            char* value;
        } string_literal;
        
        struct {
            char* value;
        } atom_literal;
        
        struct {
            ASTList* statements;
        } block;
        
        struct {
            char* name;
            ASTNode* type_annotation;
        } field_decl;
        
        struct {
            char* name;
            ASTList* params;
            ASTNode* return_type;
            ASTNode* body;
        } method_decl;
        
        struct {
            char* name;
            ASTNode* type_annotation;
        } param_decl;
        
        struct {
            char* type_name;
            ASTList* generic_args;
        } type_annotation;
        
        struct {
            ASTList* params;
        } generic_params;
        
        struct {
            ASTNode* pattern;
            ASTNode* guard;
            ASTNode* body;
        } match_arm;
        
        struct {
            ASTNode* pattern;
        } pattern;
        
        struct {
            ASTNode* expr;
        } expression;
        
        struct {
            ASTNode* function;
            ASTList* args;
        } call_expr;
        
        struct {
            ASTNode* object;
            char* member;
        } member_access;
        
        struct {
            char* target;
            ASTNode* value;
        } assignment;
    };
};

// Function prototypes
ASTNode* ast_node_new(ASTNodeType type);
void ast_node_free(ASTNode* node);
ASTList* ast_list_new(void);
void ast_list_free(ASTList* list);
void ast_list_add(ASTList* list, ASTNode* node);
ASTNode* ast_list_get(ASTList* list, int index);

// AST creation helpers
ASTNode* ast_create_program(ASTList* declarations);
ASTNode* ast_create_struct_decl(char* name, ASTList* generic_params, ASTList* fields);
ASTNode* ast_create_trait_decl(char* name, ASTList* generic_params, ASTList* methods);
ASTNode* ast_create_impl_block(char* trait_name, char* type_name, ASTList* generic_params, ASTList* methods);
ASTNode* ast_create_let_binding(char* name, ASTNode* value, ASTNode* type_annotation);
ASTNode* ast_create_match_expr(ASTNode* expr, ASTList* arms);
ASTNode* ast_create_pipe_expr(ASTNode* left, ASTNode* right);
ASTNode* ast_create_identifier(char* name);
ASTNode* ast_create_number_literal(char* value);
ASTNode* ast_create_string_literal(char* value);
ASTNode* ast_create_atom_literal(char* value);
ASTNode* ast_create_block(ASTList* statements);
ASTNode* ast_create_field_decl(char* name, ASTNode* type_annotation);
ASTNode* ast_create_method_decl(char* name, ASTList* params, ASTNode* return_type, ASTNode* body);

// Utility functions
void ast_print(ASTNode* node, int indent);
const char* ast_node_type_to_string(ASTNodeType type);

#endif