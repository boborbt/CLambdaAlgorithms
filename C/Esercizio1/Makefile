include ../Makefile.vars
CFLAGS+=-I../include
LDFLAGS+=-L../lib


all: bin/measure_times bin/test_read

clean:
	$(RM) -rf bin/*
	$(RM) -rf build/*


bin/measure_times: src/measure_times.c ../include/heap_sort.h ../include/quick_sort.h ../include/insertion_sort.h ../include/dataset.h  ../lib/libcontainers_st.a
	$(CC) $(CFLAGS) -o bin/measure_times src/measure_times.c -lcontainers_st $(LDFLAGS)

bin/test_read: src/test_read.c ../include/dataset.h ../lib/libcontainers_st.a
	$(CC) $(CFLAGS) -o bin/test_read src/test_read.c -lcontainers_st $(LDFLAGS)
