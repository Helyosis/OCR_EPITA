# Makefile

CPPFLAGS = -MMD 
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic `pkg-config --cflags sdl2 SDL2_image` -O2
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

SRC = main.c ImageProcessing.c ImageProcessing/GrayScale.c ImageProcessing/Pixels.c ImageProcessing/BlackAndWhite.c ImageProcessing/Morphology.c ImageProcessing/NoiseReduction.c Utils.c ImageProcessing/Rotation.c ImageProcessing/HoughTransform.c NeuralNetwork/MatUtils.c NeuralNetwork/NeuralNet.c NeuralNetwork/NeuralNetInit.c NeuralNetwork/TrainNeuralNet.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ}

.PHONY: clean
clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} main     # remove main program

	${RM} out*.bmp # remove out files

-include ${DEP}

# END
