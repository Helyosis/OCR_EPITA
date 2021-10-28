#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL.h>
#include <math.h>
#include <err.h>
#pragma GCC diagnostic pop

#include "Pixels.h"
#include "HoughTransform.h"
#include "../Utils.h"

#ifndef M_PI
#define M_PI 3.1415927
#endif

#define RAD(A)  (M_PI*((double)(A))/180.0)

void free_houghTransform_result(houghTransform_result *to_free) {
    free(to_free->values);
    free(to_free);
}

houghTransform_result* HoughTransform(SDL_Surface *source) {
    int Ny = source->h;
    int Nx = source->w;

    double rho   = 1.0;
    double theta = 1.0;

    int Ntheta = 180 / theta;
    int Nrho   = (int) sqrt(Nx * Nx + Ny * Ny) / rho;

    double dtheta = M_PI / (double)Ntheta;
    double drho   = sqrt(Nx * Nx + Ny * Ny) / Nrho;

    long* accum = calloc(Ntheta * Nrho, sizeof(*accum));
    if (accum == NULL) errx(-1, "Wasn't able to allocate accum");

    for (int y = 0; y < Ny; ++y) {
        for (int x = 0; x < Nx; ++x) {
            int pixelIntensity = I(source, x, y);
            if (pixelIntensity != 0 && pixelIntensity != 0xFF)
                warn("pixel at image(y=%d, x=%d) is not black or white",
                     y, x);

            if (I(source, x, y) == 0xFF) { // White pixel
                for (int itheta = 0; itheta < Ntheta; ++itheta) {
                    double theta = itheta * dtheta;
                    double rho   = x * cos(theta) + (Ny - y) * sin(theta);
                    int    irho  = rho / drho;
                    if (irho > 0 && irho < Nrho)
                        ++accum[itheta * Nrho + irho];
                }
            }
        }
    }

    // We only keep the highest values
    int threshold = 600;
    long nbLines = 0;
    for (int i = 0; i  < Ntheta * Nrho; i += 1) {
        if (accum[i] < threshold) {
            accum[i] = 0;
        } else {
            ++nbLines;
        }
    }

    rho_theta_tuple* lines = calloc(nbLines, sizeof(*lines));
    if (lines == NULL) errx(-1, "Wasn't able to allocate lines");
    int lineIndex = 0;
    for (int itheta = 0; itheta < Ntheta; ++itheta) {
        for (int irho = 0; irho < Nrho; ++irho) {
            if (accum[itheta * Nrho + irho] != 0) {
                lines[lineIndex].rho = irho * drho;
                lines[lineIndex++].theta = itheta * dtheta;
            }
        }
    }

    houghTransform_result* result = malloc(sizeof(*result));
    if (result == NULL) errx(-1, "Wasn't able to allocate houghTransorm_result");
    result->values = lines;
    result->nbLines = nbLines;

    free(accum);

    return result;
}

void DrawHoughlines(SDL_Surface *source, houghTransform_result *parameters) {
    int nbLines = parameters->nbLines;
    rho_theta_tuple *lines = parameters->values;

    for (int i = 0; i < nbLines; ++i) {
        double rho   = lines->rho;
        double theta = lines->theta;
        lines++; // Next element

        printf("Lines with rho=%f and theta=%f\n", rho, theta);

        double a = cos(theta);
        double b = sin(theta);

        int x0 = floor(a * rho);
        int y0 = floor(b * rho);
        int x1 = clamp(floor(x0 + 1000 * (-b)), 0, source->w - 1);
        int y1 = clamp(floor(y0 + 1000 * a)   , 0, source->h - 1);
        int x2 = clamp(floor(x0 - 1000 * (-b)), 0, source->h - 1);
        int y2 = clamp(floor(y0 - 1000 * a)   , 0, source->h - 1);

        drawLine(source, x1, y1, x2, y2, 0xFF00FFFF);
    }
}
