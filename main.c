<<<<<<< HEAD
#include <stdio.h>
#include "test_app.h"

int main() {
    printf("hey salut :)");
    
    return 0;
}
=======
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <err.h>
#include <stdio.h>
#include "ImageProcessing/GrayScale.h"
#include "ImageProcessing/NoiseReduction.h"
#include "ImageProcessing/BlackAndWhite.h"
#include "ImageProcessing/HoughTransform.h"
#include "Utils.h"

void displaySurface(SDL_Renderer* renderer, SDL_Surface* surface) {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int main(int argc, char **argv)
{
    if (argc == 1)
        errx(1, "Please specify a image path, (optional: with a save path)");

    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);



    SDL_Surface *original_image = IMG_Load(argv[1]);
    SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(original_image);

    if (!image) { // Loading failed so ptr is null
        printf("[-] IMG_Load: %s\n", SDL_GetError());
        errx(1, "Error");
    }
    printf("[+] Successfully loaded %s (w=%d, h=%d)\n",
           argv[1], image->w, image->h);

    SDL_Window *window =
        SDL_CreateWindow("My GUI lol",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         image->w, image->h, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    wait_for_keypressed();
    Apply_grayscale_filter(image);
    printf("[*] Applied grayscale\n");
    
    displaySurface(renderer, image);

    wait_for_keypressed();
    GaussianBlur_inPlace(image);
    printf("[*] Reduced noise\n");

    displaySurface(renderer, image);
    
    wait_for_keypressed();
    AdaptiveThresholding_inPlace(image);
    printf("[*] Applied adaptive threshold (mean - C method)\n");

    displaySurface(renderer, image);
    
    //wait_for_keypressed();
    //HoughTransform(image);
    //printf("[*] Applied Hough Transform\n");
    printf("[-] Hough Transform is not implemented yet. Ignoring it.\n");
    printf("[-] Boundaries detection is not implemented yet. Ignoring it.\n");
    printf("[-] Perspective transform is not implemented yet. Ignoring it.\n");

     
    SDL_SaveBMP(image, argc > 2 ? argv[2] : "out.bmp");
    printf("Saved images !\n");
    //errx(0, "Delete me after"); // DELETE ME AFTER TESTS

    while (!quit)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }
    }

    SDL_Quit();
    IMG_Quit();
    return 0;
}
>>>>>>> 0d24310 (Refactor code)
