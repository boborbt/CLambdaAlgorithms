CFLAGS=-O3 -I../include
# CFLAGS=-g -O0 -I../include

all: bin/tests bin/measure_times

clean:
	$(RM) -rf bin/*
	$(RM) -rf build/*

bin/tests: src/tests.c src/quick_sort.h src/merge_sort.h src/heap_sort.h build/quick_sort.o build/merge_sort.o build/heap_sort.o
	$(CC) $(CFLAGS) -o bin/tests src/tests.c build/quick_sort.o build/merge_sort.o build/heap_sort.o

build/quick_sort.o: src/quick_sort.h src/quick_sort.c
	$(CC) $(CFLAGS) -c src/quick_sort.c -o build/quick_sort.o


build/heap_sort.o: src/heap_sort.h src/heap_sort.c
	$(CC) $(CFLAGS) -c src/heap_sort.c -o build/heap_sort.o


build/merge_sort.o: src/merge_sort.h src/merge_sort.c
	$(CC) $(CFLAGS) -c src/merge_sort.c -o build/merge_sort.o

bin/measure_times: src/measure_times.c src/heap_sort.h src/quick_sort.h ../include/dataset.h build/quick_sort.o build/heap_sort.o ../lib/dataset.o build/merge_sort.o
	$(CC) $(CFLAGS) -o bin/measure_times src/measure_times.c build/quick_sort.o ../lib/dataset.o build/merge_sort.o build/heap_sort.o
