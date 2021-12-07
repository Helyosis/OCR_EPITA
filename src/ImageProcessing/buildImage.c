#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <stdlib.h>
#pragma GCC diagnostic pop


int buildGrid(char* gridFileName, char* out_filename) {
    //char *current_path;
    //current_path=(char *)malloc(100*sizeof(char));
    //getcwd(current_path,100);
    printf(current_path);
    SDL_Surface *original_image = IMG_Load("/tmp/resultGrid.jpg");
    SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(original_image);
    if (!image) { // Loading failed so ptr is null
        printf("[-] IMG_Load: %s\n", SDL_GetError());
        error_s("Error");
    }
    char *grid = calloc(sizeof(char) * 81, sizeof(char));
    loadGrid(gridFileName, grid);
    SDL_Surface **numbers = malloc(9*sizeof(SDL_Surface*));
    for(int n=0; n<10; n++){
        char* path = strcat("/tmp/Numbers", strcat(n+'0', ".jpg"));
        numbers[n] = SDL_ConvertSurfaceFormat(IMG_Load(path,
                                            SDL_PIXELFORMAT_ARGB8888, 0);
    }
    SDL_Rect* dstrect;
    dstrect.w = 80;
    dstrect.h = 100;
    dstrect.x = 1;
    dstrect.y = 1;
    for(int i = 0; i<9; i++){
        for(int j = 0; j<9; j++){
            SDL_BlitSurface(numbers[grid[i*9+j]],NULL,image,dstrect);
            dstrect.x += 80;
        }
        dstrect.x = 0;
        dstrect.y += 100;
    }
    SDL_FreeSurface(image);
    return 0;
}

void loadGrid(char* filename,char* grid){
    FILE* input;
    input = fopen(filename, "r");
    if(input == NULL)
    {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }
    int i = 0;
    char c;
    while ((c = fgetc(input)) != EOF)
    {
        if(c == ' ' || c == '\n')
            continue;
        else
        {
            grid[i] = c;
            i++;
        }
    }
    fclose(input);
}
