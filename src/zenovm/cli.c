#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "zenovm.h"
#include "backends/backend.h"

#define ZENOVM_VERSION "0.1.0"

void print_usage(const char* program_name) {
    printf("ZenoVM - Zenoscript Virtual Machine and Multi-Target Compiler\n");
    printf("Version: %s\n\n", ZENOVM_VERSION);
    printf("Usage: %s [options] <command> [file]\n\n", program_name);
    
    printf("Commands:\n");
    printf("  compile    Compile Zenoscript to target language\n");
    printf("  backends   List available backends\n");
    printf("  version    Show version information\n");
    printf("  help       Show this help message\n\n");
    
    printf("Options:\n");
    printf("  -t, --target <backend>    Target backend (default: c)\n");
    printf("  -o, --output <file>       Output file\n");
    printf("  -O, --optimize <level>    Optimization level (0-3)\n");
    printf("  -v, --verbose             Verbose output\n");
    printf("  -d, --debug               Enable debug mode\n");
    printf("  --emit-ir                 Emit intermediate representation\n");
    printf("  -h, --help                Show help\n\n");
    
    printf("Examples:\n");
    printf("  %s compile --target=c input.zs -o output.c\n", program_name);
    printf("  %s compile --target=go input.zs\n", program_name);
    printf("  %s backends\n", program_name);
    printf("  %s --help\n", program_name);
}

void print_version(void) {
    printf("ZenoVM %s\n", ZENOVM_VERSION);
    printf("Zenoscript Multi-Target Virtual Machine\n");
    printf("Copyright (c) 2024 Zenoscript Project\n");
}

void print_backends(ZenoVM* vm) {
    printf("Available backends:\n");
    char** backends = zenovm_list_backends(vm);
    
    if (backends) {
        for (int i = 0; backends[i]; i++) {
            ZenoBackend* backend = zenovm_get_backend(vm, backends[i]);
            if (backend) {
                printf("  %-12s %s - %s\n", 
                       backend->name, 
                       backend->version, 
                       backend->description);
            }
        }
        
        // Free the list
        for (int i = 0; backends[i]; i++) {
            free(backends[i]);
        }
        free(backends);
    } else {
        printf("  No backends available\n");
    }
}

int cmd_compile(ZenoVM* vm, int argc, char* argv[]) {
    ZenoCompileOptions* options = zeno_default_options();
    char* input_file = NULL;
    
    // Parse compile-specific options
    int opt;
    int option_index = 0;
    
    static struct option long_options[] = {
        {"target",    required_argument, 0, 't'},
        {"output",    required_argument, 0, 'o'},
        {"optimize",  required_argument, 0, 'O'},
        {"verbose",   no_argument,       0, 'v'},
        {"debug",     no_argument,       0, 'd'},
        {"emit-ir",   no_argument,       0, 'i'},
        {"help",      no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "t:o:O:vdih", long_options, &option_index)) != -1) {
        switch (opt) {
            case 't':
                options->target_backend = strdup(optarg);
                break;
            case 'o':
                options->output_file = strdup(optarg);
                break;
            case 'O':
                options->opt_level = atoi(optarg);
                if (options->opt_level < 0 || options->opt_level > 3) {
                    fprintf(stderr, "Error: Optimization level must be 0-3\n");
                    return 1;
                }
                break;
            case 'v':
                options->verbose = true;
                break;
            case 'd':
                options->debug_info = true;
                break;
            case 'i':
                options->emit_ir = true;
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            case '?':
                return 1;
            default:
                break;
        }
    }
    
    // Get input file
    if (optind < argc) {
        input_file = argv[optind];
        options->source_file = strdup(input_file);
    } else {
        fprintf(stderr, "Error: No input file specified\n");
        return 1;
    }
    
    // Set default target if not specified
    if (!options->target_backend) {
        options->target_backend = strdup("c");
    }
    
    // Check if backend exists
    ZenoBackend* backend = zenovm_get_backend(vm, options->target_backend);
    if (!backend) {
        fprintf(stderr, "Error: Backend '%s' not found\n", options->target_backend);
        printf("Available backends:\n");
        print_backends(vm);
        return 1;
    }
    
    if (options->verbose) {
        printf("Compiling '%s' using '%s' backend...\n", input_file, options->target_backend);
    }
    
    // Set options and compile
    zenovm_set_options(vm, options);
    
    int result = zenovm_compile_file(vm, input_file, options->output_file, options->target_backend);
    
    if (result != 0) {
        fprintf(stderr, "Compilation failed\n");
        if (zenovm_has_errors(vm)) {
            char** errors = zenovm_get_errors(vm);
            for (int i = 0; errors && errors[i]; i++) {
                fprintf(stderr, "Error: %s\n", errors[i]);
            }
        }
        return 1;
    }
    
    if (options->verbose) {
        printf("Compilation successful\n");
        zenovm_print_stats(vm);
    }
    
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    // Create ZenoVM instance
    ZenoVM* vm = zenovm_create();
    if (!vm) {
        fprintf(stderr, "Error: Failed to create ZenoVM instance\n");
        return 1;
    }
    
    // Register all backends
    backend_register_all(vm);
    
    // Parse global options and commands
    char* command = argv[1];
    
    if (strcmp(command, "compile") == 0) {
        return cmd_compile(vm, argc - 1, argv + 1);
    } else if (strcmp(command, "backends") == 0) {
        print_backends(vm);
        return 0;
    } else if (strcmp(command, "version") == 0) {
        print_version();
        return 0;
    } else if (strcmp(command, "help") == 0 || strcmp(command, "--help") == 0) {
        print_usage(argv[0]);
        return 0;
    } else {
        fprintf(stderr, "Error: Unknown command '%s'\n", command);
        print_usage(argv[0]);
        return 1;
    }
    
    zenovm_free(vm);
    return 0;
}