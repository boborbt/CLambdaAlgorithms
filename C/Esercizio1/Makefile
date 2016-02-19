# CFLAGS=-g -O0
CFLAGS=-Os

all: tests measure_times

clean:
	$(RM) tests *.o

tests: tests.c quick_sort.h quick_sort.o
	$(CC) $(CFLAGS) -o tests tests.c quick_sort.o

quick_sort.o: quick_sort.h quick_sort.c
	$(CC) $(CFLAGS) -c quick_sort.c

dataset.o: dataset.c dataset.h
	$(CC) $(CFLAGS) -c dataset.c

measure_times: measure_times.c quick_sort.h quick_sort.o dataset.h dataset.o
	$(CC) $(CFLAGS) -o measure_times measure_times.c quick_sort.o dataset.o
