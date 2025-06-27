#include "parser.h"

Parser* parser_new(Lexer* lexer) {
    Parser* parser = malloc(sizeof(Parser));
    parser->lexer = lexer;
    parser->error_count = 0;
    parser->error_message = NULL;
    
    // Initialize tokens
    parser->current_token = lexer_next_token(lexer);
    parser->peek_token = lexer_next_token(lexer);
    
    return parser;
}

void parser_free(Parser* parser) {
    if (parser) {
        token_free(&parser->current_token);
        token_free(&parser->peek_token);
        free(parser->error_message);
        free(parser);
    }
}

void parser_error(Parser* parser, const char* message) {
    parser->error_count++;
    if (parser->error_message) {
        free(parser->error_message);
    }
    parser->error_message = strdup(message);
    printf("Parse error at line %d, column %d: %s\n", 
           parser->current_token.line, parser->current_token.column, message);
}

int parser_has_errors(Parser* parser) {
    return parser->error_count > 0;
}

const char* parser_get_error(Parser* parser) {
    return parser->error_message;
}

void parser_advance(Parser* parser) {
    token_free(&parser->current_token);
    parser->current_token = parser->peek_token;
    parser->peek_token = lexer_next_token(parser->lexer);
}

int parser_check(Parser* parser, TokenType type) {
    return parser->current_token.type == type;
}

int parser_match(Parser* parser, TokenType type) {
    if (parser_check(parser, type)) {
        parser_advance(parser);
        return 1;
    }
    return 0;
}

void parser_expect(Parser* parser, TokenType type) {
    if (!parser_match(parser, type)) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), 
                "Expected %s, got %s", 
                token_type_to_string(type), 
                token_type_to_string(parser->current_token.type));
        parser_error(parser, error_msg);
    }
}

// Skip newlines and comments
static void parser_skip_noise(Parser* parser) {
    while (parser->current_token.type == TOKEN_NEWLINE || 
           parser->current_token.type == TOKEN_COMMENT) {
        parser_advance(parser);
    }
}

ASTNode* parser_parse(Parser* parser) {
    return parser_parse_program(parser);
}

ASTNode* parser_parse_program(Parser* parser) {
    ASTList* declarations = ast_list_new();
    
    parser_skip_noise(parser);
    
    while (!parser_check(parser, TOKEN_EOF)) {
        ASTNode* decl = parser_parse_declaration(parser);
        if (decl) {
            ast_list_add(declarations, decl);
        }
        parser_skip_noise(parser);
    }
    
    return ast_create_program(declarations);
}

ASTNode* parser_parse_declaration(Parser* parser) {
    switch (parser->current_token.type) {
        case TOKEN_STRUCT:
            return parser_parse_struct_decl(parser);
        case TOKEN_TRAIT:
            return parser_parse_trait_decl(parser);
        case TOKEN_IMPL:
            return parser_parse_impl_block(parser);
        case TOKEN_LET:
            return parser_parse_let_binding(parser);
        default:
            return parser_parse_expression(parser);
    }
}

ASTNode* parser_parse_struct_decl(Parser* parser) {
    parser_expect(parser, TOKEN_STRUCT);
    
    if (!parser_check(parser, TOKEN_IDENTIFIER)) {
        parser_error(parser, "Expected struct name");
        return NULL;
    }
    
    char* name = strdup(parser->current_token.value);
    parser_advance(parser);
    
    ASTList* generic_params = NULL;
    if (parser_match(parser, TOKEN_LANGLE)) {
        generic_params = parser_parse_generic_params(parser);
        parser_expect(parser, TOKEN_RANGLE);
    }
    
    ASTList* fields = NULL;
    if (parser_match(parser, TOKEN_LBRACE)) {
        fields = parser_parse_field_list(parser);
        parser_expect(parser, TOKEN_RBRACE);
    } else {
        parser_expect(parser, TOKEN_SEMICOLON);
        fields = ast_list_new(); // Empty struct
    }
    
    return ast_create_struct_decl(name, generic_params, fields);
}

