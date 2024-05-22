#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SERVER_IP "0.0.0.0"  // Localhost IP
#define SERVER_PORT "30602"  // Designated port number

// Function declarations
void errExit(int xnum, char *msg);
void *handle_messages(void *sockfd);

// Main function: initializes connection and starts messaging thread
int main(int argc, char *argv[]) {
    int server_fd;
    struct addrinfo hints, *airesult;

    memset(&hints, 0, sizeof(hints)); //Nulls the hints data location
				      //Don't really understand why this
				      //is done but it works
    hints.ai_family = AF_INET; //Don't understand
    hints.ai_socktype = SOCK_STREAM; //Don't understand

    //Automatically sets the local IP and correct server port for listening
    if (getaddrinfo(SERVER_IP, SERVER_PORT, &hints, &airesult) != 0) {
        perror("getaddrinfo() error");
        exit(1);
    }
    // creates a sever_fd for the correct server
    // Don't really understand this fully
    server_fd = socket(airesult->ai_family, airesult->ai_socktype, airesult->ai_protocol);
    if (server_fd == -1) { // Checks for socking errors
        perror("socket() error");
        exit(1);
    }

    //Checks for connection errors
    if (connect(server_fd, airesult->ai_addr, airesult->ai_addrlen) == -1) {
        perror("connect() error");
        exit(1);
    }

    // Create a thread for handling both sending and receiving messages
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, handle_messages, (void *)&server_fd) < 0) {
        perror("could not create thread");
        return 1;
    }

    // Wait for the thread to finish
    pthread_join(thread_id, NULL);

    //Freeing and closing to prevent mem leak
    freeaddrinfo(airesult);
    close(server_fd);
    return 0;
}

//Error exit handle
//Got this straight from an example file
void errExit(int xnum, char *msg) {
    fprintf(stderr, "\n%s\n", msg);
    exit(xnum);
}

// Function to handle both sending and receiving messages
void *handle_messages(void *sockfd) {
    //intialization of all necessary variables
    int sock = *(int *)sockfd;
    char buffer[BUF_SIZE];
    fd_set read_fds;
    struct timeval tv;
    
    while (1) { //this while runs until client disconects
		//Got a lot of this from an example program
        FD_ZERO(&read_fds); //Don't understand
        FD_SET(sock, &read_fds); //Don't understand
        FD_SET(STDIN_FILENO, &read_fds); //Don't understand

        tv.tv_sec = 2; 
        tv.tv_usec = 0;

        if (select(sock + 1, &read_fds, NULL, NULL, &tv) == -1) {
            perror("select() failed");
            break;
        } else if (select(sock + 1, &read_fds, NULL, NULL, &tv) == 0) {
            continue;
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            fgets(buffer, BUF_SIZE, stdin); //Gets input from client console
            if (strncmp(buffer, "exit\n", 5) == 0) {//If message is exit we 
						    //close the client
                printf("Disconnecting...\n");
                break;
            }

	    //Sends info to sever from this client
            if (send(sock, buffer, strlen(buffer), 0) < 0) {
                perror("send failed");
                break;
            }
        }
	//This checks if we have stuff to read from the client
	//Not really sure about how it works
        if (FD_ISSET(sock, &read_fds)) {
            int nbytes = recv(sock, buffer, BUF_SIZE, 0);
            if (nbytes <= 0) { //Checks for issues with server
			       //This mostly occurs if server 
			       //is closed before client
                printf("Server disconnected or receive error.\n");
                break;
            }
            buffer[nbytes] = '\0'; //Sets end of string so we know how much to
				   //send to the server and not catch
				   //Unintended chars

            printf("%s\n", buffer);//Prints the string the console 
				   //of this client
        }
    }
    close(sock); //This avoids mem leak
    return NULL;
}

