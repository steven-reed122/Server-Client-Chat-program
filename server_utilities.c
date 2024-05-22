#include "server_utilities.h"
#include "username_utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>


#define BUF_SIZE 100

// Simulates sending a message to the client by printing to the console
void send_to_client(int sock, const char *message) {
    send(sock, message, strlen(message), 0);
}

// Simulates receiving input from the client by reading from the console
void receive_from_client(int sock, char *buffer, size_t buffer_size) {
    int read_size;
    if ((read_size = recv(sock, buffer, buffer_size, 0)) <= 0) {
        buffer[0] = '\0'; // Ensure buffer is empty if input fails
    } else {
        // Remove any trailing newline character
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

//Cuts the whitespace and \n character of the end of 
//messages
//Makes it easier to check passwords with less errors
void trim_trailing_whitespace(char *str) {
    int index = -1;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            index = i;
        }
    }
    str[index + 1] = '\0';
}

// Prompts and allows the user to choose an option when
// The attempt to use a username that already has an existing
// password file
int option_selector(int sock) {
    int option;
    char buffer[BUF_SIZE];
    while (1) {
        send_to_client(sock, "Choose an option:\n1. Choose new username\n2. Enter password\n");
        receive_from_client(sock, buffer, BUF_SIZE);
	option = buffer[0];
         if (strlen(buffer) == 1) {
            if (option == '1' || option == '2') {
                return option;
            } else {
                send_to_client(sock, "Invalid option. Please enter 1 or 2.\n");
            }
        } else {
            send_to_client(sock, "Invalid input. Please enter a number.\n");
        }
    }
}


//Function for integrating reading files
//And username/password checking
char* read_file_contents(int sock, const char* filename, size_t* file_size) {
    FILE *file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        send_to_client(sock, "username is available");
        return NULL;
    }

    fseek(file_ptr, 0, SEEK_END);
    *file_size = (size_t)ftell(file_ptr);
    rewind(file_ptr);

    char *file_contents = (char *)malloc(*file_size + 1);
    if (file_contents == NULL) {
        fclose(file_ptr);
        send_to_client(sock, "Memory error\n");
        return NULL;
    }

    fread(file_contents, 1, *file_size, file_ptr);
    file_contents[*file_size] = '\0';
    fclose(file_ptr);
    trim_trailing_whitespace(file_contents);
    return file_contents;
}


//Function for integrating reading files
//And username/password checking
void write_new_file(int sock, const char* filename) {
    char input[BUF_SIZE];
    FILE *file_ptr = fopen(filename, "w");
    if (file_ptr == NULL) {
        char error_message[BUF_SIZE];
        snprintf(error_message, BUF_SIZE, "Error opening the file %s for writing password\n", filename);
        send_to_client(sock, error_message);
        return;
    }

    send_to_client(sock, "Enter new password: ");
    receive_from_client(sock, input, BUF_SIZE);
    trim_trailing_whitespace(input);
    fprintf(file_ptr, "%s", input);

    char success_message[BUF_SIZE];
    snprintf(success_message, BUF_SIZE, "New username '%s' created and password recorded\n", filename);
    send_to_client(sock, success_message);
    fclose(file_ptr);
}

int compare_user_input(const char* user_input, const char* file_contents) {
    return strcmp(user_input, file_contents) == 0;
}

//Gets username until a valid username is entered
char* filename_checker(int sock, AllUsernames *u) {
    char* invalid_filename = "username_file";
    char *filename = malloc(BUF_SIZE);
    if (!filename) {
        send_to_client(sock, "Memory allocation failed.\n");
        return NULL;
    }

    while (1) {
        send_to_client(sock, "Enter username: ");
        receive_from_client(sock, filename, BUF_SIZE);
        trim_trailing_whitespace(filename);
        if (strcmp(filename, invalid_filename) == 0 || username_exists(filename, u)) {
            send_to_client(sock, "Username is invalid, pick something else.\n");
        } else {
            return filename;
        }
    }
}


//Used for getting path to the username_file_directory file
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

