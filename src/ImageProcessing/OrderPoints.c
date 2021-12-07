#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#pragma GCC diagnostic pop

#include "OrderPoints.h"


orderedPoints orderPoints(SDL_Surface* src) {
    // The top left (or lower right) will have the mimimum (or maximum) sum of x + y
    size_t minSum = src->w + src->h; // By default we'll say it's far right
    Point ul = {0, 0};

    size_t maxSum = 0;
    Point lr = {0, 0};

    int minDiff = src->w > src->h ? src->w : src->h;
    Point ll = {0, 0};

    int maxDiff = -1 * minDiff;
    Point ur = {0, 0};

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
                ll.x = x; ll.y = y;
            }

            if (diff > maxDiff) {
                maxDiff = diff;
                ur.x = x; ur.y = y;
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
