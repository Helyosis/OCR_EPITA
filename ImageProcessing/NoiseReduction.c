#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include "Pixels.h"

// Define smoothing parameters for the weights funciton
// Can (and should) be tweaked
#define SIGMA_D 12
#define SIGMA_R 16

// Returns intensity of the point (i, j) in source
int I(SDL_Surface *source, int i, int j) {
    return getPixel(source, i, j) & 0xFF;
}

// 
double distance(int i, int j, int k, int l) {
}

/* Calculate the weight of the point (i, j)
 * against another point (k, l)
 */
double calculateWeight(SDL_Surface *source, int i, int j, int k, int l) {
    return 0;
}
