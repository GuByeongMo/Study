CC = gcc
CFLAGS = -Wall -g

all: test_smalloc

test_smalloc: test_smalloc.o smalloc.o
	$(CC) $(CFLAGS) -o test_smalloc test_smalloc.o smalloc.o

test_smalloc.o: test_smalloc.c smalloc.h
	$(CC) $(CFLAGS) -c test_smalloc.c

smalloc.o: smalloc.c smalloc.h
	$(CC) $(CFLAGS) -c smalloc.c

clean:
	rm -f *.o test_smalloc 