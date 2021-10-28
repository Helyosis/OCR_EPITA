#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include <stdint.h>
#pragma GCC diagnostic pop
#include "Pixels.h"

#define KUWAHARA_RANGE 4

#define GAUSSIAN_RANGE 5
#define GAUSSIAN_BLUR_SIGMA 1.4

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif

// Utility function to convert grayscale value to adequate ARGB pixel
uint32_t insensityToARGB(unsigned char v) {
    return (uint32_t) (0xFF << 24 | v << 16 | v << 8 | v);
}

// Returns the distance between the points (i, j) and the point (k, l)
double distance(int i, int j, int k, int l) {
    return sqrt((k - i) * (k - i) + (j - l) * (j - l));
}

/* Calculate the new value of the point on this point
 * It uses the Kuwahara Filtering technique to calculate it
 */
unsigned char Kuwahara_calculateValue(SDL_Surface *source, int x_start, int y_start) {
    double sums[4];
    double means[4];
    double sumDeviations[4];
    double stds[4];

    // First square
    int i = 0;
    sums[i] = 0;
    for (int x = x_start; x <= x_start + KUWAHARA_RANGE; ++x) {
        for (int y = y_start; y <= y_start + KUWAHARA_RANGE; ++y) {
            sums[i] += I(source, x, y);
        }
    }
    means[i] = sums[i] / ((KUWAHARA_RANGE + 1) * (KUWAHARA_RANGE + 1));
    sumDeviations[i] = 0;
    for (int x = x_start; x <= x_start + KUWAHARA_RANGE; ++x) {
        for (int y = y_start; y <= y_start + KUWAHARA_RANGE; ++y) {
            sumDeviations[i] += (I(source, x, y) - means[i]) * (I(source, x, y) - means[i]);
        }
    }
    stds[i] = sumDeviations[i] / ((KUWAHARA_RANGE + 1) * (KUWAHARA_RANGE + 1));


    // Second square
    i++;
    sums[i] = 0;
    for (int x = x_start - KUWAHARA_RANGE; x <= x_start; ++x) {
        for (int y = y_start; y <= y_start + KUWAHARA_RANGE; ++y) {
            sums[i] += I(source, x, y);
        }
    }
    means[i] = sums[i] / ((KUWAHARA_RANGE + 1) * (KUWAHARA_RANGE + 1));
    sumDeviations[i] = 0;
    for (int x = x_start - KUWAHARA_RANGE; x <= x_start + KUWAHARA_RANGE; ++x) {
        for (int y = y_start; y <= y_start + KUWAHARA_RANGE; ++y) {
            sumDeviations[i] += (I(source, x, y) - means[i]) * (I(source, x, y) - means[i]);
        }
    }
    stds[i] = sumDeviations[i] / ((KUWAHARA_RANGE + 1) * (KUWAHARA_RANGE + 1));

    // Third square
    i++;
    sums[i] = 0;
    for (int x = x_start - KUWAHARA_RANGE; x <= x_start; ++x) {
        for (int y = y_start - KUWAHARA_RANGE; y <= y_start; ++y) {
            sums[i] += I(source, x, y);
        }
    }
    means[i] = sums[i] / ((KUWAHARA_RANGE + 1) * (KUWAHARA_RANGE + 1));
    sumDeviations[i] = 0;
    for (int x = x_start; x <= x_start + KUWAHARA_RANGE; ++x) {
        for (int y = y_start - KUWAHARA_RANGE; y <= y_start; ++y) {
            sumDeviations[i] += (I(source, x, y) - means[i]) * (I(source, x, y) - means[i]);
        }
    }
    stds[i] = sumDeviations[i] / ((KUWAHARA_RANGE + 1) * (KUWAHARA_RANGE + 1));
    
    // Fourth square
    i++;
    sums[i] = 0;
    for (int x = x_start; x <= x_start + KUWAHARA_RANGE; ++x) {
        for (int y = y_start - KUWAHARA_RANGE; y <= y_start; ++y) {
            sums[i] += I(source, x, y);
        }
    }
    means[i] = sums[i] / ((KUWAHARA_RANGE + 1) * (KUWAHARA_RANGE + 1));
    sumDeviations[i] = 0;
    for (int x = x_start; x <= x_start + KUWAHARA_RANGE; ++x) {
        for (int y = y_start - KUWAHARA_RANGE; y <= y_start; ++y) {
            sumDeviations[i] += (I(source, x, y) - means[i]) * (I(source, x, y) - means[i]);
        }
    }
    stds[i] = sumDeviations[i] / ((KUWAHARA_RANGE + 1) * (KUWAHARA_RANGE + 1));

    // Sorry for making a completely horrible mess, simplest way to write (should refactor lator tho)

    double minStd = stds[0];
    double minMean = means[0];
    for (int i = 1; i < 4; ++i) {
        if (stds[i] < minStd) {
            minMean = means[i];
            minStd = stds[i];
        }
    }

    return minMean;
}


