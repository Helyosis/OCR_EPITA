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
#include "../Utils.h"
#include "../Verbose.h"

SDL_Surface* CannyFilter(SDL_Surface* source) {
    SDL_Surface *dest;
    dest = SDL_CreateRGBSurface (0, source->w, source->h,
                                 source->format->BitsPerPixel,
                                 source->format->Rmask, source->format->Gmask,
                                 source->format->Bmask, source->format->Amask);
    if (dest == NULL) {
        error_s("Coudln't create surface");
    }

    double** sobel = SobelFilters(source);
    double* nonmax = NonMaxSuppression(sobel[0],sobel[1],source->h,source->w);
    double* thresholded = DoubleThreshold(nonmax, source->w * source->h);
    double* hysteresised = Hysteresis(thresholded, source->h, source->w);

    for (int i = 0; i < source->w; ++i) {
        for (int j = 0; j < source->h; ++j) {
            int intensity = sobel[0][j * source->w + i];
            putPixel(dest, i, j, intensityToARGB(intensity));
        }
    }

    free(sobel[0]);
    free(sobel[1]);
    free(sobel);
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
