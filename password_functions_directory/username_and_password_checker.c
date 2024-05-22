#include "username_and_password_checker.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DIRECTORY_PATH "./username_file_directory/"

char* username_and_password_checker(void) {
    char *filename;
    char *file_contents;
    size_t file_size;
    int option;

    while (1) {
        // Generate filename with directory path
        filename = filename_checker();
        if (!filename) continue; // Filename input failed, try again
	
	char* fullPath = concatenate_file_path(filename, DIRECTORY_PATH);
    	
        // Update file operations with directory path
        file_contents = read_file_contents(fullPath, &file_size);
        if (file_contents != NULL) {
	    free(fullPath); // Don't forget to free the memory!
            printf("Username '%s' already exists\n", filename);
            option = option_selector();
            if (option == 1) {
                free(file_contents);
                continue; // Try another filename
            } else if (option == 2) {
                char input[1024];
                printf("Enter password: ");
                fgets(input, sizeof(input), stdin);
                trim_trailing_whitespace(input);
                if (compare_user_input(input, file_contents)) {
                    printf("Password is correct\n");
                    free(file_contents);
                    break; // Set success to break the loop
                } else {
                    printf("Password is incorrect. Try again.\n");
                }
            }
            free(file_contents);
        } else {
            // Update write_new_file function with directory path
            write_new_file(fullPath);
	    free(fullPath);
            break;
        }
    }
    
    return filename;
}

