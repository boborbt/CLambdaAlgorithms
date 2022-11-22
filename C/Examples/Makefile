include ../Makefile.vars

BASEDIR=..
.PHONY: Common Sorting Dictionaries Graphs DynamicProgramming

all: Common Sorting Dictionaries Graphs DynamicProgramming

clean:
	$(MAKE) -C Common clean
	$(MAKE) -C Sorting clean
	$(MAKE) -C Dictionaries clean
	$(MAKE) -C Graphs clean
	$(MAKE) -C DynamicProgramming clean
	

# Experiments

Common:
	tput bold; tput setaf 2; echo "Making all in Common"; tput sgr 0
	$(MAKE) -C Common

Sorting:  $(BASEDIR)/lib/libcontainers.a
	tput bold; tput setaf 2; echo "Making all in Sorting"; tput sgr 0
	$(MAKE) -C Sorting

Dictionaries: $(BASEDIR)/lib/libcontainers.a
	tput bold; tput setaf 2; echo "Making all in Dictionaries"; tput sgr 0
	$(MAKE) -C Dictionaries

Graphs: $(BASEDIR)/lib/libcontainers.a
	tput bold; tput setaf 2; echo "Making all in Graphs"; tput sgr 0
	$(MAKE) -C Graphs

DynamicProgramming: $(BASEDIR)/lib/libcontainers.a
	tput bold; tput setaf 2; echo "Making all in DynamicProgramming"; tput sgr 0
	$(MAKE) -C DynamicProgramming
