#include "../include/archive.h"
#include "../include/cslop.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void cleanup_arguments(struct arguments *arguments) {
    arguments->args = NULL;
    arguments->arg_count = 0;
}

int main(int argc, char *argv[]) {
    if (argc < 1 || !argv) {
        fprintf(stderr, "Invalid program invocation\n");
        return EXIT_FAILURE;
    }

    struct arguments arguments = {0};
    int exit_code = EXIT_SUCCESS;

    if (init_cli(argc, argv, &arguments) != 0) {
        print_error("Failed to initialize CLI");
        return EXIT_FAILURE;
    }

    if (!arguments.quiet) {
        print_program_header(&arguments);
    }

    if (arguments.arg_count == 0) {
        print_error("No archive files specified");
        cleanup_arguments(&arguments);
        return EXIT_FAILURE;
    }

    if (arguments.output_dir) {
        if (access(arguments.output_dir, W_OK) != 0) {
            print_error(strerror(errno));
            cleanup_arguments(&arguments);
            return EXIT_FAILURE;
        }
    }

    struct extract_options options = {
        .destination = arguments.output_dir,
        .create_subdirectory = !arguments.no_subdirectory,
        .preserve_permissions = arguments.preserve_perms,
        .overwrite = arguments.overwrite};

    for (int i = 0; i < arguments.arg_count; i++) {
        if (!arguments.args[i])
            continue;

        if (arguments.verbose) {
            printf("\n");
            print_info("Processing archive", arguments.args[i]);
        }

        if (access(arguments.args[i], R_OK) != 0) {
            print_error(strerror(errno));
            exit_code = EXIT_FAILURE;
            continue;
        }

        if (extract_archive(arguments.args[i], &options) != 0) {
            exit_code = EXIT_FAILURE;
        }
    }

    cleanup_arguments(&arguments);
    return exit_code;
}