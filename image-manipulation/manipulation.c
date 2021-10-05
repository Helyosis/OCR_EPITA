#include <SDL2.h>
#include <math.h>

// Image manipulation functions

//Rotation, rotate an image in place
//@param image: the image to rotate
//@param angle: the angle to rotate the image by
void rotate(SDL_Surface *image, double angle){
    int h, w = image->w,image->h; 
    double angle_rad = angle * (M_PI / 180); //convert the angle from degrees to radians
    int center_x = w / 2;
    int center_y = h / 2;
    for(int i = 0; i < w;i++){
        for(int j= 0; j < h; j++){
            // Compute the coordinates from the image
            new_x = int((x - center_x) * cos(angle_rad) - (y - center_y) * sin(angle_rad) + center_x);
            new_y = int((x - center_x) * sin(angle_rad) + (y - center_y) * cos(angle_rad) + center_y);
            // Check if the new coordinates are within the image
            if(0 <= new_x && new_x < w && 0 <= new_y && new_y < h){
                // Copy the pixel from the old image to the new image
                image->pixels[new_x + new_y * w] = image->pixels[x + y * w];
            }
        }
    }
}