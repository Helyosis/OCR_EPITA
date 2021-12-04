#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL.h>
#include <SDL_image.h>
#pragma GCC diagnostic pop
double *Fill_matrix(orderedPoints points);
SDL_Surface *HomographicTransform(SDL_Surface *src, orderedPoints points);
#include "Pixels.h"