LIBS = -lm -lmingw32 -lSDL2main -lSDL2
FLAGS = -std=c99 -pedantic -Wpedantic -Wall -Werror -O3 -Wl,-subsystem,windows
CC = gcc

apps = walkers walkers2 gaussian perlinNoise perlinNoise2 joyDivision \
MunchingSquares moire sierpinski koch noOverlap circlePacking euler euler \
fractalTree l-systemTree mandel julia fractalPanic lorenz life langton wator \
forestFire lissajous lissajous2 rosace hypnose diffusion metaballs metaballs2 \
fireworks trust gravity gravity2 triangle circle shape balls \
particules floodFill demonsOfCyclicSpace maze maze2 plasma fire colors colors2 \
noise noise3d iterations doublePendule henon

all: $(apps) Floyd-Steinberg tunnel

$(apps): %: %.c
	$(CC) $< $(FLAGS) $(LIBS) -o $@

Floyd-Steinberg: Floyd-Steinberg.c
	$(CC) $^ $(FLAGS) $(LIBS) -lSDL2_Image -o $@

tunnel: tunnel.c
	$(CC) $^ $(FLAGS) $(LIBS) -lSDL2_Image -o $@
