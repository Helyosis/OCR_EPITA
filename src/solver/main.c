#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku.h"

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    FILE *input;
    input = fopen(argv[1], "r");
    if(input == NULL)
    {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }
    //removing the space in the file
    char *grid = calloc(sizeof(char) * 81, sizeof(char));
    int i = 0;
    char c;
    while ((c = fgetc(input)) != EOF)
    {
        if(c == ' ' || c == '\n')
        {
            continue;
        }
        else
        {
            grid[i] = c;
            i++;
        }
    }
    fclose(input);
    //solving the grid
    int solved = Solve(grid, 9, 9);
    if(solved == 0)
    {
        printf("Error: Could not solve the grid\n");
        return 1;
    }
    //saving the grid
    FILE *output;
    output = fopen(strcat(argv[1], ".result"), "w");
    for (size_t i = 0; i < 81; i++)
    {
        if(i % 3 == 0 && i != 0){
            char *c = " ";
            fprintf(output,"%s" , c);
        }
        if (i % 9 == 0 && i !=0)
        {
            char *c = "\n";
            fprintf(output,"%s" , c);
        }
        if (i % 27 == 0 && i != 0)
        {
            char *c = "\n";
            fprintf(output,"%s" , c);
        }
        fprintf(output, "%c", grid[i]);
    }
    fclose(output);
    free(grid);
    return 0;
}