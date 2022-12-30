SRCS=main.c mymathlib.c
EXEC=exec
CC=gcc
CFLAGS=-Wall -Wextra -O2 -std=gnu11 -g -c
OBJS=$(SRCS:.c=.o)

all: $(EXEC)

-include $(OBJS:.o=.d)

.c.o:
	$(CC) $(CFLAGS) -MD -o $*.o $<

	$(CC) -MM $< -MF $*.d

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(EXEC)
	rm -f *.o *.d

.PHONY: all clean