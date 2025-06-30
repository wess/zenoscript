#include "zenovm.h"
#include "backends/backend.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ZenoVM* zenovm_create(void) {
    ZenoVM* vm = malloc(sizeof(ZenoVM));
    if (!vm) return NULL;
    
    // Initialize backend registry
    vm->backends = malloc(sizeof(ZenoBackendRegistry));
    vm->backends->backends = malloc(sizeof(ZenoBackend*) * 10);  // Initial capacity
    vm->backends->count = 0;
    vm->backends->capacity = 10;
    
    // Initialize runtime
    vm->runtime = zeno_default_runtime();
    vm->options = zeno_default_options();
    
    // Initialize compilation pipeline
    vm->lexer = NULL;
    vm->parser = NULL;
    vm->current_module = NULL;
    
    // Initialize error handling
    vm->error_count = 0;
    vm->error_messages = NULL;
    
    // Initialize statistics
    vm->last_compile_time = 0.0;
    vm->last_output_size = 0;
    vm->total_compilations = 0;
    
    return vm;
}

void zenovm_free(ZenoVM* vm) {
    if (!vm) return;
    
    // Free backends
    if (vm->backends) {
        for (int i = 0; i < vm->backends->count; i++) {
            if (vm->backends->backends[i]) {
                vm->backends->backends[i]->cleanup(vm->backends->backends[i]);
            }
        }
        free(vm->backends->backends);
        free(vm->backends);
    }
    
    // Free runtime
    if (vm->runtime) {
        free(vm->runtime);
    }
    
    // Free options
    if (vm->options) {
        free(vm->options->target_backend);
        free(vm->options->output_file);
        free(vm->options->source_file);
        free(vm->options);
    }
    
    // Free compilation pipeline
    if (vm->lexer) {
        lexer_free(vm->lexer);
    }
    if (vm->parser) {
        parser_free(vm->parser);
    }
    if (vm->current_module) {
        zir_module_free(vm->current_module);
    }
    
    // Free error messages
    if (vm->error_messages) {
        for (int i = 0; i < vm->error_count; i++) {
            free(vm->error_messages[i]);
        }
        free(vm->error_messages);
    }
    
    free(vm);
}

int zenovm_register_backend(ZenoVM* vm, ZenoBackend* backend) {
    if (!vm || !vm->backends || !backend) return -1;
    
    // Resize if needed
    if (vm->backends->count >= vm->backends->capacity) {
        vm->backends->capacity *= 2;
        vm->backends->backends = realloc(vm->backends->backends, 
                                        sizeof(ZenoBackend*) * vm->backends->capacity);
    }
    
    // Initialize backend
    if (backend->init) {
        int result = backend->init(backend);
        if (result != 0) {
            return result;
        }
    }
    
    vm->backends->backends[vm->backends->count] = backend;
    vm->backends->count++;
    
    return 0;
}

ZenoBackend* zenovm_get_backend(ZenoVM* vm, const char* name) {
    if (!vm || !vm->backends || !name) return NULL;
    
    for (int i = 0; i < vm->backends->count; i++) {
        ZenoBackend* backend = vm->backends->backends[i];
        if (backend && backend->name && strcmp(backend->name, name) == 0) {
            return backend;
        }
    }
    
    return NULL;
}

char** zenovm_list_backends(ZenoVM* vm) {
    if (!vm || !vm->backends) return NULL;
    
    char** list = malloc(sizeof(char*) * (vm->backends->count + 1));
    
    for (int i = 0; i < vm->backends->count; i++) {
        ZenoBackend* backend = vm->backends->backends[i];
        if (backend && backend->name) {
            list[i] = strdup(backend->name);
        } else {
            list[i] = strdup("unknown");
        }
    }
    
    list[vm->backends->count] = NULL;
    return list;
}

