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

int processImage(char* in_filename, char* out_filename) {
    SDL_Surface *original_image = IMG_Load(in_filename);
    SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(original_image);
    if (!image) { // Loading failed so ptr is null
        printf("[-] IMG_Load: %s\n", SDL_GetError());
        errx(1, "Error");
    }

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("My GUI lol",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          image->w, image->h, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);


    printf("[+] Successfully loaded %s (w=%d, h=%d)\n",
           in_filename, image->w, image->h);
    displaySurface(renderer, image);
    Apply_grayscale_filter(image);
    displaySurface(renderer, image);
    printf("[*] Applied grayscale\n");

    //wait_for_keypressed();
    GaussianBlur_inPlace(image);
    displaySurface(renderer, image);
    printf("[*] Reduced noise\n");

    //CannyFilter_inPlace(image);
    AdaptiveThresholding_inPlace(image);
    printf("[*] Applied adaptive threshold (mean - C method)\n");

    dilate_in_place(image);
    
    //MorphologyOpen(image);
    //MorphologyClose(image);

    printf("[*] Applied Noise Reduction2\n");

    //MorphologyOpen(image);
    //MorphologyClose(image);

    //wait_for_keypressed();

    BiggestBlob_result bb_res = findBiggestBlob(image);

    SDL_Surface* result = bb_res.res;
    
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

    SDL_FreeSurface(image);

    SDL_Quit();
    IMG_Quit();
    return 0;
}
