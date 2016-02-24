CFLAGS=-g -I../include

.PHONY: clean all

all: bin/tests bin/measure_times

clean:
	$(RM) -f build/* bin/*


bin/tests: src/tests.c build/search_tree.o src/dictionary.h
	$(CC) $(CFLAGS) -o bin/tests build/search_tree.o src/tests.c

bin/measure_times: src/measure_times.c build/search_tree.o ../lib/dataset.o
	$(CC) $(CFLAGS) -o bin/measure_times src/measure_times.c build/search_tree.o ../lib/dataset.o

build/search_tree.o: src/dictionary.h src/search_tree.c
	$(CC) $(CFLAGS) -c src/search_tree.c -o build/search_tree.o
