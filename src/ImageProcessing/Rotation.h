#ifndef MANIPULATION_H
#define MANIPULATION_H

#include "SDL2/SDL.h"

SDL_Surface *rotation(SDL_Surface *image, double angle);
SDL_Surface *Rotation_shearing(SDL_Surface *image, double angle);
#endif