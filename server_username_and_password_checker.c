#include "server_username_and_password_checker.h"
#include "username_utilities.h"
#include "server_utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DIRECTORY_PATH "./username_file_directory/"


//This is the primary function that handles getting 
//and validating of password
//It utilizes the server_utilities.c functions
char* username_and_password_checker(int sock, AllUsernames *u) {
    char *filename;
    char *file_contents;
    size_t file_size;
    int option;

    while (1) {
        // Generate filename with directory path
        filename = filename_checker(sock, u);
        if (!filename) continue; // Filename input failed, try again
	
	char* fullPath = concatenate_file_path(filename, DIRECTORY_PATH);
    	
        // Update file operations with directory path
        file_contents = read_file_contents(sock, fullPath, &file_size);
        if (file_contents != NULL) {
	    free(fullPath); // Don't forget to free the memory!
            send_to_client(sock, "Username already exists\n");
            option = option_selector(sock);
            if (option == '1') {
                free(file_contents);
                continue; // Try another filename
            } else if (option == '2') {
                char input[1024];
                send_to_client(sock, "Enter password: ");
                receive_from_client(sock, input, sizeof(input));
                trim_trailing_whitespace(input);
                if (compare_user_input(input, file_contents)) {
                    send_to_client(sock, "Password is correct\n");
                    free(file_contents);
                    break; // Set success to break the loop
                } else {
                    send_to_client(sock, "Password is incorrect. Try again.\n");
                }
            }
            free(file_contents);
        } else {
            // Update write_new_file function with directory path
            write_new_file(sock, fullPath);
	    free(fullPath);
            break;
        }
    }
    
    return filename;
}

