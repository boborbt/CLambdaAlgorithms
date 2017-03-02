include ../Makefile.vars
CFLAGS+=-I../include
LDFLAGS+=-L../lib

.PHONY: clean all tests

all: bin build  bin/measure_times_st bin/measure_times_hash bin/measure_times_rb bin/create_multy_way_trees bin/multy_way_tree_main

bin:
	mkdir bin

build:
	mkdir build

clean:
	$(RM) -rf build
	$(RM) -rf bin


bin/measure_times_st: src/measure_times.c ../include/keys.h  ../include/stack.h ../lib/libcontainers_st.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/measure_times_st src/measure_times.c  -lcontainers_st $(LDFLAGS)

bin/measure_times_hash: src/measure_times.c ../include/keys.h  ../lib/libcontainers_hash.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/measure_times_hash src/measure_times.c -lcontainers_hash $(LDFLAGS)

bin/measure_times_rb: src/measure_times.c ../include/keys.h  ../lib/libcontainers_rb.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/measure_times_rb src/measure_times.c -lcontainers_rb $(LDFLAGS)

bin/create_multy_way_trees: src/create_multy_way_trees.c src/multywaytree_utils.c src/multywaytree_utils.h ../include/multy_way_tree.h  ../include/array.h ../lib/libcontainers_rb.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/create_multy_way_trees src/create_multy_way_trees.c src/multywaytree_utils.c -lcontainers_rb $(LDFLAGS)

bin/multy_way_tree_main: src/multy_way_tree_main.c src/multywaytree_utils.c src/multywaytree_utils.h ../include/multy_way_tree.h  ../include/array.h ../lib/libcontainers_rb.a Makefile ../Makefile.vars
		$(CC) $(CFLAGS) -o bin/multy_way_tree_main src/multy_way_tree_main.c src/multywaytree_utils.c  -lcontainers_rb $(LDFLAGS)
