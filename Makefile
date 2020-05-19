##Adapted from http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

##Gets a list of source files from /src
src = $(wildcard src/*.c)

CC=gcc
CFLAGS= -g -Wall -std=gnu99 -I -O3.
DEPS = $(src:.h)
OBJ = $(src:.c=.o)
EXE = scheduler

##Create .o files from .c files. Searches for .c files with same .o names given in OBJ
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

##Create executable linked file from object files.
$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

##Delete object files
clean:
	/bin/rm $(OBJ)

##Performs clean (i.e. delete object files) and deletes executable
clobber: clean
	/bin/rm $(EXE)