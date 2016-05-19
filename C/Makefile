include Makefile.vars

CFLAGS+=-Iinclude
LDFLAGS+=-Llib

.PHONY: Esercizio1 Esercizio2 Esercizio3 libs clean experiment1 experiment2 experiment3 tests

all: build lib bin libs Esercizio1 Esercizio2 Esercizio3

clean:
	$(MAKE) -C Esercizio1 clean
	$(MAKE) -C Esercizio2 clean
	$(MAKE) -C Esercizio3 clean
	$(RM) -f lib/*.a
	$(RM) -f build/*.o
	$(RM) -rf bin/*

HEADERS=include/*.h

COMMON_OBJECTS=build/dictionary.o build/graph.o  build/list.o build/keys.o build/priority_queue.o build/dataset.o build/print_time.o build/double_container.o build/unit_testing.o build/array_g.o build/insertion_sort.o build/quick_sort.o build/merge_sort.o build/heap_sort.o build/dijkstra.o build/graph_visiting.o build/array.o build/stack.o build/errors.o

build:
	mkdir build

lib:
	mkdir lib

bin:
	mkdir bin

libs: lib/libcontainers_hash.a lib/libcontainers_st.a lib/libcontainers_rb.a

lib/libcontainers_hash.a: build/hash_table.o $(COMMON_OBJECTS)
	$(LIBTOOL) lib/libcontainers_hash.a build/hash_table.o $(COMMON_OBJECTS)

lib/libcontainers_st.a: build/search_tree.o	$(COMMON_OBJECTS)
	$(LIBTOOL) lib/libcontainers_st.a build/search_tree.o	$(COMMON_OBJECTS)

lib/libcontainers_rb.a: build/rb_tree.o $(COMMON_OBJECTS)
	$(LIBTOOL) lib/libcontainers_rb.a build/rb_tree.o $(COMMON_OBJECTS)


# Source compilation

build/%.o: src/%.c $(HEADERS) Makefile Makefile.vars
	$(CC) $(CFLAGS) -c $< -o $@


# Experiments

Esercizio1:  lib/libcontainers_hash.a lib/libcontainers_rb.a lib/libcontainers_st.a
	$(MAKE) -C Esercizio1

Esercizio2: lib/libcontainers_hash.a lib/libcontainers_rb.a lib/libcontainers_st.a
	$(MAKE) -C Esercizio2

Esercizio3: lib/libcontainers_hash.a lib/libcontainers_rb.a lib/libcontainers_st.a
	$(MAKE) -C Esercizio3


tests: test_binaries
	bin/sorting_tests
	bin/dictionary_tests_st
	bin/dictionary_tests_hash
	bin/dictionary_tests_rb
	bin/graph_tests
	bin/list_tests
	bin/array_tests
	bin/errors_tests

test_binaries: bin/sorting_tests bin/dictionary_tests_st bin/dictionary_tests_hash bin/dictionary_tests_rb bin/graph_tests bin/list_tests bin/array_tests bin/errors_tests

bin/sorting_tests: tests/sorting_tests.c include/unit_testing.h lib/libcontainers_st.a
	$(CC) $(CFLAGS) tests/sorting_tests.c -o bin/sorting_tests -lcontainers_st $(LDFLAGS)

bin/dictionary_tests_st: tests/dictionary_tests.c include/unit_testing.h lib/libcontainers_st.a
	$(CC) $(CFLAGS) tests/dictionary_tests.c -o bin/dictionary_tests_st -lcontainers_st $(LDFLAGS)

bin/dictionary_tests_hash: tests/dictionary_tests.c include/unit_testing.h lib/libcontainers_hash.a
	$(CC) $(CFLAGS) tests/dictionary_tests.c -o bin/dictionary_tests_hash -lcontainers_hash $(LDFLAGS)

bin/dictionary_tests_rb: tests/dictionary_tests_rb.c include/unit_testing.h lib/libcontainers_rb.a
	$(CC) $(CFLAGS) tests/dictionary_tests_rb.c -o bin/dictionary_tests_rb -lcontainers_rb $(LDFLAGS)

bin/graph_tests: tests/graph_tests.c include/unit_testing.h lib/libcontainers_st.a
	$(CC) $(CFLAGS) tests/graph_tests.c -o bin/graph_tests -lcontainers_st $(LDFLAGS)

bin/list_tests: tests/list_tests.c include/unit_testing.h lib/libcontainers_hash.a
	$(CC) $(CFLAGS) tests/list_tests.c -o bin/list_tests -lcontainers_hash $(LDFLAGS)

bin/array_tests: tests/array_tests.c include/unit_testing.h lib/libcontainers_st.a
	$(CC) $(CFLAGS) tests/array_tests.c -o bin/array_tests -lcontainers_st $(LDFLAGS)

bin/errors_tests: tests/errors_tests.c include/unit_testing.h lib/libcontainers_st.a
	$(CC) $(CFLAGS) tests/errors_tests.c -o bin/errors_tests -lcontainers_st $(LDFLAGS)


include Makefile.exps
