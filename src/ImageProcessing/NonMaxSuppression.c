#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#include <err.h>
#pragma GCC diagnostic pop

#ifndef M_PI
#define M_PI 3.14159265359
#endif

// https://gist.github.com/FienSoP/03ed9b0eab196dde7b66f452725a42ac#file-nonmaxsuppression-py
double* NonMaxSuppression(double* img, double* D, int h, int w) {
    double* Z = calloc(h * w, sizeof(*Z));
    double* angle = calloc(h * w, sizeof(*angle));

    for (int i = 0; i < h * w; ++i) {
        angle[i] = D[i] * 180 / M_PI;
        if (angle[i] < 0) angle[i] += 180;
    }

    for (int i = 1; i < h - 1; ++i) {
        for (int j = 1; j < w - 1; ++j) {
            int q = 255;
            int r = 255;
            double curr_angle = angle[i * w + h];

            // angle ~= 0
            if ((0 <= curr_angle && curr_angle < 22.5) ||
                (157.5 <= curr_angle && curr_angle <= 180))
            {
                q = img[i * w + j + 1];
                r = img[i * w + j - 1];
            }

            // angle ~= 45
            if (22.5 <= curr_angle && curr_angle < 67.5)
            {
                q = img[(i + 1) * w + j - 1];
                r = img[(i - 1) * w + j + 1];
            }

            // angle ~= 90
            if (67.5 <= curr_angle && curr_angle < 112.5)
            {
                q = img[(i + 1) * w + j];
                r = img[(i - 1) * w + j];
            }

            // angle ~= 135
            if (112.5 <= curr_angle && curr_angle < 157.5)
            {
                q = img[(i - 1) * w + j - 1];
                r = img[(i + 1) * w + j + 1];
            }

            if (img[i * w + j] >= q && img[i * w + j] >= r)
                Z[i * w +j] = img[i * w + j];
        }
    }
    free(angle);
    return Z;
}
