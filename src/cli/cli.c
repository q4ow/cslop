#include "../include/cli.h"
#include "../include/format.h"
#include "version.h"
#include <stdio.h>
#include <stdlib.h>

const char *argp_program_version = BINARY_VERSION;
const char *argp_program_bug_address = "q4ow@proton.me";

static char doc[] = "CSLOP -- A sloppy archive extractor";

static char args_doc[] = "[ARGS...]";

static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output", 0},
    {"quiet",   'q', 0, 0, "Don't produce any output", 0},
    {"output",  'o', "FILE", 0, "Output to FILE instead of standard output", 0},
    {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'v':
            arguments->verbose = 1;
            break;
        case 'q':
            arguments->quiet = 1;
            break;
        case 'o':
            arguments->output_file = arg;
            break;
        case ARGP_KEY_ARG:
            arguments->args = &state->argv[state->next - 1];
            arguments->arg_count = state->argc - state->next + 1;
            state->next = state->argc;
            break;
        case ARGP_KEY_END:
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

void init_cli(int argc, char *argv[], struct arguments *arguments) {
    arguments->verbose = 0;
    arguments->quiet = 0;
    arguments->output_file = NULL;
    arguments->args = NULL;
    arguments->arg_count = 0;

    argp_parse(&argp, argc, argv, 0, 0, arguments);
}

void print_program_header(void) {
    print_title(BINARY_NAME);
    print_info("Version", BINARY_VERSION);
    print_info("Author", BINARY_AUTHOR);
    printf("\n");
}
