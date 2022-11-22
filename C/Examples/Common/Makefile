include ../../Makefile.vars

all: lib lib/libexcommon.a

clean:
	$(RM) -f lib/*

CFLAGS += -I../../include -Iinclude


lib:
	@mkdir lib

lib/libexcommon.a: src/exsorting_dataset.c include/exsorting_dataset.h
	$(CC) $(CFLAGS) -c src/exsorting_dataset.c -o build/exsorting_dataset.o
	$(LIBTOOL) lib/libexcommon.a build/*.o