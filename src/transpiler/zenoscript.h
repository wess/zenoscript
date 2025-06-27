#ifndef ZENOSCRIPT_H
#define ZENOSCRIPT_H

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "codegen.h"

// Main transpiler interface
typedef struct {
    char* input_file;
    char* output_file;
    char* source_code;
    int verbose;
    int debug;
} ZenoscriptOptions;

// Main functions
int zenoscript_transpile_file(const char* input_file, const char* output_file, ZenoscriptOptions* options);
char* zenoscript_transpile_string(const char* source, ZenoscriptOptions* options);
void zenoscript_print_version(void);
void zenoscript_print_help(void);

// Utility functions
char* zenoscript_read_file(const char* filename);
int zenoscript_write_file(const char* filename, const char* content);
void zenoscript_print_tokens(const char* source);
void zenoscript_print_ast(const char* source);

#endif