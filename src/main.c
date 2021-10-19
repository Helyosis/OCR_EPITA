#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <err.h>
#include <stdio.h>
#include "ImageProcessing/GrayScale.h"
#include "ImageProcessing/NoiseReduction.h"
#include "ImageProcessing/BlackAndWhite.h"
#include "ImageProcessing/HoughTransform.h"
#include "ImageProcessing/Rotation.h"

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
    /*Apply_grayscale_filter(image);
    printf("[*] Applied grayscale\n");
    GaussianBlur_inPlace(image);
    printf("[*] Reduced noise\n");
    AdaptiveThresholding_inPlace(image);
    printf("[*] Applied adaptive threshold (mean - C method)\n");

    printf("[-] Hough transform is not implemented yet. Skipping.\n");
    printf("[-] Perspective transformation is not implemented yet. Skipping.\n");
    */
    SDL_Surface *img = Rotation_shearing(image,45);
    SDL_SaveBMP(Rotation_shearing(image,45), argc > 2 ? argv[2] : "out.bmp");

    printf("Saved images !\n");

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

    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_Quit();
    IMG_Quit();
    return 0;
}
