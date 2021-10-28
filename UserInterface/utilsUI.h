#ifndef UTILSUI_H
#define UTILSUI_H

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);
void save_image(SDL_Surface* img,char *path);
SDL_Surface* load_image(char *path);

#endif