ASTNode* parser_parse_trait_decl(Parser* parser) {
    parser_expect(parser, TOKEN_TRAIT);
    
    if (!parser_check(parser, TOKEN_IDENTIFIER)) {
        parser_error(parser, "Expected trait name");
        return NULL;
    }
    
    char* name = strdup(parser->current_token.value);
    parser_advance(parser);
    
    ASTList* generic_params = NULL;
    if (parser_match(parser, TOKEN_LANGLE)) {
        generic_params = parser_parse_generic_params(parser);
        parser_expect(parser, TOKEN_RANGLE);
    }
    
    parser_expect(parser, TOKEN_LBRACE);
    ASTList* methods = ast_list_new();
    
    parser_skip_noise(parser);
    while (!parser_check(parser, TOKEN_RBRACE) && !parser_check(parser, TOKEN_EOF)) {
        if (parser_check(parser, TOKEN_IDENTIFIER)) {
            char* method_name = strdup(parser->current_token.value);
            parser_advance(parser);
            
            parser_expect(parser, TOKEN_LPAREN);
            ASTList* params = parser_parse_parameter_list(parser);
            parser_expect(parser, TOKEN_RPAREN);
            
            ASTNode* return_type = NULL;
            if (parser_match(parser, TOKEN_COLON)) {
                return_type = parser_parse_type_annotation(parser);
            }
            
            parser_expect(parser, TOKEN_SEMICOLON);
            
            ASTNode* method = ast_create_method_decl(method_name, params, return_type, NULL);
            ast_list_add(methods, method);
            
            free(method_name);
        } else {
            parser_error(parser, "Expected method declaration");
            break;
        }
        parser_skip_noise(parser);
    }
    
    parser_expect(parser, TOKEN_RBRACE);
    
    return ast_create_trait_decl(name, generic_params, methods);
}

ASTNode* parser_parse_impl_block(Parser* parser) {
    parser_expect(parser, TOKEN_IMPL);
    
    ASTList* generic_params = NULL;
    if (parser_match(parser, TOKEN_LANGLE)) {
        generic_params = parser_parse_generic_params(parser);
        parser_expect(parser, TOKEN_RANGLE);
    }
    
    char* trait_name = NULL;
    char* type_name = NULL;
    
    if (parser_check(parser, TOKEN_IDENTIFIER)) {
        char* first_name = strdup(parser->current_token.value);
        parser_advance(parser);
        
        if (parser_match(parser, TOKEN_FOR)) {
            // impl TraitName for TypeName
            trait_name = first_name;
            if (parser_check(parser, TOKEN_IDENTIFIER)) {
                type_name = strdup(parser->current_token.value);
                parser_advance(parser);
            } else {
                parser_error(parser, "Expected type name after 'for'");
                free(first_name);
                return NULL;
            }
        } else {
            // impl TypeName
            type_name = first_name;
        }
    } else {
        parser_error(parser, "Expected identifier in impl block");
        return NULL;
    }
    
    parser_expect(parser, TOKEN_LBRACE);
    ASTList* methods = ast_list_new();
    
    parser_skip_noise(parser);
    while (!parser_check(parser, TOKEN_RBRACE) && !parser_check(parser, TOKEN_EOF)) {
        if (parser_check(parser, TOKEN_IDENTIFIER)) {
            char* method_name = strdup(parser->current_token.value);
            parser_advance(parser);
            
            parser_expect(parser, TOKEN_LPAREN);
            ASTList* params = parser_parse_parameter_list(parser);
            parser_expect(parser, TOKEN_RPAREN);
            
            ASTNode* return_type = NULL;
            if (parser_match(parser, TOKEN_COLON)) {
                return_type = parser_parse_type_annotation(parser);
            }
            
            ASTNode* body = parser_parse_block(parser);
            
            ASTNode* method = ast_create_method_decl(method_name, params, return_type, body);
            ast_list_add(methods, method);
            
            free(method_name);
        } else {
            parser_error(parser, "Expected method declaration");
            break;
        }
        parser_skip_noise(parser);
    }
    
    parser_expect(parser, TOKEN_RBRACE);
    
    return ast_create_impl_block(trait_name, type_name, generic_params, methods);
}

