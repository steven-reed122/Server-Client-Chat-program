// username_functions.h

#ifndef USERNAME_FUNCTIONS_H
#define USERNAME_FUNCTIONS_H

// Function prototypes
int username_checker(const char *newUsername, char **existingUsernames, int count);
char **username_adder(const char *newUsername, char ***existingUsernames, int *count);
void print_usernames(char **usernames, int count);

#endif

