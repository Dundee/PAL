CC	=	gcc
CFLAGS	=	-Wall -pedantic -g
LIBS	=

default: main

main: main.c

test: binomial_heap.c heap_test.c
	$(CC) $(CFLAGS) -o test binomial_heap.c heap_test.c

clean:
	rm main
