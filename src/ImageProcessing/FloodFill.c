#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#include <sys/queue.h>
#include <err.h>
#pragma GCC diagnostic pop

#include "Pixels.h"
#include "FloodFill.h"

size_t floodFill(SDL_Surface* src, Point seed,
                   uint32_t oldColor, uint32_t newColor) {

    struct slisthead head;
    SLIST_INIT(&head);

    struct PointQueue* p = malloc(sizeof(*p));
    p->point = seed;
    SLIST_INSERT_HEAD(&head, p, next);

    size_t nbPoint = 0;
    while (!SLIST_EMPTY(&head)) {
        p = SLIST_FIRST(&head);
        SLIST_REMOVE_HEAD(&head, next);

        int x = p->point.x;
        int y = p->point.y;
        free(p);
        // Invalid point, we ignore
        if (x < 0 || x >= src->w || y < 0 || y >= src->h) {
            continue;
        }
        if (getPixel(src, x, y) == oldColor) {
            putPixel(src, x, y, newColor);
            ++nbPoint;

            p = malloc(sizeof(*p));
            p->point.x = x - 1;
            p->point.y = y;
            SLIST_INSERT_HEAD(&head, p, next);

            p = malloc(sizeof(*p));
            p->point.x = x + 1;
            p->point.y = y;
            SLIST_INSERT_HEAD(&head, p, next);

            p = malloc(sizeof(*p));
            p->point.x = x;
            p->point.y = y - 1;
            SLIST_INSERT_HEAD(&head, p, next);

            p = malloc(sizeof(*p));
            p->point.x = x;
            p->point.y = y + 1;
            SLIST_INSERT_HEAD(&head, p, next);
        }
    }

    
    return nbPoint;
}
