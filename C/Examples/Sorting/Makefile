include ../../Makefile.vars

BASEDIR=../..
CFLAGS+=-I$(BASEDIR)/include -I../Common/include
LDFLAGS+=-L$(BASEDIR)/lib -L../Common/lib


all: bin bin/measure_times bin/test_read bin/array_comparison bin/sorting-17-18 bin/list-sort-18-19

bin:
	mkdir bin

clean:
	$(RM) -rf bin


bin/sorting-17-18: src/sorting-17-18.c $(BASEDIR)/include/array.h $(BASEDIR)/include/mem.h $(BASEDIR)/include/print_time.h $(BASEDIR)/lib/libcontainers.a Makefile $(BASEDIR)/Makefile.vars
	$(CC) $(CFLAGS) -o bin/sorting-17-18 src/sorting-17-18.c -lcontainers $(LDFLAGS)

bin/list-sort-18-19: src/list-sort-18-19.c $(BASEDIR)/include/list.h $(BASEDIR)/include/mem.h $(BASEDIR)/include/print_time.h $(BASEDIR)/lib/libcontainers.a Makefile $(BASEDIR)/Makefile.vars
	$(CC) $(CFLAGS) -o bin/list-sort-18-19 src/list-sort-18-19.c -lcontainers  $(LDFLAGS)


bin/measure_times: src/measure_times.c $(BASEDIR)/include/heap_sort.h $(BASEDIR)/include/quick_sort.h $(BASEDIR)/include/insertion_sort.h ../Common/include/exsorting_dataset.h  $(BASEDIR)/lib/libcontainers.a Makefile $(BASEDIR)/Makefile.vars
	$(CC) $(CFLAGS) -o bin/measure_times src/measure_times.c -lcontainers -lexcommon $(LDFLAGS)

bin/test_read: src/test_read.c $(BASEDIR)/include/dataset.h $(BASEDIR)/lib/libcontainers.a Makefile $(BASEDIR)/Makefile.vars
	$(CC) $(CFLAGS) -o bin/test_read src/test_read.c -lcontainers -lexcommon $(LDFLAGS)

bin/array_comparison: src/array_comparison.c $(BASEDIR)/lib/libcontainers.a Makefile $(BASEDIR)/Makefile.vars
	$(CC) $(CFLAGS) -o bin/array_comparison src/array_comparison.c -lcontainers $(LDFLAGS)
