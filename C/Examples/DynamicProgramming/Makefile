include ../../Makefile.vars

BASEDIR=../..
CFLAGS+=-I$(BASEDIR)/include
LDFLAGS+=-L$(BASEDIR)/lib

.PHONY: clean all tests

all: bin build  bin/autocorrect bin/edit_distance_graph

bin:
	@mkdir bin

build:
	@mkdir build

clean:
	@$(RM) -rf build
	@$(RM) -rf bin


bin/autocorrect: src/autocorrect.c $(BASEDIR)/include/array.h  $(BASEDIR)/include/iterator.h $(BASEDIR)/include/iterator_functions.h $(BASEDIR)/include/editing_distance.h $(BASEDIR)/lib/libcontainers.a Makefile $(BASEDIR)/Makefile.vars
	@$(CC) $(CFLAGS) -o bin/autocorrect src/autocorrect.c  -lcontainers $(LDFLAGS)

bin/edit_distance_graph: src/edit_distance_graph.c
	@$(CC) $(CFLAGS) -o bin/edit_distance_graph src/edit_distance_graph.c
