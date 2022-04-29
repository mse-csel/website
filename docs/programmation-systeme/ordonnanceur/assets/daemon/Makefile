DIRS=$(filter-out Makefile daemon.txt, $(wildcard *))

all clean install:
	for dir in $(DIRS); do $(MAKE) $@ -C $$dir; done

