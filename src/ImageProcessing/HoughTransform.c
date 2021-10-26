#include <SDL.h>
#include <math.h>
#include "Pixels.h"
#include "HoughTransform.h"
#include "../Utils.h"

#ifndef M_PI
#define M_PI 3.1415927
#endif

#define RAD(A)  (M_PI*((double)(A))/180.0)


houghTransorm_result* HoughTransform(SDL_Surface *source) {
    
    int x_max = source->w;
    int y_max = source->h;

    double theta_max = M_PI; 
    double theta_min = 0f;

    double r_min = 0f;
    double r_max = sqrt(x_max * x_max +  y_max * y_max);

    int r_dim = 200; 
    int theta_dim = 300;

    uint32_t* hough_accumulator = calloc(r_dim * theta_dim, sizeof(uint32_t));

    for (int y = 0; y < y_max; ++y) {
        for (int x = 0; x < x_max; ++x) {
            
            for (int i_theta = 0; i_theta < theta_dim; ++i_theta) {
                
            }
        }
    }

    // We only save the points with a high enough score.
    // Each of these saved points will represent one line
    uint32_t threshold = 130;
    int nbLines = 0;
    for (int i = 0; i < Ntheta * Nrho; ++i) {
        if (hough_accumulator[i] < threshold) {
            hough_accumulator[i] = 0;
        } else {
            nbLines++;
        }
    }

    rho_theta_tuple* lines = calloc(nbLines * sizeof(rho_theta_tuple), sizeof(rho_theta_tuple));
    rho_theta_tuple* i = lines;
    for (int i_theta = 0; i_theta < Ntheta; ++i_theta) {
        for (int i_rho = 0; i_rho < Nrho; ++i_rho) {
            if (hough_accumulator[Ntheta * i_theta + i_rho]) {
                i->theta = i_theta * dtheta;
                i->rho = i_rho * drho;
                ++i;;
            }
        }
    }

    free(hough_accumulator);
    houghTransorm_result* result = calloc(1, sizeof(result));
    result->values = lines;
    result->nbLines = nbLines;
    return result;
}

void DrawHoughlines(SDL_Surface *source, houghTransorm_result *parameters) {
    int nbLines = parameters->nbLines;
    rho_theta_tuple *lines = parameters->values;

    for (int i = 0; i < nbLines; ++i) {
        double rho   = lines->rho;
        double theta = lines->theta;
        lines++; // Next element

        printf("Lines with rho=%f and theta=%f", rho, theta);

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