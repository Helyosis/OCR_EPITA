#include <SDL2/SDL.h>
#include <err.h>

SDL_Surface** cutSudoku(SDL_Surface* source) {
    SDL_Surface** surfacelist = calloc(81, sizeof(SDL_Surface*));
    int i = 0;
    SDL_Rect srcrect, dstrect;
    srcrect.w = source->w / 9 - 20;
    srcrect.h = source->w / 9 - 20;

    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = srcrect.w;
    dstrect.h = srcrect.h;

    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            srcrect.x = x * source->w / 9 + 10;
            srcrect.y = y * source->h / 9 + 10;
            SDL_Surface* new_surface = SDL_CreateRGBSurface(
                source->flags,
                srcrect.w,
                srcrect.h,
                source->format->BitsPerPixel,
                source->format->Rmask,
                source->format->Gmask,
                source->format->Bmask,
                source->format->Amask
                );
            if (SDL_BlitSurface(source, &srcrect, new_surface, &dstrect) != 0) {
                errx(-1, SDL_GetError());
            }
            surfacelist[i++] = new_surface;
            printf("Surface n%d (w=%d, h=%d), x = %d, y = %d\n",
                   i, new_surface->w, new_surface->h, x, y);
        }
    }

    return surfacelist;
}