ASTNode* parser_parse_let_binding(Parser* parser) {
    parser_expect(parser, TOKEN_LET);
    
    if (!parser_check(parser, TOKEN_IDENTIFIER)) {
        parser_error(parser, "Expected variable name");
        return NULL;
    }
    
    char* name = strdup(parser->current_token.value);
    parser_advance(parser);
    
    ASTNode* type_annotation = NULL;
    if (parser_match(parser, TOKEN_COLON)) {
        type_annotation = parser_parse_type_annotation(parser);
    }
    
    parser_expect(parser, TOKEN_ASSIGN);
    ASTNode* value = parser_parse_expression(parser);
    
    return ast_create_let_binding(name, value, type_annotation);
}

ASTNode* parser_parse_expression(Parser* parser) {
    return parser_parse_pipe_expression(parser);
}

ASTNode* parser_parse_pipe_expression(Parser* parser) {
    ASTNode* expr = parser_parse_match_expression(parser);
    
    while (parser_match(parser, TOKEN_PIPE)) {
        ASTNode* right = parser_parse_match_expression(parser);
        expr = ast_create_pipe_expr(expr, right);
    }
    
    return expr;
}

ASTNode* parser_parse_match_expression(Parser* parser) {
    if (parser_match(parser, TOKEN_MATCH)) {
        ASTNode* expr = parser_parse_primary(parser);
        parser_expect(parser, TOKEN_LBRACE);
        ASTList* arms = parser_parse_match_arms(parser);
        parser_expect(parser, TOKEN_RBRACE);
        return ast_create_match_expr(expr, arms);
    }
    
    return parser_parse_primary(parser);
}

ASTNode* parser_parse_primary(Parser* parser) {
    switch (parser->current_token.type) {
        case TOKEN_IDENTIFIER:
            return parser_parse_identifier(parser);
        case TOKEN_NUMBER:
        case TOKEN_STRING:
        case TOKEN_ATOM:
            return parser_parse_literal(parser);
        case TOKEN_LPAREN: {
            parser_advance(parser);
            ASTNode* expr = parser_parse_expression(parser);
            parser_expect(parser, TOKEN_RPAREN);
            return expr;
        }
        case TOKEN_LBRACE:
            return parser_parse_block(parser);
        default:
            parser_error(parser, "Unexpected token in expression");
            return NULL;
    }
}

ASTNode* parser_parse_identifier(Parser* parser) {
    if (!parser_check(parser, TOKEN_IDENTIFIER)) {
        parser_error(parser, "Expected identifier");
        return NULL;
    }
    
    char* name = strdup(parser->current_token.value);
    parser_advance(parser);
    return ast_create_identifier(name);
}

ASTNode* parser_parse_literal(Parser* parser) {
    switch (parser->current_token.type) {
        case TOKEN_NUMBER: {
            char* value = strdup(parser->current_token.value);
            parser_advance(parser);
            return ast_create_number_literal(value);
        }
        case TOKEN_STRING: {
            char* value = strdup(parser->current_token.value);
            parser_advance(parser);
            return ast_create_string_literal(value);
        }
        case TOKEN_ATOM: {
            char* value = strdup(parser->current_token.value);
            parser_advance(parser);
            return ast_create_atom_literal(value);
        }
        default:
            parser_error(parser, "Expected literal");
            return NULL;
    }
}

ASTNode* parser_parse_block(Parser* parser) {
    parser_expect(parser, TOKEN_LBRACE);
    
    ASTList* statements = ast_list_new();
    parser_skip_noise(parser);
    
    while (!parser_check(parser, TOKEN_RBRACE) && !parser_check(parser, TOKEN_EOF)) {
        ASTNode* stmt = parser_parse_expression(parser);
        if (stmt) {
            ast_list_add(statements, stmt);
        }
        parser_skip_noise(parser);
    }
    
    parser_expect(parser, TOKEN_RBRACE);
    return ast_create_block(statements);
}

ASTNode* parser_parse_type_annotation(Parser* parser) {
    if (!parser_check(parser, TOKEN_IDENTIFIER)) {
        parser_error(parser, "Expected type name");
        return NULL;
    }
    
    ASTNode* node = ast_node_new(AST_TYPE_ANNOTATION);
    node->type_annotation.type_name = strdup(parser->current_token.value);
    parser_advance(parser);
    
    // Handle generic arguments
    if (parser_match(parser, TOKEN_LANGLE)) {
        node->type_annotation.generic_args = ast_list_new();
        
        do {
            ASTNode* arg = parser_parse_type_annotation(parser);
            if (arg) {
                ast_list_add(node->type_annotation.generic_args, arg);
            }
        } while (parser_match(parser, TOKEN_COMMA));
        
        parser_expect(parser, TOKEN_RANGLE);
    }
    
    return node;
}

