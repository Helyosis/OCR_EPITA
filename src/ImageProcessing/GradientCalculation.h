#ifndef GRADIENTCALCULATION_H
#define GRADIENTCALCULATION_H

#include <SDL.h>

/*
 * Apply the Sobel Filter on a SDL_Surface.
 * It returns the intensity of the edges, with the angle
 * result[0] is the intensity, result[1] is the theta
 * Don't forget to free both array and the result.
 */
double** SobelFilters(SDL_Surface* source);

#endif
