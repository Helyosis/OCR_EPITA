# Makefile

CPPFLAGS = -MMD 
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wno-type-limits  -std=c99 -O2 `pkg-config --cflags sdl2 SDL2_image gtk+-3.0` -export-dynamic -g3
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

<<<<<<< HEAD:Makefile
SRC = main.c ImageProcessing/GrayScale.c ImageProcessing/Pixels.c ImageProcessing/BlackAndWhite.c ImageProcessing/NoiseReduction.c ImageProcessing/HoughTransform.c Utils.c
=======
SRC = main.c ImageProcessing/GrayScale.c ImageProcessing/Pixels.c ImageProcessing/BlackAndWhite.c ImageProcessing/NoiseReduction.c solver/sudoku.c Utils.c ImageProcessing/Rotation.c ImageProcessing/HoughTransform.c
>>>>>>> origin/dev:src/Makefile
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
