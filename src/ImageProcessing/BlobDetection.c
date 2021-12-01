#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#include <stdio.h>
#pragma GCC diagnostic pop

#include "Pixels.h"

SDL_Surface findBiggestBlob(SDL_Surface* src) {
    uint32_t WHITE = 0xffffffff;
    uint32_t BLACK = 0xff000000;
    uint32_t BLUE  = 0xff00ffff;
    
    SDL_Surface *dest;
    dest = SDL_CreateRGBSurface(0,
                                surface->w,
                                surface->h,
                                surface->format->BitsPerPixel,
                                surface->format->Rmask,
                                surface->format->Gmask,
                                surface->format->Bmask,
                                surface->format->Amask);

    memcpy(dest->pixels, src->pixels, src->w * src->h);
    
    size_t max = 0;
    Point maxPoint;

    for (int y = 0; y < src->w; ++y) {
        for (int x = 0; x < src->h; ++x) {
            if (getPixel(dest, x, y) == BLACK) {
                int area = floodFill(dest);
            }
        }
    }
}
