CC=gcc
CFLAGS= -g3 -Wall -Wextra -std=c99 -O2 -lm `pkg-config --cflags --libs sdl2 SDL2_image gtk+-3.0` -export-dynamic
LDFLAGS = -lm `pkg-config --libs sdl2 SDL2_image gtk+-3.0`


SRC = interface.c ../ImageProcessing/Rotation.c ../ImageProcessing/GrayScale.c ../ImageProcessing/Pixels.c ../ImageProcessing/BlackAndWhite.c ../ImageProcessing/NoiseReduction.c ../ImageProcessing/HoughTransform.c utilsUI.c ../Utils.c ../ImageProcessing.c  
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
TMP = main.glade~ Image/gaussian.png Image/greyscale.png Image/thresholding.png Image/Kuwahara.png Image/Hough.png Image/actual.png Image/tmpimg.png Image/actualrot.png

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
