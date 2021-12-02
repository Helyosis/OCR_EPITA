#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>
#pragma GCC diagnostic pop

#include "Pixels.h"

// Define Pi
#define M_PI 3.14159265


// Image manipulation functions

//rotation, rotate an image in place
//@param image: the image to rotate
//@param angle: the angle to rotate the image by
//@return: the rotated image
SDL_Surface *rotation(SDL_Surface *image, double angle){
    //Get the image dimensions
    int w = image->w;
    int h = image->h;

    //Create a new surface to hold the rotated image
    SDL_Surface *rotated_image; 
    rotated_image = SDL_CreateRGBSurfaceWithFormat(0, image->w, image->h, image->format->BitsPerPixel, image->format->format);

    double angle_rad = angle * (M_PI / 180); //convert the angle from degrees to radians
    int center_x = w/2;
    int center_y = h/2;
    double cos_angle = cos(angle_rad);
    double sin_angle = sin(angle_rad);
    for(int x = 0; x < w;x++){
        for(int y = 0; y < h; y++){
            // Computes the offsets from the center of the image
            double xOff = (x - center_x);
            double yOff = (y - center_y);
            // Compute the coordinates from the image
            int new_x = round(xOff * cos_angle + yOff * sin_angle + center_x);
            int new_y = round(yOff * cos_angle - xOff * sin_angle + center_y);
            // Check if the new coordinates are within the image
            if(0 <= new_x && new_x < w && 0 <= new_y && new_y < h){
                // Copy the pixel from the old image to the new image
                Uint32 pixel = getPixel(image, x, y);
                putPixel(rotated_image, new_x, new_y, pixel);
            }
        }
    }
    SDL_FreeSurface(image);
    return rotated_image;
}


//Rotation_shearing, rotate an image and shear it
//@param image: the image to rotate
//@param angle: the angle to rotate the image by
//@return: the rotated and sheared image

SDL_Surface *Rotation_shearing(SDL_Surface *image, double angle){
    //Get the image dimensions
    int w = image->w;
    int h = image->h;
    //Create a new surface to hold the rotated image
    SDL_Surface *rotated_image; 
    rotated_image = SDL_CreateRGBSurface(0, w, h, 32,0,0,0,0);

    double angle_rad = angle * (M_PI / 180); //convert the angle from degrees to radians
    int center_x = w/2;
    int center_y = h/2;
    double sin_angle = sin(angle_rad);
    double tan_angle = tan(angle_rad/2);
    for(int x = 0; x < w;x++){
        for(int y = 0; y < h; y++){
            // Computes the offsets from the center of the image
            double xOff = (x - center_x);
            double yOff = (y - center_y);
            // Compute the coordinates from the image
            //shear 1
            int new_x = round(xOff - yOff* tan_angle);
            int new_y = yOff;
            //shear 2
            new_y = round(new_x * sin_angle + new_y);
            //shear 3
            new_x = round(new_x - new_y*tan_angle);

            new_y += center_y;
            new_x += center_x;
            // Check if the new coordinates are within the image
            if(0 <= new_x && new_x < w && 0 <= new_y && new_y < h){
                // Copy the pixel from the old image to the new image
                Uint32 pixel = getPixel(image, x, y);
                putPixel(rotated_image, new_x, new_y, pixel);
            }
        }
    }
    return rotated_image;
}
