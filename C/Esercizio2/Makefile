include ../Makefile.vars
CFLAGS+=-I../include
LDFLAGS+=-L../lib

.PHONY: clean all tests

all: bin/tests_st bin/measure_times_st bin/tests_hash bin/measure_times_hash

clean:
	$(RM) -rf build/*
	$(RM) -rf bin/*

tests: bin/tests_st bin/tests_hash
	bin/tests_st
	bin/tests_hash

bin/tests_st: src/tests.c ../include/dictionary.h ../include/key_info.h  ../lib/libcontainers_st.a
	$(CC) $(CFLAGS) -o bin/tests_st src/tests.c -lcontainers_st $(LDFLAGS)

bin/measure_times_st: src/measure_times.c ../include/key_info.h  ../lib/libcontainers_st.a
	$(CC) $(CFLAGS) -o bin/measure_times_st src/measure_times.c  -lcontainers_st $(LDFLAGS)

bin/tests_hash: src/tests.c ../include/dictionary.h ../include/key_info.h ../lib/libcontainers_hash.a
	$(CC) $(CFLAGS) -o bin/tests_hash src/tests.c -lcontainers_hash $(LDFLAGS)

bin/measure_times_hash: src/measure_times.c ../include/key_info.h  ../lib/libcontainers_hash.a
	$(CC) $(CFLAGS) -o bin/measure_times_hash src/measure_times.c -lcontainers_hash $(LDFLAGS)
