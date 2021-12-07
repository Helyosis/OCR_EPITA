#ifndef UTILS_H
#define UTILS_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL.h>
#pragma GCC diagnostic pop

#ifdef DEFINE_GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif
// These defines are used for the main.c arguments parsing
#define FOREACH_MODE(MODE) \
        MODE(GUI)   \
        MODE(IMAGE) \
        MODE(TRAIN) \
        MODE(PREDICT)\

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum MODE_ENUM {
    FOREACH_MODE(GENERATE_ENUM)
};

EXTERN const char *MODE_STRING[];

#define NB_ENUM (sizeof(MODE_STRING) / sizeof(*MODE_STRING))

typedef struct {
    char* inputFile;
    char* outputFile;
    char* nnInputFile;
    int showImage;
    int nbIterations;
    int minibatch_size;
    int nbImages;
    double learningRate;
    enum MODE_ENUM mode;
} t_options;

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

/*
 * Convert a string to its uppercase equivalent
 * The same as applying toupper() to every char of the string
 */
void strtoupper(char* s);

#endif
