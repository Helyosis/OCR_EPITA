#ifndef PIXELS_H
#define PIXELS_H

Uint32 getPixel(SDL_Surface *surface, int x, int y);

void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
<<<<<<< HEAD
void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, uint32_t pixel);
int I(SDL_Surface *source, int x, int y);
void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, uint32_t pixel);
=======

void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, uint32_t pixel);

int I(SDL_Surface *source, int x, int y);

>>>>>>> 1af53c7d8e5a92a1a25ffbe6a21d02d1fbc91ccb

#endif
