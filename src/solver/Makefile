CC = gcc

CFLAGS = -Wall -Wextra -std=c99 -O2
SRC = main.c sudoku.c

OBJ = $(SRC:.c=.o)

all : main

main: $(OBJ)

.PHONY: clean
clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} main     # remove main program