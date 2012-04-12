CFLAGS+=-Wall -g

test-client:	statsd-client.o test-client.o
	$(CC) -o test-client *.o

statsd-client.o:	statsd-client.h statsd-client.c

clean:
	rm -f *.o test-client
