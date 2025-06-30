#include "zir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ZIR Node creation and management
ZIRNode* zir_node_new(ZIRNodeType type) {
    ZIRNode* node = malloc(sizeof(ZIRNode));
    if (!node) return NULL;
    
    memset(node, 0, sizeof(ZIRNode));
    node->type = type;
    node->zir_type = NULL;
    node->line = 0;
    node->column = 0;
    node->source_file = NULL;
    node->has_optional_return = false;
    node->has_optional_parens = false;
    
    return node;
}

void zir_node_free(ZIRNode* node) {
    if (!node) return;
    
    // Free type information
    if (node->zir_type) {
        zir_type_free(node->zir_type);
    }
    
    // Free source file
    if (node->source_file) {
        free(node->source_file);
    }
    
    // Free node-specific data
    switch (node->type) {
        case ZIR_MODULE:
            free(node->module.name);
            zir_list_free(node->module.declarations);
            zir_list_free(node->module.imports);
            break;
            
        case ZIR_STRUCT_DECL:
            free(node->struct_decl.name);
            zir_list_free(node->struct_decl.generic_params);
            zir_list_free(node->struct_decl.fields);
            break;
            
        case ZIR_TRAIT_DECL:
            free(node->trait_decl.name);
            zir_list_free(node->trait_decl.generic_params);
            zir_list_free(node->trait_decl.methods);
            break;
            
        case ZIR_FUNCTION_DECL:
            free(node->function_decl.name);
            zir_list_free(node->function_decl.params);
            if (node->function_decl.return_type) {
                zir_type_free(node->function_decl.return_type);
            }
            zir_node_free(node->function_decl.body);
            break;
            
        case ZIR_LET_BINDING:
            free(node->let_binding.name);
            zir_node_free(node->let_binding.value);
            if (node->let_binding.type_annotation) {
                zir_type_free(node->let_binding.type_annotation);
            }
            break;
            
        case ZIR_MATCH_EXPR:
            zir_node_free(node->match_expr.expr);
            zir_list_free(node->match_expr.arms);
            break;
            
        case ZIR_PIPE_EXPR:
            zir_node_free(node->pipe_expr.left);
            zir_node_free(node->pipe_expr.right);
            if (node->pipe_expr.result_type) {
                zir_type_free(node->pipe_expr.result_type);
            }
            break;
            
        case ZIR_CALL_EXPR:
            zir_node_free(node->call_expr.function);
            zir_list_free(node->call_expr.args);
            break;
            
        case ZIR_IDENTIFIER:
            free(node->identifier.name);
            if (node->identifier.resolved_type) {
                zir_type_free(node->identifier.resolved_type);
            }
            break;
            
        case ZIR_LITERAL:
            if (node->literal.lit_type == ZIR_LIT_STRING) {
                free(node->literal.string_value);
            } else if (node->literal.lit_type == ZIR_LIT_ATOM) {
                free(node->literal.atom_value);
            }
            break;
            
        case ZIR_BLOCK:
            zir_list_free(node->block.statements);
            zir_node_free(node->block.return_expr);
            break;
            
        case ZIR_IF_EXPR:
            zir_node_free(node->if_expr.condition);
            zir_node_free(node->if_expr.then_branch);
            zir_node_free(node->if_expr.else_branch);
            break;
            
        case ZIR_RETURN_STMT:
            zir_node_free(node->return_stmt.value);
            break;
            
        case ZIR_ASSIGNMENT:
            free(node->assignment.target);
            zir_node_free(node->assignment.value);
            break;
            
        case ZIR_MEMBER_ACCESS:
            zir_node_free(node->member_access.object);
            free(node->member_access.member);
            if (node->member_access.member_type) {
                zir_type_free(node->member_access.member_type);
            }
            break;
            
        case ZIR_BINARY_OP:
            zir_node_free(node->binary_op.left);
            zir_node_free(node->binary_op.right);
            break;
            
        case ZIR_ARRAY_LITERAL:
            zir_list_free(node->array_literal.elements);
            if (node->array_literal.element_type) {
                zir_type_free(node->array_literal.element_type);
            }
            break;
            
        case ZIR_OBJECT_LITERAL:
            zir_list_free(node->object_literal.fields);
            break;
            
        default:
            break;
    }
    
    free(node);
}

// ZIR List management
ZIRList* zir_list_new(void) {
    ZIRList* list = malloc(sizeof(ZIRList));
    if (!list) return NULL;
    
    list->capacity = 10;
    list->nodes = malloc(sizeof(ZIRNode*) * list->capacity);
    list->count = 0;
    
    return list;
}

void zir_list_free(ZIRList* list) {
    if (!list) return;
    
    for (int i = 0; i < list->count; i++) {
        zir_node_free(list->nodes[i]);
    }
    
    free(list->nodes);
    free(list);
}

