include Makefile.vars

CFLAGS+=-Iinclude

.PHONY: Esercizio1 Esercizio2 Esercizio3 lib clean experiment1 experiment2 experiment3

all: lib Esercizio1 Esercizio2 Esercizio3

clean:
	$(MAKE) -C Esercizio1 clean
	$(MAKE) -C Esercizio2 clean
	$(MAKE) -C Esercizio3 clean
	$(RM) -f lib/*.a
	$(RM) -f build/*.o

lib: lib/libcontainers_hash.a lib/libcontainers_st.a

lib/libcontainers_hash.a: build/dictionary.o build/graph.o build/hash_table.o build/key_info.o build/priority_queue.o build/dataset.o build/print_time.o build/double_container.o build/unit_testing.o build/array_g.o build/heap_sort.o build/insertion_sort.o build/quick_sort.o build/merge_sort.o build/heap_sort.o build/dijkstra.o build/graph_visiting.o
	$(LIBTOOL) lib/libcontainers_hash.a build/dictionary.o build/graph.o build/hash_table.o build/key_info.o build/priority_queue.o build/dataset.o build/print_time.o build/double_container.o build/unit_testing.o build/array_g.o build/heap_sort.o build/insertion_sort.o build/quick_sort.o build/merge_sort.o build/heap_sort.o build/dijkstra.o build/graph_visiting.o

lib/libcontainers_st.a: build/stack.o build/dictionary.o build/graph.o build/search_tree.o build/key_info.o build/priority_queue.o build/dataset.o build/print_time.o build/double_container.o build/unit_testing.o build/array_g.o build/heap_sort.o build/insertion_sort.o build/quick_sort.o build/merge_sort.o build/heap_sort.o build/dijkstra.o build/graph_visiting.o
	$(LIBTOOL) lib/libcontainers_st.a build/dictionary.o build/graph.o build/search_tree.o build/key_info.o build/priority_queue.o build/dataset.o build/print_time.o build/double_container.o build/unit_testing.o build/stack.o build/array_g.o build/heap_sort.o build/insertion_sort.o build/quick_sort.o build/merge_sort.o build/heap_sort.o build/dijkstra.o build/graph_visiting.o

build/search_tree.o: include/dictionary.h src/search_tree.c include/key_info.h
		$(CC) $(CFLAGS) -c src/search_tree.c -o build/search_tree.o

build/stack.o: include/stack.h
		$(CC) $(CFLAGS) -c src/stack.c -o build/stack.o

build/hash_table.o: include/dictionary.h src/hash_table.c include/key_info.h
	$(CC) $(CFLAGS) -c src/hash_table.c -o build/hash_table.o

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


Esercizio1:
	$(MAKE) -C Esercizio1

Esercizio2:
	$(MAKE) -C Esercizio2

Esercizio3:
	$(MAKE) -C Esercizio3


tests:
	$(MAKE) -C Esercizio1 tests
	$(MAKE) -C Esercizio2 tests
	$(MAKE) -C Esercizio3 tests

experiment1:
	Esercizio1/bin/measure_times -q ../../DatasetsGenerators/records/records.csv
	Esercizio1/bin/measure_times -H ../../DatasetsGenerators/records/records.csv
	Esercizio1/bin/measure_times -m ../../DatasetsGenerators/records/records.csv

experiment2:
	Esercizio2/bin/measure_times_hash 1 ../../DatasetsGenerators/records/records.csv
	Esercizio2/bin/measure_times_hash 2 ../../DatasetsGenerators/records/records.csv
	Esercizio2/bin/measure_times_hash 3 ../../DatasetsGenerators/records/records.csv

	Esercizio2/bin/measure_times_st 1 ../../DatasetsGenerators/records/records.csv
	Esercizio2/bin/measure_times_st 2 ../../DatasetsGenerators/records/records.csv
	Esercizio2/bin/measure_times_st 3 ../../DatasetsGenerators/records/records.csv

experiment3:
	Esercizio3/bin/measure_times -d ../../DatasetsGenerators/cities/italian_dist_graph.csv torino catania
	Esercizio3/bin/measure_times -c ../../DatasetsGenerators/cities/italian_dist_graph.csv

experiments: experiment1 experiment2 experiment3
