# This is a c++ code. Using g++ compiler.
CC=g++

# Compiler flags.
CFLAGS=-c

# make all runs atpg.
all: atpg

# make atpg
atpg: atpg.o
	$(CC) atpg.o -o atpg

atpg.o: atpg.cc
	$(CC) $(CFLAGS) atpg.cc

# make clean
clean:
	rm -rf *o atpg

