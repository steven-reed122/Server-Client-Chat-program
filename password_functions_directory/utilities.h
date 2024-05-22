#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdlib.h> // For size_t definition

void trim_trailing_whitespace(char *str);
int option_selector(void);
char* read_file_contents(const char* filename, size_t* file_size);
void write_new_file(const char* filename);
int compare_user_input(const char* user_input, const char* file_contents);
char* filename_checker(void);
char* concatenate_file_path(const char* filename, const char* filepath);

#endif // UTILITIES_H
