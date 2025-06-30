#include "codegen.h"

CodeGenerator* codegen_new(void) {
    CodeGenerator* gen = malloc(sizeof(CodeGenerator));
    gen->capacity = 4096;
    gen->buffer = malloc(gen->capacity);
    gen->length = 0;
    gen->indent_level = 0;
    gen->buffer[0] = '\0';
    return gen;
}

void codegen_free(CodeGenerator* gen) {
    if (gen) {
        free(gen->buffer);
        free(gen);
    }
}

static void codegen_ensure_capacity(CodeGenerator* gen, int additional) {
    if (gen->length + additional >= gen->capacity) {
        gen->capacity = (gen->capacity + additional) * 2;
        gen->buffer = realloc(gen->buffer, gen->capacity);
    }
}

void codegen_write(CodeGenerator* gen, const char* str) {
    if (!str) return;
    
    int len = strlen(str);
    codegen_ensure_capacity(gen, len + 1);
    
    strcpy(gen->buffer + gen->length, str);
    gen->length += len;
}

void codegen_write_line(CodeGenerator* gen, const char* str) {
    codegen_write_indent(gen);
    codegen_write(gen, str);
    codegen_write(gen, "\n");
}

void codegen_write_indent(CodeGenerator* gen) {
    for (int i = 0; i < gen->indent_level; i++) {
        codegen_write(gen, "  ");
    }
}

void codegen_increase_indent(CodeGenerator* gen) {
    gen->indent_level++;
}

void codegen_decrease_indent(CodeGenerator* gen) {
    if (gen->indent_level > 0) {
        gen->indent_level--;
    }
}

char* codegen_generate(ASTNode* ast) {
    if (!ast) return NULL;
    
    CodeGenerator* gen = codegen_new();
    codegen_generate_program(gen, ast);
    
    char* result = strdup(gen->buffer);
    codegen_free(gen);
    return result;
}

void codegen_generate_program(CodeGenerator* gen, ASTNode* node) {
    if (node->type != AST_PROGRAM) return;
    
    for (int i = 0; i < node->program.declarations->count; i++) {
        ASTNode* decl = node->program.declarations->nodes[i];
        
        switch (decl->type) {
            case AST_STRUCT_DECL:
                codegen_generate_struct_decl(gen, decl);
                break;
            case AST_TRAIT_DECL:
                codegen_generate_trait_decl(gen, decl);
                break;
            case AST_IMPL_BLOCK:
                codegen_generate_impl_block(gen, decl);
                break;
            case AST_LET_BINDING:
                codegen_generate_let_binding(gen, decl);
                break;
            default:
                codegen_generate_expression(gen, decl);
                break;
        }
        
        codegen_write(gen, "\n");
    }
}

void codegen_generate_struct_decl(CodeGenerator* gen, ASTNode* node) {
    if (node->type != AST_STRUCT_DECL) return;
    
    codegen_write(gen, "type ");
    codegen_write(gen, node->struct_decl.name);
    
    if (node->struct_decl.generic_params && node->struct_decl.generic_params->count > 0) {
        codegen_generate_generic_params(gen, node->struct_decl.generic_params);
    }
    
    codegen_write(gen, " = ");
    
    if (node->struct_decl.fields->count == 0) {
        codegen_write(gen, "{}");
    } else {
        codegen_write(gen, "{\n");
        codegen_increase_indent(gen);
        
        for (int i = 0; i < node->struct_decl.fields->count; i++) {
            codegen_generate_field_decl(gen, node->struct_decl.fields->nodes[i]);
        }
        
        codegen_decrease_indent(gen);
        codegen_write(gen, "}");
    }
    
    codegen_write(gen, ";");
}

void codegen_generate_trait_decl(CodeGenerator* gen, ASTNode* node) {
    if (node->type != AST_TRAIT_DECL) return;
    
    codegen_write(gen, "interface ");
    codegen_write(gen, node->trait_decl.name);
    
    if (node->trait_decl.generic_params && node->trait_decl.generic_params->count > 0) {
        codegen_generate_generic_params(gen, node->trait_decl.generic_params);
    }
    
    codegen_write(gen, " {\n");
    codegen_increase_indent(gen);
    
    for (int i = 0; i < node->trait_decl.methods->count; i++) {
        ASTNode* method = node->trait_decl.methods->nodes[i];
        codegen_write_indent(gen);
        codegen_write(gen, method->method_decl.name);
        codegen_write(gen, "(");
        
        if (method->method_decl.params) {
            codegen_generate_parameter_list(gen, method->method_decl.params);
        }
        
        codegen_write(gen, ")");
        
        if (method->method_decl.return_type) {
            codegen_write(gen, ": ");
            codegen_generate_type_annotation(gen, method->method_decl.return_type);
        }
        
        codegen_write(gen, ";\n");
    }
    
    codegen_decrease_indent(gen);
    codegen_write(gen, "}");
}

