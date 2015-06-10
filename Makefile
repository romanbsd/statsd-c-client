major_version=2
version=$(major_version).0.1

LOCAL_DIR=/usr/local
LIB_DIR=$(LOCAL_DIR)/lib
INCLUDE_DIR=$(LOCAL_DIR)/include/statsd
CCFLAGS=
CFLAGS+=-Wall -g
LIBNAME=libstatsdclient.so
LIBFLAGS=


ifeq ($(OS),Windows_NT)
    CCFLAGS += -soname=$(LIBNAME) -D WIN32
	LIBFLAGS += -shared
    ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        CCFLAGS += -D AMD64
    endif
    ifeq ($(PROCESSOR_ARCHITECTURE),x86)
        CCFLAGS += -D IA32
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -install_name $(LIBNAME) -D OSX
		LIBFLAGS += -dynamiclib
	else
		LIBFLAGS += -shared
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CCFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        CCFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        CCFLAGS += -D ARM
    endif
endif

lib: libstatsdclient.so

all:  test-client uptimed

uptimed:  uptimed.o libstatsdclient.so
	$(CC) -o uptimed uptimed.o -L . -lstatsdclient 

test-client:	libstatsdclient.so test-client.o
	$(CC) -o test-client test-client.o -L . -lstatsdclient

libstatsdclient.so:	statsd-client.c
	$(CC)  -fPIC -o $@.$(version) $(LIBFLAGS) $(CCFLAGS) $<
	ln -sf $@.$(version) $@

install: lib
	cp libstatsdclient.so.$(version) $(LIB_DIR)
	ln -sf $(LIB_DIR)/libstatsdclient.so.$(version) $(LIB_DIR)/$(LIBNAME)
	ln -sf $(LIB_DIR)/libstatsdclient.so.$(version) $(LIB_DIR)/$(LIBNAME).$(major_version)
	mkdir -p $(INCLUDE_DIR)
	cp statsd-client.h $(INCLUDE_DIR)

clean:
	rm -f *.o *.so* test-client uptimed
