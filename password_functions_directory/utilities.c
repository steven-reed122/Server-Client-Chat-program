#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void trim_trailing_whitespace(char *str) {
    int index = -1;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char) str[i])) {
            index = i;
        }
    }
    str[index + 1] = '\0';
}

int option_selector(void) {
    int option;
    char c;
    while (1) {
        printf("Choose an option:\n1. Choose new username\n2. Enter password\n");
        if (scanf("%d", &option) == 1) {
            while ((c = getchar()) != '\n' && c != EOF);
            if (option == 1 || option == 2) {
                return option;
            } else {
                printf("Invalid option. Please enter 1 or 2.\n");
            }
        } else {
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Please enter a number.\n");
        }
    }
}

char* read_file_contents(const char* filename, size_t* file_size) {
    FILE *file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        printf("username is available\n");
        return NULL;
    }

    fseek(file_ptr, 0, SEEK_END);
    *file_size = (size_t)ftell(file_ptr);
    rewind(file_ptr);

    char *file_contents = (char *)malloc(*file_size + 1);
    if (file_contents == NULL) {
        fclose(file_ptr);
        printf("Memory error\n");
        return NULL;
    }

    fread(file_contents, 1, *file_size, file_ptr);
    file_contents[*file_size] = '\0';

    fclose(file_ptr);
    trim_trailing_whitespace(file_contents);
    return file_contents;
}

void write_new_file(const char* filename) {
    char input[1024];
    FILE *file_ptr = fopen(filename, "w");
    if (file_ptr == NULL) {
        printf("Error opening the file %s for writing password\n", filename);
	return;
    }
    
    printf("Enter new password: ");
    fgets(input, sizeof(input), stdin);
    trim_trailing_whitespace(input);
    fprintf(file_ptr, "%s", input);
    printf("New username created and password recorded\n");
    fclose(file_ptr);
}

int compare_user_input(const char* user_input, const char* file_contents) {
    return strcmp(user_input, file_contents) == 0;
}

char* filename_checker(void) {
    char* invalid_filename = "username_file";
    char *filename = malloc(256);
    if (!filename) {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    
    while (1) {
        printf("Enter username: ");
        if (!fgets(filename, 256, stdin)) {
            free(filename);
            return NULL;
        }
        trim_trailing_whitespace(filename);
        if (strcmp(filename, invalid_filename) == 0) {
            printf("Username is invalid, pick something else.\n");
        } else {
            return filename;
        }
    }
}

char* concatenate_file_path(const char* filename, const char* filepath) {
    // Calculate total length for the new string
    size_t totalLength = strlen(filepath) + strlen(filename) + 1; // +1 for the null terminator

    // Allocate memory for the new string
    char* newPath = malloc(totalLength);
    if (newPath == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return NULL; // Failed to allocate memory
    }

    // Copy filepath into the new allocated space
    strcpy(newPath, filepath);

    // Concatenate filename onto the new path
    strcat(newPath, filename);

    return newPath;
}
