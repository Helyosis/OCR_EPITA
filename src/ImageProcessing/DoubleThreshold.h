#ifndef DOUBLETHRESHOLD_H
#define DOUBLETHRESHOLD_H

#include <SDL.h>

/* Categorize each pixels of img into 3 categories
 *   - Strong: The pixel's intensity is high enough to be sure it is an edge
 *   - Weak: The intensity if not high enough but not low enough to be sure
 *   - Zero: The intensity is near 0, we know for sure it is not an edge
 * We do this to help binarize the image later, we just care about the weak points
 */
double* DoubleThreshold(double* img, int nb_pixels);

/* Transform every weak pixels into either strong or zero.
 * If a pixel is around (diagonals included) at least 1 strong pixel, it is strong
 * Else, it is not strong therefore zero
 */
double* Hysteresis(double* img, int h, int w);

#endif
