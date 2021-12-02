#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#include <stdio.h>
#pragma GCC diagnostic pop

#include "FloodFill.h"
#include "BlobDetection.h"
#include "Pixels.h"


BiggestBlob_result findBiggestBlob(SDL_Surface* src) {
    uint32_t WHITE = 0xffffffff;
    uint32_t BLACK = 0xff000000;
    uint32_t BLUE  = 0xff00ffff;

    SDL_Surface *dest;
    dest = SDL_CreateRGBSurface(0,
                                src->w,
                                src->h,
                                src->format->BitsPerPixel,
                                src->format->Rmask,
                                src->format->Gmask,
                                src->format->Bmask,
                                src->format->Amask);

    memcpy(dest->pixels, src->pixels, src->w * src->h * sizeof(uint32_t));

    size_t max = 0;
    Point maxPoint;

    for (int y = 0; y < src->h; ++y) {
        for (int x = 0; x < src->w; ++x) {
            if (getPixel(dest, x, y) == WHITE) {
                Point seed = {x, y};
                size_t area = floodFill(dest, seed, WHITE, BLUE);
                if (area > max) {
                    max = area;
                    maxPoint = seed;
                }
            }
        }
    }

    printf("[*] Found the biggest blob starting at x = %d and y = %d, of size %ld\n",
           maxPoint.x, maxPoint.y, max);
    floodFill(dest, maxPoint, BLUE, WHITE);
        
    for (int y = 0; y < src->h; ++y) {
        for (int x = 0; x < src->w; ++x) {
            Point seed = {x, y};
            if (x != maxPoint.x && y != maxPoint.y &&
                getPixel(dest, x, y) == BLUE) {
                floodFill(dest, seed, BLUE, BLACK);
            }
        }
    }

    return (BiggestBlob_result) {dest, max};
}
