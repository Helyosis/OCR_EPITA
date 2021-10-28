#include <SDL.h>
#include <SDL_image.h>
#include "Pixels.h"

#define DILATION_MATRIX {0,1,0,1,1,1,0,1,0}
#define DILATION_MATRIX_SIZE 3

SDL_Surface* dilate(SDL_Surface* surface) {
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
            Uint32 maxPix = getPixel(surface, x, y);
            int dx = (DILATION_MATRIX_SIZE - 1) / 2;
            int dy = dx;
            for (int i = 0; i < DILATION_MATRIX_SIZE * DILATION_MATRIX_SIZE; ++i) {
                dx++;
                if (dx == DILATION_MATRIX_SIZE) {
                    dx = (DILATION_MATRIX_SIZE - 1) / 2;
                    dy++;
                }

                if (getPixel(image, x, y) > maxPix)
                    maxPix = I(image
            }
        }
    }
}

void dilate_in_place(SDL_Surface* image) {
}
