CC = gcc

CFLAGS = -Iinclude

SRCS = main.c

TARGET = syslens

all:
		$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
		rm -f $(TARGET)