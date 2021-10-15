#ifndef PIXELS_H
#define PIXELS_H

Uint32 getPixel(SDL_Surface *surface, int x, int y);
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int I(SDL_Surface *source, int x, int y);
void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, uint32_t pixel);

#endif
