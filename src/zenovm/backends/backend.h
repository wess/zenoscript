#ifndef BACKEND_H
#define BACKEND_H

#include "../zir.h"

// Forward declarations to avoid circular dependencies
typedef struct ZenoVM ZenoVM;
typedef struct ZenoBackend ZenoBackend;

// Common backend utilities and helper functions

// String builder for code generation
typedef struct {
    char* buffer;
    size_t capacity;
    size_t length;
    int indent_level;
} StringBuilder;

StringBuilder* sb_create(void);
void sb_free(StringBuilder* sb);
void sb_append(StringBuilder* sb, const char* str);
void sb_appendf(StringBuilder* sb, const char* format, ...);
void sb_append_line(StringBuilder* sb, const char* str);
void sb_append_indent(StringBuilder* sb);
void sb_increase_indent(StringBuilder* sb);
void sb_decrease_indent(StringBuilder* sb);
char* sb_to_string(StringBuilder* sb);
void sb_clear(StringBuilder* sb);

// Common type mapping utilities
typedef struct {
    ZIRTypeKind zir_type;
    char* target_type;
} TypeMapping;

char* backend_map_type_generic(ZIRType* zir_type, TypeMapping* mappings, int mapping_count);
char* backend_escape_string(const char* str);
char* backend_sanitize_identifier(const char* name);

// Common expression generation patterns
char* backend_generate_binary_op_generic(ZIRNode* node, 
                                        char* (*generate_expr)(ZIRNode*),
                                        char* left_str, char* right_str);

char* backend_generate_call_expr_generic(ZIRNode* node,
                                        char* (*generate_expr)(ZIRNode*),
                                        bool use_optional_parens);

// Template system for common patterns
typedef struct {
    char* name;
    char* template_str;
} CodeTemplate;

char* backend_apply_template(const char* template_str, ...);

// Common templates
extern CodeTemplate TEMPLATE_STRUCT_DECL;
extern CodeTemplate TEMPLATE_FUNCTION_DECL;
extern CodeTemplate TEMPLATE_IF_STMT;
extern CodeTemplate TEMPLATE_MATCH_ARM;

// Backend registration helpers
void backend_register_all(ZenoVM* vm);

// Backend function prototypes
ZenoBackend* c_backend_create(void);

#endif