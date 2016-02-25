CFLAGS=-g -I../include

.PHONY: clean all tests

all: bin/tests_st bin/measure_times_st bin/tests_hash bin/measure_times_hash

clean:
	$(RM) -f build/* bin/*

tests: bin/tests_st bin/tests_hash
	bin/tests_st
	bin/tests_hash

bin/tests_st: src/tests.c build/search_tree.o src/dictionary.h src/key_info.h build/key_info.o
	$(CC) $(CFLAGS) -o bin/tests_st build/search_tree.o build/key_info.o src/tests.c

bin/measure_times_st: src/measure_times.c build/search_tree.o ../lib/dataset.o src/key_info.h build/key_info.o
	$(CC) $(CFLAGS) -o bin/measure_times_st src/measure_times.c build/search_tree.o ../lib/dataset.o build/key_info.o

bin/tests_hash: src/tests.c build/hash_table.o src/dictionary.h src/key_info.h build/key_info.o
	$(CC) $(CFLAGS) -o bin/tests_hash build/hash_table.o build/key_info.o src/tests.c

bin/measure_times_hash: src/measure_times.c build/hash_table.o ../lib/dataset.o src/key_info.h build/key_info.o
	$(CC) $(CFLAGS) -o bin/measure_times_hash src/measure_times.c build/hash_table.o ../lib/dataset.o build/key_info.o


build/search_tree.o: src/dictionary.h src/search_tree.c src/key_info.h
	$(CC) $(CFLAGS) -c src/search_tree.c -o build/search_tree.o

build/hash_table.o: src/dictionary.h src/hash_table.c src/key_info.h
	$(CC) $(CFLAGS) -c src/hash_table.c -o build/hash_table.o

build/key_info.o: src/key_info.h src/key_info.c
	$(CC) $(CFLAGS) -c src/key_info.c -o build/key_info.o
