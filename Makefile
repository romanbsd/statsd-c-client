CFLAGS+=-Wall -g

all:  test-client uptimed

major_version=2
version=$(major_version).0.1

uptimed:  uptimed.o libstatsdclient.so
	$(CC) -o uptimed uptimed.o -L . -lstatsdclient 

test-client:	libstatsdclient.so test-client.o
	$(CC) -o test-client test-client.o -L . -lstatsdclient

libstatsdclient.so:	statsd-client.c
	$(CC) -shared -fPIC -o $@.$(version) -Wl,-soname=libstatsdclient.so.$(major_version) $<
	ln -sf $@.$(version) $@

clean:
	rm -f *.o *.so* test-client uptimed
