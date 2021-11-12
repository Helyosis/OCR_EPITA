#ifndef MORPHOLOGY
#define MORPHOLOGY
#include <SDL2/SDL.h>
void dilate_in_place(SDL_Surface* image);
void erode_in_place(SDL_Surface* image);
#endif