int zenovm_compile_file(ZenoVM* vm, const char* input_file, const char* output_file, const char* backend_name) {
    if (!vm || !input_file || !backend_name) return -1;
    
    // Read input file
    FILE* file = fopen(input_file, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", input_file);
        return -1;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Read file content
    char* source = malloc(size + 1);
    size_t bytes_read = fread(source, 1, size, file);
    source[bytes_read] = '\0';
    fclose(file);
    
    // Compile
    char* output = zenovm_compile_string(vm, source, backend_name);
    free(source);
    
    if (!output) {
        return -1;
    }
    
    // Write output
    const char* out_file = output_file;
    if (!out_file) {
        // Generate default output filename
        static char default_output[256];
        ZenoBackend* backend = zenovm_get_backend(vm, backend_name);
        if (backend && backend->file_extension) {
            snprintf(default_output, sizeof(default_output), "output%s", backend->file_extension);
        } else {
            strcpy(default_output, "output.txt");
        }
        out_file = default_output;
    }
    
    FILE* out = fopen(out_file, "w");
    if (!out) {
        fprintf(stderr, "Error: Cannot create output file '%s'\n", out_file);
        free(output);
        return -1;
    }
    
    fputs(output, out);
    fclose(out);
    free(output);
    
    if (vm->options && vm->options->verbose) {
        printf("Output written to '%s'\n", out_file);
    }
    
    return 0;
}

char* zenovm_compile_string(ZenoVM* vm, const char* source, const char* backend_name) {
    if (!vm || !source || !backend_name) return NULL;
    
    vm->total_compilations++;
    
    // Get backend
    ZenoBackend* backend = zenovm_get_backend(vm, backend_name);
    if (!backend) {
        fprintf(stderr, "Error: Backend '%s' not found\n", backend_name);
        return NULL;
    }
    
    // For now, return a simple stub output
    // TODO: Implement full compilation pipeline
    
    char* output = malloc(1024);
    snprintf(output, 1024, 
        "/* Generated by ZenoVM using %s backend */\n"
        "/* Source: */\n"
        "/*\n%s\n*/\n"
        "\n"
        "/* TODO: Implement full compilation pipeline */\n"
        "#include <stdio.h>\n"
        "\n"
        "int main() {\n"
        "    printf(\"Hello from ZenoVM!\\n\");\n"
        "    return 0;\n"
        "}\n",
        backend->name, source);
    
    return output;
}

int zenovm_has_errors(ZenoVM* vm) {
    return vm ? vm->error_count > 0 : 0;
}

char** zenovm_get_errors(ZenoVM* vm) {
    return vm ? vm->error_messages : NULL;
}

void zenovm_print_stats(ZenoVM* vm) {
    if (!vm) return;
    
    printf("ZenoVM Statistics:\n");
    printf("  Total compilations: %d\n", vm->total_compilations);
    printf("  Last compile time: %.2fms\n", vm->last_compile_time);
    printf("  Last output size: %zu bytes\n", vm->last_output_size);
}

char* zenovm_get_version(void) {
    return strdup("ZenoVM 0.1.0");
}

ZenoCompileOptions* zeno_default_options(void) {
    ZenoCompileOptions* options = malloc(sizeof(ZenoCompileOptions));
    
    options->mode = ZENO_MODE_TRANSPILE;
    options->opt_level = ZENO_OPT_NONE;
    options->target_backend = NULL;
    options->debug_info = false;
    options->verbose = false;
    options->emit_ir = false;
    options->output_file = NULL;
    options->source_file = NULL;
    
    return options;
}

ZenoRuntime* zeno_default_runtime(void) {
    ZenoRuntime* runtime = malloc(sizeof(ZenoRuntime));
    
    runtime->use_gc = false;
    runtime->use_manual_memory = true;
    runtime->heap_size = 1024 * 1024;  // 1MB default
    runtime->runtime_lib_path = NULL;
    runtime->allocate = malloc;
    runtime->deallocate = free;
    runtime->gc_collect = NULL;
    runtime->error_handler = NULL;
    
    return runtime;
}

// Stub implementations for remaining functions
int zenovm_set_options(ZenoVM* vm, ZenoCompileOptions* options) {
    if (!vm || !options) return -1;
    vm->options = options;
    return 0;
}

int zenovm_set_runtime(ZenoVM* vm, ZenoRuntime* runtime) {
    if (!vm || !runtime) return -1;
    vm->runtime = runtime;
    return 0;
}

ZIRModule* zenovm_parse(ZenoVM* vm, const char* source, const char* filename) {
    // TODO: Implement parsing to ZIR
    return NULL;
}

ZIRModule* zenovm_analyze(ZenoVM* vm, ZIRModule* module) {
    // TODO: Implement analysis pass
    return module;
}

ZIRModule* zenovm_optimize(ZenoVM* vm, ZIRModule* module) {
    // TODO: Implement optimization pass
    return module;
}

char* zenovm_generate(ZenoVM* vm, ZIRModule* module, const char* backend_name) {
    // TODO: Implement code generation
    return strdup("/* TODO: Code generation */");
}

void zenovm_clear_errors(ZenoVM* vm) {
    if (!vm) return;
    
    if (vm->error_messages) {
        for (int i = 0; i < vm->error_count; i++) {
            free(vm->error_messages[i]);
        }
        free(vm->error_messages);
        vm->error_messages = NULL;
    }
    
    vm->error_count = 0;
}

char* zenovm_serialize_ir(ZIRModule* module) {
    // TODO: Implement IR serialization
    return strdup("/* TODO: IR serialization */");
}

ZIRModule* zenovm_deserialize_ir(const char* data) {
    // TODO: Implement IR deserialization
    return NULL;
}

int zenovm_execute(ZenoVM* vm, ZIRModule* module) {
    // TODO: Implement JIT execution
    return -1;
}