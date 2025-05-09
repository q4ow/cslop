#include <stdio.h>
#include <stdlib.h>
#include "../include/cslop.h"
#include "../include/archive.h"


int main(int argc, char *argv[]) {
    struct arguments arguments;
    
    init_cli(argc, argv, &arguments);

    if (!arguments.quiet) {
        print_program_header();
    }

    if (arguments.arg_count == 0) {
        print_error("No archive files specified");
        return 1;
    }

    struct extract_options options = {
        .destination = arguments.output_dir,
        .create_subdirectory = !arguments.no_subdirectory,
        .preserve_permissions = arguments.preserve_perms,
        .overwrite = arguments.overwrite
    };

    int success = 0;
    for (int i = 0; i < arguments.arg_count; i++) {
        if (arguments.verbose) {
            printf("\n");
            print_info("Processing archive", arguments.args[i]);
        }
        
        if (extract_archive(arguments.args[i], &options) != 0) {
            success = 1;
        }
    }

    return 0;
}