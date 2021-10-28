#include <stdio.h>
#include <err.h>

//IsValid checks if the sudoku grid is valid
//@param grid the sudoku grid
//@return 1 if the grid is valid, 0 if not
int IsValid(char board[], size_t rows,size_t cols) {
    size_t i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if ((board[i * cols + j] > '9' || board[i * cols + j] < '0') && board[i * cols + j] != '.') {
	            return 0;
            }
        }
    }
    return 1;
}

//Clean Will replace all the dots by some 0
//@param grid the grid of the sudoku
//@return void

void Clean(char board[], size_t rows, size_t cols){
  if(!IsValid(board,rows,cols)){
    errx(1,"Invalid GRID\n");
  }
  size_t i,j;
  for (i = 0; i < rows; ++i) {
    for (j = 0; j < cols; ++j) {
      if (board[i*cols+j] == '.') {
	    board[i*cols+j] = '0';
      }
    }
  }
  return;
  
}

//AlreadyInColumn checks if the number is already in the column
//@param grid the sudoku grid
//@param x the column to check
//@param val the number to check
//@return 1 if the number is already in the column, 0 if not

int AlreadyInColumn(char grid[], size_t rows, size_t cols, int x, int val){
    int result = 1;
    size_t i = 0;
    while (i < rows && result) {
        if (grid[x * cols + i] == val+'0')
        {
            result = 0;
        }
        i++;
    }
    return !result;
}


//AlreadyInLine checks if the number is already in the line
//@param grid the sudoku grid
//@param y the line to check
//@param val the number to check
//@return 1 if the number is already in the line, 0 if not
int AlreadyInLine(char grid[], size_t cols, int y, int val){
    int result = 1;
    size_t i = 0;
    while (i < cols && result) {
        if (grid[i * cols + y] == val + '0')
        {
            result = 0;
        }
        i++;
    }
    return !result;
}


//AlreadyInSquare checks if the number is already in the square
//@param grid the sudoku grid
//@param x the x coordinate of the square
//@param y the y coordinate of the square
//@param val the number to check
//@return 1 if the number is already in the square, 0 if not
int AlreadyInSquare(char grid[], size_t cols, int x, int y, int val){
    int result = 1;
    int i = x-x%3;
    int j;
    while (i < x+3 - x%3 && result) {
        j = y-y%3;
        while (j < y+3 - y%3 && result) {
            if (grid[i * cols + j] == val + '0')
            {
                result = 0;
            }
            j++;
        }
        i++;
    }
    return !result;
}

//SolveRec solves the sudoku grid recursively
//@param grid the sudoku grid
//@param x the x coordinate of the square
//@param y the y coordinate of the square
//@return 1 if the grid is solved, 0 if not
int SolveRec(char grid[], size_t rows, size_t cols, int x, int y) {
    if (x > (int)rows -1){
        x = 0;
        y++;
    }
    if (y > (int)cols -1){
        return 1;
    }
    if (grid[x * cols + y] == '.' || grid[x*cols +y] == '0'){
        for (size_t i = 1; i < rows+1; i++){
            if (!AlreadyInColumn(grid, rows, cols, x, i) && !AlreadyInLine(grid, cols, y, i) && !AlreadyInSquare(grid, cols, x, y, i)){
	      grid[x * cols + y] = (char)(i + '0');
                int result = SolveRec(grid, rows, cols, x+1, y);
                if (!result){
                    grid[x * cols + y] = '0';
                }
                else {
                    return 1;
                }
            }
        }
    }
    else {
        return SolveRec(grid, rows, cols, x+1, y);
    }
    return 0;
}

//Solve solves the sudoku grid
//@param grid the sudoku grid
//@return 1 if the grid is solved, 0 if not
int Solve(char grid[], size_t rows, size_t cols) {
    Clean(grid,rows,cols);
    return SolveRec(grid, rows, cols, 0, 0);
}

//PrintGrid prints the sudoku grid
//@param grid the sudoku grid
void PrintGrid(char grid[], size_t rows, size_t cols) {
    char separator[] = "-------------------------";
    for (size_t i = 0; i < rows; i++) {
        if(i % 3 == 0) {
            printf("\n");
           printf("%s", separator);
        }
        printf("\n");
        for (size_t j = 0; j < cols; j++) {
            if(j % 3 == 0) {
                printf("| ");
            }
            if (j == cols - 1) 
            {
                printf("%c ", grid[i * cols + j]);
            }
            else {
                printf("%c ", grid[i * cols + j]);
            }
        }
        printf("|");
    }
    printf("\n%s\n", separator);
}