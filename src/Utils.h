#ifndef UTILS_H
#define UTILS_H

/*
 * Clamp value such that it remains between min and max
 * If it is not between these boundaries, return either min or max (either if it too low or too high)
 */
int clamp(int value, int min, int max);

/*
 * Hangs program execution until the user press a key
 */
void wait_for_keypressed();

/*
 * Update the renderer to set the new surface as the only graphics rendered
 */
void displaySurface(SDL_Renderer* renderer, SDL_Surface* surface);

/*
 * Build a 2D Gaussian Kernel using the parameters provided
 */
double* Build2DGaussianKernel(size_t kernelRadius, double sigma);

#endif
