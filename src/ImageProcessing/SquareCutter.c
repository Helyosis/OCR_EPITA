#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <SDL.h>
#pragma GCC diagnostic pop

#include "SquareCutter.h"
#include "Pixels.h"
#include "FloodFill.h"
#include "../Verbose.h"

struct stat st = {0};

int cleanupAndIdentify(SDL_Surface* digitSurface);

void cutSudoku(SDL_Surface* src) {
    size_t nbDigitsPerLine = 9;
    int size = src->w / nbDigitsPerLine;
    
    SDL_Rect srcrect = {0, 0, size, size};
    SDL_Surface* dest = dest = SDL_CreateRGBSurface(0,
                                size,
                                size,
                                src->format->BitsPerPixel,
                                src->format->Rmask,
                                src->format->Gmask,
                                src->format->Bmask,
                                src->format->Amask);
    if (dest == NULL) {
        error_s("Couldn't create SDL_Surface with error %s", SDL_GetError());
    }

    if (stat(SC_DESTDIR, &st) == -1) {
        mkdir(SC_DESTDIR, 0700);
    }
    for (size_t x = 0; x < nbDigitsPerLine; x += 1) {
        for (size_t y = 0; y < nbDigitsPerLine; y += 1) {
            char dstFile[50];
            sprintf(dstFile, "%s/x%ld-y%ld.bmp", SC_DESTDIR, x, y);

            srcrect.x = x * size;
            srcrect.y = y * size;


            SDL_BlitSurface(src, &srcrect, dest, NULL);

            if (cleanupAndIdentify(dest) == 0) {
                SDL_SaveBMP(dest, dstFile);
            }
        }
    }

    SDL_FreeSurface(dest);
}

int cleanupAndIdentify(SDL_Surface* digitSurface) {
    // If we remove more than half of the pixels, it could be dangerous and we'd remove 
    // the digit without paying attention
    size_t max = digitSurface->w * digitSurface->h / 2;

    // There is some strange bug if I start at (0, 0). Better ignore it and continue
    Point seeds[4] = {{3, 3}, {digitSurface->w - 4, 3}, {digitSurface->w - 4, digitSurface->h - 4}, {3, digitSurface->h - 4}};
    // We remove the useless border around the digits, and try on all the sides to be sure to catch most cases
    size_t nbToRemove = 0;
    for (int i = 0; i < 4; ++i)
        nbToRemove += floodFill(digitSurface, seeds[i], WHITE, BLUE);

    uint32_t new_color = nbToRemove > max ? WHITE : BLACK;
    for (int i = 0; i < 4; ++i) {
        floodFill(digitSurface, seeds[i], BLUE, new_color);
    }

    // We iterate over the pixels array to count the number of black pixels
    // If it's too low, we cannot be sure it is an empty spot.
    size_t min = digitSurface->w * digitSurface->h * 9 / 10; // 70% black pixels should do the trick
    size_t nbBlackPixel = 0;
    for (int x = 0; x < digitSurface->w; ++x)
    for (int y = 0; y < digitSurface->h; ++y)
        if (getPixel(digitSurface, x, y) == BLACK) ++nbBlackPixel;
    
    if (nbBlackPixel > min) {
        return 1; // It is an empty spot, we don't have to save it.
    } else {
        return 0;
    }
}