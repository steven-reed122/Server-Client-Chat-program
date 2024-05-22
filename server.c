#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h> // Needed for struct sockaddr_in
#include "server_username_and_password_checker.h"
#include "username_utilities.h"
#include "mutex.h"

#define MAX_CLIENTS 100 //defines max number of clients
#define BUF_SIZE 1024 //defines max size of messages that can be sent
#define SERVER_PORT 30602 //defines my specific server port

int client_sockets[MAX_CLIENTS] = {0}; //got this from example code
				       //not super sure about what it does

//Function pre-definitions

//Function for handling clients
//Handles everything to do with sending and recieving messages 
//Also handles usernames and passwords
void *client_handler(void *socket_desc); 

//Function for combining usernames with messages
//Adss <username> -> to message
void add_username_to_message(char *username, char *message,int read_size); //

// Global variable
AllUsernames all_usernames; //structure for storing usernames array and socks

int main(void) {
    int listen_fd, client_fd; //file descriptors for server and client
    struct sockaddr_in addr, sin; //struct for storing info
				  //not super sure but I based 
				  //this off the example file
    socklen_t addrlen = sizeof(struct sockaddr_in); //also got this from 
						    //example file
    int opt = 1;
    init_all_usernames(&all_usernames, MAX_CLIENTS); //This initiates the username 
					    //struct and creates space for
					    //100 clients

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    // checks for socket errors
    if (listen_fd == -1) {
        perror("socket() error");
        exit(1);
    }

    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt() error");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);  // Set port to designated number
    addr.sin_addr.s_addr = INADDR_ANY;;

    if (bind(listen_fd, (struct sockaddr *)&addr, addrlen) < 0) {
        perror("bind() error");
        exit(1);
    }

    // Retrieve the port number assigned to the socket
    if (getsockname(listen_fd, (struct sockaddr *)&sin, &addrlen) == -1) {
        perror("getsockname() error");
        exit(1);
    }

    if (listen(listen_fd, 10) < 0) {
        perror("listen() error");
        exit(1);
    }

    printf("Server is listening on port %d\n", ntohs(sin.sin_port));  // Print the port number


    //Loop for handling listening for, connecting,
    //and disconneting clients from the server
    //Loop runs until server is shut down
    while (1) {
        client_fd = accept(listen_fd, NULL, NULL);
        if (client_fd < 0) {
            perror("accept() error");
            continue;
        }

        pthread_t thread; //creates a new thread
        int *new_sock = malloc(sizeof(int)); //creates a sock for a new client
        *new_sock = client_fd; //declares that new sock with correct client_fd

        pthread_mutex_lock(&lock); //Locks other code to prevent errors
				   //Loop for removing client elegantly
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = client_fd;
                break;
            }
        }
        pthread_mutex_unlock(&lock);//unlock mutex

        if (pthread_create(&thread, NULL, client_handler, (void *)new_sock) < 0) {
            perror("could not create thread");
            close(client_fd);
        } else {
            printf("Handler assigned for client %d\n", client_fd);
        }
    }

    close(listen_fd);
    return 0;
}

void *client_handler(void *socket_desc) {
    int sock = *(int *)socket_desc;//stores the sock
    char client_message[BUF_SIZE]; //stores the current client message
    int read_size; //used to figure out how much was read and manage buffer
    char *filepath ="./username_file_directory/username_file"; //path the the currnet users file

    // Ensure a unique username is provided by the client
    char *username = receive_and_check_username(sock, &all_usernames);
    //loop function that handles all username and password getting
    //calls the server_username_and_password_checker() function internally
    //This function handles the getting and validating of 
    //the username and password
    if (username == NULL) { //If it gets a null username it shuts down
        fprintf(stderr, "Failed to obtain a valid username.\n");
        close(sock); //need to send the close message for server error handling
        free(socket_desc); //needs to free socker_desc pointer
			   //This will avoid memory leak
	//no need to free username since  the pointer is null
        return NULL;
    }

    //send confirmation/explanation message 
    //when client is connected and 
    //username and password are confirmed
    char *welcome_msg = "Server says: connection established!\nStart typing and then hit enter to send message\nType 'exit' then press enter to close program\n"; 
    send(sock, welcome_msg, strlen(welcome_msg), 0); 
    
    //while loop for handling recieving and sending messages
    while ((read_size = recv(sock, client_message, BUF_SIZE, 0)) > 0) {
        client_message[read_size] = '\0';
        pthread_mutex_lock(&lock); //locking mutex prevents other code from 
				   //executing at the same time and 
				   //interrupting the following code
	add_username_to_message(username, client_message, read_size); 
	//adds the username and arrow in front of the 
	//text to send to other clients	
	
	//For loop that sends message to all clients
	//Iterates through all clients and sends them all the message
	for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] != 0 && client_sockets[i] != sock) {
		send(client_sockets[i], client_message, (size_t)strlen(client_message), 0); //send function
            }
        }
        pthread_mutex_unlock(&lock); //unlocks mutex so other code can run
        memset(client_message, 0, BUF_SIZE); //Resets client message memory block to all 0 so they can be refilled again without fear of corruption
    }

    if (read_size <= 0) { //Closes the socket if the client is disconnected
        fprintf(stderr, "Client disconnected: %d\n", sock);
	//Prints disconnect message on server side
        pthread_mutex_lock(&lock); //locks mutex so other code 
				   //cannot interrupt the following
        for (int i = 0; i < MAX_CLIENTS; i++) { //nulls the closed socket
						//Iterates all the sockets
						//and finds the disconnected
						//one and sets it to 0 so 
						//it knows its closed
            if (client_sockets[i] == sock) {
                client_sockets[i] = 0;
                break; //breaks the for loop
            }
        }
        pthread_mutex_unlock(&lock); //unlock mutex to allow other code to run
        close(sock); //closes the socket
    }

    //Frees everything
    username_remover(username, &all_usernames, sock); //supposed to remove 
						      //username from the 
						      //username array
						      //***Doesn't work***
    free(username); //frees username string memory
    free(socket_desc); //free current socket descriptor
    remove_user_by_sock(sock, filepath); //removes username and sock 
					 //combo from file
					 //Kind of pointless because I ended
					 //up storing current users in a 
					 //struct but I think its
					 //interesting so I left this in here
    return NULL;
}

// Function to add username to the message
void add_username_to_message(char *username, char *message, int read_size) {
    // Calculate the length of the username and the "-> " string
    int username_length = strlen(username);
    int arrow_length = 4;  // Length of " -> " including null terminator

    // Move the original message to the right to 
    // make space for the username and arrow
    memmove(message + (size_t)username_length + (size_t)arrow_length, message, (size_t)strlen(message) + 1);

    // Copy the username to the beginning of the message
    memcpy(message, username, (size_t)username_length);

    // Add the " -> " after the username
    memcpy(message + username_length, " -> ", (size_t)arrow_length);
}



