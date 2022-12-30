SRCS=main.c mymathlib.c
EXEC=exec
CC=gcc
CFLAGS=-Wall -Wextra -O2 -std=gnu11 -g -c

OBJS=$(SRCS:.c=.o)

all: $(EXEC)

.c.o:
	$(CC) $(CFLAGS) -o $*.o $<

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(EXEC)
	rm -f *.o

.PHONY: all clean 