CC = gcc 
DEPS = file_rw.h matrix.h matrix_ops.h parse_tree.h prompt.h var_list.h tests.h
OBJS = linear_algebra.o file_rw.o matrix.o matrix_ops.o parse_tree.o prompt.o var_list.o tests.o
LIBS = -lm
DEBUG = -g
CSTAND = -std=c99
CFLAGS = $(CSTAND) -Wall -c $(DEBUG)
LFAGS = -Wall $(DEBUG)

linear_algebra: $(OBJS)
	$(CC) $(LFAGS) $(OBJS) -o linear_algebra $(LIBS)

linear_algebra.o: $(DEPS)
	$(CC) $(CFLAGS) linear_algebra.c

file_rw.o: $(DEPS)
	$(CC) $(CFLAGS) file_rw.c

matrix.o: $(DEPS)
	$(CC) $(CFLAGS) matrix.c

matrix_ops.o: $(DEPS)
	$(CC) $(CFLAGS) matrix_ops.c

parse_tree.o: $(DEPS)
	$(CC) $(CFLAGS) parse_tree.c

prompt.o: $(DEPS)
	$(CC) $(CFLAGS) prompt.c

var_list.o: $(DEPS)
	$(CC) $(CFLAGS) var_list.c

tests.o: $(DEPS)
	$(CC) $(CFLAGS) tests.c

.PHONY: clean

clean:
	-rm -rf *.o linear_algebra
