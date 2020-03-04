include ../Makefile.vars

CFLAGS+=-I../include
LDFLAGS+=-L../lib


all: bin bin/measure_times bin/test_read bin/array_comparison bin/sorting-17-18 bin/list-sort-18-19

bin:
	mkdir bin

clean:
	$(RM) -rf bin


bin/sorting-17-18: src/sorting-17-18.c ../include/array.h ../include/mem.h ../include/print_time.h ../lib/libcontainers.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/sorting-17-18 src/sorting-17-18.c -lcontainers $(LDFLAGS)

bin/list-sort-18-19: src/list-sort-18-19.c ../include/list.h ../include/mem.h ../include/print_time.h ../lib/libcontainers.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/list-sort-18-19 src/list-sort-18-19.c -lcontainers $(LDFLAGS)


bin/measure_times: src/measure_times.c ../include/heap_sort.h ../include/quick_sort.h ../include/insertion_sort.h ../include/dataset.h  ../lib/libcontainers.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/measure_times src/measure_times.c src/exsorting_dataset.c -lcontainers $(LDFLAGS)

bin/test_read: src/test_read.c ../include/dataset.h ../lib/libcontainers.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/test_read src/test_read.c -lcontainers $(LDFLAGS)

bin/array_comparison: src/array_comparison.c ../lib/libcontainers.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/array_comparison src/array_comparison.c -lcontainers $(LDFLAGS)
