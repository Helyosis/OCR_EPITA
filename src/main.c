#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <err.h>
#include <stdio.h>
#include "ImageProcessing/GrayScale.h"
#include "ImageProcessing/NoiseReduction.h"
#include "ImageProcessing/BlackAndWhite.h"
#include "ImageProcessing/HoughTransform.h"
<<<<<<< HEAD:main.c
#include "Utils.h"
#include "UserInterface/interface.h"

void displaySurface(SDL_Renderer* renderer, SDL_Surface* surface) {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
=======
#include "ImageProcessing/Rotation.h"
>>>>>>> origin/dev:src/main.c

int main(int argc, char **argv)
{
    if (argc == 1)
        errx(1, "Please specify a image path, (optional: with a save path)");
    initInterface(argc,argv);
    bool quit = false;
    SDL_Event event;
<<<<<<< HEAD:main.c

    SDL_Init(SDL_INIT_VIDEO);



=======
    
    SDL_Init(SDL_INIT_VIDEO);
    
>>>>>>> origin/dev:src/main.c
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
<<<<<<< HEAD:main.c

    SDL_Window *window =
        SDL_CreateWindow("My GUI lol",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         image->w, image->h, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    wait_for_keypressed();
    Apply_grayscale_filter(image);
=======
    /*Apply_grayscale_filter(image);
>>>>>>> origin/dev:src/main.c
    printf("[*] Applied grayscale\n");
    
    displaySurface(renderer, image);

    wait_for_keypressed();
    GaussianBlur_inPlace(image);
    printf("[*] Reduced noise\n");

    displaySurface(renderer, image);
    
    wait_for_keypressed();
    AdaptiveThresholding_inPlace(image);
    printf("[*] Applied adaptive threshold (mean - C method)\n");

<<<<<<< HEAD:main.c
    displaySurface(renderer, image);
    
    //wait_for_keypressed();
    //HoughTransform(image);
    //printf("[*] Applied Hough Transform\n");
    printf("[-] Hough Transform is not implemented yet. Ignoring it.\n");
    printf("[-] Boundaries detection is not implemented yet. Ignoring it.\n");
    printf("[-] Perspective transform is not implemented yet. Ignoring it.\n");
=======
    printf("[-] Hough transform is not implemented yet. Skipping.\n");
    printf("[-] Perspective transformation is not implemented yet. Skipping.\n");
    */
    SDL_Surface *img = Rotation_shearing(image,90);
    SDL_SaveBMP(Rotation_shearing(image,90), argc > 2 ? argv[2] : "out.bmp");
>>>>>>> origin/dev:src/main.c

     
    SDL_SaveBMP(image, argc > 2 ? argv[2] : "out.bmp");
    printf("Saved images !\n");
<<<<<<< HEAD:main.c
    //errx(0, "Delete me after"); // DELETE ME AFTER TESTS
=======

    SDL_Window *window =
        SDL_CreateWindow("My GUI lol",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         image->w, image->h, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);

    SDL_Texture *canvas = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        image->w, image->h);
>>>>>>> origin/dev:src/main.c

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
