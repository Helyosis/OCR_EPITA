#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL.h>
#include <SDL_image.h>
#include <err.h>
#pragma GCC diagnostic pop
#include "../ImageProcessing/Pixels.h"

// Function: Result_construct
// Description: Constructs a the png with the grid solved
// Inputs: char *grid - the solved grid
// Returns: SDL_Surface * - the solved grid

SDL_Surface *Result_construct(char *grid, char *basegrid)
{
    SDL_Surface *result = IMG_Load("../Images/resultGrid.jpg");
    if (result == NULL)
        errx(1, "IMG_Load: %s", IMG_GetError());
    
    int x = 0;
    int y = 0;
    for(int i = 0; i < 81; i++)
    {
        if(grid[i] != '0')
        {
            SDL_Rect rect = {x, y, 0, 0};
            char *text = malloc(2);
            text[0] = grid[i];
            text[1] = '\0';
            char *path = malloc(strlen("../Ressources/Numbers/") + strlen(text) + strlen("-1.png") + 1);
            strcpy(path, "../Ressources/Numbers/");
            strcat(path, text);
            strcat(path, "-1.png");
            SDL_Surface *number = IMG_Load(path);
            free(path);
            free(text);
            if (number == NULL)
                errx(1, "IMG_Load: %s", IMG_GetError());

            if(basegrid[i] == '.'){
                //chages the color of number to red
                for(int j = 0; j < number->w; j++){
                    for(int k = 0; k < number->h; k++){
                        Uint32 pixel = getPixel(number, j, k);
                        if(pixel != 0x00000000){
                            putPixel(number, j, k, 0xFF0000FF);
                        }
                    }
                }
            }
            SDL_BlitSurface(number, NULL, result, &rect);
            SDL_FreeSurface(number);
        }
        x += 100;
        if(x == 900)
        {
            x = 0;
            y += 100;
        }
    }
    return result;
}