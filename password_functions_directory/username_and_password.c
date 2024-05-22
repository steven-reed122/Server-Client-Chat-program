#include <stdio.h>
#include <stdlib.h> // For free()
#include "username_and_password_checker.h"

int main(void) {
    char* filename = username_and_password_checker();
    if (filename) {
        printf("Process completed for file: %s\n", filename);
        free(filename);
    } else {
        printf("No valid filename was provided or an error occurred.\n");
    }
    return 0;
}
