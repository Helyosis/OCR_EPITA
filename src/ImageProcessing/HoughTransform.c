#include <SDL2/SDL.h>
#include <math.h>
#include "Pixels.h"
#include <err.h>
#include "../Utils.h"

#ifndef M_PI
#define M_PI 3.1415927
#endif

#define RAD(A)  (M_PI*((double)(A))/180.0)

typedef struct {
    double rho;
    double theta;
} rho_theta_tuple;

void HoughTransform(SDL_Surface *source) {
    double rho = 1.0;
    double theta = 1.0;

    int Ntheta = floor(180.0 / theta);
    int Nrho   = floor(sqrt(source->w * source->w + source->h * source->h)) / rho;

    double dtheta = M_PI / Ntheta;
    double drho   = sqrt(source->w * source->w + source->h * source->h) / Nrho;

    uint32_t* hough_accumulator = malloc(Ntheta * Nrho);

    for (int y = 0; y < source->h; ++y) {
        for (int x = 0; x < source->w; ++x){
            if (I(source, x, y)) { // Not a white pixel (so a black pixel, i.e. an edge)
                for (int i_theta = 0; i_theta < Ntheta; ++i_theta) {
                    theta = i_theta * dtheta;
                    rho = i_theta * cos(theta) + (source->h - y) * sin(theta);

                    int i_rho = floor(rho / drho);

                    if (i_rho > 0 && i_rho < Nrho)
                        hough_accumulator[Ntheta * i_theta + i_rho]++;
                }
            }
        }
    }

    // We only save the points with a high enough score.
    // Each of these saved points will represent one line
    uint32_t threshold = 1000000;
    for (int i = 0; i < Ntheta * Nrho; ++i) {
        if (hough_accumulator[i] < threshold) hough_accumulator[i] = 0;
    }

    rho_theta_tuple* lines = malloc(Ntheta * Nrho * sizeof(rho_theta_tuple));
    if (lines == NULL)
        errx(1, "Cannot alocate memory.");
    int line_index = 0;
    for (int i_theta = 0; i_theta < Ntheta; ++i_theta) {
        for (int i_rho = 0; i_rho < Nrho; ++i_rho) {
            if (hough_accumulator[Ntheta * i_theta + i_rho]) {
                lines[line_index].theta = i_theta * dtheta;
                lines[line_index].rho = i_rho * drho;
                ++line_index;;
            }
        }
    }

    for(int i = 0; i < line_index; ++i) {
        double theta = lines[i].theta;
        double rho   = lines[i].rho;
        double a = cos(theta);
        double b = sin(theta);
        double x0 = a*rho;
        double y0 = b*rho;
        int x1 = clamp(x0 + 1000*(-b), 0, source->w - 1);
        int y1 = clamp(y0 + 1000*(a),  0, source->h - 1);
        int x2 = clamp(x0 - 1000*(-b), 0, source->w - 1);
        int y2 = clamp(y0 - 1000*(a),  0, source->h - 1);

        drawLine(source, x1, y1, x2, y2, 0xff00ffff);
    }
}
