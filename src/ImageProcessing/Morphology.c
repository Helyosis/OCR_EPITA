#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL.h>
#include <SDL_image.h>
#pragma GCC diagnostic pop
#include "Pixels.h"

#define DILATION_MATRIX_SIZE 3

SDL_Surface* dilate(SDL_Surface* surface) {
    const int DILATION_MATRIX[]= {1,1,1,1,1,1,1,1,1};
    SDL_Surface *dest;
    dest = SDL_CreateRGBSurface(0,
                                surface->w,
                                surface->h,
                                surface->format->BitsPerPixel,
                                surface->format->Rmask,
                                surface->format->Gmask,
                                surface->format->Bmask,
                                surface->format->Amask);
    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            int maxPix = I(surface, x, y);
            int dx = -(DILATION_MATRIX_SIZE - 1) / 2;
            int dy = dx;
            for (int i = 0; i < DILATION_MATRIX_SIZE * DILATION_MATRIX_SIZE;++i)
            {
                dx++;
                if (dx == DILATION_MATRIX_SIZE) {
                    dx = -(DILATION_MATRIX_SIZE - 1) / 2;
                    dy++;
                }
                if (x+dx>=0 && y+dy>=0 && x+dx<surface->w && y+dy<surface->h){
                    if (I(surface, x+dx, y+dy) > maxPix)
                    {
                        if(DILATION_MATRIX[i])
                            maxPix = I(surface, x+dx, y+dy);
                    }
                }
            }
            uint32_t pixel = 0xff000000;
            pixel |=  maxPix<< 16;
            pixel |=  maxPix<< 8;
            pixel |= maxPix;
            putPixel(dest, x, y, pixel);
        }
    }
    return dest;
}

void dilate_in_place(SDL_Surface* image) {
    SDL_Surface *dest;
    dest = dilate(image);
    for (int i = DILATION_MATRIX_SIZE; i < image->w-DILATION_MATRIX_SIZE; ++i) {
        for (int j = DILATION_MATRIX_SIZE; j <image->h-DILATION_MATRIX_SIZE;++j)
        {
            uint32_t pixel = getPixel(dest, i, j);
            putPixel(image, i, j, pixel);
        }
    }
    free(dest);
}

SDL_Surface* erode(SDL_Surface* surface) {
    const int ERODE_MATRIX[]= {0,1,0,1,1,1,0,1,0};
    SDL_Surface *dest;
    dest = SDL_CreateRGBSurface(0,
                                surface->w,
                                surface->h,
                                surface->format->BitsPerPixel,
                                surface->format->Rmask,
                                surface->format->Gmask,
                                surface->format->Bmask,
                                surface->format->Amask);
    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            int minPix = I(surface, x, y);
            int dx = -(DILATION_MATRIX_SIZE - 1) / 2;
            int dy = dx;
            for (int i = 0; i < DILATION_MATRIX_SIZE * DILATION_MATRIX_SIZE;++i)
            {
                if (dx == DILATION_MATRIX_SIZE) {
                    dx = -(DILATION_MATRIX_SIZE - 1) / 2;
                    dy++;
                }
                if (x+dx>=0 && y+dy>=0 && x+dx<surface->w && y+dy<surface->h){
                    if(I(surface, x+dx, y+dy) < minPix)
                    {
                        if(ERODE_MATRIX[i])
                            minPix = I(surface, x+dx, y+dy);
                    }
                }
                dx++;
            }
            uint32_t pixel = 0xff000000;
            pixel |=  minPix<< 16;
            pixel |=  minPix<< 8;
            pixel |= minPix;
            putPixel(dest, x, y, pixel);
        }
    }
    return dest;
}

void erode_in_place(SDL_Surface* image) {
    SDL_Surface *dest;
    dest = erode(image);
    for (int i = DILATION_MATRIX_SIZE; i < image->w-DILATION_MATRIX_SIZE; ++i) {
        for (int j = DILATION_MATRIX_SIZE; j <image->h-DILATION_MATRIX_SIZE;++j)
        {
            uint32_t pixel = getPixel(dest, i, j);
            putPixel(image, i, j, pixel);
        }
    }
    free(dest);
}

void MorphologyClose(SDL_Surface* image) {
    dilate_in_place(image);
    erode_in_place(image);
}

void MorphologyOpen(SDL_Surface* image) {
    erode_in_place(image);
    dilate_in_place(image);
}
