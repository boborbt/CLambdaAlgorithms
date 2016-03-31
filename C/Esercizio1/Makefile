include ../Makefile.vars
CFLAGS+=-I../include
LDFLAGS+=-L../lib


all: bin/tests bin/measure_times bin/test_read

tests: bin/tests
	bin/tests

clean:
	$(RM) -rf bin/*
	$(RM) -rf build/*

bin/tests: src/tests.c src/quick_sort.h src/merge_sort.h src/heap_sort.h build/quick_sort.o build/merge_sort.o build/heap_sort.o build/insertion_sort.o build/array_g.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/tests src/tests.c build/quick_sort.o build/merge_sort.o build/heap_sort.o build/insertion_sort.o build/array_g.o ../build/unit_testing.o

build/array_g.o: src/array_g.h src/array_g.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o build/array_g.o src/array_g.c

build/insertion_sort.o: src/insertion_sort.h src/insertion_sort.c
	$(CC) $(CFLAGS) -c src/insertion_sort.c -o build/insertion_sort.o

build/quick_sort.o: src/quick_sort.h src/quick_sort.c
	$(CC) $(CFLAGS) -c src/quick_sort.c -o build/quick_sort.o


build/heap_sort.o: src/heap_sort.h src/heap_sort.c
	$(CC) $(CFLAGS) -c src/heap_sort.c -o build/heap_sort.o


build/merge_sort.o: src/merge_sort.h src/merge_sort.c
	$(CC) $(CFLAGS) -c src/merge_sort.c -o build/merge_sort.o

bin/measure_times: src/measure_times.c src/heap_sort.h src/quick_sort.h src/insertion_sort.h ../include/dataset.h build/quick_sort.o build/heap_sort.o build/array_g.o ../lib/libcontainers_st.a build/merge_sort.o build/insertion_sort.o
	$(CC) $(CFLAGS) -o bin/measure_times src/measure_times.c build/quick_sort.o build/merge_sort.o build/heap_sort.o build/insertion_sort.o build/array_g.o -lcontainers_st $(LDFLAGS)

bin/test_read: src/test_read.c ../include/dataset.h ../lib/libcontainers_st.a
	$(CC) $(CFLAGS) -o bin/test_read src/test_read.c -lcontainers_st $(LDFLAGS)
