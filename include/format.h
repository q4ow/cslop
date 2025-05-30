#ifndef FORMAT_H
#define FORMAT_H

#include <stdio.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

int print_title(const char *title);
int print_info(const char *label, const char *value);
int print_success(const char *message);
int print_warning(const char *message);
int print_error(const char *message);

#endif
