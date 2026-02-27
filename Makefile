CC = gcc

CFLAGS = -Iinclude

SRCS = src/main.c src/proc.c

TARGET = syslens

all:
		$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
		rm -f $(TARGET)