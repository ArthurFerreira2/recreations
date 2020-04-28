#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS 60
#define nbParticules 512

struct particule {
  float x, y;
  float aa, al;
  float vx, vy;
  Uint8 age;
} firework[nbParticules];


void initFirework() {
  for (int i=0; i<nbParticules; i++) {
    firework[i].x = SIZE/2;
    firework[i].y = SIZE/3;
    firework[i].vx = 0;
    firework[i].vy = 0;
    firework[i].aa = ((float)rand() / (float)RAND_MAX) * 2 * 3.1415926536;
    firework[i].al = (((float)rand() / (float)RAND_MAX) + 0.1) / 100;
    firework[i].age = rand()%150 + 105;
  }
}


int main(int argc, char **argv) {
  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;
  SDL_Rect canvas  = {0, 0, SIZE, SIZE};

  SDL_bool allDone = SDL_FALSE;
  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window   *window   = SDL_CreateWindow("Fireworks", SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED, SIZE, SIZE, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  initFirework();

  while(isRunning) {
    frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 32);
    SDL_RenderFillRect(renderer, &canvas);

    allDone = SDL_TRUE;

    for (int i=0; i<nbParticules; i++) {
      firework[i].vx += cos(firework[i].aa)*firework[i].al;
      firework[i].vy += sin(firework[i].aa)*firework[i].al + .003;
      firework[i].x  += firework[i].vx;
      firework[i].y  += firework[i].vy;

      if (firework[i].age != 0) {
        firework[i].age--;
        SDL_SetRenderDrawColor(renderer, firework[i].age, firework[i].age, firework[i].age, 255);
        SDL_RenderDrawPoint(renderer, firework[i].x, firework[i].y);
        allDone = SDL_FALSE;
      }
    }

    if (allDone) initFirework();

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime)
      SDL_Delay(frameDelay - frameTime);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
