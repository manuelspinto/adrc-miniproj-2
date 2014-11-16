CC=gcc
CFLAGS=-g -c -Wall -pedantic
LIBS=-lm

default: internet

internet: internet.o
	$(CC) -o internet internet.o $(LIBS)

internet.o: internet.c internet.h
	$(CC) $(CFLAGS) internet.c

clean::
	rm -f *.o core a.out internet *~
