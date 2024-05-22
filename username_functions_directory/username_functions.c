// username_functions.c

#include "username_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int username_checker(const char *newUsername, char **existingUsernames, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(newUsername, existingUsernames[i]) == 0) {
            return 0; // Username is not available
        }
    }
    return 1; // Username is available
}

char **username_adder(const char *newUsername, char ***existingUsernames, int *count) {
    if (username_checker(newUsername, *existingUsernames, *count)) {
        // Increase the count and resize the array
        (*count)++;
        *existingUsernames = realloc(*existingUsernames, ((size_t)*count) * sizeof(char*));
        if (*existingUsernames == NULL) {
            fprintf(stderr, "Failed to allocate memory\n");
            exit(1); // Handle realloc failure
        }
        // Allocate memory for the new username and add it to the array
        (*existingUsernames)[*count - 1] = malloc(strlen(newUsername) + 1);
        strcpy((*existingUsernames)[*count - 1], newUsername);
        printf("%s has been added.\n", newUsername);
        return *existingUsernames;
    } else {
        printf("%s is not available.\n", newUsername);
    }
    return NULL;
}

void print_usernames(char **usernames, int count) {
    printf("Current usernames:\n");
    for (int i = 0; i < count; i++) {
        printf("%s\n", usernames[i]);
    }
}

