CC=gcc
CFLAGS= -Wno-format -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic -pedantic

all:
	$(CC) -o main1 main.c interface.c $(CFLAGS)
