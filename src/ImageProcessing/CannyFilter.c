#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#include <err.h>
#pragma GCC diagnostic pop

#include "Pixels.h"
#include "GradientCalculation.h"
#include "NonMaxSuppression.h"
#include "DoubleThreshold.h"

SDL_Surface* CannyFilter(SDL_Surface* source) {
    SDL_Surface *dest;
    dest = SDL_CreateRGBSurface (0, source->w, source->h,
                                 source->format->BitsPerPixel,
                                 source->format->Rmask, source->format->Gmask,
                                 source->format->Bmask, source->format->Amask);
    if (dest == NULL) {
        errx(1, SDL_GetError());
    }

    double** result = SobelFilters(source);
    double* nonmax = NonMaxSuppression(result[0],result[1],source->h,source->w);
    double* thresholded = DoubleThreshold(nonmax, source->w * source->h);
    double* hysteresised = Hysteresis(thresholded, source->h, source->w);
    
    for (int i = 0; i < source->w; ++i) {
        for (int j = 0; j < source->h; ++j) {
            int intensity = hysteresised[j * source->w + i];
            uint32_t pixel = 0xff000000;
            pixel |= intensity << 16;
            pixel |= intensity << 8;
            pixel |= intensity;
            putPixel(dest, i, j, pixel);
        }
    }

    free(result[0]);
    free(result[1]);
    free(result);
    free(nonmax);
    free(thresholded);
    free(hysteresised);
    
    return dest;
}

void CannyFilter_inPlace(SDL_Surface* source) {
    SDL_Surface *dest;
    dest = CannyFilter(source);

    for (int i = 0; i < source->w; ++i) {
        for (int j = 0; j < source->h; ++j) {
            uint32_t pixel = getPixel(dest, i, j);
            putPixel(source, i, j, pixel);
        }
    }

    SDL_FreeSurface(dest);
}
