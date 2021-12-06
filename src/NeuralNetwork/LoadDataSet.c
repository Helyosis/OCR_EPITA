#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <dirent.h>
#include "../ImageProcessing/Pixels.h"
#pragma GCC diagnostic pop

#include "LoadDataSet.h"

//Get the label in the file name
int getLabel(char* str){
    size_t i=0;
    while(str[i]!='-')
        i++;
    return atoi(&str[i+1]);
}

//Load an image
struct tImage* loadImg(char* filename){
    SDL_Surface *original_image = IMG_Load(filename);
    SDL_Surface *image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(original_image);
    if (!image) { // Loading failed so ptr is null
        printf("[-] IMG_Load: %s\n", SDL_GetError());
        errx(1, "Error");
    }
    double* pixVect=malloc(image->w*image->h*sizeof(double));
    struct tImage* d=malloc(sizeof(struct tImage));
    if(pixVect == NULL || d == NULL)
        errx(1, "[-] loadImg: Not enough memory");
    for (int x = 0; x < image->w; x++)
    {
        for (int y = 0; y < image->h; y++) 
        {
            double pix = (double) I(image, x, y);
            pixVect[x+y*image->w] = pix;
        }
    }
    d->label = getLabel(filename);
    d->pixVect = pixVect;
    SDL_FreeSurface(image);
    return d;
}
//free all
void freeImVect(struct tImage** v, size_t nbImages){
    for(size_t i=0;i<nbImages;i++){
        free(v[i]->pixVect);
        free(v[i]);
    }
    free(v);
}
//Load all the training images
struct tImage** imageVect(size_t nbImages){
    struct dirent *dir;
    char* dirName="./bddImages/";
    DIR *d = opendir(dirName); 
    struct tImage** dataSet=malloc(sizeof(struct tImage)*nbImages);
    if(dataSet == NULL)
        errx(1, "[-] imageVect: Not enough memory");
    if(d == NULL)
        errx(1, "[-] imageVect: directory \"./bddImages\" does not exist");
    size_t i=0;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if(dir->d_name[0]!='.')
            {
                char *result = malloc(strlen(dirName) + strlen(dir->d_name) + 1); 
                if(result == NULL)
                    errx(1, "[-] imageVect: Not enough memory");
                strcpy(result, dirName);
                strcat(result, dir->d_name);
                dataSet[i] = loadImg(result);
                free(result);
                i++;
            }
            if(i==nbImages)
                break;
        }
        closedir(d);
    }
    return dataSet;
}