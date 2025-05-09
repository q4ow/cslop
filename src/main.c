#include "../include/archive.h"
#include "../include/cslop.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    struct arguments arguments;

    init_cli(argc, argv, &arguments);

    if (!arguments.quiet) {
        print_program_header(&arguments);
    }

    if (arguments.arg_count == 0) {
        print_error("No archive files specified");
        return 1;
    }

    struct extract_options options = {
        .destination = arguments.output_dir,
        .create_subdirectory = !arguments.no_subdirectory,
        .preserve_permissions = arguments.preserve_perms,
        .overwrite = arguments.overwrite};

    int exit_code = 0;
    for (int i = 0; i < arguments.arg_count; i++) {
        if (arguments.verbose) {
            printf("\n");
            print_info("Processing archive", arguments.args[i]);
        }

        if (extract_archive(arguments.args[i], &options) != 0) {
            exit_code = 1;
        }
    }

    return exit_code;
}