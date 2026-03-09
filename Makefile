CC = gcc

CFLAGS = -Iinclude -g

SRCS = src/main.c src/proc.c src/sys.c src/mem.c

TARGET = syslens

all:
		$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
		rm -f $(TARGET)

install:
	cp $(TARGET) /usr/bin
	@echo "installed"

uninstall:
	rm -f /usr/bin/$(TARGET)
	@echo "uninstalled"