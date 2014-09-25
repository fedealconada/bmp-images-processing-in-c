CFLAGS=-Wall -std=c99
main: main.c matriz.c matriz.h
	$(CC) $(CFLAGS) -o main main.c matriz.c
