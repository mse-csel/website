exec: main.o mymathlib.o
	gcc -o exec main.o mymathlib.o

main.o: main.c mymathlib.h
	gcc -c -Wall -Wextra -O2 -std=gnu11 -g -o main.o main.c

mymathlib.o: mymathlib.c mymathlib.h
	gcc -c -Wall -Wextra -O2 -std=gnu11 -g -o mymathlib.o mymathlib.c