void codegen_generate_impl_block(CodeGenerator* gen, ASTNode* node) {
    if (node->type != AST_IMPL_BLOCK) return;
    
    if (node->impl_block.trait_name) {
        // Trait implementation - generate functional object
        codegen_write(gen, "const ");
        codegen_write(gen, node->impl_block.type_name);
        codegen_write(gen, node->impl_block.trait_name);
        codegen_write(gen, " = {\n");
        codegen_increase_indent(gen);
        
        for (int i = 0; i < node->impl_block.methods->count; i++) {
            ASTNode* method = node->impl_block.methods->nodes[i];
            codegen_write_indent(gen);
            codegen_write(gen, method->method_decl.name);
            codegen_write(gen, ": (obj: ");
            codegen_write(gen, node->impl_block.type_name);
            
            if (method->method_decl.params && method->method_decl.params->count > 0) {
                codegen_write(gen, ", ");
                codegen_generate_parameter_list(gen, method->method_decl.params);
            }
            
            codegen_write(gen, ")");
            
            if (method->method_decl.return_type) {
                codegen_write(gen, ": ");
                codegen_generate_type_annotation(gen, method->method_decl.return_type);
            }
            
            codegen_write(gen, " => ");
            
            if (method->method_decl.body) {
                codegen_generate_block(gen, method->method_decl.body);
            } else {
                codegen_write(gen, "{}");
            }
            
            if (i < node->impl_block.methods->count - 1) {
                codegen_write(gen, ",");
            }
            codegen_write(gen, "\n");
        }
        
        codegen_decrease_indent(gen);
        codegen_write(gen, "};");
    } else {
        // Type implementation - generate class
        codegen_write(gen, "class ");
        codegen_write(gen, node->impl_block.type_name);
        codegen_write(gen, "Impl");
        
        if (node->impl_block.generic_params && node->impl_block.generic_params->count > 0) {
            codegen_generate_generic_params(gen, node->impl_block.generic_params);
        }
        
        codegen_write(gen, " {\n");
        codegen_increase_indent(gen);
        
        for (int i = 0; i < node->impl_block.methods->count; i++) {
            codegen_generate_method_decl(gen, node->impl_block.methods->nodes[i]);
        }
        
        codegen_decrease_indent(gen);
        codegen_write(gen, "}");
    }
}

void codegen_generate_let_binding(CodeGenerator* gen, ASTNode* node) {
    if (node->type != AST_LET_BINDING) return;
    
    codegen_write(gen, "const ");
    codegen_write(gen, node->let_binding.name);
    
    if (node->let_binding.type_annotation) {
        codegen_write(gen, ": ");
        codegen_generate_type_annotation(gen, node->let_binding.type_annotation);
    }
    
    codegen_write(gen, " = ");
    codegen_generate_expression(gen, node->let_binding.value);
    codegen_write(gen, ";");
}

void codegen_generate_match_expr(CodeGenerator* gen, ASTNode* node) {
    if (node->type != AST_MATCH_EXPR) return;
    
    codegen_write(gen, "(() => {\n");
    codegen_increase_indent(gen);
    codegen_write_line(gen, "const __match_value = ");
    codegen_generate_expression(gen, node->match_expr.expr);
    codegen_write(gen, ";\n");
    
    for (int i = 0; i < node->match_expr.arms->count; i++) {
        ASTNode* arm = node->match_expr.arms->nodes[i];
        
        if (i == 0) {
            codegen_write_indent(gen);
            codegen_write(gen, "if (");
        } else {
            codegen_write_indent(gen);
            codegen_write(gen, "} else if (");
        }
        
        // Generate pattern matching condition
        ASTNode* pattern = arm->match_arm.pattern;
        if (pattern->type == AST_IDENTIFIER && strcmp(pattern->identifier.name, "_") == 0) {
            codegen_write(gen, "true");
        } else if (pattern->type == AST_ATOM_LITERAL) {
            codegen_write(gen, "__match_value === ");
            codegen_write(gen, codegen_atom_to_symbol(pattern->atom_literal.value));
        } else {
            codegen_write(gen, "__match_value === ");
            codegen_generate_expression(gen, pattern);
        }
        
        // Add guard condition if present
        if (arm->match_arm.guard) {
            codegen_write(gen, " && (");
            codegen_generate_expression(gen, arm->match_arm.guard);
            codegen_write(gen, ")");
        }
        
        codegen_write(gen, ") {\n");
        codegen_increase_indent(gen);
        codegen_write_indent(gen);
        codegen_write(gen, "return ");
        codegen_generate_expression(gen, arm->match_arm.body);
        codegen_write(gen, ";\n");
        codegen_decrease_indent(gen);
    }
    
    codegen_write_line(gen, "} else {");
    codegen_increase_indent(gen);
    codegen_write_line(gen, "throw new Error(\"Non-exhaustive match\");");
    codegen_decrease_indent(gen);
    codegen_write_line(gen, "}");
    
    codegen_decrease_indent(gen);
    codegen_write(gen, "})()");
}

