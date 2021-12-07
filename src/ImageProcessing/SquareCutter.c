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
#include "BlobDetection.h"
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
    // We only want to keep the biggest blob, aka the digit
    BiggestBlob_result digitBlob = findBiggestBlob(digitSurface);
    SDL_BlitSurface(digitBlob.res, NULL, digitSurface, NULL);
    SDL_FreeSurface(digitBlob.res);

    Point midPoint = {digitSurface->w / 2, digitSurface->h / 2};
    size_t centerBlobsize = floodFill(digitSurface, midPoint, BLACK, BLUE);
    floodFill(digitSurface, midPoint, BLUE, BLACK);
    if (centerBlobsize < 20)
        return 0; // We indicate it's a digit
    return 1; // Else it empty and we don't save it
}