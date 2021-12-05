#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL_image.h>
#include <math.h>
#pragma GCC diagnostic pop

// Utility function to convert grayscale value to adequate ARGB pixel
uint32_t intensityToARGB(unsigned char v) {
    return (uint32_t) (0xFF << 24 | v << 16 | v << 8 | v);
}

// Returns the distance between the points (i, j) and the point (k, l)
double distance(int i, int j, int k, int l) {
    return sqrt((k - i) * (k - i) + (j - l) * (j - l));
}

uint32_t getPixel(SDL_Surface *surface, int x, int y)
{
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
        printf("Bruh moment at x = %d, y = %d\n", x, y);    
    }

    /* Here p is the address to the pixel we want to retrieve */

    //SDL_LockSurface(surface);
    uint32_t result =
        *(uint32_t *) ((uint8_t *)
            surface->pixels
                + y * surface->pitch
                + x * surface->format->BytesPerPixel);
    //SDL_UnlockSurface(surface);

    return result;

    /*
    SDL_LockSurface(surface);
    uint8_t *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    printf("%d \n", *p);
    SDL_UnlockSurface(surface);
    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(uint16_t *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(uint32_t *)p;

    default:
        return 0;
    }*/
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
        *(uint16_t *)p = pixel;
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

void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, uint32_t pixel) {

    int i;
    double x = x1 - x0;
    double y = y1 - y0;
    double length = sqrt( x*x + y*y );
    double addx = x / length;
    double addy = y / length;
    x = x0;
    y = y0;
    for (i = 0; i < length; ++i) {
        putPixel(Screen, x, y, pixel );
        x += addx;
        y += addy;
    }
}
