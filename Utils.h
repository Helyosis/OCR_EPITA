#ifndef UTILS_H
#define UTILS_H

/*
 * Clamp value such that it remains between min and max
 * If it is not between these boundaries, return either min or max (either if it too low or too high)
 */
int clamp(int value, int min, int max);

#endif