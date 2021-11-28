#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include <stdint.h>
#pragma GCC diagnostic pop

#include "Pixels.h"
#include "../Utils.h"

#define KUWAHARA_RANGE 4

#define GAUSSIAN_RANGE 2
#define GAUSSIAN_BLUR_SIGMA 1.3

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif



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


double Gaussian(double x, double sigma) {
    double x2 = x * x;
    double sigma2 = sigma * sigma;
    return exp(-x2 / (2 * sigma2));
}

double* Build_GaussianKernel() {
    int kernelSize = 2 * GAUSSIAN_RANGE + 1;
    double sum = 0;
    double* kernel = malloc(kernelSize * sizeof(*kernel));

    for (int x = 0; x < kernelSize; ++x) {
        double value = Gaussian(x - GAUSSIAN_RANGE, GAUSSIAN_BLUR_SIGMA);
        kernel[x] = value;
        sum += value;
    }

    for (int x = 0; x < kernelSize; ++x) {
        kernel[x] /= sum;
    }

    return kernel;
}

SDL_Surface* GaussianBlur(SDL_Surface *source) {
    SDL_Surface* dest =
        SDL_CreateRGBSurface (0, source->w, source->h,
                              source->format->BitsPerPixel,
                              source->format->Rmask,
                              source->format->Gmask,
                              source->format->Bmask,
                              source->format->Amask
            );

    double* kernel = Build_GaussianKernel();
    int kernelSize = 2 * GAUSSIAN_RANGE + 1;

    unsigned char* tempData = malloc(source->w * source->h * sizeof(*tempData));

    // First pass
    for (int y = 0; y < source->h; ++y) {
        for (int x = GAUSSIAN_RANGE; x < source->w - GAUSSIAN_RANGE; ++x) {
            double sum = 0;
            for (int w = 0; w < kernelSize; ++w) {
                sum += I(source, x + w - GAUSSIAN_RANGE, y) * kernel[w];
            }
            tempData[y * source->w + x] = clamp(sum, 0, 255);
        }
    }

    // Second pass
    for (int y = GAUSSIAN_RANGE; y < source->h - GAUSSIAN_RANGE; ++y) {
        for (int x = GAUSSIAN_RANGE; x < source->w - GAUSSIAN_RANGE; ++x) {
            double sum = 0;
            for (int w = 0; w < kernelSize; ++w) {
                sum += tempData[(y + w - GAUSSIAN_RANGE) * source->w + x] * kernel[w];
            }
            putPixel(dest, x, y, intensityToARGB(clamp(sum, 0, 255)));
        }
    }

    free(kernel);
    free(tempData);

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
            putPixel(dest, i, j, intensityToARGB(intensity));
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
