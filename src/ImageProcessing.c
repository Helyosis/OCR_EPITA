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
#include "Utils.h"
#include "Verbose.h"
#include "Result/construct.h"

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
    SDL_Renderer *renderer;
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


    Apply_grayscale_filter(image);
    log_s("Applied grayscale");

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

    if (options.showImage){
        displaySurface(renderer, image);
        wait_for_keypressed();
    }


    size_t threshold = 50;
    removeSmallBlob(image, threshold, WHITE, BLACK);
    log_s("Removed all blobs smaller than %d pixels", threshold);

    if (options.showImage) {
        displaySurface(renderer, image);
        wait_for_keypressed();
    }

    orderedPoints points = findGridCorner(image, renderer, options);
    log_s("Found corner with ul(x = %d, y = %d)", points.ul.x, points.ul.y);

    MorphologyClose(image);
    MorphologyOpen(image);
    log_s("Applied Morphology operations");

    warn_s("Perspective transformation is not implemented yet. Skipping.");
    
    SDL_Surface *Homographic = HomographicTransform(image, points, 252);

    if (options.showImage) {
        displaySurface(renderer, Homographic);
           wait_for_keypressed();
    }

    SDL_SaveBMP(Homographic, "Homographic.bmp");
 
    drawLine(image, points.ul.x, points.ul.y, points.ur.x, points.ur.y, 0xff00ffff);
    drawLine(image, points.ur.x, points.ur.y, points.lr.x, points.lr.y, 0xff00ffff);
    drawLine(image, points.lr.x, points.lr.y, points.ll.x, points.ll.y, 0xff00ffff);
    drawLine(image, points.ll.x, points.ll.y, points.ul.x, points.ul.y, 0xff00ffff);

    SDL_SaveBMP(image, options.outputFile);
    info_s("Saved image under filename %s", options.outputFile);

    cutSudoku(Homographic);
    log_s("Saved digits in directory %s", SC_DESTDIR);

    char sdk[] = "435269781682571493197834562826195347374682915951743628519326874248957136763418259";
    char srdb[] = "...26.7.168..7..9.19...45..82.1...4...46.29...5...3.28..93...74.4..5..367.3.18...";
    SDL_Surface *res = Result_construct(sdk, srdb);
    if(options.showImage) {
        displaySurface(renderer, res);
        wait_for_keypressed();
    }
    SDL_SaveBMP(res, "result.bmp");
    SDL_FreeSurface(res);
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
    SDL_SaveBMP(bb_res.res, "out2.bmp");

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
