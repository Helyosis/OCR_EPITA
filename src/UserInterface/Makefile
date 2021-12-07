CC=gcc
CFLAGS= -g3 -Wall -Wextra -std=c99 -O2 -lm `pkg-config --cflags --libs sdl2 SDL2_image gtk+-3.0` -export-dynamic
LDFLAGS = -lm `pkg-config --libs sdl2 SDL2_image gtk+-3.0`

SRC = interface.c ../main.c ../ImageProcessing.c ../ImageProcessing/GrayScale.c ../ImageProcessing/Pixels.c ../ImageProcessing/BlackAndWhite.c ../Result/construct.c ../ImageProcessing/NoiseReduction.c ../ImageProcessing/Morphology.c ../ImageProcessing/FloodFill.c ../ImageProcessing/BlobDetection.c ../ImageProcessing/OrderPoints.c ../Utils.c ../ImageProcessing/Rotation.c ../ImageProcessing/HoughTransform.c ../ImageProcessing/GradientCalculation.c ../ImageProcessing/CannyFilter.c ../ImageProcessing/NonMaxSuppression.c ../ImageProcessing/DoubleThreshold.c ../ImageProcessing/SquareCutter.c ../NeuralNetwork/MatUtils.c ../NeuralNetwork/NeuralNet.c ../NeuralNetwork/NeuralNetInit.c ../NeuralNetwork/TrainNeuralNet.c ../solver/sudoku.c ../Verbose.c ../ImageProcessing/HomographicTransphorm.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
TMP = main.glade~ Image/*.png

all: main

main: ${SRC}
	gcc -o interface ${SRC} ${CFLAGS} ${LDFLAGS}

.PHONY: clean
clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} ${TMP}   # remove temporary files
	${RM} interface     # remove main program

-include ${DEP}

# END
