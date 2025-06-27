#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* buffer;
    int capacity;
    int length;
    int indent_level;
} CodeGenerator;

// Code generator creation and cleanup
CodeGenerator* codegen_new(void);
void codegen_free(CodeGenerator* codegen);

// Main code generation function
char* codegen_generate(ASTNode* ast);

// Internal functions
void codegen_write(CodeGenerator* gen, const char* str);
void codegen_write_line(CodeGenerator* gen, const char* str);
void codegen_write_indent(CodeGenerator* gen);
void codegen_increase_indent(CodeGenerator* gen);
void codegen_decrease_indent(CodeGenerator* gen);

// AST node generation functions
void codegen_generate_program(CodeGenerator* gen, ASTNode* node);
void codegen_generate_struct_decl(CodeGenerator* gen, ASTNode* node);
void codegen_generate_trait_decl(CodeGenerator* gen, ASTNode* node);
void codegen_generate_impl_block(CodeGenerator* gen, ASTNode* node);
void codegen_generate_let_binding(CodeGenerator* gen, ASTNode* node);
void codegen_generate_match_expr(CodeGenerator* gen, ASTNode* node);
void codegen_generate_pipe_expr(CodeGenerator* gen, ASTNode* node);
void codegen_generate_identifier(CodeGenerator* gen, ASTNode* node);
void codegen_generate_literal(CodeGenerator* gen, ASTNode* node);
void codegen_generate_block(CodeGenerator* gen, ASTNode* node);
void codegen_generate_type_annotation(CodeGenerator* gen, ASTNode* node);
void codegen_generate_field_decl(CodeGenerator* gen, ASTNode* node);
void codegen_generate_method_decl(CodeGenerator* gen, ASTNode* node);
void codegen_generate_match_arm(CodeGenerator* gen, ASTNode* node);
void codegen_generate_expression(CodeGenerator* gen, ASTNode* node);

// Helper functions
void codegen_generate_generic_params(CodeGenerator* gen, ASTList* params);
void codegen_generate_parameter_list(CodeGenerator* gen, ASTList* params);
char* codegen_escape_string(const char* str);
char* codegen_atom_to_symbol(const char* atom);

#endif