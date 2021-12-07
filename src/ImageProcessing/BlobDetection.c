#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#include <stdio.h>
#pragma GCC diagnostic pop

#include "FloodFill.h"
#include "BlobDetection.h"
#include "Pixels.h"
#include "../Verbose.h"


BiggestBlob_result findBiggestBlob(SDL_Surface* src) {
    SDL_Surface *dest;
    dest = SDL_CreateRGBSurface(0,
                                src->w,
                                src->h,
                                src->format->BitsPerPixel,
                                src->format->Rmask,
                                src->format->Gmask,
                                src->format->Bmask,
                                src->format->Amask);
    if (dest == NULL)
        error_s("Not enoug memory !");
    SDL_BlitSurface(src, NULL, dest, NULL);
    //memcpy(dest->pixels, src->pixels, src->w * src->h * sizeof(uint32_t));

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

    return (BiggestBlob_result) {dest, maxPoint, max};
}


void removeSmallBlob(SDL_Surface* source, size_t threshold, uint32_t foregroundColor, uint32_t backgroundColor) {
    if (foregroundColor == BLUE) {
        error_s("removeSmallBlob: Invalid foregroundColor of %#x", foregroundColor);
    }

    for (int y = 0; y < source->h; ++y) {
        for (int x = 0; x < source->w; ++x) {
            if (getPixel(source, x, y) != foregroundColor) continue;
            Point seed = {x, y};
            size_t blobSize = floodFill(source, seed, foregroundColor, BLUE);
            if (blobSize <= threshold) {
                floodFill(source, seed, BLUE, backgroundColor);
            }
        }
    }  

    // Cleanup loop
    for (int y = 0; y < source->h; ++y) {
        for (int x = 0; x < source->w; ++x) {
            Point seed = {x, y};
            if (getPixel(source, x, y) == BLUE) floodFill(source, seed, BLUE, foregroundColor);
        }
    }
}