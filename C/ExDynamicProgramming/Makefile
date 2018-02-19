include ../Makefile.vars
CFLAGS+=-I../include
LDFLAGS+=-L../lib

.PHONY: clean all tests

all: bin build  bin/autocorrect

bin:
	@mkdir bin

build:
	@mkdir build

clean:
	@$(RM) -rf build
	@$(RM) -rf bin


bin/autocorrect: src/autocorrect.c ../include/array.h  ../include/iterator.h ../include/iterator_functions.h ../include/editing_distance.h ../lib/libcontainers_hash.a Makefile ../Makefile.vars
	@$(CC) $(CFLAGS) -o bin/autocorrect src/autocorrect.c  -lcontainers_hash $(LDFLAGS)
