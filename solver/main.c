#include <stdio.h>
#include "sudoku.h"
int main()
{
  char board[] = "...26.7.168..7..9.19...45..82.1...4...46.29...5...3.28..93...74.4..5..367.3.18...";
  Solve(board, 9,9);
  printf("%s\n", board);

  return 0;
}
