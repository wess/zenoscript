#include "backend.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// StringBuilder implementation
StringBuilder* sb_create(void) {
    StringBuilder* sb = malloc(sizeof(StringBuilder));
    if (!sb) return NULL;
    
    sb->capacity = 1024;
    sb->buffer = malloc(sb->capacity);
    sb->length = 0;
    sb->indent_level = 0;
    sb->buffer[0] = '\0';
    
    return sb;
}

void sb_free(StringBuilder* sb) {
    if (sb) {
        free(sb->buffer);
        free(sb);
    }
}

static void sb_ensure_capacity(StringBuilder* sb, size_t additional) {
    if (sb->length + additional >= sb->capacity) {
        sb->capacity = (sb->capacity + additional) * 2;
        sb->buffer = realloc(sb->buffer, sb->capacity);
    }
}

void sb_append(StringBuilder* sb, const char* str) {
    if (!str) return;
    
    size_t len = strlen(str);
    sb_ensure_capacity(sb, len + 1);
    
    strcpy(sb->buffer + sb->length, str);
    sb->length += len;
}

void sb_appendf(StringBuilder* sb, const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    // Calculate needed space
    int needed = vsnprintf(NULL, 0, format, args);
    va_end(args);
    
    if (needed > 0) {
        sb_ensure_capacity(sb, needed + 1);
        
        va_start(args, format);
        vsnprintf(sb->buffer + sb->length, needed + 1, format, args);
        va_end(args);
        
        sb->length += needed;
    }
}

void sb_append_line(StringBuilder* sb, const char* str) {
    sb_append_indent(sb);
    sb_append(sb, str);
    sb_append(sb, "\n");
}

void sb_append_indent(StringBuilder* sb) {
    for (int i = 0; i < sb->indent_level; i++) {
        sb_append(sb, "  ");  // 2 spaces per indent level
    }
}

void sb_increase_indent(StringBuilder* sb) {
    sb->indent_level++;
}

void sb_decrease_indent(StringBuilder* sb) {
    if (sb->indent_level > 0) {
        sb->indent_level--;
    }
}

char* sb_to_string(StringBuilder* sb) {
    return strdup(sb->buffer);
}

void sb_clear(StringBuilder* sb) {
    sb->length = 0;
    sb->buffer[0] = '\0';
}

// Type mapping utilities
char* backend_map_type_generic(ZIRType* zir_type, TypeMapping* mappings, int mapping_count) {
    if (!zir_type) return strdup("void");
    
    for (int i = 0; i < mapping_count; i++) {
        if (mappings[i].zir_type == zir_type->kind) {
            return strdup(mappings[i].target_type);
        }
    }
    
    // Fallback to ZIR type name if available
    if (zir_type->name) {
        return strdup(zir_type->name);
    }
    
    return strdup("unknown_type");
}

char* backend_escape_string(const char* str) {
    if (!str) return strdup("");
    
    StringBuilder* sb = sb_create();
    
    for (const char* p = str; *p; p++) {
        switch (*p) {
            case '"':  sb_append(sb, "\\\""); break;
            case '\\': sb_append(sb, "\\\\"); break;
            case '\n': sb_append(sb, "\\n"); break;
            case '\r': sb_append(sb, "\\r"); break;
            case '\t': sb_append(sb, "\\t"); break;
            default:
                if (*p >= 32 && *p <= 126) {
                    char temp[2] = {*p, '\0'};
                    sb_append(sb, temp);
                } else {
                    sb_appendf(sb, "\\x%02x", (unsigned char)*p);
                }
                break;
        }
    }
    
    char* result = sb_to_string(sb);
    sb_free(sb);
    return result;
}

char* backend_sanitize_identifier(const char* name) {
    if (!name) return strdup("unknown");
    
    StringBuilder* sb = sb_create();
    
    for (const char* p = name; *p; p++) {
        if ((*p >= 'a' && *p <= 'z') || 
            (*p >= 'A' && *p <= 'Z') || 
            (*p >= '0' && *p <= '9') || 
            *p == '_') {
            char temp[2] = {*p, '\0'};
            sb_append(sb, temp);
        } else {
            sb_append(sb, "_");
        }
    }
    
    char* result = sb_to_string(sb);
    sb_free(sb);
    return result;
}

// Register all backends
void backend_register_all(ZenoVM* vm) {
    // Register C backend
    ZenoBackend* c_backend = c_backend_create();
    zenovm_register_backend(vm, c_backend);
    
    // TODO: Register other backends as they're implemented
}