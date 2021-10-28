#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <err.h>
#include <string.h>
#pragma GCC diagnostic pop

#include "ImageProcessing.h"
#include "NeuralNetwork/TrainNeuralNet.h"

int main(int argc, char **argv)
{
    if (argc == 1)
        errx(1, "Please specify a mode to use.");

    if (!strcmp(argv[1], "train")) {
        char* filename = argc == 3 ? argv[2] : "neuralnetwork_save.txt";
        trainNn(20000, filename);
    }
    else if (!strcmp(argv[1], "image")) {
        char* in_filename = argc == 3 ?  argv[2] : "../Images/image_01.jpg";
        char* out_filename = argc == 4 ? argv[3] : "out.bmp";
        return processImage(in_filename, out_filename);
    }
    else {
        errx(1, "Unrecognized parameters: \"%s\"", argv[1]);
    }

    return -1;
}


