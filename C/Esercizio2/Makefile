include ../Makefile.vars
CFLAGS+=-I../include
LDFLAGS+=-L../lib

.PHONY: clean all tests

all:  bin/measure_times_st bin/measure_times_hash

clean:
	$(RM) -rf build/*
	$(RM) -rf bin/*


bin/measure_times_st: src/measure_times.c ../include/key_info.h  ../include/stack.h ../lib/libcontainers_st.a
	$(CC) $(CFLAGS) -o bin/measure_times_st src/measure_times.c  -lcontainers_st $(LDFLAGS)

bin/measure_times_hash: src/measure_times.c ../include/key_info.h  ../lib/libcontainers_hash.a
	$(CC) $(CFLAGS) -o bin/measure_times_hash src/measure_times.c -lcontainers_hash $(LDFLAGS)
