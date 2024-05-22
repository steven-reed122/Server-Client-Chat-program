// main.c

#include "username_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int numberOfUsernames = 3;
    char **usernames = malloc((size_t)numberOfUsernames * sizeof(char*));
    usernames[0] = strdup("Alice");
    usernames[1] = strdup("Bob");
    usernames[2] = strdup("Charlie");

    // Print initial list of usernames
    print_usernames(usernames, numberOfUsernames);

    // User input interaction
    char newUsername[100]; // Buffer to hold the new username
    int keepRunning = 1;  // Control variable for the loop
    char choice[10];      // Buffer for user's choice to continue or not

    while (keepRunning) {
        printf("Enter a new username to add (or type 'exit' to quit): ");
        scanf("%99s", newUsername);
        if (strcmp(newUsername, "exit") == 0) {
            break;
        }

        char **updatedUsernames = username_adder(newUsername, &usernames, &numberOfUsernames);
        if (updatedUsernames != NULL) {
            print_usernames(usernames, numberOfUsernames);
        }

        printf("Do you want to add another username? (yes/no): ");
        scanf("%9s", choice);
        if (strcmp(choice, "no") == 0) {
            keepRunning = 0;
        }
    }

    // Free all allocated memory
    for (int i = 0; i < numberOfUsernames; i++) {
        free(usernames[i]);
    }
    free(usernames);

    return 0;
}
