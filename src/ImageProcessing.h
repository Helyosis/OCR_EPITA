#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include "Utils.h"
#include "ImageProcessing/OrderPoints.h"
int processImage(t_options options);
orderedPoints findGridCorner(SDL_Surface* image, SDL_Renderer* renderer, t_options options);

#endif
