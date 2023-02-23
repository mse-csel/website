.c.o:
	@echo "(CC) $<"
	@$(CC) $(CFLAGS) –MD -o $*.o $<

$(EXEC): $(OBJS)
	@echo "(LD) $@"
	@$(CC) $(LDFLAGS) -o $@ $^