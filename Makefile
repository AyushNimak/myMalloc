CC = gcc
CFLAGS = -Wall -fsanitize=address -std=c99 -O2

all: memtest memgrind mytest clean

memtest.o: memtest.c mymalloc.h
	$(CC) $(CFLAGS)	-c memtest.c

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c memgrind.c

mytest.o: mytest.c mymalloc.h
	$(CC) $(CFLAGS) -c mytest.c

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c mymalloc.c

memtest: memtest.o mymalloc.o
	$(CC) $(CFLAGS) memtest.o mymalloc.o -o memtest

memgrind: memgrind.o mymalloc.o 
	$(CC) $(CFLAGS) memgrind.o mymalloc.o -o memgrind

mytest: mytest.o mymalloc.o 
	$(CC) $(CFLAGS) mytest.o mymalloc.o -o mytest

clean: 
	rm *.o

