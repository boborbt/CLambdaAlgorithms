include ../../Makefile.vars

BASEDIR=../..
CFLAGS+=-I$(BASEDIR)/include -I../Common/include
LDFLAGS+=-L$(BASEDIR)/lib -L../Common/lib

.PHONY: clean all tests

all: bin build  bin/measure_times bin/create_multy_way_trees bin/multy_way_tree_main bin/measure_times2

bin:
	@mkdir bin

build:
	@mkdir build

clean:
	@$(RM) -rf build
	@$(RM) -rf bin


bin/measure_times: src/measure_times.c $(BASEDIR)/include/keys.h  $(BASEDIR)/include/stack.h $(BASEDIR)/lib/libcontainers.a Makefile $(BASEDIR)/Makefile.vars
	@$(CC) $(CFLAGS) -o bin/measure_times src/measure_times.c  -lcontainers -lexcommon $(LDFLAGS)

bin/measure_times2: src/measure_times2.c $(BASEDIR)/include/keys.h  $(BASEDIR)/include/array.h $(BASEDIR)/include/dictionary.h $(BASEDIR)/lib/libcontainers.a Makefile $(BASEDIR)/Makefile.vars
	@$(CC) $(CFLAGS) -o bin/measure_times2 src/measure_times2.c  -lcontainers $(LDFLAGS)

bin/create_multy_way_trees: src/create_multy_way_trees.c src/multywaytree_utils.c src/multywaytree_utils.h $(BASEDIR)/include/multy_way_tree.h  $(BASEDIR)/include/array.h $(BASEDIR)/lib/libcontainers.a Makefile $(BASEDIR)/Makefile.vars
	@$(CC) $(CFLAGS) -o bin/create_multy_way_trees src/create_multy_way_trees.c src/multywaytree_utils.c -lcontainers $(LDFLAGS)

bin/multy_way_tree_main: src/multy_way_tree_main.c src/multywaytree_utils.c src/multywaytree_utils.h $(BASEDIR)/include/multy_way_tree.h  $(BASEDIR)/include/array.h $(BASEDIR)/lib/libcontainers.a Makefile $(BASEDIR)/Makefile.vars
		@$(CC) $(CFLAGS) -o bin/multy_way_tree_main src/multy_way_tree_main.c src/multywaytree_utils.c  -lcontainers $(LDFLAGS)
