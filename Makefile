LIBS = -lmingw32 -lSDL2main -lSDL2
# you can add  -Wl,-subsystem,windows  to gets rid of the console window
FLAGS = -std=c99 -pedantic -Wpedantic -Wall -Werror -O3 -Wl,-subsystem,windows
CC = gcc

walkers: walkers.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

walkers2: walkers2.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

gaussian: gaussian.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

perlinNoise: perlinNoise.c
	$(CC) $^ $(FLAGS) $(LIBS) -lm -o $@

perlinNoise2: perlinNoise2.c
	$(CC) $^ $(FLAGS) $(LIBS) -lm -o $@

MunchingSquares: MunchingSquares.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

sierpinski: sierpinski.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

euler: euler.c
	$(CC) $^ $(FLAGS) $(LIBS) -lm -o $@

fractalTree:fractalTree.c
	$(CC) $^ $(FLAGS) $(LIBS) -lm -o $@

l-systemTree: l-systemTree.c
	$(CC) $^ $(FLAGS) $(LIBS) -lm -o $@

mandel: mandel.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

julia: julia.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

lorenz0: lorenz0.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

lorenz: lorenz.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

lorenz2: lorenz2.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

life: life.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

wator: wator.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

forestFire: forestFire.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

lissajous: lissajous.c
	$(CC) $^ $(FLAGS) $(LIBS) -lm -o $@

lissajous2: lissajous2.c
	$(CC) $^ $(FLAGS) $(LIBS) -lm -o $@

rosace: rosace.c
	$(CC) $^ $(FLAGS) $(LIBS) -lm -o $@

hypnose: hypnose.c
	$(CC) $^ $(FLAGS) $(LIBS) -lm -o $@

diffusion: diffusion.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

metaballs: metaballs.c
	$(CC) $^ $(FLAGS) $(LIBS) -lm -o $@

metaballs2: metaballs2.c
	$(CC) $^ $(FLAGS) $(LIBS) -lm -o $@

fireworks: fireworks.c
	$(CC) $^ $(FLAGS) $(LIBS) -lm -o $@

gravity: gravity.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

gravity2: gravity2.c
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

all: walkers walkers2 gaussian perlinNoise perlinNoise2 MunchingSquares euler \
fractalTree l-systemTree mandel julia lorenz life wator forestFire lissajous \
lissajous2 rosace hypnose diffusion metaballs metaballs2 fireworks \
gravity gravity2
