#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#include <sys/queue.h>
#pragma GCC diagnostic pop

#include "Pixels.h"
#include "FloodFill.h"

size_t __floodFill(SDL_Surface src, Point seed,
                   unsigned char oldColor, unsigned char newColor) {
    SLIST_HEAD(, PointQueue) head;
    SLIST_INIT(&head);

    SLIST_INSERT_HEAD(&head, seed, next); // On insere seed dans la queue

    Point nextPoint = {3, 42};
    SLIST_INSERT_HEAD(&head, nextPoint, next);

    SLIST_FOREACH(p, &head, next) {
/* ici p est l'element courant */
        printf("element : %d - %d\n", p.x, p.y);
    }
}
