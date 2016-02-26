
CFLAGS=-I./include -O0 -g

.PHONY: Esercizio1 Esercizio2 Esercizio3 lib clean

all: lib Esercizio1 Esercizio2 Esercizio3

clean:
	$(MAKE) -C Esercizio1 clean
	$(MAKE) -C Esercizio2 clean
	$(MAKE) -C Esercizio3 clean
	$(RM) -f lib/*.o

lib: lib/dataset.o

lib/dataset.o: src/dataset.c include/dataset.h
	$(CC) $(CFLAGS) -c src/dataset.c -o lib/dataset.o

Esercizio1:
	$(MAKE) -C Esercizio1

Esercizio2:
	$(MAKE) -C Esercizio2

Esercizio3:
	$(MAKE) -C Esercizio3


tests:
	$(MAKE) -C Esercizio1 tests
	$(MAKE) -C Esercizio2 tests
	$(MAKE) -C Esercizio3 tests
