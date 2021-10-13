# Makefile

CPPFLAGS = -MMD 
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 `pkg-config --cflags sdl2 SDL2_image` -g3
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

SRC = main.c ImageProcessing/GrayScale.c ImageProcessing/Pixels.c ImageProcessing/BlackAndWhite.c ImageProcessing/NoiseReduction.c Utils.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ}

.PHONY: clean
clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} main     # remove main program

-include ${DEP}

# END
