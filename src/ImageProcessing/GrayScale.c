#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#pragma GCC diagnostic pop

#include "Pixels.h"
#include "../Verbose.h"

uint32_t TransformPixel_Grayscale(uint32_t pixel) {
    uint8_t r = pixel >> 16 & 0xFF;
    uint8_t g = pixel >> 8 & 0xFF;
    uint8_t b = pixel & 0xFF;
    // We calculate the appropriate gray value using this formula
    uint8_t v = (r + g + b) / 3;
    return (uint32_t) (0xFF << 24 | v << 16 | v << 8 | v);
}

void Apply_grayscale_filter(SDL_Surface* surface) {
    if (surface->format->format != SDL_PIXELFORMAT_ARGB8888)
        error_s(
             "[-] Invalid format for surface,"
             "expected SDL_PIXELFORMAT_ARGB8888\n"
            );
    // We lock the surface to make sure it's not modified while we do our thing
    //SDL_LockSurface(surface);
    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            // printf("y = %d, x = %d", y, x);
            Uint32 pixel = getPixel(surface, x, y);
            pixel = TransformPixel_Grayscale(pixel);
            putPixel(surface, x, y, pixel);
        }
    }
    //SDL_UnlockSurface(surface);

}

