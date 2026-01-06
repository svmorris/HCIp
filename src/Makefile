# Compiler
CC = gcc

# Compiler flags
# -Wall and -Wextra for basic warnings
CFLAGS = -Wall -Wextra -std=c11 -g

# Output binary
TARGET = parser

# Source files (all in same directory)
SRCS = main.c spectable.c

# Object files (derived automatically)
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile step
%.o: %.c spectable.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
