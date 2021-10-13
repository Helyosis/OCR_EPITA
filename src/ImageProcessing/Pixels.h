#ifndef PIXELS_H
#define PIXELS_H

Uint32 getPixel(SDL_Surface *surface, int x, int y);
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
<<<<<<< HEAD:src/ImageProcessing/Pixels.h
void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, uint32_t pixel);

=======
int I(SDL_Surface *source, int x, int y);
>>>>>>> 0d24310 (Refactor code):ImageProcessing/Pixels.h

#endif
