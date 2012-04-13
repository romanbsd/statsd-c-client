CFLAGS+=-Wall -g

all:  test-client uptimed

uptimed:  uptimed.o statsd-client.o
	$(CC) -o uptimed uptimed.o statsd-client.o

test-client:	statsd-client.o test-client.o
	$(CC) -o test-client statsd-client.o test-client.o

statsd-client.o:	statsd-client.h statsd-client.c

clean:
	rm -f *.o test-client
