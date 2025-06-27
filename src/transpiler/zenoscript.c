#include "zenoscript.h"

#define ZENOSCRIPT_VERSION "1.0.0"

char* zenoscript_read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return NULL;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate buffer and read file
    char* buffer = malloc(size + 1);
    size_t read_size = fread(buffer, 1, size, file);
    buffer[read_size] = '\0';
    
    fclose(file);
    return buffer;
}

int zenoscript_write_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Cannot write to file '%s'\n", filename);
        return 0;
    }
    
    fputs(content, file);
    fclose(file);
    return 1;
}

char* zenoscript_transpile_string(const char* source, ZenoscriptOptions* options) {
    if (!source) {
        fprintf(stderr, "Error: No source code provided\n");
        return NULL;
    }
    
    // Create lexer
    Lexer* lexer = lexer_new(source);
    if (!lexer) {
        fprintf(stderr, "Error: Failed to create lexer\n");
        return NULL;
    }
    
    // Create parser
    Parser* parser = parser_new(lexer);
    if (!parser) {
        fprintf(stderr, "Error: Failed to create parser\n");
        lexer_free(lexer);
        return NULL;
    }
    
    // Parse source code
    ASTNode* ast = parser_parse(parser);
    if (!ast || parser_has_errors(parser)) {
        fprintf(stderr, "Error: Parsing failed\n");
        if (parser_has_errors(parser)) {
            fprintf(stderr, "Parse error: %s\n", parser_get_error(parser));
        }
        parser_free(parser);
        lexer_free(lexer);
        return NULL;
    }
    
    if (options && options->debug) {
        printf("=== AST ===\n");
        ast_print(ast, 0);
        printf("\n=== Generated TypeScript ===\n");
    }
    
    // Generate TypeScript code
    char* typescript_code = codegen_generate(ast);
    
    // Cleanup
    ast_node_free(ast);
    parser_free(parser);
    lexer_free(lexer);
    
    return typescript_code;
}

int zenoscript_transpile_file(const char* input_file, const char* output_file, ZenoscriptOptions* options) {
    if (!input_file) {
        fprintf(stderr, "Error: No input file specified\n");
        return 0;
    }
    
    // Read input file
    char* source = zenoscript_read_file(input_file);
    if (!source) {
        return 0;
    }
    
    if (options && options->verbose) {
        printf("Transpiling '%s'...\n", input_file);
    }
    
    // Transpile source code
    char* typescript_code = zenoscript_transpile_string(source, options);
    free(source);
    
    if (!typescript_code) {
        return 0;
    }
    
    // Write output
    if (output_file) {
        int success = zenoscript_write_file(output_file, typescript_code);
        if (success && options && options->verbose) {
            printf("Output written to '%s'\n", output_file);
        }
        free(typescript_code);
        return success;
    } else {
        // Print to stdout
        printf("%s", typescript_code);
        free(typescript_code);
        return 1;
    }
}

void zenoscript_print_tokens(const char* source) {
    Lexer* lexer = lexer_new(source);
    Token token;
    
    printf("=== Tokens ===\n");
    do {
        token = lexer_next_token(lexer);
        printf("%s", token_type_to_string(token.type));
        if (token.value) {
            printf(": %s", token.value);
        }
        printf(" (line %d, col %d)\n", token.line, token.column);
        token_free(&token);
    } while (token.type != TOKEN_EOF);
    
    lexer_free(lexer);
}

void zenoscript_print_ast(const char* source) {
    Lexer* lexer = lexer_new(source);
    Parser* parser = parser_new(lexer);
    ASTNode* ast = parser_parse(parser);
    
    if (ast && !parser_has_errors(parser)) {
        printf("=== AST ===\n");
        ast_print(ast, 0);
        ast_node_free(ast);
    } else {
        printf("Error: Failed to parse source code\n");
        if (parser_has_errors(parser)) {
            printf("Parse error: %s\n", parser_get_error(parser));
        }
    }
    
    parser_free(parser);
    lexer_free(lexer);
}

void zenoscript_print_version(void) {
    char version[32];
    strcpy(version, ZENOSCRIPT_VERSION); // fallback version
    
    // List of possible paths to the VERSION file
    const char* possible_paths[] = {
        "VERSION",           // Current directory
        "../VERSION",        // One level up
        "../../VERSION",     // Two levels up  
        "../../../VERSION",  // Three levels up (from build/transpiler to project root)
        NULL
    };
    
    // Try each path until we find the VERSION file
    for (int i = 0; possible_paths[i] != NULL; i++) {
        FILE* version_file = fopen(possible_paths[i], "r");
        if (version_file) {
            if (fgets(version, sizeof(version), version_file)) {
                // Remove trailing newline if present
                char* newline = strchr(version, '\n');
                if (newline) {
                    *newline = '\0';
                }
            }
            fclose(version_file);
            break;
        }
    }
    
    printf("Zenoscript %s\n", version);
    printf("A functional programming language that compiles to TypeScript\n");
}

void zenoscript_print_help(void) {
    printf("Zeno - Functional Programming Language Transpiler (Core Binary)\n\n");
    printf("USAGE:\n");
    printf("    zeno [OPTIONS] <input-file> [output-file]\n\n");
    printf("OPTIONS:\n");
    printf("    -h, --help       Show this help message\n");
    printf("    -v, --version    Show version information\n");
    printf("    -V, --verbose    Enable verbose output\n");
    printf("    -d, --debug      Enable debug output (show AST)\n\n");
    printf("EXAMPLES:\n");
    printf("    zeno main.zs               # Output to stdout\n");
    printf("    zeno main.zs main.ts       # Output to file\n");
    printf("    zeno --debug main.zs       # Show AST and output\n\n");
    printf("NOTE:\n");
    printf("    This is the core transpiler binary. For full CLI features including\n");
    printf("    project management (init, setup, repl), use the main 'zeno' command.\n");
}