void codegen_generate_pipe_expr(CodeGenerator* gen, ASTNode* node) {
    if (node->type != AST_PIPE_EXPR) return;
    
    // Handle method chaining for built-in methods
    if (node->pipe_expr.right->type == AST_IDENTIFIER) {
        char* method_name = node->pipe_expr.right->identifier.name;
        
        // Check if it's a built-in string/array method
        if (strcmp(method_name, "trim") == 0 || 
            strcmp(method_name, "toUpperCase") == 0 ||
            strcmp(method_name, "toLowerCase") == 0 ||
            strcmp(method_name, "length") == 0) {
            
            codegen_generate_expression(gen, node->pipe_expr.left);
            codegen_write(gen, ".");
            codegen_write(gen, method_name);
            if (strcmp(method_name, "length") != 0) {
                codegen_write(gen, "()");
            }
            return;
        }
    }
    
    // Default function call transformation: value |> func => func(value)
    codegen_generate_expression(gen, node->pipe_expr.right);
    codegen_write(gen, "(");
    codegen_generate_expression(gen, node->pipe_expr.left);
    codegen_write(gen, ")");
}

void codegen_generate_identifier(CodeGenerator* gen, ASTNode* node) {
    if (node->type != AST_IDENTIFIER) return;
    codegen_write(gen, node->identifier.name);
}

void codegen_generate_literal(CodeGenerator* gen, ASTNode* node) {
    switch (node->type) {
        case AST_NUMBER_LITERAL:
            codegen_write(gen, node->number_literal.value);
            break;
        case AST_STRING_LITERAL: {
            char* escaped = codegen_escape_string(node->string_literal.value);
            codegen_write(gen, "\"");
            codegen_write(gen, escaped);
            codegen_write(gen, "\"");
            free(escaped);
            break;
        }
        case AST_ATOM_LITERAL: {
            char* symbol = codegen_atom_to_symbol(node->atom_literal.value);
            codegen_write(gen, symbol);
            free(symbol);
            break;
        }
        default:
            break;
    }
}

void codegen_generate_block(CodeGenerator* gen, ASTNode* node) {
    if (node->type != AST_BLOCK) return;
    
    codegen_write(gen, "{\n");
    codegen_increase_indent(gen);
    
    for (int i = 0; i < node->block.statements->count; i++) {
        codegen_write_indent(gen);
        codegen_generate_expression(gen, node->block.statements->nodes[i]);
        codegen_write(gen, ";\n");
    }
    
    codegen_decrease_indent(gen);
    codegen_write_indent(gen);
    codegen_write(gen, "}");
}

void codegen_generate_type_annotation(CodeGenerator* gen, ASTNode* node) {
    if (node->type != AST_TYPE_ANNOTATION) return;
    
    codegen_write(gen, node->type_annotation.type_name);
    
    if (node->type_annotation.generic_args && node->type_annotation.generic_args->count > 0) {
        codegen_write(gen, "<");
        for (int i = 0; i < node->type_annotation.generic_args->count; i++) {
            if (i > 0) codegen_write(gen, ", ");
            codegen_generate_type_annotation(gen, node->type_annotation.generic_args->nodes[i]);
        }
        codegen_write(gen, ">");
    }
}

void codegen_generate_field_decl(CodeGenerator* gen, ASTNode* node) {
    if (node->type != AST_FIELD_DECL) return;
    
    codegen_write_indent(gen);
    codegen_write(gen, node->field_decl.name);
    codegen_write(gen, ": ");
    codegen_generate_type_annotation(gen, node->field_decl.type_annotation);
    codegen_write(gen, ";\n");
}

