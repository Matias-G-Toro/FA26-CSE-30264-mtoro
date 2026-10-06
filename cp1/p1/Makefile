# Compiler and flags
CC       = gcc
CFLAGS   = -Wall -Wextra -std=c11 -g
LDFLAGS  = -lcrypto

# Executable name
TARGET   = cgrab

# Source and object files
# By default, grabs cgrab.c or all .c files in the directory
SRCS     = cgrab.c
OBJS     = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Linking step
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compilation step (object files)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Deep clean (cleans build artifacts + downloaded scan outputs)
spotless: clean
	rm -rf scans

.PHONY: all clean spotless
