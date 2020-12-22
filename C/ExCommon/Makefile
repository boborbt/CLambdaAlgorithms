include ../Makefile.vars

all: bin build lib lib/libexcommon.a

clean:
	$(RM) -f lib/*
	$(RM) -f bin/*

CFLAGS += -I../include -Iinclude

bin:
	@mkdir bin

lib:
	@mkdir lib

build:
	@mkdir build

lib/libexcommon.a: src/exsorting_dataset.c include/exsorting_dataset.h
	$(CC) $(CFLAGS) -c src/exsorting_dataset.c -o build/exsorting_dataset.o
	$(LIBTOOL) lib/libexcommon.a build/*.o