
CFLAGS=-I./include -O0 -g

.PHONY: Esercizio1 Esercizio2 lib clean

all: lib Esercizio1 Esercizio2

clean:
	$(MAKE) -C Esercizio1 clean
	$(RM) -f lib/*.o

lib: lib/dataset.o

lib/dataset.o: src/dataset.c include/dataset.h
	$(CC) $(CFLAGS) -c src/dataset.c -o lib/dataset.o

Esercizio1:
	$(MAKE) -C Esercizio1

Esercizio2:
	$(MAKE) -C Esercizio2

tests:
	$(MAKE) -C Esercizio1 tests
	$(MAKE) -C Esercizio2 tests
