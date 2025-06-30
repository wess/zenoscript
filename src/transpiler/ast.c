#include "ast.h"

ASTNode* ast_node_new(ASTNodeType type) {
    ASTNode* node = malloc(sizeof(ASTNode));
    memset(node, 0, sizeof(ASTNode));
    node->type = type;
    return node;
}

void ast_node_free(ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_PROGRAM:
            ast_list_free(node->program.declarations);
            break;
            
        case AST_STRUCT_DECL:
            free(node->struct_decl.name);
            ast_list_free(node->struct_decl.generic_params);
            ast_list_free(node->struct_decl.fields);
            break;
            
        case AST_TRAIT_DECL:
            free(node->trait_decl.name);
            ast_list_free(node->trait_decl.generic_params);
            ast_list_free(node->trait_decl.methods);
            break;
            
        case AST_IMPL_BLOCK:
            free(node->impl_block.trait_name);
            free(node->impl_block.type_name);
            ast_list_free(node->impl_block.generic_params);
            ast_list_free(node->impl_block.methods);
            break;
            
        case AST_LET_BINDING:
            free(node->let_binding.name);
            ast_node_free(node->let_binding.value);
            ast_node_free(node->let_binding.type_annotation);
            break;
            
        case AST_MATCH_EXPR:
            ast_node_free(node->match_expr.expr);
            ast_list_free(node->match_expr.arms);
            break;
            
        case AST_PIPE_EXPR:
            ast_node_free(node->pipe_expr.left);
            ast_node_free(node->pipe_expr.right);
            break;
            
        case AST_FUNCTION_DECL:
            free(node->function_decl.name);
            ast_list_free(node->function_decl.params);
            ast_node_free(node->function_decl.return_type);
            ast_node_free(node->function_decl.body);
            break;
            
        case AST_IDENTIFIER:
            free(node->identifier.name);
            break;
            
        case AST_NUMBER_LITERAL:
            free(node->number_literal.value);
            break;
            
        case AST_STRING_LITERAL:
            free(node->string_literal.value);
            break;
            
        case AST_ATOM_LITERAL:
            free(node->atom_literal.value);
            break;
            
        case AST_BLOCK:
            ast_list_free(node->block.statements);
            break;
            
        case AST_FIELD_DECL:
            free(node->field_decl.name);
            ast_node_free(node->field_decl.type_annotation);
            break;
            
        case AST_METHOD_DECL:
            free(node->method_decl.name);
            ast_list_free(node->method_decl.params);
            ast_node_free(node->method_decl.return_type);
            ast_node_free(node->method_decl.body);
            break;
            
        case AST_PARAM_DECL:
            free(node->param_decl.name);
            ast_node_free(node->param_decl.type_annotation);
            break;
            
        case AST_TYPE_ANNOTATION:
            free(node->type_annotation.type_name);
            ast_list_free(node->type_annotation.generic_args);
            break;
            
        case AST_GENERIC_PARAMS:
            ast_list_free(node->generic_params.params);
            break;
            
        case AST_MATCH_ARM:
            ast_node_free(node->match_arm.pattern);
            ast_node_free(node->match_arm.guard);
            ast_node_free(node->match_arm.body);
            break;
            
        case AST_PATTERN:
            ast_node_free(node->pattern.pattern);
            break;
            
        case AST_EXPRESSION:
            ast_node_free(node->expression.expr);
            break;
            
        case AST_CALL_EXPR:
            ast_node_free(node->call_expr.function);
            ast_list_free(node->call_expr.args);
            break;
            
        case AST_MEMBER_ACCESS:
            ast_node_free(node->member_access.object);
            free(node->member_access.member);
            break;
            
        case AST_ASSIGNMENT:
            free(node->assignment.target);
            ast_node_free(node->assignment.value);
            break;
    }
    
    free(node);
}

ASTList* ast_list_new(void) {
    ASTList* list = malloc(sizeof(ASTList));
    list->nodes = NULL;
    list->count = 0;
    list->capacity = 0;
    return list;
}

