include ../Makefile.vars
CFLAGS+=-I../include
LDFLAGS+=-L../lib

.PHONY: clean all tests

all: bin build  bin/measure_times bin/create_multy_way_trees bin/multy_way_tree_main

bin:
	@mkdir bin

build:
	@mkdir build

clean:
	@$(RM) -rf build
	@$(RM) -rf bin


bin/measure_times: src/measure_times.c ../include/keys.h  ../include/stack.h ../lib/libcontainers.a Makefile ../Makefile.vars
	@$(CC) $(CFLAGS) -o bin/measure_times src/measure_times.c  -lcontainers $(LDFLAGS)

bin/create_multy_way_trees: src/create_multy_way_trees.c src/multywaytree_utils.c src/multywaytree_utils.h ../include/multy_way_tree.h  ../include/array.h ../lib/libcontainers.a Makefile ../Makefile.vars
	@$(CC) $(CFLAGS) -o bin/create_multy_way_trees src/create_multy_way_trees.c src/multywaytree_utils.c -lcontainers $(LDFLAGS)

bin/multy_way_tree_main: src/multy_way_tree_main.c src/multywaytree_utils.c src/multywaytree_utils.h ../include/multy_way_tree.h  ../include/array.h ../lib/libcontainers.a Makefile ../Makefile.vars
		@$(CC) $(CFLAGS) -o bin/multy_way_tree_main src/multy_way_tree_main.c src/multywaytree_utils.c  -lcontainers $(LDFLAGS)
