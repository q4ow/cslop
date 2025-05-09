#ifndef CLI_H
#define CLI_H

#include <argp.h>

struct arguments {
  int verbose;
  int quiet;
  char *output_dir;
  int no_subdirectory;
  int preserve_perms;
  int overwrite;
  char **args;
  int arg_count;
};

void init_cli(int argc, char *argv[], struct arguments *arguments);

void print_program_header(void);

#endif