void zir_list_add(ZIRList* list, ZIRNode* node) {
    if (!list || !node) return;
    
    // Resize if needed
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->nodes = realloc(list->nodes, sizeof(ZIRNode*) * list->capacity);
    }
    
    list->nodes[list->count] = node;
    list->count++;
}

ZIRNode* zir_list_get(ZIRList* list, int index) {
    if (!list || index < 0 || index >= list->count) {
        return NULL;
    }
    
    return list->nodes[index];
}

// ZIR Type management
ZIRType* zir_type_new(ZIRTypeKind kind) {
    ZIRType* type = malloc(sizeof(ZIRType));
    if (!type) return NULL;
    
    type->kind = kind;
    type->name = NULL;
    type->generic_args = NULL;
    type->element_type = NULL;
    type->param_types = NULL;
    type->is_optional = false;
    type->is_mutable = false;
    
    return type;
}

void zir_type_free(ZIRType* type) {
    if (!type) return;
    
    free(type->name);
    zir_list_free(type->generic_args);
    zir_type_free(type->element_type);
    zir_list_free(type->param_types);
    free(type);
}

ZIRType* zir_type_clone(ZIRType* type) {
    if (!type) return NULL;
    
    ZIRType* clone = zir_type_new(type->kind);
    clone->name = type->name ? strdup(type->name) : NULL;
    clone->is_optional = type->is_optional;
    clone->is_mutable = type->is_mutable;
    
    // TODO: Deep clone generic_args, element_type, param_types
    
    return clone;
}

bool zir_type_equals(ZIRType* a, ZIRType* b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
    
    if (a->kind != b->kind) return false;
    if (a->is_optional != b->is_optional) return false;
    if (a->is_mutable != b->is_mutable) return false;
    
    // Compare names
    if (a->name && b->name) {
        return strcmp(a->name, b->name) == 0;
    } else if (!a->name && !b->name) {
        return true;
    } else {
        return false;
    }
}

// ZIR Module management
ZIRModule* zir_module_new(char* name, char* source_file) {
    ZIRModule* module = malloc(sizeof(ZIRModule));
    if (!module) return NULL;
    
    module->name = strdup(name);
    module->source_file = strdup(source_file);
    module->declarations = zir_list_new();
    module->imports = zir_list_new();
    module->exports = zir_list_new();
    
    return module;
}

void zir_module_free(ZIRModule* module) {
    if (!module) return;
    
    free(module->name);
    free(module->source_file);
    zir_list_free(module->declarations);
    zir_list_free(module->imports);
    zir_list_free(module->exports);
    free(module);
}

// ZIR creation helpers
ZIRNode* zir_create_identifier(char* name) {
    ZIRNode* node = zir_node_new(ZIR_IDENTIFIER);
    if (!node) return NULL;
    
    node->identifier.name = strdup(name);
    node->identifier.resolved_type = NULL;
    
    return node;
}

ZIRNode* zir_create_string_literal(char* value) {
    ZIRNode* node = zir_node_new(ZIR_LITERAL);
    if (!node) return NULL;
    
    node->literal.lit_type = ZIR_LIT_STRING;
    node->literal.string_value = strdup(value);
    
    return node;
}

ZIRNode* zir_create_int_literal(long value) {
    ZIRNode* node = zir_node_new(ZIR_LITERAL);
    if (!node) return NULL;
    
    node->literal.lit_type = ZIR_LIT_INT;
    node->literal.int_value = value;
    
    return node;
}

ZIRNode* zir_create_bool_literal(bool value) {
    ZIRNode* node = zir_node_new(ZIR_LITERAL);
    if (!node) return NULL;
    
    node->literal.lit_type = ZIR_LIT_BOOL;
    node->literal.bool_value = value;
    
    return node;
}

ZIRNode* zir_create_atom_literal(char* value) {
    ZIRNode* node = zir_node_new(ZIR_LITERAL);
    if (!node) return NULL;
    
    node->literal.lit_type = ZIR_LIT_ATOM;
    node->literal.atom_value = strdup(value);
    
    return node;
}

// Debugging and inspection
void zir_print(ZIRNode* node, int indent) {
    if (!node) return;
    
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    
    printf("%s", zir_node_type_to_string(node->type));
    
    switch (node->type) {
        case ZIR_IDENTIFIER:
            printf(": %s", node->identifier.name);
            break;
        case ZIR_LITERAL:
            printf(": ");
            switch (node->literal.lit_type) {
                case ZIR_LIT_STRING:
                    printf("\"%s\"", node->literal.string_value);
                    break;
                case ZIR_LIT_INT:
                    printf("%ld", node->literal.int_value);
                    break;
                case ZIR_LIT_BOOL:
                    printf("%s", node->literal.bool_value ? "true" : "false");
                    break;
                case ZIR_LIT_ATOM:
                    printf(":%s", node->literal.atom_value);
                    break;
                default:
                    printf("unknown");
                    break;
            }
            break;
        default:
            break;
    }
    
    printf("\n");
}

