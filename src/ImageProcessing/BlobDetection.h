#ifndef BLOBDETECTION_H
#define BLOBDETECTION_H

typedef struct {
    SDL_Surface* res;
    size_t size;
} BiggestBlob_result;

/*
 * Find the biggest continuous surface (called a blob) on an black and white image.
 * and return an SDL_Surface of the same size, with the surface marked on white
 * on a black background.
 */
BiggestBlob_result findBiggestBlob(SDL_Surface* src);

#endif
