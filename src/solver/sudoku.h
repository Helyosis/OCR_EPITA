#include <stdio.h>
#ifndef SUDOKU_H
#define SUDOKU_H
int AlreadyInSquare(char grid[], size_t cols, int x, int y, int val);
int AlreadyInColumn(char grid[], size_t rows, size_t cols, int x, int val);
int Solve(char grid[], size_t rows, size_t cols);
int AlreadyInLine(char grid[], size_t cols, int y, int val);
#endif