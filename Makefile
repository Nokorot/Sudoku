# Config
PRGNAME = sudoku

CC = gcc
CFLAGS = -g3 -O0 -Wall -pedantic
# LDFLAGS = 

# Files
SRC = sudoku.c utils.c grid_print.c
OBJ = .$(SRC:.c=.o)

# Rules
.PHONY: all, clean

all: sudoku

sudoku: $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $(OBJ)

.sudoku.o: sudoku.c utils.h
	$(CC) -c $(CFLAGS) -o $@ $<

.utils.o: utils.c utils.h
	$(CC) -c $(CFLAGS) -o $@ $<

.grid_print.o: grid_print.c grid_print.h
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(PRGNAME) $(OBJ)