void codegen_generate_method_decl(CodeGenerator* gen, ASTNode* node) {
    if (node->type != AST_METHOD_DECL) return;
    
    codegen_write_indent(gen);
    codegen_write(gen, node->method_decl.name);
    codegen_write(gen, "(");
    
    if (node->method_decl.params) {
        codegen_generate_parameter_list(gen, node->method_decl.params);
    }
    
    codegen_write(gen, ")");
    
    if (node->method_decl.return_type) {
        codegen_write(gen, ": ");
        codegen_generate_type_annotation(gen, node->method_decl.return_type);
    }
    
    codegen_write(gen, " ");
    
    if (node->method_decl.body) {
        codegen_generate_block(gen, node->method_decl.body);
    } else {
        codegen_write(gen, "{}");
    }
    
    codegen_write(gen, "\n");
}

void codegen_generate_call_expr(CodeGenerator* gen, ASTNode* node) {
    if (node->type != AST_CALL_EXPR) return;
    
    // Generate the function name/expression
    codegen_generate_expression(gen, node->call_expr.function);
    
    // Generate the argument list
    codegen_write(gen, "(");
    if (node->call_expr.args && node->call_expr.args->count > 0) {
        for (int i = 0; i < node->call_expr.args->count; i++) {
            if (i > 0) codegen_write(gen, ", ");
            codegen_generate_expression(gen, node->call_expr.args->nodes[i]);
        }
    }
    codegen_write(gen, ")");
}

void codegen_generate_expression(CodeGenerator* gen, ASTNode* node) {
    switch (node->type) {
        case AST_IDENTIFIER:
            codegen_generate_identifier(gen, node);
            break;
        case AST_NUMBER_LITERAL:
        case AST_STRING_LITERAL:
        case AST_ATOM_LITERAL:
            codegen_generate_literal(gen, node);
            break;
        case AST_PIPE_EXPR:
            codegen_generate_pipe_expr(gen, node);
            break;
        case AST_MATCH_EXPR:
            codegen_generate_match_expr(gen, node);
            break;
        case AST_BLOCK:
            codegen_generate_block(gen, node);
            break;
        case AST_CALL_EXPR:
            codegen_generate_call_expr(gen, node);
            break;
        default:
            break;
    }
}

void codegen_generate_generic_params(CodeGenerator* gen, ASTList* params) {
    if (!params || params->count == 0) return;
    
    codegen_write(gen, "<");
    for (int i = 0; i < params->count; i++) {
        if (i > 0) codegen_write(gen, ", ");
        codegen_write(gen, params->nodes[i]->identifier.name);
    }
    codegen_write(gen, ">");
}

void codegen_generate_parameter_list(CodeGenerator* gen, ASTList* params) {
    if (!params) return;
    
    for (int i = 0; i < params->count; i++) {
        if (i > 0) codegen_write(gen, ", ");
        
        ASTNode* param = params->nodes[i];
        codegen_write(gen, param->param_decl.name);
        
        if (param->param_decl.type_annotation) {
            codegen_write(gen, ": ");
            codegen_generate_type_annotation(gen, param->param_decl.type_annotation);
        }
    }
}

char* codegen_escape_string(const char* str) {
    if (!str) return NULL;
    
    int len = strlen(str);
    char* escaped = malloc(len * 2 + 1); // Worst case: every char is escaped
    int j = 0;
    
    for (int i = 0; i < len; i++) {
        switch (str[i]) {
            case '"':
                escaped[j++] = '\\';
                escaped[j++] = '"';
                break;
            case '\\':
                escaped[j++] = '\\';
                escaped[j++] = '\\';
                break;
            case '\n':
                escaped[j++] = '\\';
                escaped[j++] = 'n';
                break;
            case '\t':
                escaped[j++] = '\\';
                escaped[j++] = 't';
                break;
            case '\r':
                escaped[j++] = '\\';
                escaped[j++] = 'r';
                break;
            default:
                escaped[j++] = str[i];
                break;
        }
    }
    
    escaped[j] = '\0';
    return escaped;
}

char* codegen_atom_to_symbol(const char* atom) {
    if (!atom || atom[0] != ':') return NULL;
    
    int len = strlen(atom);
    char* symbol = malloc(len + 20); // Extra space for Symbol.for("...")
    
    sprintf(symbol, "Symbol.for(\"%s\")", atom + 1); // Skip the ':'
    
    return symbol;
}