#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    Lexer* lexer;
    Token current_token;
    Token peek_token;
    int error_count;
    char* error_message;
} Parser;

// Parser creation and cleanup
Parser* parser_new(Lexer* lexer);
void parser_free(Parser* parser);

// Main parsing function
ASTNode* parser_parse(Parser* parser);

// Error handling
void parser_error(Parser* parser, const char* message);
int parser_has_errors(Parser* parser);
const char* parser_get_error(Parser* parser);

// Token management
void parser_advance(Parser* parser);
int parser_check(Parser* parser, TokenType type);
int parser_match(Parser* parser, TokenType type);
void parser_expect(Parser* parser, TokenType type);

// Parsing functions for different constructs
ASTNode* parser_parse_program(Parser* parser);
ASTNode* parser_parse_declaration(Parser* parser);
ASTNode* parser_parse_struct_decl(Parser* parser);
ASTNode* parser_parse_trait_decl(Parser* parser);
ASTNode* parser_parse_impl_block(Parser* parser);
ASTNode* parser_parse_let_binding(Parser* parser);
ASTNode* parser_parse_expression(Parser* parser);
ASTNode* parser_parse_pipe_expression(Parser* parser);
ASTNode* parser_parse_match_expression(Parser* parser);
ASTNode* parser_parse_primary(Parser* parser);
ASTNode* parser_parse_identifier(Parser* parser);
ASTNode* parser_parse_literal(Parser* parser);
ASTNode* parser_parse_block(Parser* parser);
ASTNode* parser_parse_type_annotation(Parser* parser);
ASTList* parser_parse_generic_params(Parser* parser);
ASTList* parser_parse_parameter_list(Parser* parser);
ASTList* parser_parse_field_list(Parser* parser);
ASTList* parser_parse_match_arms(Parser* parser);
ASTNode* parser_parse_match_arm(Parser* parser);
ASTNode* parser_parse_pattern(Parser* parser);

#endif