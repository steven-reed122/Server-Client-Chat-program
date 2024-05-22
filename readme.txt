***COMPILATION and RUNNING****

!!!!!!!!!To compile all files use the "make" command in the terminal.!!!!!!!!

All logical makefile commands are valid.
server runs with "server_program"
client runs with "client_program"

To compile the server program use the command: "make server_program" or

gcc `flags` -o server_program server.c server_username_and_password_checker.c server_utilities.c

To compile the client program use the command: "make client_program" or

gcc `flags` -o client_program client.c

***TIPS***

1) Make sure you have an existing "username_file_directory" directory 
inside of the direcotry where you wish to execute the server program.

2) Use the word "exit" to disconnect and shut down the client properly
It must be the first and only word you send in a message for this to work

3) program will crash if you connect more than 100 clients. This is an arbitrary number. 
To change this, go to main in server.c file an change #define MAX_CLIENTS line to any num

4) spaces are not valid password characters

5) If a file exists in the username_file_directory, the name of that file is 
considered to be an existing username. 
To remove a username, go into the username_file_directory 
and delete the file

***HOW IT WORKS***

Server program handles all username and password functions as well as 
the sending of messages between clients. Client program only recieves 
and sends data to and from the server

All functions for usernames are in the username_utilities.c file

All functions for passwords are in the server_utilities.c file

These functions do have some overlap and call each other sometimes 
condsidering that usernames and password are fairly itnertwined but the 
functions are in different files.

Passwords are saved in the "username_file_directory" directory so that 
they persist between server instantiations. The name of the file is the 
username and the contents of the file are the corresponding password.


***CURRENT BUGS***

1) Exiting the program as one user does not mean that someone else can 
then use that same username. For some reason the username array is not 
being cleared properly when a client exits.

2) Program will crash if you connect more than 100 clients. 
Check tip 3 to see how to fix this.