double** Build_GaussianKernel(int ksize) {
    // We scale the sigma value in proportion to the radius
    // Setting the minimum standard deviation as a baseline
    #ifdef GAUSSIAN_BLUR_SIGMA
    double sigma = GAUSSIAN_BLUR_SIGMA;
    #else
    double sigma = 0.3 * (((double) ksize - 1) * 0.5 - 1) + 0.8;
    #endif
    int kernelWidth = 2 * ksize + 1;

    // Initialise the kernel with all elements equals to 0
    double **kernel = malloc(sizeof(int*) * kernelWidth);
    for(int i = 0; i < kernelWidth; i++) {
        kernel[i] = malloc(sizeof(int*) * kernelWidth);

        for (int j = 0; j < kernelWidth; ++j) kernel[i][j] = 0;
    }

    for (int x = 0; x < kernelWidth; ++x) {
        for (int y = 0; y < kernelWidth; ++y) {
            kernel[x][y] = 0;
        }
    }

    double sum = 0;

    // x and y represents the value of the pixels AROUNG the center pixel
    for (int x = -ksize; x <= ksize; ++x) {
        for (int y = -ksize; y <= ksize; ++y) {
            double exponentNumerator = (double) (-(x * x + y *y));
            double exponentDenominator = 2 * sigma * sigma;
            double eExpression = exp(exponentNumerator / exponentDenominator);
            double kernelValue = (eExpression / 2 * M_PI);

            // We add radius to the indices to prevent out of bound issues because x and y can be negative
            kernel[x + ksize][y + ksize] = kernelValue;
            sum += kernelValue;
        }
    }

    // Normalize the kernel
    // This ensures that all of the values in the kernel together add up to 1
    for (int x = 0; x < kernelWidth; ++x) {
        for (int y = 0; y < kernelWidth; ++y) {
            kernel[x][y] /= sum;
        }
    }

    return kernel;
}

unsigned char GaussianBlur_calculateValue(SDL_Surface *source, int x_start, int y_start, double **kernel) {
    //int kernelWidth = 2 * GAUSSIAN_RANGE + 1;
    unsigned char sum = 0;

    for (int kernelY = -GAUSSIAN_RANGE; kernelY < GAUSSIAN_RANGE; ++kernelY) {
        for (int kernelX = -GAUSSIAN_RANGE; kernelX < GAUSSIAN_RANGE; ++kernelX) {
            double kernelValue = kernel[kernelX + GAUSSIAN_RANGE][kernelY + GAUSSIAN_RANGE];
            sum += I(source, x_start - kernelX, y_start - kernelY) * kernelValue;
        }
    }
    
    return sum;
}

SDL_Surface* GaussianBlur(SDL_Surface *source) {
    SDL_Surface *dest;
    dest = SDL_CreateRGBSurface (0, source->w, source->h, source->format->BitsPerPixel, 0, 0, 0, 0);
    double **kernel = Build_GaussianKernel(GAUSSIAN_RANGE);
    
    for (int y_start = GAUSSIAN_RANGE; y_start < source->h - GAUSSIAN_RANGE; ++y_start) {
        for (int x_start = GAUSSIAN_RANGE; x_start < source->w - GAUSSIAN_RANGE; ++x_start) {
            unsigned char intensity = GaussianBlur_calculateValue(source, x_start, y_start, kernel);
            putPixel(dest, x_start, y_start, insensityToARGB(intensity));
        }
    }

    for (int i = 0; i < 2 * GAUSSIAN_RANGE + 1; ++i) {
        free(kernel[i]);
    }
    free(kernel);
    
    return dest;
}

void GaussianBlur_inPlace(SDL_Surface *source) {
    SDL_Surface *dest;
    dest = GaussianBlur(source);
    
    for (int i = GAUSSIAN_RANGE; i < source->w - GAUSSIAN_RANGE; ++i) {
        for (int j = GAUSSIAN_RANGE; j < source->h - GAUSSIAN_RANGE; ++j) {
            uint32_t pixel = getPixel(dest, i, j);
            putPixel(source, i, j, pixel);
        }
    }

    SDL_FreeSurface(dest);
}

/* Returns a SDL_Surface corresponding to the source with reduced noise
 * using the Kuwahara filter technique, this technique is not bad but could be better
 */
SDL_Surface* KuwaharaFilter(SDL_Surface *source) {
    //RANGE = (source->w < source->h ? source->w : source->h) / 2 - 1;
    SDL_Surface *dest;
    dest = SDL_CreateRGBSurface (0, source->w, source->h, source->format->BitsPerPixel, 0, 0, 0, 0);
    for (int i = KUWAHARA_RANGE + 1; i < source->w - KUWAHARA_RANGE - 1; ++i) {
        for (int j = KUWAHARA_RANGE + 1; j < source->h - KUWAHARA_RANGE - 1; ++j) {
            unsigned char intensity = Kuwahara_calculateValue(source, i, j);
            putPixel(dest, i, j, insensityToARGB(intensity));
        }
    }

    return dest;
}

void KuwaharaFilter_inPlace(SDL_Surface *source) {
    SDL_Surface *dest;
    dest = KuwaharaFilter(source);
    
    for (int i = KUWAHARA_RANGE; i < source->w - KUWAHARA_RANGE; ++i) {
        for (int j = KUWAHARA_RANGE; j < source->h - KUWAHARA_RANGE; ++j) {
            uint32_t pixel = getPixel(dest, i, j);
            putPixel(source, i, j, pixel);
        }
    }

    SDL_FreeSurface(dest);
}