ASTList* parser_parse_generic_params(Parser* parser) {
    ASTList* params = ast_list_new();
    
    do {
        if (parser_check(parser, TOKEN_IDENTIFIER)) {
            ASTNode* param = ast_create_identifier(parser->current_token.value);
            ast_list_add(params, param);
            parser_advance(parser);
        } else {
            parser_error(parser, "Expected generic parameter");
            break;
        }
    } while (parser_match(parser, TOKEN_COMMA));
    
    return params;
}

ASTList* parser_parse_parameter_list(Parser* parser) {
    ASTList* params = ast_list_new();
    
    parser_skip_noise(parser);
    if (parser_check(parser, TOKEN_RPAREN)) {
        return params;
    }
    
    do {
        if (parser_check(parser, TOKEN_IDENTIFIER)) {
            char* name = strdup(parser->current_token.value);
            parser_advance(parser);
            
            ASTNode* type_annotation = NULL;
            if (parser_match(parser, TOKEN_COLON)) {
                type_annotation = parser_parse_type_annotation(parser);
            }
            
            ASTNode* param = ast_node_new(AST_PARAM_DECL);
            param->param_decl.name = name;
            param->param_decl.type_annotation = type_annotation;
            
            ast_list_add(params, param);
        } else {
            parser_error(parser, "Expected parameter name");
            break;
        }
    } while (parser_match(parser, TOKEN_COMMA));
    
    return params;
}

ASTList* parser_parse_field_list(Parser* parser) {
    ASTList* fields = ast_list_new();
    
    parser_skip_noise(parser);
    while (!parser_check(parser, TOKEN_RBRACE) && !parser_check(parser, TOKEN_EOF)) {
        if (parser_check(parser, TOKEN_IDENTIFIER)) {
            char* name = strdup(parser->current_token.value);
            parser_advance(parser);
            
            parser_expect(parser, TOKEN_COLON);
            ASTNode* type_annotation = parser_parse_type_annotation(parser);
            parser_expect(parser, TOKEN_SEMICOLON);
            
            ASTNode* field = ast_create_field_decl(name, type_annotation);
            ast_list_add(fields, field);
            
            free(name);
        } else {
            parser_error(parser, "Expected field name");
            break;
        }
        parser_skip_noise(parser);
    }
    
    return fields;
}

ASTList* parser_parse_match_arms(Parser* parser) {
    ASTList* arms = ast_list_new();
    
    parser_skip_noise(parser);
    while (!parser_check(parser, TOKEN_RBRACE) && !parser_check(parser, TOKEN_EOF)) {
        ASTNode* arm = parser_parse_match_arm(parser);
        if (arm) {
            ast_list_add(arms, arm);
        }
        parser_skip_noise(parser);
    }
    
    return arms;
}

ASTNode* parser_parse_match_arm(Parser* parser) {
    ASTNode* pattern = parser_parse_pattern(parser);
    
    ASTNode* guard = NULL;
    if (parser_match(parser, TOKEN_WHEN)) {
        guard = parser_parse_expression(parser);
    }
    
    parser_expect(parser, TOKEN_ARROW);
    ASTNode* body = parser_parse_expression(parser);
    
    ASTNode* arm = ast_node_new(AST_MATCH_ARM);
    arm->match_arm.pattern = pattern;
    arm->match_arm.guard = guard;
    arm->match_arm.body = body;
    
    return arm;
}

ASTNode* parser_parse_pattern(Parser* parser) {
    // Simple pattern parsing - can be extended for more complex patterns
    switch (parser->current_token.type) {
        case TOKEN_UNDERSCORE:
            parser_advance(parser);
            return ast_create_identifier("_");
        case TOKEN_IDENTIFIER:
        case TOKEN_NUMBER:
        case TOKEN_STRING:
        case TOKEN_ATOM:
            return parser_parse_literal(parser);
        default:
            parser_error(parser, "Expected pattern");
            return NULL;
    }
}