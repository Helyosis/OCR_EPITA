#ifndef FLOODFILL_H
#define FLOODFILL_H

#include <SDL.h>
#include <sys/queue.h>

#include "Pixels.h"

struct PointQueue {
    Point point;

    SLIST_ENTRY(PointQueue) next;
};

SLIST_HEAD(slisthead, PointQueue);

size_t floodFill(SDL_Surface* src, Point seed,
                 uint32_t oldColor, uint32_t newColor);

#endif
