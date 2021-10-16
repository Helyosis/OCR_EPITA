#include "SDL2/SDL.h"
#include <math.h>

// Image manipulation functions

//Rotation, rotate an image in place
//@param image: the image to rotate
//@param angle: the angle to rotate the image by
void rotate(SDL_Surface *image, double angle, SDL_Surface *rotated){
    int w = image->w;
    int h = image->h; 
    double angle_rad = angle * (M_PI / 180); //convert the angle from degrees to radians
    int center_x = w / 2;
    int center_y = h / 2;
    SDL_LockSurface(rotated);
    for(int y = 0; y < w;y++){
        for(int x= 0; x < h; x++){
            // Compute the coordinates from the image
            int new_x = (y - center_x) * cos(angle_rad) - (x - center_y) * sin(angle_rad) + center_x;
            int new_y = (y - center_x) * sin(angle_rad) + (x - center_y) * cos(angle_rad) + center_y;
            // Check if the new coordinates are within the image
            if(0 <= new_x && new_x < w && 0 <= new_y && new_y < h){
                // Copy the pixel from the old image to the new image
                Uint32 pixel = get_pixel(image, y, x);
                put_pixel(rotated, new_x, new_y, pixel);
            }
        }
    }
    SDL_UnlockSurface(rotated);
}