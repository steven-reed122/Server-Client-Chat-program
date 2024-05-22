#ifndef USERNAME_UTILITIES_H
#define USERNAME_UTILITIES_H

#include <stddef.h>

// Define the AllUsernames struct so it is defined the
// same in all files
typedef struct {
    char **username_array;
    size_t count;
    size_t size;
} AllUsernames;

// Function declarations

//Initiates the username array with a given size
//Mallocs the correct amount of memory
void init_all_usernames(AllUsernames *u, size_t initial_size);

//Handles all the other username functions
//And integrates these functiosn with the password side
char *receive_and_check_username(int sock, AllUsernames *u);

//Checks if a username is already inside of the
//AllUsernames usernames array
int username_exists(const char *username, AllUsernames *u);

//Adds a username to the AllUsernames usernames array
//Sock must be passed so we can associate the usrname
//with the correct sock
void username_adder(char *username, AllUsernames *u, int sock);

//Removes a username from the AllUsernames usernames array
//sock must be passed to remove correct username
void username_remover(char *username, AllUsernames *u, int sock);

//ALL other functions are not really userd
//I attempted to store current usernames in a file
//But that proved harder that storing
//Them in a struct
void save_username_and_socket(const char *username, int sock, const char *filepath);
char *find_username_by_sock(int sock, const char *filepath);
int remove_user_by_sock(int sock, const char *filepath);

#endif // USERNAME_UTILITIES_H

