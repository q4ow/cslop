#include <stdio.h>
#include "version.h"

int main(void) {
    printf("Name: %s\n", BINARY_NAME);
    printf("Version: %s\n", BINARY_VERSION);
    printf("Author: %s\n", BINARY_AUTHOR);
    return 0;
}
