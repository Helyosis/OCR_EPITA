CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -O2 -lm `pkg-config --cflags --libs sdl2 SDL2_image gtk+-3.0` -export-dynamic -O2 -g3
LDFLAGS = -lm `pkg-config --libs sdl2 SDL2_image gtk+-3.0`

SRC = ../Verbose.c utilsUI.c interface.c ../ImageProcessing.c ../ImageProcessing/GrayScale.c ../ImageProcessing/Pixels.c ../ImageProcessing/BlackAndWhite.c ../Result/construct.c ../ImageProcessing/NoiseReduction.c ../ImageProcessing/Morphology.c ../ImageProcessing/FloodFill.c ../ImageProcessing/BlobDetection.c ../ImageProcessing/OrderPoints.c ../Utils.c ../ImageProcessing/Rotation.c ../ImageProcessing/HoughTransform.c ../ImageProcessing/GradientCalculation.c ../ImageProcessing/CannyFilter.c ../ImageProcessing/NonMaxSuppression.c ../ImageProcessing/DoubleThreshold.c ../ImageProcessing/SquareCutter.c ../solver/sudoku.c ../ImageProcessing/HomographicTransphorm.c ../NeuralNetwork/MatUtils.c ../NeuralNetwork/NeuralNet.c ../NeuralNetwork/NeuralNetInit.c ../NeuralNetwork/TrainNeuralNet.c ../NeuralNetwork/LoadDataSet.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
TMP = main.glade~ Image/*.png *.bmp

all: main

main: ${SRC}
	gcc -o interface ${SRC} ${CFLAGS} ${LDFLAGS} && cp -r ../../Ressources/Numbers /tmp/ && cp ../../Images/resultGrid.jpg /tmp/
	
.PHONY: clean
clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} ${TMP}   # remove temporary files
	${RM} interface     # remove main program
	${RM} -rf extractedDigits 
-include ${DEP}

# END
