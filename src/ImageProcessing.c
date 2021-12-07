#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
#pragma GCC diagnostic pop

#include "ImageProcessing/GrayScale.h"
#include "ImageProcessing/NoiseReduction.h"
#include "ImageProcessing/BlackAndWhite.h"
#include "ImageProcessing/CannyFilter.h"
#include "ImageProcessing/HoughTransform.h"
#include "ImageProcessing/Rotation.h"
#include "ImageProcessing/HoughTransform.h"
#include "ImageProcessing/Morphology.h"
#include "ImageProcessing/BlobDetection.h"
#include "ImageProcessing/OrderPoints.h"
#include "ImageProcessing/Pixels.h"
#include "ImageProcessing/HomographicTransphorm.h"
#include "ImageProcessing/Pixels.h"
#include "ImageProcessing/SquareCutter.h"
#include "ImageProcessing/FloodFill.h"
#include "Utils.h"
#include "Verbose.h"
#include "Result/construct.h"
#include "NeuralNetwork/NeuralNet.h"
#include "solver/sudoku.h"

orderedPoints findGridCorner(SDL_Surface* image, SDL_Renderer* renderer, t_options options);

int processImage(t_options options) {
    info_s("Running processImage with inputFile = %s", options.inputFile);
    SDL_Surface *original_image = IMG_Load(options.inputFile);
    SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(original_image);
    if (!image) { // Loading failed so ptr is null
        error_s("Failed to load image: %s", SDL_GetError());
    }

    SDL_Window *window;
    SDL_Renderer *renderer = NULL;
    if (options.showImage) {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("My GUI lol",
                                              SDL_WINDOWPOS_UNDEFINED,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              image->w, image->h, 0);
        renderer = SDL_CreateRenderer(window, -1, 0);
    }

    log_s("Successfully loaded %s (w=%d, h=%d)",
           options.inputFile, image->w, image->h);

    if (options.showImage) {
        displaySurface(renderer, image);
        wait_for_keypressed();
    }

    GaussianBlur_inPlace(image);

    log_s("Reduced noise");

    if (options.showImage) {
        displaySurface(renderer, image);
        wait_for_keypressed();
    }

    AdaptiveThresholding_inPlace(image);
    log_s("Applied adaptive threshold (mean - C method)");

    size_t threshold = 50;
    removeSmallBlob(image, threshold, WHITE, BLACK);
    log_s("Removed all blobs smaller than %d pixels", threshold);

    if (options.showImage) {
        displaySurface(renderer, image);
        wait_for_keypressed();
    }

    MorphologyClose(image);
    MorphologyOpen(image);
    log_s("Applied Morphology operations");

    orderedPoints points = findGridCorner(image, renderer, options);
    log_s("Found corner with ul(x = %d, y = %d)", points.ul.x, points.ul.y);
    
    floodFill(image, points.ul, WHITE, BLACK);
    log_s("Removed the grid");

    if (options.showImage) {
        displaySurface(renderer, image);
        wait_for_keypressed();
    }

    SDL_Surface *Homographic = HomographicTransform(image, points, 252);
    log_s("Applied homographic transform");

    if (options.showImage) {
        displaySurface(renderer, Homographic);
           wait_for_keypressed();
    }

    SDL_SaveBMP(Homographic, "Homographic.bmp");

    SDL_SaveBMP(image, options.outputFile);
    info_s("Saved image under filename %s", options.outputFile);

    cutSudoku(Homographic);
    log_s("Saved digits in directory %s", SC_DESTDIR);


    //double *resultnet = predict(/*le path de l'image, l'endroit où ils ont été sauvegardé*/);
    //double max = 0;
    //for(int i = 0; i < 8; i++){
    //    if(resultnet[i] > max){
    //        max = resultnet[i];
    //    }
    //}
    char *startingGrid = calloc(82, sizeof(char));
    for(int i = 0; i < 82; i++){
        startingGrid[i] = '.';
    }
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            char *path = malloc(60);
            sprintf(path, "%s/x%d-y%d.bmp", SC_DESTDIR, i, j);
            double resultnet[] = {0.0};//predict(path, PATH_WEIGHT);
            double max = -1;
            for (size_t k = 0; k < 8; k++)
            {
                if(resultnet[k] > max){
                    if (!AlreadyInColumn(startingGrid, 3, 3, i, (int)resultnet[k]) && !AlreadyInLine(startingGrid, 3, j, (int)resultnet[k]) && !AlreadyInSquare(startingGrid, 3, i, j, (int)startingGrid[k])){
                       max = resultnet[k];
                    }
                }
                
            }
            startingGrid[i*9+j] = (char)((int)(max) + '0');

            free(path);
        }
    }
    char *solved = calloc(82, sizeof(char));
    strcpy(solved, startingGrid);
    Solve(solved, 3, 3);
    SDL_Surface *resu = Result_construct(solved, startingGrid);
    if(options.showImage) {
        displaySurface(renderer, resu);
        wait_for_keypressed();
    }

    SDL_SaveBMP(resu, "result.bmp");
    SDL_FreeSurface(resu);
    SDL_FreeSurface(image);
    SDL_FreeSurface(Homographic);
    
    SDL_Quit();
    IMG_Quit();
    
    return 0;
}

orderedPoints findGridCorner(SDL_Surface* image, SDL_Renderer* renderer, t_options options) {
    dilate_in_place(image);

    BiggestBlob_result bb_res = findBiggestBlob(image);
    log_s("Found the biggest blob starting at x = %d, y = %d, of size %d",
            bb_res.point.x, bb_res.point.y, bb_res.size);

    SDL_Surface* result = bb_res.res;

    erode_in_place(result);
    
    orderedPoints points = orderPoints(result);

    if (options.showImage) {
        displaySurface(renderer, result);
        wait_for_keypressed();
    }
    SDL_FreeSurface(result);

    return points;
}
