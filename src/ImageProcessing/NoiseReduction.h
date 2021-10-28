#ifndef NOISEREDUCTION
#define NOISEREDUCTION

#include <SDL.h>

double** Build_GaussianKernel(int ksize);

SDL_Surface* KuwaharaFilter(SDL_Surface *source);
void KuwaharaFilter_inPlace(SDL_Surface *source);

SDL_Surface* GaussianBlur(SDL_Surface *source);
void GaussianBlur_inPlace(SDL_Surface *source);

#endif