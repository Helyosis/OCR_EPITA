#include <stdio.h>
#ifndef SUDOKU_H
#define SUDOKU_H

int IsValid(char grid[], size_t rows, size_t cols);

void Solve(char grid[], size_t rows, size_t cols);

void PrintGrid(char grid[], size_t rows, size_t cols);
#endif