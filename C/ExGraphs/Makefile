include ../Makefile.vars
CFLAGS+=-I../include
LDFLAGS+=-L../lib

.PHONY: all clean

all: bin bin/measure_times bin/dump_graph

bin:
	mkdir bin

clean:
	@$(RM) -rf build
	@$(RM) -rf bin


bin/measure_times: src/measure_times.c ../include/dijkstra.h ../include/graph.h ../lib/libcontainers.a ../Makefile.vars Makefile
	@$(CC) $(CFLAGS) -o bin/measure_times src/measure_times.c -lcontainers $(LDFLAGS)

bin/dump_graph: src/dump_graph.c ../include/graph.h ../include/iterator_functions.h ../lib/libcontainers.a ../Makefile.vars Makefile
	@$(CC) $(CFLAGS) -o bin/dump_graph src/dump_graph.c -lcontainers $(LDFLAGS)
