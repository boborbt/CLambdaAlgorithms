include Makefile.vars

CFLAGS+=-Iinclude
LDFLAGS+=-Llib

.PHONY: Esercizio1 Esercizio2 Esercizio3 lib clean experiment1 experiment2 experiment3 tests

all: lib Esercizio1 Esercizio2 Esercizio3

clean:
	$(MAKE) -C Esercizio1 clean
	$(MAKE) -C Esercizio2 clean
	$(MAKE) -C Esercizio3 clean
	$(RM) -f lib/*.a
	$(RM) -f build/*.o
	$(RM) -rf bin/*

lib: lib/libcontainers_hash.a lib/libcontainers_st.a lib/libcontainers_rb.a

lib/libcontainers_hash.a: build/dictionary.o build/graph.o build/hash_table.o build/list.o build/key_info.o build/priority_queue.o build/dataset.o build/print_time.o build/double_container.o build/unit_testing.o build/array_g.o build/insertion_sort.o build/quick_sort.o build/merge_sort.o build/heap_sort.o build/dijkstra.o build/graph_visiting.o build/array.o
	$(LIBTOOL) lib/libcontainers_hash.a build/dictionary.o build/graph.o build/hash_table.o build/list.o build/key_info.o build/priority_queue.o build/dataset.o build/print_time.o build/double_container.o build/unit_testing.o build/array_g.o build/insertion_sort.o build/quick_sort.o build/merge_sort.o build/heap_sort.o build/dijkstra.o build/graph_visiting.o build/array.o

lib/libcontainers_st.a: build/stack.o build/dictionary.o build/graph.o build/search_tree.o build/key_info.o build/priority_queue.o build/dataset.o build/print_time.o build/double_container.o build/unit_testing.o build/array_g.o build/insertion_sort.o build/quick_sort.o build/merge_sort.o build/heap_sort.o build/dijkstra.o build/graph_visiting.o build/array.o
	$(LIBTOOL) lib/libcontainers_st.a build/dictionary.o build/graph.o build/search_tree.o build/key_info.o build/priority_queue.o build/dataset.o build/print_time.o build/double_container.o build/unit_testing.o build/stack.o build/array_g.o build/insertion_sort.o build/quick_sort.o build/merge_sort.o build/heap_sort.o build/dijkstra.o build/graph_visiting.o build/array.o

lib/libcontainers_rb.a: build/stack.o build/dictionary.o build/graph.o build/rb_tree.o build/key_info.o build/priority_queue.o build/dataset.o build/print_time.o build/double_container.o build/unit_testing.o build/array_g.o build/insertion_sort.o build/quick_sort.o build/merge_sort.o build/heap_sort.o build/dijkstra.o build/graph_visiting.o build/array.o
	$(LIBTOOL) lib/libcontainers_rb.a build/dictionary.o build/graph.o build/rb_tree.o build/key_info.o build/priority_queue.o build/dataset.o build/print_time.o build/double_container.o build/unit_testing.o build/stack.o build/array_g.o build/insertion_sort.o build/quick_sort.o build/merge_sort.o build/heap_sort.o build/dijkstra.o build/graph_visiting.o build/array.o

build/array.o: include/array.h include/array_g.h src/array.c
	$(CC) $(CFLAGS) -c src/array.c -o build/array.o

build/search_tree.o: include/dictionary.h src/search_tree.c include/key_info.h
		$(CC) $(CFLAGS) -c src/search_tree.c -o build/search_tree.o

build/stack.o: src/stack.c include/stack.h
		$(CC) $(CFLAGS) -c src/stack.c -o build/stack.o

build/hash_table.o: include/dictionary.h src/hash_table.c include/key_info.h include/list.h
	$(CC) $(CFLAGS) -c src/hash_table.c -o build/hash_table.o

build/list.o: include/list.h src/list.c
	$(CC) $(CFLAGS) -c src/list.c -o build/list.o

build/rb_tree.o: include/dictionary.h src/rb_tree.c include/key_info.h
	$(CC) $(CFLAGS) -c src/rb_tree.c -o build/rb_tree.o

build/key_info.o: include/key_info.h src/key_info.c
	$(CC) $(CFLAGS) -c src/key_info.c -o build/key_info.o

build/dictionary.o: include/dictionary.h src/dictionary.c include/key_info.h
	$(CC) $(CFLAGS) -c src/dictionary.c -o build/dictionary.o

build/graph.o: src/graph.c include/graph.h include/dictionary.h
	$(CC) $(CFLAGS) -c src/graph.c -o build/graph.o

build/priority_queue.o: src/priority_queue.c include/priority_queue.h
	$(CC) $(CFLAGS) -c src/priority_queue.c -o build/priority_queue.o

build/dataset.o: src/dataset.c include/dataset.h
	$(CC) $(CFLAGS) -c src/dataset.c -o build/dataset.o

build/print_time.o: src/print_time.c include/print_time.h
	$(CC) $(CFLAGS) -c src/print_time.c -o build/print_time.o

build/double_container.o: src/double_container.c include/double_container.h
	$(CC) $(CFLAGS) -c src/double_container.c -o build/double_container.o

build/unit_testing.o: src/unit_testing.c include/unit_testing.h
	$(CC) $(CFLAGS) -c src/unit_testing.c -o build/unit_testing.o

build/array_g.o: src/array_g.c include/array_g.h
	$(CC) $(CFLAGS) -c src/array_g.c -o build/array_g.o

build/insertion_sort.o: src/insertion_sort.c include/insertion_sort.h
	$(CC) $(CFLAGS) -c src/insertion_sort.c -o build/insertion_sort.o

build/quick_sort.o: src/quick_sort.c include/quick_sort.h
	$(CC) $(CFLAGS) -c src/quick_sort.c -o build/quick_sort.o

build/merge_sort.o: src/merge_sort.c include/merge_sort.h
	$(CC) $(CFLAGS) -c src/merge_sort.c -o build/merge_sort.o

build/heap_sort.o: src/heap_sort.c include/heap_sort.h
	$(CC) $(CFLAGS) -c src/heap_sort.c -o build/heap_sort.o

build/dijkstra.o: src/dijkstra.c include/dijkstra.h include/graph.h include/dictionary.h
	$(CC) $(CFLAGS) -c src/dijkstra.c -o build/dijkstra.o

build/graph_visiting.o: src/graph_visiting.c include/graph_visiting.h include/graph.h
	$(CC) $(CFLAGS) -c src/graph_visiting.c -o build/graph_visiting.o


Esercizio1:  lib/libcontainers_hash.a lib/libcontainers_rb.a lib/libcontainers_st.a
	$(MAKE) -C Esercizio1

Esercizio2: lib/libcontainers_hash.a lib/libcontainers_rb.a lib/libcontainers_st.a
	$(MAKE) -C Esercizio2

Esercizio3: lib/libcontainers_hash.a lib/libcontainers_rb.a lib/libcontainers_st.a
	$(MAKE) -C Esercizio3


tests: bin/sorting_tests bin/dictionary_tests_st bin/dictionary_tests_hash bin/dictionary_tests_rb bin/graph_tests bin/list_tests bin/array_tests
	bin/sorting_tests
	bin/dictionary_tests_st
	bin/dictionary_tests_hash
	bin/dictionary_tests_rb
	bin/graph_tests
	bin/list_tests
	bin/array_tests

bin/sorting_tests: tests/sorting_tests.c lib/libcontainers_st.a
	$(CC) $(CFLAGS) tests/sorting_tests.c -o bin/sorting_tests -lcontainers_st $(LDFLAGS)

bin/dictionary_tests_st: tests/dictionary_tests.c lib/libcontainers_st.a
	$(CC) $(CFLAGS) tests/dictionary_tests.c -o bin/dictionary_tests_st -lcontainers_st $(LDFLAGS)

bin/dictionary_tests_hash: tests/dictionary_tests.c lib/libcontainers_hash.a
	$(CC) $(CFLAGS) tests/dictionary_tests.c -o bin/dictionary_tests_hash -lcontainers_hash $(LDFLAGS)

bin/dictionary_tests_rb: tests/dictionary_tests_rb.c lib/libcontainers_rb.a
	$(CC) $(CFLAGS) tests/dictionary_tests_rb.c -o bin/dictionary_tests_rb -lcontainers_rb $(LDFLAGS)

bin/graph_tests: tests/graph_tests.c lib/libcontainers_st.a
	$(CC) $(CFLAGS) tests/graph_tests.c -o bin/graph_tests -lcontainers_st $(LDFLAGS)

bin/list_tests: tests/list_tests.c lib/libcontainers_hash.a
	$(CC) $(CFLAGS) tests/list_tests.c -o bin/list_tests -lcontainers_hash $(LDFLAGS)

bin/array_tests: tests/array_tests.c lib/libcontainers_st.a
	$(CC) $(CFLAGS) tests/array_tests.c -o bin/array_tests -lcontainers_st $(LDFLAGS)



experiment1: Esercizio1
	Esercizio1/bin/measure_times -q $(RECORDS_DATASET)
	Esercizio1/bin/measure_times -H $(RECORDS_DATASET)
	Esercizio1/bin/measure_times -m $(RECORDS_DATASET)

experiment2: Esercizio2
	Esercizio2/bin/measure_times_hash 1 $(RECORDS_DATASET)
	Esercizio2/bin/measure_times_hash 2 $(RECORDS_DATASET)
	Esercizio2/bin/measure_times_hash 3 $(RECORDS_DATASET)

	Esercizio2/bin/measure_times_st 1 $(RECORDS_DATASET)
	Esercizio2/bin/measure_times_st 2 $(RECORDS_DATASET)
	Esercizio2/bin/measure_times_st 3 $(RECORDS_DATASET)

	Esercizio2/bin/measure_times_rb 1 $(RECORDS_DATASET)
	Esercizio2/bin/measure_times_rb 2 $(RECORDS_DATASET)
	Esercizio2/bin/measure_times_rb 3 $(RECORDS_DATASET)


experiment3: Esercizio3
	Esercizio3/bin/measure_times -d $(DIST_DATASET) torino catania
	Esercizio3/bin/measure_times -c $(DIST_DATASET)

experiments: experiment1 experiment2 experiment3
