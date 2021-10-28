#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <err.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}


//Saving image.png 
void save_image(SDL_Surface* img,char *path){
	IMG_SavePNG(img,path);
	SDL_FreeSurface(img);
}

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}
/*
SDL_Surface* resize_img(SDL_Surface* img, int x, int y){
	int RX = x/img->w;
	int RY = y/img->h;

	for(int i=0;i<x;i++){
		for(int j=0;j<y;j++){
			Uint32 pixel = get_pixel(img,i,j);
			Uint8 r, g, b;	
		}
	}

}
*/
