# Define the compiler
CC = gcc

# Define the compiler flags
CFLAGS = -Wall -Iinclude

# Define the source files and object files
SRCS = $(shell find ./src -type f -name '*.c')
OBJS = $(SRCS:.c=.o)

# Define the header files
HEADERS = $(shell find ./include -type f -name '*.h')

# Define the executable name
EXEC = Baby-Blue

# Default target
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Rule to build object files from source files
src/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean
