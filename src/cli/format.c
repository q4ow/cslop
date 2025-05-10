#include "../include/format.h"
#include <stdio.h>
#include <string.h>

int print_title(const char *title) {
    if (!title) {
        fprintf(stderr, "print_title: NULL title\n");
        return -1;
    }

    if (printf("%s%s%s\n", ANSI_COLOR_CYAN, title, ANSI_COLOR_RESET) < 0) {
        return -1;
    }

    int len = strlen(title);
    for (int i = 0; i < len; i++) {
        if (printf("%s─%s", ANSI_COLOR_CYAN, ANSI_COLOR_RESET) < 0) {
            return -1;
        }
    }
    return printf("\n") < 0 ? -1 : 0;
}

int print_info(const char *label, const char *value) {
    if (!label || !value) {
        fprintf(stderr, "print_info: NULL parameter\n");
        return -1;
    }
    return printf("%s%s:%s %s\n", ANSI_COLOR_YELLOW, label, ANSI_COLOR_RESET,
                  value) < 0
               ? -1
               : 0;
}

int print_success(const char *message) {
    if (!message) {
        fprintf(stderr, "print_success: NULL message\n");
        return -1;
    }
    return printf("%s✓ %s%s\n", ANSI_COLOR_GREEN, message, ANSI_COLOR_RESET) < 0
               ? -1
               : 0;
}

int print_warning(const char *message) {
    if (!message) {
        fprintf(stderr, "print_warning: NULL message\n");
        return -1;
    }
    return printf("%s! %s%s\n", ANSI_COLOR_YELLOW, message, ANSI_COLOR_RESET) <
                   0
               ? -1
               : 0;
}

int print_error(const char *message) {
    if (!message) {
        fprintf(stderr, "print_error: NULL message\n");
        return -1;
    }
    return fprintf(stderr, "%s✗ %s%s\n", ANSI_COLOR_RED, message,
                   ANSI_COLOR_RESET) < 0
               ? -1
               : 0;
}
