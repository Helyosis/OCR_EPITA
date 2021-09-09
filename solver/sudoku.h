#include <stdio.h>
#ifndef SUDOKU_H
#define SUDOKU_H

int IsValid(char grid[], size_t rows, size_t cols);

int IsColumnSolved(char grid[], size_t rows, size_t cols, int x);
int IsLineSolved(char grid[], size_t rows, size_t cols, int y);
int IsSquareSolved(char grid[], size_t rows, size_t cols, int x, int y);
int IsSolved(char grid[], size_t rows, size_t cols);


int AlreadyInColumn(char grid[], size_t rows, size_t cols, int x, int val);
int AlreadyInLine(char grid[], size_t rows, size_t cols, int y, int val);
int AlreadyInSquare(char grid[], size_t rows, size_t cols, int x, int y, int val);


int SolveRec(char grid[], size_t rows, size_t cols,int x , int y);
void Solve(char grid[], size_t rows, size_t cols);

#endif