void ast_list_free(ASTList* list) {
    if (!list) return;
    
    for (int i = 0; i < list->count; i++) {
        ast_node_free(list->nodes[i]);
    }
    
    free(list->nodes);
    free(list);
}

void ast_list_add(ASTList* list, ASTNode* node) {
    if (list->count >= list->capacity) {
        list->capacity = list->capacity == 0 ? 8 : list->capacity * 2;
        list->nodes = realloc(list->nodes, list->capacity * sizeof(ASTNode*));
    }
    
    list->nodes[list->count++] = node;
}

ASTNode* ast_list_get(ASTList* list, int index) {
    if (index < 0 || index >= list->count) {
        return NULL;
    }
    return list->nodes[index];
}

// AST creation helpers
ASTNode* ast_create_program(ASTList* declarations) {
    ASTNode* node = ast_node_new(AST_PROGRAM);
    node->program.declarations = declarations;
    return node;
}

ASTNode* ast_create_struct_decl(char* name, ASTList* generic_params, ASTList* fields) {
    ASTNode* node = ast_node_new(AST_STRUCT_DECL);
    node->struct_decl.name = strdup(name);
    node->struct_decl.generic_params = generic_params;
    node->struct_decl.fields = fields;
    return node;
}

ASTNode* ast_create_trait_decl(char* name, ASTList* generic_params, ASTList* methods) {
    ASTNode* node = ast_node_new(AST_TRAIT_DECL);
    node->trait_decl.name = strdup(name);
    node->trait_decl.generic_params = generic_params;
    node->trait_decl.methods = methods;
    return node;
}

ASTNode* ast_create_impl_block(char* trait_name, char* type_name, ASTList* generic_params, ASTList* methods) {
    ASTNode* node = ast_node_new(AST_IMPL_BLOCK);
    node->impl_block.trait_name = trait_name ? strdup(trait_name) : NULL;
    node->impl_block.type_name = strdup(type_name);
    node->impl_block.generic_params = generic_params;
    node->impl_block.methods = methods;
    return node;
}

ASTNode* ast_create_let_binding(char* name, ASTNode* value, ASTNode* type_annotation) {
    ASTNode* node = ast_node_new(AST_LET_BINDING);
    node->let_binding.name = strdup(name);
    node->let_binding.value = value;
    node->let_binding.type_annotation = type_annotation;
    return node;
}

ASTNode* ast_create_match_expr(ASTNode* expr, ASTList* arms) {
    ASTNode* node = ast_node_new(AST_MATCH_EXPR);
    node->match_expr.expr = expr;
    node->match_expr.arms = arms;
    return node;
}

ASTNode* ast_create_pipe_expr(ASTNode* left, ASTNode* right) {
    ASTNode* node = ast_node_new(AST_PIPE_EXPR);
    node->pipe_expr.left = left;
    node->pipe_expr.right = right;
    return node;
}

ASTNode* ast_create_identifier(char* name) {
    ASTNode* node = ast_node_new(AST_IDENTIFIER);
    node->identifier.name = strdup(name);
    return node;
}

ASTNode* ast_create_number_literal(char* value) {
    ASTNode* node = ast_node_new(AST_NUMBER_LITERAL);
    node->number_literal.value = strdup(value);
    return node;
}

ASTNode* ast_create_string_literal(char* value) {
    ASTNode* node = ast_node_new(AST_STRING_LITERAL);
    node->string_literal.value = strdup(value);
    return node;
}

ASTNode* ast_create_atom_literal(char* value) {
    ASTNode* node = ast_node_new(AST_ATOM_LITERAL);
    node->atom_literal.value = strdup(value);
    return node;
}

ASTNode* ast_create_block(ASTList* statements) {
    ASTNode* node = ast_node_new(AST_BLOCK);
    node->block.statements = statements;
    return node;
}

ASTNode* ast_create_field_decl(char* name, ASTNode* type_annotation) {
    ASTNode* node = ast_node_new(AST_FIELD_DECL);
    node->field_decl.name = strdup(name);
    node->field_decl.type_annotation = type_annotation;
    return node;
}

