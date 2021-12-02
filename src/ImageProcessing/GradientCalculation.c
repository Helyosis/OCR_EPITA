#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL.h>
#include <math.h>
#pragma GCC diagnostic pop

#include "Pixels.h"

double** SobelFilters(SDL_Surface* source) {
    const int Kx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    const int Ky[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

    int* Ix = calloc(source->h * source->w, sizeof(*Ix));
    int* Iy = calloc(source->h * source->w, sizeof(*Iy));

    int i = 0;
    for (int y = 0; y < source->h; ++y) {
        for (int x = 0; x < source->w; ++x) {
            int j = 0;
            for (int dx = -1; dx <= 1; ++dx ) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (x + dx >= 0 && x + dx < source->w &&
                        y + dy >= 0 && y + dy < source->h)
                    {
                        Ix[i] += I(source, x + dx, y + dy) * Kx[j];
                        Iy[i] += I(source, x + dx, y + dy) * Ky[j];
                        ++j;
                    }
                }
            }
            ++i;
        }
    }

    double* G = calloc(source->h * source->w, sizeof(*G));
    double* theta = calloc(source->h * source->w, sizeof(*theta));

    double maxi = 0;
    for (int i = 0; i < source->h * source->w; ++i) {
        G[i] = sqrt(Ix[i] * Ix[i] + Iy[i] * Iy[i]);
        if (G[i] > maxi) maxi = G[i];

        theta[i] = atan2(Iy[i], Ix[i]);
    }

    for (int i = 0; i < source->h * source->w; ++i) {
        G[i] = G[i] / maxi * 255;
    }

    double** result = calloc(2, sizeof(*result));
    result[0] = G;
    result[1] = theta;

    return result;
}
