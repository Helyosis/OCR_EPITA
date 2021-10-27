#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

typedef struct {
    double rho;
    double theta;
} rho_theta_tuple;

typedef struct {
    rho_theta_tuple* values;
    int nbLines;
} houghTransorm_result;

/*
 * Perform the Hough Transform algorihthm on the SDL_Surface source
 * Returns the houghTransform_result struct containing the list of lines detected along with the numbers of lines
 */
houghTransorm_result* HoughTransform(SDL_Surface *source);

/*
 * Draw the Hough Lines on the SDL_Surface source
 */
void DrawHoughlines(SDL_Surface *source, houghTransorm_result *parameters);

#endif
