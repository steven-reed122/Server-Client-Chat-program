# Compiler to use
CC=gcc

# Flags for compilation
CFLAGS= `flags`

# Server program executable name
SERVER_TARGET=server_program
# Client program executable name
CLIENT_TARGET=client_program

# Server Source files
SERVER_SOURCES=server.c server_username_and_password_checker.c server_utilities.c username_utilities.c
# Client Source file
CLIENT_SOURCES=client.c

# Server Object files
SERVER_OBJECTS=$(SERVER_SOURCES:.c=.o)
# Client Object file
CLIENT_OBJECTS=$(CLIENT_SOURCES:.c=.o)

# Default target
all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Server compilation
$(SERVER_TARGET): $(SERVER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Client compilation
$(CLIENT_TARGET): $(CLIENT_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Generic rule for compiling source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean up
clean:
	rm -f $(SERVER_OBJECTS) $(CLIENT_OBJECTS) $(SERVER_TARGET) $(CLIENT_TARGET)

# Phony targets
.PHONY: all clean

