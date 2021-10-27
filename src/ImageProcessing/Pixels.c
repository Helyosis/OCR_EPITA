#include <SDL.h>
#include <SDL_image.h>
#include <math.h>


Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}


void putPixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

int I(SDL_Surface *source, int x, int y) {
    return getPixel(source, x, y) & 0xFF;
}

<<<<<<< HEAD
void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, Uint32 pixel) {
=======
void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, uint32_t pixel) {

>>>>>>> 1af53c7d8e5a92a1a25ffbe6a21d02d1fbc91ccb
    int i;
    double x = x1 - x0;
    double y = y1 - y0;
    double length = sqrt( x*x + y*y );
    double addx = x / length;
    double addy = y / length;
        x = x0;
        y = y0;

        for ( i = 0; i < length; i += 1) {
            putPixel(Screen, x, y, pixel );
        x += addx;
        y += addy;
        }
}
<<<<<<< HEAD
=======

>>>>>>> 1af53c7d8e5a92a1a25ffbe6a21d02d1fbc91ccb
