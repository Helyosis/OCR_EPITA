#ifndef CANNYFILTER_H
#define CANNYFILTER_H

#include <SDL.h>

/* Apply the Canny Filter edge detection algorithm
 * Return a new SDL_Surface, don't forget to free source
 */
SDL_Surface* CannyFilter(SDL_Surface* source);

void CannyFilter_inPlace(SDL_Surface* source);

#endif
