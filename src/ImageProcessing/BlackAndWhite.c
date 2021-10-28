#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL.h>
#include <SDL_image.h>
#pragma GCC diagnostic pop
#include "Pixels.h"

#define ADAPTIVETHRESHOLDING_RANGE  2
#define ADAPTIVETHRESHOLING_C       2


// Adapative thresholding using the "mean - C" method.
uint32_t AdaptiveThresholding_calculateThreshold(SDL_Surface* surface, int y, int x, int range) {
    uint32_t mean = 0;
    uint32_t c = ADAPTIVETHRESHOLING_C;
    uint32_t sum = 0;
    uint32_t nb_pixels = 0;

    for (int dy = -range; dy < range; ++dy) {
        if (0 <= y + dy && y + dy < surface->h) {
            for (int dx = -range; dx < range; ++dx) {
                if (0 <= x + dx && x + dx < surface->w) {
                    sum += I(surface, x + dx, y + dy);
                    nb_pixels++;
                }
            }
        }
    }

    // Not sure about this, but it's avoid Division by 0 error
    if (nb_pixels > 0)
        mean = sum / nb_pixels;
    
    if (mean >= c) {
        mean -= c;
    } else {
        mean = 0;
    }
    return mean;
}


SDL_Surface *AdapativeThresholding(SDL_Surface* surface) {
    SDL_Surface *dest;
    dest = SDL_CreateRGBSurface(0,
                                surface->w,
                                surface->h,
                                surface->format->BitsPerPixel,
                                surface->format->Rmask,
                                surface->format->Gmask,
                                surface->format->Bmask,
                                surface->format->Amask);
    int range = ADAPTIVETHRESHOLDING_RANGE;
    for (int x = 0; x < surface->w; ++x) {
        for (int y = 0; y < surface->h; ++y) {
            //printf("y = %d, x = %d, ", y, x);
            uint32_t threshold = 0;
            threshold = AdaptiveThresholding_calculateThreshold(surface, y, x, range);
            //printf("threshold = %d\n", threshold);
            uint32_t pixel_value = 0;
            pixel_value = I(surface, x, y); // To suppress the warning in valgrind ?
            if (pixel_value > threshold) {
                putPixel(dest, x, y, 0xFF000000);
            }
            else {
                putPixel(dest, x, y, 0xFFFFFFFF); // Black pixel
            }
        }
    }

    return dest;
}

void AdaptiveThresholding_inPlace(SDL_Surface *source) {
    SDL_Surface *dest;
    dest = AdapativeThresholding(source);

    for (int x = 0; x < source->w; ++x) {
        for (int y = 0; y < source->h; ++y) {
            uint32_t pixel = getPixel(dest, x, y);
            putPixel(source, x, y, pixel);
        }
    }

    SDL_FreeSurface(dest);
}