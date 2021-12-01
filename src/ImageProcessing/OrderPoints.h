#ifndef ORDERPOINTS_H
#define ORDERPOINTS_H

#include "Pixels.h"


/*
 * ul -> Upper left
 * ur -> Upper right
 * ll -> Lower left
 * lr -> Lower right
 */
typedef struct {
    Point ul;
    Point ur;
    Point ll;
    Point lr;
} orderedPoints;

/*
 * return four points representing the corner of the rectangle
 * Should be used ONLY on the result of a blob detection algorithm
 * or else it'll return non-sense
 */
orderedPoints orderPoints(SDL_Surface* src);

#endif
