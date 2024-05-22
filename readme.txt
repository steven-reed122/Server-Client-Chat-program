***DESCRIPTION***

This is code for a corresponding pair of a server and client for a chat protocol.

When ther server is running, clients can connect to it and send messages to other connected clients.

Clients can see the username of the message senders and all clients must choose a username and enter an 
existing corresponding password or create a password upon login.

***COMPILATION and RUNNING****

!!!!!!!!!To compile all files use the "make" command in the terminal.!!!!!!!!

I recommend downloading the files into a folder and then working with them using a command line interface.
I used the built in mac terminal. I used the gcc compiler and I am unsure how other compilers will function with this.

All logical makefile commands are valid.
server runs with the command "server_program"
client runs with the command "client_program"

To compile the server program use the command: "make server_program" or

gcc -o server_program server.c server_username_and_password_checker.c server_utilities.c

To compile the client program use the command: "make client_program" or

gcc -o client_program client.c

***TIPS***

1) Make sure you have an existing "username_file_directory" directory 
inside of the direcotry where you wish to execute the server program.

2) Use the word "exit" to disconnect and shut down the client properly
It must be the first and only word you send in a message for this to work

3) program will crash if you connect more than 100 clients. This is an arbitrary number. 
To change this, go to server.c file and change #define MAX_CLIENTS line to any number.
Be careful not to overload your computer as each client causes a noticable amount of lag.

4) Spaces are not valid password characters

5) If a file exists in the username_file_directory, the name of that file is 
considered to be an existing username. 
To remove a username, go into the username_file_directory 
and delete the file

6) Look at the corresponding directories to see how username and password functions are implemented.

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

1) Program will crash if you connect more than 100 clients. 
Check tip 3 to see how to fix this.
