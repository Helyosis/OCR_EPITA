CC = gcc

CFLAGS = -Wall -Wextra -std=c99 -O2 --pedantic-errors
SRC = main.c sudoku.c

OBJ = $(SRC:.c=.o)
PRG= sudoku_solver
all : main

main: $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) -o $(PRG)

.PHONY: clean
clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} ${PRG}     # remove main program