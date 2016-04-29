include ../Makefile.vars
CFLAGS+=-I../include
LDFLAGS+=-L../lib

.PHONY: all clean

all: bin/measure_times

clean:
	$(RM) -rf build/*
	$(RM) -rf bin/*


bin/measure_times: src/measure_times.c ../include/dijkstra.h ../include/graph.h ../lib/libcontainers_hash.a
	$(CC) $(CFLAGS) -o bin/measure_times src/measure_times.c -lcontainers_hash $(LDFLAGS)
