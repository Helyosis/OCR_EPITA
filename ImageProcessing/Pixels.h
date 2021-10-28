#ifndef PIXELS_H
#define PIXELS_H

Uint32 getPixel(SDL_Surface *surface, int x, int y);

void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

int I(SDL_Surface *source, int i, int j);

#endif
