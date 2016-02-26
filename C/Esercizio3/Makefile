include ../Makefile.vars

.PHONY: tests all clean

all: tests

tests: bin/tests

clean:
	$(RM) -rf build/*
	$(RM) -rf bin/*

bin/tests: build/dijkstra.o build/graph.o build/priority_queue.o ../Esercizio2/build/hash_table.o ../Esercizio2/build/key_info.o ../include/unit_testing.h src/tests.c
	$(CC) $(CFLAGS) -o bin/tests build/dijkstra.o build/graph.o build/priority_queue.o ../Esercizio2/build/hash_table.o ../Esercizio2/build/key_info.o src/tests.c

build/dijkstra.o: src/dijkstra.c src/dijkstra.h src/graph.h ../Esercizio2/src/dictionary.h
	$(CC) $(CFLAGS) -c src/dijkstra.c -o build/dijkstra.o

build/graph.o: src/graph.c src/graph.h ../Esercizio2/src/dictionary.h
	$(CC) $(CFLAGS) -c src/graph.c -o build/graph.o

build/priority_queue.o: src/priority_queue.c src/priority_queue.h
	$(CC) $(CFLAGS) -c src/priority_queue.c -o build/priority_queue.o
