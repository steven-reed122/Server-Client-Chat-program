#ifndef SERVER_UTILITIES_H
#define SERVER_UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "username_utilities.h"


// Functions for simulating client-server communication
void send_to_client(int sock, const char *message);
void receive_from_client(int sock, char *buffer, size_t buffer_size);

// Utility functions
void trim_trailing_whitespace(char *str);
int option_selector(int sock);
char *read_file_contents(int sock, const char *filename, size_t *file_size);
void write_new_file(int sock, const char *filename);
int compare_user_input(const char *user_input, const char *file_contents);
char *filename_checker(int sock, AllUsernames *u);
char* concatenate_file_path(const char* filename, const char* filepath); 
#endif // SERVER_UTILITIES_H

