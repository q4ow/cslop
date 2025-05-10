#include "../include/cli.h"
#include "../include/format.h"
#include "version.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

const char *argp_program_version = BINARY_VERSION;
const char *argp_program_bug_address = "q4ow@proton.me";

static char doc[] =
    "CSLOP -- A smart archive extractor that handles various archive formats\v"
    "Archive files can be of any supported type (zip, tar, gz, bz2, xz, 7z, "
    "rar).\n"
    "Multiple archives can be extracted at once.";

static char args_doc[] = "ARCHIVE...";

static struct argp_option options[] = {
    {"directory", 'd', "DIR", 0, "Extract files into DIR", 1},
    {"no-directory", 'n', 0, 0, "Do not create a subdirectory for extraction",
     1},
    {"preserve", 'p', 0, 0, "Preserve original file permissions", 1},
    {"overwrite", 'f', 0, 0, "Force overwrite of existing files", 1},

    {0, 0, 0, 0, "Output control:", 2},
    {"verbose", 'v', 0, 0, "Show detailed progress information", 2},
    {"quiet", 'q', 0, 0, "Suppress all non-error output", 2},
    {0}};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    if (!arguments)
        return ARGP_ERR_UNKNOWN;

    switch (key) {
    case 'v':
        arguments->verbose = 1;
        break;
    case 'q':
        arguments->quiet = 1;
        break;
    case 'd':
        if (!arg)
            return ARGP_ERR_UNKNOWN;
        arguments->output_dir = arg;
        break;
    case 'n':
        arguments->no_subdirectory = 1;
        break;
    case 'p':
        arguments->preserve_perms = 1;
        break;
    case 'f':
        arguments->overwrite = 1;
        break;
    case ARGP_KEY_ARG:
        arguments->args = &state->argv[state->next - 1];
        arguments->arg_count = state->argc - state->next + 1;
        state->next = state->argc;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 1) {
            argp_usage(state);
        }
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int init_cli(int argc, char *argv[], struct arguments *arguments) {
    if (!arguments || !argv) {
        return -1;
    }

    memset(arguments, 0, sizeof(struct arguments));

    error_t result = argp_parse(&argp, argc, argv, 0, 0, arguments);
    if (result != 0) {
        return -1;
    }

    if (arguments->verbose && arguments->quiet) {
        print_error("Cannot use both --verbose and --quiet");
        return -1;
    }

    if (arguments->output_dir) {
        if (access(arguments->output_dir, F_OK) != 0) {
            if (mkdir(arguments->output_dir, 0755) != 0) {
                print_error(strerror(errno));
                return -1;
            }
        }
    }

    return 0;
}

void print_program_header(const struct arguments *args) {
    if (!args || args->quiet)
        return;

    print_title(BINARY_NAME);

    if (args->verbose) {
        print_info("Version", BINARY_VERSION);
        print_info("Author", BINARY_AUTHOR);
    }

    printf("\n");
}
