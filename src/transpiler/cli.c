#include "zenoscript.h"
#include <getopt.h>

int main(int argc, char* argv[]) {
    ZenoscriptOptions options = {0};
    
    // Command line options
    static struct option long_options[] = {
        {"help",    no_argument,       0, 'h'},
        {"version", no_argument,       0, 'v'},
        {"verbose", no_argument,       0, 'V'},
        {"debug",   no_argument,       0, 'd'},
        {0, 0, 0, 0}
    };
    
    int opt;
    int option_index = 0;
    
    while ((opt = getopt_long(argc, argv, "hvVd", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                zenoscript_print_help();
                return 0;
            case 'v':
                zenoscript_print_version();
                return 0;
            case 'V':
                options.verbose = 1;
                break;
            case 'd':
                options.debug = 1;
                break;
            default:
                fprintf(stderr, "Try 'zeno --help' for more information.\n");
                return 1;
        }
    }
    
    // Check for input file
    if (optind >= argc) {
        fprintf(stderr, "Error: No input file specified\n");
        fprintf(stderr, "Try 'zeno --help' for more information.\n");
        return 1;
    }
    
    const char* input_file = argv[optind];
    const char* output_file = (optind + 1 < argc) ? argv[optind + 1] : NULL;
    
    
    // Normal transpilation
    int success = zenoscript_transpile_file(input_file, output_file, &options);
    return success ? 0 : 1;
}