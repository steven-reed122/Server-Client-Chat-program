#include "username_utilities.h"
#include "server_username_and_password_checker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <pthread.h>
#include "server_utilities.h"
#include "mutex.h"

//Look in the correspondoing .h file for more documentation

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

//Checks if a username is already inside of the
//AllUsernames usernames array
int username_exists(const char *username, AllUsernames *u) {
    for (size_t i = 0; i < u->count; i++) {
        if (strcmp(u->username_array[i], username) == 0) {
            return 1;  // Username exists
        }
    }
    return 0;  // Username does not exist
}

//Adds a username to the AllUsernames usernames array
//Sock must be passed so we can associate the usrname
//with the correct sock
void username_adder(char *username, AllUsernames *u, int sock) {
    if (u->count == u->size) {
        u->size *= 2;
        char **new_array = (char **)realloc(u->username_array, u->size * sizeof(char *));
        if (!new_array) {
            perror("Failed to reallocate memory for username array");
            return;  // Handle realloc failure properly
        }
        u->username_array = new_array;
    }
    u->username_array[u->count++] = strdup(username);
    char *filepath = "username_file_directory/username_file";
    save_username_and_socket(username, sock, filepath);
}

//Removes a username from the AllUsernames usernames array
//sock must be passed to remove correct username
void username_remover(char *username, AllUsernames *u, int sock) {
    pthread_mutex_lock(&lock);  // Ensure thread safety

    for (int i = 0; i < u->count; i++) {
        if (strcmp(u->username_array[i], username) == 0) {
            free(u->username_array[i]);  // Free the memory allocated for the username

            // Shift all usernames down one slot from the point of removal
            for (int j = i; j < u->count - 1; j++) {
                u->username_array[j] = u->username_array[j + 1];
            }

            u->username_array[u->count - 1] = NULL; // Clear the last slot
            u->count--;  // Decrement the count of usernames
            break;  // Exit the loop after removal
        }
    }

    pthread_mutex_unlock(&lock);  // Unlock after modification
}

//Initiates the username array with a given size
//Mallocs the correct amount of memory
void init_all_usernames(AllUsernames *u, size_t initial_size) {
    u->username_array = (char **)malloc(initial_size * sizeof(char *));
    if (!u->username_array) {
        perror("Failed to allocate memory for username array");
        exit(EXIT_FAILURE);
    }
    u->count = 0;
    u->size = initial_size;
}

//Handles all the other username functions
//And integrates these functiosn with the password side
char* receive_and_check_username(int sock, AllUsernames *usernames) {
    char *username;

    while (1) {
	username = username_and_password_checker(sock, usernames);

        if (!username_exists(username, usernames)) {
            username_adder(username, usernames, sock);
            return username;
        } else {
            char *error_msg = "Username already in use, try another: ";
            send(sock, error_msg, strlen(error_msg), 0);
            free(username);
        }
    }
}

//The following functions attempt to store the current users in a file
//Turned out to be a bit of a bust 
//I left the functions because I though they were interesting

void save_username_and_socket(const char *username, int sock, const char *filepath) {
    FILE *file = fopen(filepath, "a"); // Open the file in append mode
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Write the username and socket descriptor to the file
    fprintf(file, "%s,%d\n", username, sock);

    // Close the file
    fclose(file);
}

char *find_username_by_sock(int sock, const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    char line[1024];
    char *username = malloc(256); // Allocate memory for the username to return
    if (!username) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    int found_sock;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%d", username, &found_sock);
        if (found_sock == sock) {
            fclose(file);
            return username; // Return the found username
        }
    }

    // If no matching socket descriptor is found, cleanup and return NULL
    fclose(file);
    free(username);
    return NULL;
}

int remove_user_by_sock(int sock, const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Failed to open file for reading");
        return -1;
    }

    FILE *temp_file = fopen("temp.txt", "w"); // Temporary file
    if (!temp_file) {
        perror("Failed to open temporary file for writing");
        fclose(file);
        return -1;
    }

    char line[1024];
    int found_sock, lines_written = 0;
    char username[256];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%d", username, &found_sock);
        if (found_sock != sock) {
            fputs(line, temp_file); // Write all other lines to the temporary file
            lines_written++;
        }
    }

    fclose(file);
    fclose(temp_file);

    // Replace the original file with the updated temporary file
    if (lines_written > 0) { // Check if there's anything written to temp file
        remove(filepath); // Delete the original file
        rename("temp.txt", filepath); // Rename temp to original file name
    } else {
        remove("temp.txt"); // Clean up empty temp file
    }

    return 0; // Success
}

