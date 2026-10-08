# Compiler and flags
CC       = gcc
CFLAGS   = -Wall -Wextra -std=gnu99 -g
LDFLAGS  = -lcrypto

# Executable name
TARGET   = cgrab

all: $(TARGET)
	$(CC) $(TARGET).c -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf scans
	rm $(TARGET)