const char* zir_node_type_to_string(ZIRNodeType type) {
    switch (type) {
        case ZIR_MODULE: return "Module";
        case ZIR_STRUCT_DECL: return "StructDecl";
        case ZIR_TRAIT_DECL: return "TraitDecl";
        case ZIR_FUNCTION_DECL: return "FunctionDecl";
        case ZIR_LET_BINDING: return "LetBinding";
        case ZIR_MATCH_EXPR: return "MatchExpr";
        case ZIR_PIPE_EXPR: return "PipeExpr";
        case ZIR_CALL_EXPR: return "CallExpr";
        case ZIR_IDENTIFIER: return "Identifier";
        case ZIR_LITERAL: return "Literal";
        case ZIR_BLOCK: return "Block";
        case ZIR_IF_EXPR: return "IfExpr";
        case ZIR_RETURN_STMT: return "ReturnStmt";
        case ZIR_ASSIGNMENT: return "Assignment";
        case ZIR_MEMBER_ACCESS: return "MemberAccess";
        case ZIR_BINARY_OP: return "BinaryOp";
        case ZIR_UNARY_OP: return "UnaryOp";
        case ZIR_ARRAY_LITERAL: return "ArrayLiteral";
        case ZIR_OBJECT_LITERAL: return "ObjectLiteral";
        default: return "Unknown";
    }
}

const char* zir_type_to_string(ZIRType* type) {
    if (!type) return "null";
    
    switch (type->kind) {
        case ZIR_TYPE_VOID: return "void";
        case ZIR_TYPE_BOOL: return "bool";
        case ZIR_TYPE_INT: return "int";
        case ZIR_TYPE_FLOAT: return "float";
        case ZIR_TYPE_STRING: return "string";
        case ZIR_TYPE_ARRAY: return "array";
        case ZIR_TYPE_OBJECT: return "object";
        case ZIR_TYPE_FUNCTION: return "function";
        case ZIR_TYPE_GENERIC: return "generic";
        case ZIR_TYPE_USER_DEFINED: return type->name ? type->name : "user_defined";
        case ZIR_TYPE_ATOM: return "atom";
        default: return "unknown";
    }
}

// Stub implementations for remaining functions
ZIRNode* zir_create_module(char* name, ZIRList* declarations) {
    ZIRNode* node = zir_node_new(ZIR_MODULE);
    if (!node) return NULL;
    
    node->module.name = strdup(name);
    node->module.declarations = declarations;
    node->module.imports = zir_list_new();
    
    return node;
}

// Additional stub implementations
ZIRNode* zir_create_struct_decl(char* name, ZIRList* generic_params, ZIRList* fields) {
    ZIRNode* node = zir_node_new(ZIR_STRUCT_DECL);
    if (!node) return NULL;
    
    node->struct_decl.name = strdup(name);
    node->struct_decl.generic_params = generic_params;
    node->struct_decl.fields = fields;
    node->struct_decl.is_empty = (fields == NULL || fields->count == 0);
    
    return node;
}

// More stub implementations...
ZIRNode* zir_create_trait_decl(char* name, ZIRList* generic_params, ZIRList* methods) { return NULL; }
ZIRNode* zir_create_function_decl(char* name, ZIRList* params, ZIRType* return_type, ZIRNode* body) { return NULL; }
ZIRNode* zir_create_let_binding(char* name, ZIRNode* value, ZIRType* type_annotation) { return NULL; }
ZIRNode* zir_create_match_expr(ZIRNode* expr, ZIRList* arms) { return NULL; }
ZIRNode* zir_create_pipe_expr(ZIRNode* left, ZIRNode* right) { return NULL; }
ZIRNode* zir_create_call_expr(ZIRNode* function, ZIRList* args) { return NULL; }
ZIRNode* zir_create_block(ZIRList* statements) { return NULL; }
ZIRNode* zir_create_if_expr(ZIRNode* condition, ZIRNode* then_branch, ZIRNode* else_branch) { return NULL; }
ZIRNode* zir_create_binary_op(ZIRBinaryOp op, ZIRNode* left, ZIRNode* right) { return NULL; }

void zir_analyze_types(ZIRModule* module) { /* TODO */ }
void zir_resolve_symbols(ZIRModule* module) { /* TODO */ }
void zir_optimize(ZIRModule* module) { /* TODO */ }
char* zir_serialize(ZIRModule* module) { return strdup("/* TODO: ZIR serialization */"); }
ZIRModule* zir_deserialize(char* data) { return NULL; }