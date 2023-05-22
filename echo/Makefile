# Makefile for echo
#
# You may modify this file any way you like (except for the handin
# rule). You instructor will type "make" on your specific Makefile to
# build your proxy from sources.

CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread

all: echoclient echoserveri

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.c

echoclient.o: echoclient.c csapp.h
	$(CC) $(CFLAGS) -c echoclient.c

echoserveri.o: echoserveri.c csapp.h
	$(CC) $(CFLAGS) -c echoserveri.c

echo.o: echo.c csapp.hand
	$(CC) $(CLFAGS) -c echo.c

echoclient: echoclient.o csapp.o
	$(CC) $(CFLAGS) echoclient.o csapp.o -o echoclient $(LDFLAGS)

echoserveri: echoserveri.o echo.c csapp.o
	$(CC) $(CFLAGS) echoserveri.o echo.c csapp.o -o echoserveri $(LDFLAGS)

clean:
	rm -f *~ *.o