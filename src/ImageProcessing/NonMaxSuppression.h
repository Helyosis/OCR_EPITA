#ifndef NONMAXSUPPRESSION_H
#define NONMAXSUPPRESSION_H

#include <SDL.h>

/* Source: https://gist.github.com/FienSoP/03ed9b0eab196dde7b66f452725a42ac#file-nonmaxsuppression-py
 * Remove all non_maximum line, using the results from Sobel filters.
 * For more info, see: https://towardsdatascience.com/canny-edge-detection-step-by-step-in-python-computer-vision-b49c3a2d8123
 */
double* NonMaxSuppression(double* img, double* D, int h, int w);

#endif
