#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <err.h>
#include <stdio.h>
#include "ImageProcessing/GrayScale.h"
#include "ImageProcessing/NoiseReduction.h"
#include "ImageProcessing/BlackAndWhite.h"
#include "image-manipulation/manipulation.h"
#include "NeuralNetwork/TrainNeuralNet.h"

int main()
{
    trainNn(3);
    return 0;
}
