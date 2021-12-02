#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <err.h>
#include <stddef.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma GCC diagnostic pop

#include "Verbose.h"
#include "ImageProcessing.h"
#include "NeuralNetwork/TrainNeuralNet.h"

// Macros used to transform enum to string
#define str(x) #x
#define xstr(x) str(x)


#define OPTSTR "hvi:o:"

#define FOREACH_MODE(MODE) \
        MODE(GUI)   \
        MODE(IMAGE)  \
        MODE(TRAIN)   \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum MODE_ENUM {
    FOREACH_MODE(GENERATE_ENUM)
};

static const char *MODE_STRING[] = {
    FOREACH_MODE(GENERATE_STRING)
};

#define NB_ENUM (sizeof(MODE_STRING) / sizeof(*MODE_STRING))

static const char* ARGS_HELP =
    "%s 3.14.15 help:\n"
    "[ Image mode specific options ]\n"
    "   -i file: Specifiy the input file\n"
    "   -o file: Specify the output file\n"
    "   --show: Show the image being processed\n"
    "[ Train mode specific options ]\n"
    "   -n nb: Specifiy the number of iterations to train the neural net\n"
    "[ General options ]\n"
    "   -v: Increase the verbose level (default 0), can be used up to 3 times\n"
    "   --mode mode: Specify the mode to use. Can be one of IMAGE/TRAIN/GUI (default is GUI)\n"
    ;

typedef struct {
    char* inputFile;
    char* outputFile;
    enum MODE_ENUM mode;
} t_options;

void usage(char* progname, int exit_code);

int main(int argc, char **argv)
{
    int c;
    char* progname = argv[0];

    t_options options = {
        NULL,
        NULL,
        GUI
    };

    static struct option long_options[] = {
        {"mode",  required_argument,    0, 'm'},
        {"help",        no_argument,    0, 'h'},
        {0,             0,              0,   0}
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
                printf("%s\n", optarg);
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

            default: // c == '?'
                usage(progname, 1);
        }
    }

    printf("mode = %s\n", MODE_STRING[options.mode]);
    if (options.inputFile != NULL)
        printf("inputFile = %s\n", options.inputFile);
    if (options.outputFile != NULL)
        printf("outputFile = %s\n", options.outputFile);
    printf("Verbose level = %d\n", VERBOSE_LEVEL);
}

void usage(char* progname, int exit_code) {
    if (exit_code == 0) 
        printf(ARGS_HELP, progname);
    else
        errx(exit_code, ARGS_HELP, progname);

    exit(exit_code);
}

void help(void) {
    
    exit(1);
}