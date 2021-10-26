#ifndef GRAYSCALE_H
#define GRAYSCALE_H

#include <stdint.h>
#include <SDL2/SDL_image.h>

uint32_t TransformPixel_Grayscale(uint32_t pixel);
void Apply_grayscale_filter(SDL_Surface* surface);

#endif
