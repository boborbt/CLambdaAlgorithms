include ../Makefile.vars
CFLAGS+=-I../include
LDFLAGS=-L../lib

.PHONY: tests all clean

all: bin/tests bin/measure_times

tests: bin/tests
	bin/tests

clean:
	$(RM) -rf build/*
	$(RM) -rf bin/*

bin/tests: build/dijkstra.o ../include/unit_testing.h src/tests.c ../lib/libcontainers_hash.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/tests build/dijkstra.o src/tests.c -lcontainers_hash

build/dijkstra.o: src/dijkstra.c src/dijkstra.h ../include/graph.h ../include/dictionary.h
	$(CC) $(CFLAGS) -c src/dijkstra.c -o build/dijkstra.o

bin/measure_times: src/measure_times.c src/dijkstra.h build/dijkstra.o ../include/graph.h ../lib/libcontainers_hash.a build/graph_visiting.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/measure_times src/measure_times.c build/dijkstra.o build/graph_visiting.o -lcontainers_hash

build/graph_visiting.o: src/graph_visiting.c src/graph_visiting.h ../include/graph.h
	$(CC) $(CFLAGS) -c src/graph_visiting.c -o build/graph_visiting.o
