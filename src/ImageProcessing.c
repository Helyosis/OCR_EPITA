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


    printf("[+] Successfully loaded %s (w=%d, h=%d)\n",
           in_filename, image->w, image->h);
    Apply_grayscale_filter(image);
    printf("[*] Applied grayscale\n");

    GaussianBlur_inPlace(image);
    printf("[*] Reduced noise\n");

    CannyFilter_inPlace(image);
    printf("[*] Applied adaptive threshold (mean - C method)\n");
    
    houghTransform_result* res = HoughTransform(image);
    DrawHoughlines(image, res);


    free_houghTransform_result(res);

    printf("[+] Drew Hough lines\n");
    printf("[-] Perspective transformation is not implemented yet. Skipping.\n");

    image = Rotation_shearing(image,105);
    IMG_SavePNG(image, out_filename);

    printf("Saved images !\n");

    SDL_Quit();
    IMG_Quit();
    return 0;
}
