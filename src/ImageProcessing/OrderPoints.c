#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#pragma GCC diagnostic pop

#include "OrderPoints.h"


orderedPoints orderPoints(SDL_Surface* src) {
    // The top left (or lower right) will have the mimimum (or maximum) sum of x + y
    size_t minSum = src->w + src->h; // By default we'll say it's far right
    Point ul;

    size_t maxSum = 0;
    Point lr;

    int minDiff = src->w > src->h ? src->w : src->h;
    Point ur;

    int maxDiff = -1 * minDiff;
    Point ll;

    for (int y = 0; y < src->h; ++y) {
        for (int x = 0; x < src->w; ++x) {
            // We don't count non-white pixels
            if (I(src, x, y) != 0xff) continue;

            size_t sum = x + y;
            int diff = x - y;

            if (sum < minSum) {
                minSum = sum;
                ul.x = x; ul.y = y;
            }

            if (sum >= maxSum) {
                maxSum = sum;
                lr.x = x; lr.y = y;
            }

            if (diff < minDiff) {
                minDiff = diff;
                ur.x = x; ur.y = y;
            }

            if (diff > maxDiff) {
                maxDiff = diff;
                ll.x = x; ll.y = y;
            }
        }
    }

    orderedPoints result;
    result.ul = ul;
    result.lr = lr;
    result.ll = ll;
    result.ur = ur;

    return result;
}
