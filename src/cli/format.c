#include "../include/format.h"
#include <stdio.h>
#include <string.h>

void print_title(const char *title) {
    printf("%s%s%s\n", ANSI_COLOR_CYAN, title, ANSI_COLOR_RESET);
    
    int len = strlen(title);
    for (int i = 0; i < len; i++) {
        printf("%s─%s", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
    }
    printf("\n");
}

void print_info(const char *label, const char *value) {
    printf("%s%s:%s %s\n", 
           ANSI_COLOR_YELLOW, 
           label, 
           ANSI_COLOR_RESET,
           value);
}

void print_success(const char *message) {
    printf("%s✓ %s%s\n", ANSI_COLOR_GREEN, message, ANSI_COLOR_RESET);
}

void print_warning(const char *message) {
    printf("%s! %s%s\n", ANSI_COLOR_YELLOW, message, ANSI_COLOR_RESET);
}

void print_error(const char *message) {
    printf("%s✗ %s%s\n", ANSI_COLOR_RED, message, ANSI_COLOR_RESET);
}
