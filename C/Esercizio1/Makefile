include ../Makefile.vars
CFLAGS+=-I../include
LDFLAGS+=-L../lib


all: bin bin/measure_times bin/test_read bin/array_comparison

bin:
	mkdir bin

clean:
	$(RM) -rf bin


bin/measure_times: src/measure_times.c ../include/heap_sort.h ../include/quick_sort.h ../include/insertion_sort.h ../include/dataset.h  ../lib/libcontainers_st.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/measure_times src/measure_times.c -lcontainers_st $(LDFLAGS)

bin/test_read: src/test_read.c ../include/dataset.h ../lib/libcontainers_st.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/test_read src/test_read.c -lcontainers_st $(LDFLAGS)

bin/array_comparison: src/array_comparison.c ../lib/libcontainers_st.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/array_comparison src/array_comparison.c -lcontainers_st $(LDFLAGS)
