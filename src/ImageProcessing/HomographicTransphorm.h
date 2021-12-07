#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL.h>
#include <SDL_image.h>
#include "OrderPoints.h"
#pragma GCC diagnostic pop

double *Fill_matrix(orderedPoints points, int size);
SDL_Surface *HomographicTransform(SDL_Surface *src, orderedPoints points, int size);
#include "Pixels.h"
