#include <stdio.h>
#include <stdbool.h>

//IsValid checks if the sudoku grid is valid
//@param grid the sudoku grid
//@return 1 if the grid is valid, 0 if not
int IsValid(char board[], size_t rows,size_t cols) {
    size_t i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if ((board[i * cols + j] > '9' || board[i * cols + j] < '0') && board[i * cols + j] != '.') {
	            printf("failed with %d\n",board[i*cols+j]);
              return false;
            }
        }
    }
    return true;
}

//Clean Will replace all the dots by some 0
//@param grid the grid of the sudoku
//@return void

void Clean(char board[], size_t rows, size_t cols){
  if(!IsValid(board,rows,cols)){
    printf("Invalid GRID\n");
    return;
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

//IsColumnSolved checks if the column is solved
//@param grid the sudoku grid
//@param x the column to check
//@return 1 if the column is solved, 0 if not
int IsColumnSolved(char board[], size_t cols, int x) {
    char str[cols];
    char eval[] = "123456789";
    for (size_t i = 0; i < cols; i++)
    {
        str[i] = board[i * cols + x]+'0';
    }
    bool solved = true;
    size_t j = 0;
    size_t k;
    while(j < cols && solved) {
        bool found = false;
        k = 0;
        while (k < cols && !found){
            if (str[j] == eval[k])
            {
                found = true;
            }
            k++;
        }   

        if (!found)
        {
            solved = false;
        }
        j++;
    }
    return solved;
}

//IsRowSolved checks if the row is solved
//@param grid the sudoku grid
//@param y the row to check
//@return 1 if the row is solved, 0 if not
int IsRowSolved(char board[], size_t rows, size_t cols, int y) {
    char str[rows];
    char eval[] = "123456789";
    for (size_t i = 0; i < rows; i++)
    {
        str[i] = board[y * cols + i] + '0';
    }
    bool solved = true;
    size_t j = 0;
    size_t k;
    while(j < rows && solved) {
        bool found = false;
        k = 0;
        while (k < rows && !found){
            if (str[j] == eval[k])
            {
                found = true;
            }
            k++;
        }
        if (!found)
        {
            solved = false;
        }
        j++;
    }
    return solved;
}

//IsSquareSolved checks if the square is solved
//@param grid the sudoku grid
//@param x the x coordinate of the square
//@param y the y coordinate of the square
//@return 1 if the square is solved, 0 if not
int IsSquareSolved(char board[], size_t cols, int x, int y) {
    char str[9];
    char eval[] = "123456789";
    for (int i = x; i < x+3; i++)
    {
        for (int j = y; j < y+3; j++){
            str[i * cols + j] = board[i * cols + j] + '0';
        }
    }
    bool solved = true;
    int j = 0;
    int k;
    while(j < 9 && solved) {
        bool found = false;
        k = 0;
        while (k < 9 && !found){
            if (str[j] == eval[k])
            {
                found = true;
            }
            k++;
        }
        if (!found)
        {
            solved = false;
        }
        j++;
    }
    return solved;
}


//IsSolved checks if the sudoku grid is solved
//@param grid the sudoku grid
//@return 1 if the grid is solved, 0 if not
int IsSolved(char board[], size_t rows, size_t cols) {
    bool result = true;
    size_t i, j = 0;
    while (i < rows && result) {
        result = IsRowSolved(board, rows, cols, i);
        i++;
    }
    while (j < cols && result)
    {
        result = IsColumnSolved(board, cols, j);
        j++;
    }
    i = 0;
    while (i < rows && result) {
        j = 0;
        while (j < cols && result) {
            result = IsSquareSolved(board, cols, i, j);
            j+=3;
        }
        i+=3;
    }
    return result;
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
    bool result = true;
    int i = x-x%3;
    int j;
    while (i < x+3 - x%3 && result) {
        j = y-y%3;
        while (j < y+3 - y%3 && result) {
            if (grid[i * cols + j] == val + '0')
            {
                result = false;
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
        return true;
    }
    if (grid[x * cols + y] == '.' || grid[x*cols +y] == '0'){
        for (size_t i = 1; i < rows+1; i++){
            if (!AlreadyInColumn(grid, rows, cols, x, i) && !AlreadyInLine(grid, cols, y, i) && !AlreadyInSquare(grid, cols, x, y, i)){
	      grid[x * cols + y] = (char)(i + '0');
                bool result = SolveRec(grid, rows, cols, x+1, y);
                if (!result){
                    grid[x * cols + y] = '0';
                }
                else {
                    return true;
                }
            }
        }
    }
    else {
        return SolveRec(grid, rows, cols, x+1, y);
    }
    return false;
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