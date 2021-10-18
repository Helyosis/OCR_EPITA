#include "SDL2/SDL.h"
#ifndef MANIPULATION_H
#define MANIPULATION_H
SDL_Surface *rotation(SDL_Surface *image, int angle);
SDL_Surface *Rotation_shearing(SDL_Surface *image, int angle);
#endif