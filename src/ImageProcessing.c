#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
#include "Utils.h"
#include "Verbose.h"

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

    orderedPoints points = findGridCorner(image, renderer, options);
    log_s("Found corner with ul(x = %d, y = %d)", points.ul.x, points.ul.y);

    AdaptiveThresholding_inPlace(image);
    log_s("Applied adaptive threshold (mean - C method)");

    MorphologyClose(image);
    MorphologyOpen(image);
    log_s("Applied Morphology operations");
    
    warn_s("Perspective transformation is not implemented yet. Skipping.");

    if (options.showImage) {}
        drawLine(image, points.ul.x, points.ul.y, points.ur.x, points.ur.y, 0xff00ffff);
        drawLine(image, points.ur.x, points.ur.y, points.lr.x, points.lr.y, 0xff00ffff);
        drawLine(image, points.lr.x, points.lr.y, points.ll.x, points.ll.y, 0xff00ffff);
        drawLine(image, points.ll.x, points.ll.y, points.ul.x, points.ul.y, 0xff00ffff);
    
<<<<<<< HEAD
    erode_in_place(result);
    displaySurface(renderer, result);
    wait_for_keypressed();
    //return 1;

    //orderedPoints points = orderPoints(result);

    /*
    putPixel(result, points.ul.x, points.ul.y, 0xff0000ff);
    putPixel(result, points.lr.x, points.lr.y, 0xff00ff00);
    putPixel(result, points.ll.x, points.ll.y, 0xffff0000);
    putPixel(result, points.ur.x, points.ur.y, 0xff00ffff);
    */
    displaySurface(renderer, result);
    

    printf("[+] Drew Hough lines\n");

    printf("[-] Perspective transformation is not implemented yet. Skipping.\n");

    wait_for_keypressed();

    SDL_SaveBMP(result, out_filename);

    printf("Saved images !\n");

    //wait_for_keypressed();
=======
    SDL_SaveBMP(image, options.outputFile);
    info_s("Saved image under filename %s", options.outputFile);
>>>>>>> 25b9e7b99825274987f455889dfeeb626eff9fe7

    SDL_FreeSurface(image);

    SDL_Quit();
    IMG_Quit();
    return 0;
}

orderedPoints findGridCorner(SDL_Surface* image, SDL_Renderer* renderer, t_options options) {
    //TODO: Try to fix CannyFilter()
    SDL_Surface* lineImage = AdaptiveThresholding(image);
    
    dilate_in_place(image);

    BiggestBlob_result bb_res = findBiggestBlob(lineImage);
    log_s("Found the biggest blob starting at x = %d, y = %d, of size %d",
            bb_res.point.x, bb_res.point.y, bb_res.size);
    SDL_SaveBMP(bb_res.res, "out2.bmp");
    SDL_FreeSurface(lineImage);

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
