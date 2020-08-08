LIBS = -lSDL2
# comment the next line if you are under Linux
WIN32-LIBS = -Wl,-subsystem,windows -lmingw32 -lSDL2main
FLAGS = -std=c99 -pedantic -Wpedantic -Wall -Werror -O3
CC = gcc

walkers: walkers.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

walkers2: walkers2.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

gaussian: gaussian.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

perlinNoise: perlinNoise.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

perlinNoise2: perlinNoise2.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

joyDivision: joyDivision.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

MunchingSquares: MunchingSquares.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

moire: moire.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

sierpinski: sierpinski.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

koch: koch.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

noOverlap: noOverlap.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

circlePacking: circlePacking.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

euler: euler.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

fractalTree:fractalTree.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

l-systemTree: l-systemTree.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

mandel: mandel.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

julia: julia.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

fractalPanic: fractalPanic.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

lorenz0: lorenz0.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

lorenz: lorenz.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

lorenz2: lorenz2.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

life: life.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

langton: langton.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

wator: wator.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

forestFire: forestFire.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

lissajous: lissajous.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

lissajous2: lissajous2.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

rosace: rosace.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

hypnose: hypnose.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

diffusion: diffusion.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

metaballs: metaballs.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

metaballs2: metaballs2.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

fireworks: fireworks.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

trust: trust.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

gravity: gravity.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

gravity2: gravity2.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

Floyd-Steinberg: Floyd-Steinberg.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lSDL2_Image -o $@

triangle: triangle.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

circle: circle.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

shape: shape.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

particules: particules.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

floodFill: floodFill.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

balls: balls.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

demonsOfCyclicSpace: demonsOfCyclicSpace.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

maze: maze.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

maze2: maze2.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

plasma: plasma.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

fire: fire.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -o $@

colors: colors.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

colors2: colors2.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

noise: noise.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

noise3d: noise3d.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

tunnel: tunnel.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -lSDL2_Image  -o $@

rose: rose.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

iterations: iterations.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

doublePendule: doublePendule.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

henon: henon.c
	$(CC) $^ $(FLAGS) $(WIN32-LIBS) $(LIBS) -lm -o $@

all: walkers walkers2 gaussian perlinNoise perlinNoise2 joyDivision \
MunchingSquares moire sierpinski koch noOverlap circlePacking euler fractalTree\
l-systemTree mandel julia fractalPanic lorenz lorenz0 lorenz2 life langton wator \
forestFire lissajous lissajous2 rosace hypnose diffusion metaballs metaballs2 \
fireworks trust gravity gravity2 Floyd-Steinberg triangle circle shape balls \
particules floodFill demonsOfCyclicSpace maze maze2 plasma fire colors colors2 \
noise noise3d tunnel rose iterations doublePendule henon
