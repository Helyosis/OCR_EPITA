#ifndef PIXELS_H
#define PIXELS_H

/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 getPixel(SDL_Surface *surface, int x, int y);

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

/*
 * Returns the intensity (in grayscale value) of 
 */
int I(SDL_Surface *source, int x, int y);

/*
 * Draw line in place in SDL_Surface, from point (x0, y0) to (x1, y1) using the same pixel colro
 */
void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, Uint32 pixel);

#endif
