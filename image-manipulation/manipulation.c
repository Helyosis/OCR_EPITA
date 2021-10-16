#include "SDL2/SDL.h"
#include <math.h>
#include "../ImageProcessing/Pixels.h"


// Define Pi
#define M_PI 3.14159265


// Image manipulation functions

//Rotation, rotate an image in place
//@param image: the image to rotate
//@param angle: the angle to rotate the image by
void rotation(SDL_Surface *image, double angle){
    int w = image->w;
    int h = image->h; 
    double angle_rad = angle * (M_PI / 180); //convert the angle from degrees to radians
    int center_x = w/2;
    int center_y = h/2;
    for(int y = 0; y < w;y++){
        for(int x = 0; x < h; x++){
            // Compute the coordinates from the image
            int new_x = cos(angle_rad) * (x - center_x) + sin(angle_rad) * (y - center_y);
            int new_y = -sin(angle_rad) * (x - center_x) + cos(angle_rad) * (y - center_y);
            // Check if the new coordinates are within the image
            if(0 <= new_x && new_x < h && 0 <= new_y && new_y < w){
                // Copy the pixel from the old image to the new image
                Uint32 pixel = getPixel(image, y, x);
                putPixel(image, new_x, new_y, pixel);
            }
        }
    }
}