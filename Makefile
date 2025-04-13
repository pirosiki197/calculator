# Makefile for Calculator Project

CC = clang
CFLAGS = -Wall -Wextra -std=c11

# Files
SRC = main.c calculator.c
HEADERS = calculator.h
OBJ = $(SRC:.c=.o)
TARGET = main

# Default rule
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile .c to .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

# Optional: for rebuild
rebuild: clean all
