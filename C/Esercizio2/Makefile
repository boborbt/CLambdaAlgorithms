include ../Makefile.vars
CFLAGS+=-I../include
LDFLAGS+=-L../lib

.PHONY: clean all tests

all:  bin/measure_times_st bin/measure_times_hash bin/measure_times_rb

clean:
	$(RM) -rf build/*
	$(RM) -rf bin/*


bin/measure_times_st: src/measure_times.c ../include/keys.h  ../include/stack.h ../lib/libcontainers_st.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/measure_times_st src/measure_times.c  -lcontainers_st $(LDFLAGS)

bin/measure_times_hash: src/measure_times.c ../include/keys.h  ../lib/libcontainers_hash.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/measure_times_hash src/measure_times.c -lcontainers_hash $(LDFLAGS)

bin/measure_times_rb: src/measure_times.c ../include/keys.h  ../lib/libcontainers_rb.a Makefile ../Makefile.vars
	$(CC) $(CFLAGS) -o bin/measure_times_rb src/measure_times.c -lcontainers_rb $(LDFLAGS)
