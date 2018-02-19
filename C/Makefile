include Makefile.vars

MAKEFLAGS += --silent

CFLAGS+=-Iinclude -g -DDEBUG
LDFLAGS+=-Llib

define exec
	@tput bold
	@echo $(1)
	@tput sgr 0

	@$(1)
endef

.PHONY: ExSorting ExDictionaries ExGraphs libs clean experiment1 experiment2 experiment3 tests

all: build lib bin libs ExSorting ExDictionaries ExGraphs ExDynamicProgramming

clean:
	$(MAKE) -C ExSorting clean
	$(MAKE) -C ExDictionaries clean
	$(MAKE) -C ExGraphs clean
	$(MAKE) -C ExDynamicProgramming
	$(RM) -rf lib
	$(RM) -rf build
	$(RM) -rf bin

HEADERS=include/*.h

COMMON_OBJECTS=build/dictionary.o build/graph.o  build/list.o build/keys.o build/priority_queue.o build/dataset.o build/print_time.o build/double_container.o build/unit_testing.o build/array_g.o build/insertion_sort.o build/quick_sort.o build/merge_sort.o build/heap_sort.o build/dijkstra.o build/graph_visiting.o build/array.o build/stack.o build/errors.o build/union_find.o build/queue.o build/kruskal.o build/multy_way_tree.o build/string_utils.o build/iterator.o build/mem.o build/array_alt.o build/editing_distance.o build/prim.o

build:
	mkdir build

lib:
	mkdir lib

bin:
	mkdir bin

libs: lib/libcontainers_hash.a lib/libcontainers_st.a lib/libcontainers_rb.a

lib/libcontainers_hash.a: build lib build/hash_table.o $(COMMON_OBJECTS) $(HEADERS)
	tput bold; echo Making $@; tput sgr 0
	$(LIBTOOL) lib/libcontainers_hash.a build/hash_table.o $(COMMON_OBJECTS)

lib/libcontainers_st.a: build lib build/search_tree.o	$(COMMON_OBJECTS) $(HEADERS)
	tput bold; echo Making $@; tput sgr 0
	$(LIBTOOL) lib/libcontainers_st.a build/search_tree.o	$(COMMON_OBJECTS)

lib/libcontainers_rb.a: build lib build/rb_tree.o $(COMMON_OBJECTS) $(HEADERS)
	tput bold; echo Making $@; tput sgr 0
	$(LIBTOOL) lib/libcontainers_rb.a build/rb_tree.o $(COMMON_OBJECTS)


# Source compilation

build/%.o: src/%.c   Makefile Makefile.vars
	tput dim; echo Making $<; tput sgr 0
	$(CC) $(CFLAGS) -c $< -o $@


# Experiments

ExSorting:  lib/libcontainers_hash.a lib/libcontainers_rb.a lib/libcontainers_st.a
	tput bold; tput setaf 2; echo "Making all in ExSorting"; tput sgr 0
	$(MAKE) -C ExSorting

ExDictionaries: lib/libcontainers_hash.a lib/libcontainers_rb.a lib/libcontainers_st.a
	tput bold; tput setaf 2; echo "Making all in ExDictionaries"; tput sgr 0
	$(MAKE) -C ExDictionaries

ExGraphs: lib/libcontainers_hash.a lib/libcontainers_rb.a lib/libcontainers_st.a
	tput bold; tput setaf 2; echo "Making all in ExGraphs"; tput sgr 0
	$(MAKE) -C ExGraphs

ExDynamicProgramming: lib/libcontainers_hash.a
	tput bold; tput setaf 2; echo "Making all in ExDynamicProgramming"; tput sgr 0
	$(MAKE) -C ExDynamicProgramming


tests: test_binaries
	$(call exec, bin/iterator_tests)
	$(call exec, bin/sorting_tests)
	$(call exec, bin/dictionary_tests_st)
	$(call exec, bin/dictionary_tests_hash)
	$(call exec, bin/dictionary_tests_rb)
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

test_binaries: build lib bin bin/sorting_tests bin/dictionary_tests_st bin/dictionary_tests_hash bin/dictionary_tests_rb bin/graph_tests bin/list_tests bin/array_tests bin/array_alt_tests bin/errors_tests bin/union_find_tests bin/queue_tests bin/priority_queue_tests bin/iterator_tests bin/multy_way_tree_tests bin/editing_distance_tests

bin/editing_distance_tests: tests/editing_distance_tests.c include/editing_distance.h lib/libcontainers_st.a $(HEADERS)
	$(CC) $(CFLAGS) tests/editing_distance_tests.c -o bin/editing_distance_tests -lcontainers_st $(LDFLAGS)

bin/iterator_tests: tests/iterator_tests.c include/unit_testing.h lib/libcontainers_st.a $(HEADERS)
	$(CC) $(CFLAGS) tests/iterator_tests.c -o bin/iterator_tests -lcontainers_st $(LDFLAGS)

bin/sorting_tests: tests/sorting_tests.c include/unit_testing.h lib/libcontainers_st.a $(HEADERS)
	$(CC) $(CFLAGS) tests/sorting_tests.c -o bin/sorting_tests -lcontainers_st $(LDFLAGS)

bin/dictionary_tests_st: tests/dictionary_tests.c include/unit_testing.h lib/libcontainers_st.a $(HEADERS)
	$(CC) $(CFLAGS) tests/dictionary_tests.c -o bin/dictionary_tests_st -lcontainers_st $(LDFLAGS)

bin/dictionary_tests_hash: tests/dictionary_tests.c include/unit_testing.h lib/libcontainers_hash.a $(HEADERS)
	$(CC) $(CFLAGS) tests/dictionary_tests.c -o bin/dictionary_tests_hash -lcontainers_hash $(LDFLAGS)

bin/dictionary_tests_rb: tests/dictionary_tests_rb.c include/unit_testing.h lib/libcontainers_rb.a $(HEADERS)
	$(CC) $(CFLAGS) tests/dictionary_tests_rb.c -o bin/dictionary_tests_rb -lcontainers_rb $(LDFLAGS)

bin/graph_tests: tests/graph_tests.c include/unit_testing.h lib/libcontainers_hash.a $(HEADERS)
	$(CC) $(CFLAGS) tests/graph_tests.c -o bin/graph_tests -lcontainers_st $(LDFLAGS)

bin/list_tests: tests/list_tests.c include/unit_testing.h lib/libcontainers_hash.a $(HEADERS)
	$(CC) $(CFLAGS) tests/list_tests.c -o bin/list_tests -lcontainers_hash $(LDFLAGS)

bin/array_tests: tests/array_tests.c include/unit_testing.h lib/libcontainers_st.a $(HEADERS)
	$(CC) $(CFLAGS) tests/array_tests.c -o bin/array_tests -lcontainers_st $(LDFLAGS)

bin/array_alt_tests: tests/array_alt_tests.c include/unit_testing.h lib/libcontainers_st.a $(HEADERS)
	$(CC) $(CFLAGS) tests/array_alt_tests.c -o bin/array_alt_tests -lcontainers_st $(LDFLAGS)

bin/errors_tests: tests/errors_tests.c include/unit_testing.h lib/libcontainers_st.a $(HEADERS)
	$(CC) $(CFLAGS) tests/errors_tests.c -o bin/errors_tests -lcontainers_st $(LDFLAGS)

bin/union_find_tests: tests/union_find_tests.c include/unit_testing.h lib/libcontainers_st.a $(HEADERS)
	$(CC) $(CFLAGS) tests/union_find_tests.c -o bin/union_find_tests -lcontainers_st $(LDFLAGS)

bin/queue_tests: tests/queue_tests.c include/unit_testing.h lib/libcontainers_st.a $(HEADERS)
	$(CC) $(CFLAGS) tests/queue_tests.c -o bin/queue_tests -lcontainers_st $(LDFLAGS)

bin/priority_queue_tests: tests/priority_queue_tests.c include/unit_testing.h lib/libcontainers_st.a $(HEADERS)
	$(CC) $(CFLAGS) tests/priority_queue_tests.c -o bin/priority_queue_tests -lcontainers_st $(LDFLAGS)

bin/multy_way_tree_tests: tests/multy_way_tree_tests.c include/unit_testing.h lib/libcontainers_st.a $(HEADERS)
	$(CC) $(CFLAGS) tests/multy_way_tree_tests.c -o bin/multy_way_tree_tests -lcontainers_st $(LDFLAGS)


include Makefile.exps