ASTNode* ast_create_method_decl(char* name, ASTList* params, ASTNode* return_type, ASTNode* body) {
    ASTNode* node = ast_node_new(AST_METHOD_DECL);
    node->method_decl.name = strdup(name);
    node->method_decl.params = params;
    node->method_decl.return_type = return_type;
    node->method_decl.body = body;
    return node;
}

ASTNode* ast_create_call_expr(ASTNode* function, ASTList* args) {
    ASTNode* node = ast_node_new(AST_CALL_EXPR);
    node->call_expr.function = function;
    node->call_expr.args = args;
    return node;
}

const char* ast_node_type_to_string(ASTNodeType type) {
    switch (type) {
        case AST_PROGRAM: return "PROGRAM";
        case AST_STRUCT_DECL: return "STRUCT_DECL";
        case AST_TRAIT_DECL: return "TRAIT_DECL";
        case AST_IMPL_BLOCK: return "IMPL_BLOCK";
        case AST_LET_BINDING: return "LET_BINDING";
        case AST_MATCH_EXPR: return "MATCH_EXPR";
        case AST_PIPE_EXPR: return "PIPE_EXPR";
        case AST_FUNCTION_DECL: return "FUNCTION_DECL";
        case AST_IDENTIFIER: return "IDENTIFIER";
        case AST_NUMBER_LITERAL: return "NUMBER_LITERAL";
        case AST_STRING_LITERAL: return "STRING_LITERAL";
        case AST_ATOM_LITERAL: return "ATOM_LITERAL";
        case AST_BLOCK: return "BLOCK";
        case AST_FIELD_DECL: return "FIELD_DECL";
        case AST_METHOD_DECL: return "METHOD_DECL";
        case AST_PARAM_DECL: return "PARAM_DECL";
        case AST_TYPE_ANNOTATION: return "TYPE_ANNOTATION";
        case AST_GENERIC_PARAMS: return "GENERIC_PARAMS";
        case AST_MATCH_ARM: return "MATCH_ARM";
        case AST_PATTERN: return "PATTERN";
        case AST_EXPRESSION: return "EXPRESSION";
        case AST_CALL_EXPR: return "CALL_EXPR";
        case AST_MEMBER_ACCESS: return "MEMBER_ACCESS";
        case AST_ASSIGNMENT: return "ASSIGNMENT";
        default: return "UNKNOWN";
    }
}

void ast_print(ASTNode* node, int indent) {
    if (!node) return;
    
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    
    printf("%s", ast_node_type_to_string(node->type));
    
    switch (node->type) {
        case AST_IDENTIFIER:
            printf(": %s", node->identifier.name);
            break;
        case AST_NUMBER_LITERAL:
            printf(": %s", node->number_literal.value);
            break;
        case AST_STRING_LITERAL:
            printf(": \"%s\"", node->string_literal.value);
            break;
        case AST_ATOM_LITERAL:
            printf(": %s", node->atom_literal.value);
            break;
        case AST_STRUCT_DECL:
            printf(": %s", node->struct_decl.name);
            break;
        case AST_TRAIT_DECL:
            printf(": %s", node->trait_decl.name);
            break;
        case AST_LET_BINDING:
            printf(": %s", node->let_binding.name);
            break;
        default:
            break;
    }
    
    printf("\n");
    
    // Print child nodes
    switch (node->type) {
        case AST_PROGRAM:
            if (node->program.declarations) {
                for (int i = 0; i < node->program.declarations->count; i++) {
                    ast_print(node->program.declarations->nodes[i], indent + 1);
                }
            }
            break;
            
        case AST_STRUCT_DECL:
            if (node->struct_decl.fields) {
                for (int i = 0; i < node->struct_decl.fields->count; i++) {
                    ast_print(node->struct_decl.fields->nodes[i], indent + 1);
                }
            }
            break;
            
        case AST_LET_BINDING:
            if (node->let_binding.value) {
                ast_print(node->let_binding.value, indent + 1);
            }
            break;
            
        case AST_PIPE_EXPR:
            ast_print(node->pipe_expr.left, indent + 1);
            ast_print(node->pipe_expr.right, indent + 1);
            break;
            
        default:
            break;
    }
}