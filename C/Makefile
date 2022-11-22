include Makefile.vars

MAKEFLAGS += --silent

CFLAGS+=-Iinclude
LDFLAGS+=-Llib

export CFLAGS
export LDFLAGS

define exec
	@tput bold
	@echo $(1)
	@tput sgr 0

	@$(1)
endef

.PHONY: Examples libs clean experiment1 experiment2 experiment3 tests

all: build lib bin libs Examples

clean:
	$(MAKE) -C Examples clean
	$(RM) -rf lib
	$(RM) -rf build
	$(RM) -rf bin

HEADERS=include/*.h

COMMON_OBJECTS=build/dictionary.o build/graph.o build/keys.o build/priority_queue.o build/print_time.o build/double_container.o build/unit_testing.o build/array_g.o build/insertion_sort.o build/quick_sort.o build/merge_sort.o build/heap_sort.o build/dijkstra.o build/graph_visiting.o build/array.o build/stack.o build/errors.o build/union_find.o build/queue.o build/kruskal.o build/multy_way_tree.o build/string_utils.o build/basic_iterators.o build/iterator.o build/mem.o build/array_alt.o build/editing_distance.o build/prim.o build/set.o build/dataset.o

build:
	mkdir build

lib:
	mkdir lib

bin:
	mkdir bin

libs: lib/libcontainers.a

lib/libcontainers.a: build lib $(OPTIONAL_OBJECTS) $(COMMON_OBJECTS) $(HEADERS)
	tput bold; echo Making $@; tput sgr 0
	$(LIBTOOL) lib/libcontainers.a $(OPTIONAL_OBJECTS) $(COMMON_OBJECTS)

# Source compilation

build/%.o: src/%.c   Makefile Makefile.vars $(HEADERS)
	tput dim; echo Making $<; tput sgr 0
	$(CC) $(CFLAGS) -c $< -o $@




tests: test_binaries
	$(call exec, bin/iterator_tests)
	$(call exec, bin/sorting_tests)
	$(call exec, bin/dictionary_tests)
	$(call exec, bin/set_tests)
	$(call exec, bin/graph_tests)
	$(call exec, bin/list_tests)
	$(call exec, bin/array_alt_tests)
	$(call exec, bin/array_tests)
	$(call exec, bin/errors_tests)
	$(call exec, bin/union_find_tests)
	$(call exec, bin/queue_tests)
	$(call exec, bin/priority_queue_tests)
	$(call exec, bin/multy_way_tree_tests)
	$(call exec, bin/editing_distance_tests)
	$(call exec, bin/basic_iterators_tests)
	$(call exec, bin/dataset_tests)

test_binaries: build lib bin bin/sorting_tests bin/dictionary_tests bin/ bin/graph_tests bin/list_tests bin/array_tests bin/array_alt_tests bin/errors_tests bin/union_find_tests bin/queue_tests bin/priority_queue_tests bin/iterator_tests bin/multy_way_tree_tests bin/editing_distance_tests bin/basic_iterators_tests bin/set_tests bin/dataset_tests

bin/editing_distance_tests: tests/editing_distance_tests.c include/editing_distance.h lib/libcontainers.a $(HEADERS)
	$(CC) $(CFLAGS) tests/editing_distance_tests.c -o bin/editing_distance_tests -lcontainers $(LDFLAGS)

bin/iterator_tests: tests/iterator_tests.c include/unit_testing.h lib/libcontainers.a $(HEADERS)
	$(CC) $(CFLAGS) tests/iterator_tests.c -o bin/iterator_tests -lcontainers $(LDFLAGS)

bin/sorting_tests: tests/sorting_tests.c include/unit_testing.h lib/libcontainers.a $(HEADERS)
	$(CC) $(CFLAGS) tests/sorting_tests.c -o bin/sorting_tests -lcontainers $(LDFLAGS)

bin/dictionary_tests: tests/dictionary_tests.c include/unit_testing.h lib/libcontainers.a $(HEADERS)
	$(CC) $(CFLAGS) tests/dictionary_tests.c -o bin/dictionary_tests -lcontainers $(LDFLAGS)

bin/graph_tests: tests/graph_tests.c include/unit_testing.h lib/libcontainers.a $(HEADERS)
	$(CC) $(CFLAGS) tests/graph_tests.c -o bin/graph_tests -lcontainers $(LDFLAGS)

bin/set_tests: tests/set_tests.c include/unit_testing.h lib/libcontainers.a $(HEADERS)
	$(CC) $(CFLAGS) tests/set_tests.c -o bin/set_tests -lcontainers $(LDFLAGS)


bin/list_tests: tests/list_tests.c include/unit_testing.h lib/libcontainers.a $(HEADERS)
	$(CC) $(CFLAGS) tests/list_tests.c -o bin/list_tests -lcontainers $(LDFLAGS)

bin/array_tests: tests/array_tests.c include/unit_testing.h lib/libcontainers.a $(HEADERS)
	$(CC) $(CFLAGS) tests/array_tests.c -o bin/array_tests -lcontainers $(LDFLAGS)

bin/array_alt_tests: tests/array_alt_tests.c include/unit_testing.h lib/libcontainers.a $(HEADERS)
	$(CC) $(CFLAGS) tests/array_alt_tests.c -o bin/array_alt_tests -lcontainers $(LDFLAGS)

bin/errors_tests: tests/errors_tests.c include/unit_testing.h lib/libcontainers.a $(HEADERS)
	$(CC) $(CFLAGS) tests/errors_tests.c -o bin/errors_tests -lcontainers $(LDFLAGS)

bin/union_find_tests: tests/union_find_tests.c include/unit_testing.h lib/libcontainers.a $(HEADERS)
	$(CC) $(CFLAGS) tests/union_find_tests.c -o bin/union_find_tests -lcontainers $(LDFLAGS)

bin/queue_tests: tests/queue_tests.c include/unit_testing.h lib/libcontainers.a $(HEADERS)
	$(CC) $(CFLAGS) tests/queue_tests.c -o bin/queue_tests -lcontainers $(LDFLAGS)

bin/priority_queue_tests: tests/priority_queue_tests.c include/unit_testing.h lib/libcontainers.a $(HEADERS)
	$(CC) $(CFLAGS) tests/priority_queue_tests.c -o bin/priority_queue_tests -lcontainers $(LDFLAGS)

bin/multy_way_tree_tests: tests/multy_way_tree_tests.c include/unit_testing.h lib/libcontainers.a $(HEADERS)
	$(CC) $(CFLAGS) tests/multy_way_tree_tests.c -o bin/multy_way_tree_tests -lcontainers $(LDFLAGS)

bin/basic_iterators_tests: lib/libcontainers.a tests/basic_iterators_tests.c $(HEADERS)
	$(CC) $(CFLAGS) tests/basic_iterators_tests.c -o bin/basic_iterators_tests $(LDFLAGS) lib/libcontainers.a

bin/dataset_tests: lib/libcontainers.a tests/dataset_tests.c $(HEADERS)
	$(CC) $(CFLAGS) tests/dataset_tests.c -o bin/dataset_tests $(LDFLAGS) lib/libcontainers.a


include Makefile.exps
