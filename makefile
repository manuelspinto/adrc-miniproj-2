CC=gcc
CFLAGS=-g -c -Wall -pedantic -pg
LIBS=-lm

default: internet

internet: internet.o
	$(CC) -o internet internet.o $(LIBS) -pg

internet.o: internet.c internet.h
	$(CC) $(CFLAGS) internet.c

clean::
	rm -f *.o core a.out internet *~