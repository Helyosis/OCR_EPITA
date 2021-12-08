#ifndef PIXELS_H
#define PIXELS_H

#include <sys/queue.h>

#define WHITE 0xffffffff
#define BLACK 0xff000000
#define BLUE  0xff00ffff

typedef struct Point {
    int x;
    int y;
} Point;

uint32_t intensityToARGB(unsigned char v);

double distance(int i, int j, int k, int l);

uint32_t getPixel(SDL_Surface *surface, int x, int y);

void putPixel(SDL_Surface *surface, int x, int y, uint32_t pixel);

void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, uint32_t pixel);

int I(SDL_Surface *source, int x, int y);


#endif
