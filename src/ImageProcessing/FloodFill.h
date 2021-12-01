#ifndef FLOODFILL_H
#define FLOODFILL_H

#include "Pixels.h"

typedef struct PointQueue {
    Point point;

    SLIST_ENTRY(PointQueue) next;
} PointQueue

#endif
