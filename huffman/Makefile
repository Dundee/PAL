CC	=	gcc
CFLAGS	=	-Wall -pedantic -g
LIBS	=

default: encode

encode: encode.c list.c

test: binomial_heap.c heap_test.c
	$(CC) $(CFLAGS) -o test binomial_heap.c heap_test.c

clean:
	rm main
