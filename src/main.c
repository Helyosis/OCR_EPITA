#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <err.h>
#include <stddef.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <SDL.h>
#include <SDL_image.h>
#pragma GCC diagnostic pop

#define DEFINE_GLOBALS

#include "Verbose.h"
#include "ImageProcessing.h"
#include "NeuralNetwork/TrainNeuralNet.h"
#include "NeuralNetwork/NeuralNet.h"
#include "Utils.h"
#include "solver/sudoku.h"

// Macros used to transform enum to string
#define str(x) #x
#define xstr(x) str(x)

int VERBOSE_LEVEL = 0;
const char* MODE_STRING[] = {
    FOREACH_MODE(GENERATE_STRING)
};

char* interfaceWD = "./UserInterface";
char* interfaceArgs[] = {"./interface", NULL};


#define OPTSTR "hvi:o:n:m:b:c:l:a:"

static const char* ARGS_HELP =
    "%s 3.14.15 help:\n"
    "[ IMAGE mode specific options ]\n"
    "   -i file: Specifiy the input file (required)\n"
    "   -o file: Specify the output file (default: out.bmp)\n"
    "   --show: Show the image being processed, one step at a time\n"
    "[ TRAIN mode specific options ]\n"
    "   -n nb: Specifiy the number of iterations to train the neural net with (default is 100 000)\n"
    "   -o file: Specify the output file to save the neural network\n"
    "   --batch-size n / -b n: Specify the numbers of elements in a minibatch size (default 100)\n"
    "   --nb-images n: Specify the number of image to train with. (default 8228)\n"
    "   --learning-rate n / --step-size n / -l n: Specify the step size (default 0.25)\n"
    "[ PREDICT mode specific options ]\n"
    "   -i file: Specify the image file to predict the digit\n"
    "   -a file: Specify the file containing the weights and biais of the neural network\n"
    "[ SOLVE mode specific options ]\n"
    "   -i file: Specify the input file containing the grid to solve (default: grid.txt)"
    "[ General options ]\n"
    "   -v: Increase the verbose level (default 0), can be used up to 3 times\n"
    "   --mode mode: Specify the mode to use. Can be one of IMAGE/TRAIN/GUI/PREDICT/SOLVE (default is GUI)\n"
    "   -h / --help: Show this help and quit\n"
    ;



void usage(char* progname, int exit_code);

int main(int argc, char **argv)
{
    int c;
    char* progname = argv[0];

    t_options options = {
        NULL,   // inputFile
        NULL,   // outputFile
        NULL,   // nnInputFile
        0,      // showImage
        100000, // nbIterations
        100,    // miniBatch size
        8228,   // NbImages
        0.25,   // Learning rate
        GUI,    // mode
    };

    static struct option long_options[] = {
        {"mode", required_argument, 0, 'm'},
        {"show", no_argument, 0, 's'},
        {"help", no_argument, 0, 'h'},
        {"batch-size", required_argument, 0, 'b'},
        {"nb-images", required_argument, 0, 'c'},
        {"learning-rate", required_argument, 0, 'l'},
        {"step-size", required_argument, 0, 'l'},
        {0, 0, 0, 0}
    };

    while (1) {
        int option_index = 0;
        c = getopt_long(
            argc, argv, OPTSTR, long_options, &option_index
        );

        if (c == -1)
            break;
        switch(c) {
        case 'v':
            VERBOSE_LEVEL += 1;
            if (VERBOSE_LEVEL > MAX_VERBOSE_LEVEL)
                usage(progname, 1);
            break;

        case 'i':
            options.inputFile = optarg;
            break;

        case 'o':
            options.outputFile = optarg;
            break;

        case 'm':
            strtoupper(optarg);
            info_s("%s\n", optarg);
            int found = 0;
            for (long unsigned int i = 0; i < NB_ENUM; ++i) {
                if (strcmp(optarg, MODE_STRING[i]) == 0) {
                    found = 1;
                    options.mode = i;
                }
            }

            if (found == 0)
                usage(progname, 1);
            break;

        case 't':
            options.mode = TRAIN;
            break;

        case 'h':
            usage(progname, 0);
            break;
        case 's':
            options.showImage = 1;
            break;
        case 'n':
            options.nbIterations = atoi(optarg);
            break;
        case 'b':
            options.minibatch_size = atoi(optarg);
            break;
        case 'c':
            options.nbImages = atoi(optarg);
            break;
        case 'l':
            options.learningRate = atof(optarg);
            break;
        case 'a':
            options.nnInputFile = optarg;
            break;

        default: // c == '?'
            printf("%c\n", c);
            usage(progname, 1);
        }
    }

    info_s("mode = %s\n", MODE_STRING[options.mode]);
    if (options.inputFile != NULL)
        info_s("inputFile = %s\n", options.inputFile);
    if (options.outputFile != NULL)
        info_s("outputFile = %s\n", options.outputFile);
    info_s("Verbose level = %d\n", VERBOSE_LEVEL);


    switch (options.mode) {
    case IMAGE:
        if (options.inputFile == NULL)
            error_s("Invalid arguments. Image mode is specified without the -i flag");
        if (options.outputFile == NULL)
            options.outputFile = "out.bmp";
        return processImage(options);
        break;

    case TRAIN:
        if (options.outputFile == NULL)
            options.outputFile = "result_training.txt";
        trainNn(options);
        break;
    case PREDICT:
        if (options.inputFile == NULL)
            error_s("Predict mode: flag -i is required");
        if (options.nnInputFile == NULL)
            options.nnInputFile = "result_training.txt";

        double* result = predict(options.inputFile, options.nnInputFile);
        for (int i = 0; i < 9; ++i)
            printf("%f ", result[i]);
        printf("\n");
        break;
    case GUI:
        if (chdir(interfaceWD) != 0)
            error_s("Invalid directory. %s does not exists or cannot enter", interfaceWD);
        execvp(interfaceArgs[0], interfaceArgs);
        break;
    case SOLVE:
        if (options.inputFile == NULL)
            options.inputFile = "grid.txt";

        SDL_Surface* solvedSudoku = ReadFromFile(options.inputFile);

        SDL_Init(SDL_INIT_VIDEO);
        SDL_Window* window = SDL_CreateWindow("Result of sudoku",
                                              SDL_WINDOWPOS_UNDEFINED,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              solvedSudoku->w, solvedSudoku->h, 0);
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

        displaySurface(renderer, solvedSudoku);
        wait_for_keypressed();

        SDL_FreeSurface(solvedSudoku);

        SDL_Quit();
        IMG_Quit();
        break;
    default:
        error_s("Mode n°%d is not implemented yet. Quitting.",options.mode);
        break;
    }
}

void usage(char* progname, int exit_code) {
    if (exit_code == 0)
        printf(ARGS_HELP, progname);
    else
        errx(exit_code, ARGS_HELP, progname);

    exit(exit_code);
}
