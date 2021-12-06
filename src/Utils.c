#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#include <math.h>
#pragma GCC diagnostic pop

#ifndef M_PI
#define M_PI 3.14159265359
#endif

int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

void displaySurface(SDL_Renderer* renderer, SDL_Surface* surface) {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

double* Build2DGaussianKernel(size_t kernelRadius, double sigma) {
    size_t kernelSize = 2 * kernelRadius + 1;
    double* kernel = malloc(kernelSize * kernelSize * sizeof(*kernel));

    double sum = 0;
    for (size_t y = 0; y < kernelSize; ++y) {
        for (size_t x = 0; x < kernelSize; ++x) {
            double expNum =
                (x - kernelRadius) * (x - kernelRadius) +
                (y - kernelRadius) * (y - kernelRadius);
            double expDen = -2 * sigma * sigma;
            double res = exp(expNum / expDen) / (2 * M_PI * sigma * sigma);
            sum += res;
            kernel[y * kernelSize + x] = res;
        }
    }

    for (size_t i = 0; i < kernelSize * kernelSize; ++i) {
        kernel[i] /= sum;
    }

    return kernel;
}

void strtoupper(char* s) {
  while (*s) {
    *s = toupper((unsigned char) *s);
    s++;
  }

}
