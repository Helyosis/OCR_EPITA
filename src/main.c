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
#pragma GCC diagnostic pop

#define DEFINE_GLOBALS

#include "Verbose.h"
#include "ImageProcessing.h"
#include "NeuralNetwork/TrainNeuralNet.h"
#include "Utils.h"

// Macros used to transform enum to string
#define str(x) #x
#define xstr(x) str(x)

int VERBOSE_LEVEL = 0;
const char* MODE_STRING[] = {
    FOREACH_MODE(GENERATE_STRING)
};

#define OPTSTR "hvi:o:n:m:"

static const char* ARGS_HELP =
    "%s 3.14.15 help:\n"
    "[ Image mode specific options ]\n"
    "   -i file: Specifiy the input file (required)\n"
    "   -o file: Specify the output file (default: out.bmp)\n"
    "   --show: Show the image being processed, one step at a time\n"
    "[ Train mode specific options ]\n"
    "   -n nb: Specifiy the number of iterations to train the neural net with (default is 100 000)\n"
    "[ General options ]\n"
    "   -v: Increase the verbose level (default 0), can be used up to 3 times\n"
    "   --mode mode: Specify the mode to use. Can be one of IMAGE/TRAIN/GUI (default is GUI)\n"
    "   -h / --help: Show this help and \n"
    ;



void usage(char* progname, int exit_code);

//int main(int argc, char **argv)
int main(int argc, char **argv)
{
    int c;
    char* progname = argv[0];

    t_options options = {
        NULL,
        NULL,
        0,
        100000,
        GUI,
    };

    static struct option long_options[] = {
        {"mode", required_argument, 0, 'm'},
        {"show", no_argument, 0, 's'},
        {"help", no_argument, 0, 'h'},
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
            
        default: // c == '?'
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
        trainNn(options.nbIterations, options.outputFile);
        break;

    default:
        error_s("Mode %s is not implemented yet. Quitting.", MODE_STRING[options.mode]);
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
