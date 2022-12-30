EXEC=exec
CC=gcc
CFLAGS=-Wall -Wextra -O2 -std=gnu11 -g -c

all: $(EXEC)

$(EXEC): main.o mymathlib.o
	$(CC) $(LDFLAGS) -o $(EXEC) main.o \
	mymathlib.o

main.o: main.c mymathlib.h
	$(CC) $(CFLAGS) -o main.o main.c

mymathlib.o: mymathlib.c mymathlib.h
	$(CC) $(CFLAGS) -o mymathlib.o \
	mymathlib.c

clean:
	rm -f $(EXEC)
	rm -f *.o

.PHONY: all clean