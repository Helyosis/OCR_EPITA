#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

typedef struct {
    double rho;
    double theta;
} rho_theta_tuple;

typedef struct {
    rho_theta_tuple* values;
    int nbLines;
} houghTransform_result;

/*
 * Perform the Hough Transform algorihthm on the SDL_Surface source
 * Returns the houghTransform_result struct containing the list of lines detected along with the numbers of lines
 */
houghTransform_result* HoughTransform(SDL_Surface *source);

/*
 * Draw the Hough Lines on the SDL_Surface source
 */
void DrawHoughlines(SDL_Surface *source, houghTransform_result *parameters);

/*
 * Free the houghTransform_result struct
 */
void free_houghTransform_result(houghTransform_result* to_free);

#endif
