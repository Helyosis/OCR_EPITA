<<<<<<< HEAD
=======
#include <stdint.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "Includes/stb/stb_image.h" // Library used for images

uint8_t* load_image(string filename) {
    int width;
    int height;
    int bpp;

    // Don't forget to free after loading
    uint8_t* rgb_image = stdbi_load(filename, &width, &height, &bpp);

    struct image imageStruct* = (image *) malloc(sizeof(struct image));
    imageStruct->rgb_image = rgb_image;
    imageStruct->width = width;
    imageStruct->height = height;
    imageStruct->bpp = bpp;

    return imageStruct*;
}

void free_image(struct image imageStruct) {
    printf("Freeing image (w=%d, h=%d, bpp=%d) to grayscale.",
           imageStruct.width,
           imageStruct.height,
           imageStruct.bpp
        );
    

}

void image_to_grayscale(struct image imageStruct*) {
    printf("Converting image (w=%d, h=%d, bpp=%d) to grayscale.",
           imageStruct.width,
           imageStruct.height,
           imageStruct.bpp
        );
}
>>>>>>> 4ec1a5b (Change newline ?)
