#ifndef BLOBDETECTION_H
#define BLOBDETECTION_H

#include "Pixels.h"

typedef struct {
    SDL_Surface* res;
    Point point;
    size_t size;
} BiggestBlob_result;

/*
 * Find the biggest continuous surface (called a blob) on an black and white image.
 * and return an SDL_Surface of the same size, with the surface marked on white
 * on a black background.
 */
BiggestBlob_result findBiggestBlob(SDL_Surface* src);


/*
 * Remove all blobs smaller than the provided threshold
 * on the source image.
 */
void removeSmallBlob(SDL_Surface* source, size_t threshold, uint32_t wantedColor, uint32_t backgroundColor);

#endif
