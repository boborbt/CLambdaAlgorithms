include ../../Makefile.vars

BASEDIR=../..
CFLAGS+=-I$(BASEDIR)/include
LDFLAGS+=-L$(BASEDIR)/lib

.PHONY: all clean

all: bin bin/measure_times bin/dump_graph bin/oc

bin:
	mkdir bin

clean:
	@$(RM) -rf build
	@$(RM) -rf bin


bin/measure_times: src/measure_times.c $(BASEDIR)/include/dijkstra.h $(BASEDIR)/include/graph.h $(BASEDIR)/lib/libcontainers.a $(BASEDIR)/Makefile.vars Makefile
	@$(CC) $(CFLAGS) -o bin/measure_times src/measure_times.c -lcontainers $(LDFLAGS)

bin/dump_graph: src/dump_graph.c $(BASEDIR)/include/graph.h $(BASEDIR)/include/iterator_functions.h $(BASEDIR)/lib/libcontainers.a $(BASEDIR)/Makefile.vars Makefile
	@$(CC) $(CFLAGS) -o bin/dump_graph src/dump_graph.c -lcontainers $(LDFLAGS)

bin/oc: src/optimal_connectivity.c $(BASEDIR)/Makefile.vars Makefile
	@$(CC) $(CFLAGS) -o bin/oc -Wl,-stack_size -Wl,0x1000000 src/optimal_connectivity.c $(LDFLAGS)
