#include <SDL2/SDL2.h>
#include <SDL2/SDL2_image.c>
#include <err.h>

Uint32 TransformPixel_Grayscale(Uint32 pixel) {
    Uint8 r = pixel >> 16 & 0xFF;
    Uint8 g = pixel >> 8 & 0xFF;
    Uint8 b = pixel & 0xFF;
    // We calculate the appropriate gray value using this formula
    Uint8 v = 0.212671f * r + 0.715160f * g + 0.072169f * b;
    
    return (Uint32) (0xFF << 24 | v << 16 | v << 8 | v);

void Apply_grayscale_filter(SDL_Surface surface) {
    if (surface->format->format != SDL_PIXELFORMAT_ARGB8888)
        errx(1,
             "[-] Invalid format for surface, expected SDL_PIXELFORMAT_ARGB8888\n"
            );
    Uint32 *pixels = (Uint32 *)image->pixels;
    for (y = 0; y < surface->h; ++y) {
        for (x = 0; x < surface->w; ++x) {
            pixel = pixels[y * surface->h + x];
            pixel TransformPixel_Grayscale(pixel);
            pixels[y * surface->h + x] = pixel;
    
}
