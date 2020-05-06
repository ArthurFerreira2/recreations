#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS 10

#define NB_CIRCLES 5
#define RADIUS_MIN 30
#define RADIUS_MAX 60
#define CIRCLE_SPEED 6
#define ALPHA 192

struct circle {
  int x;
  int y;
  int r;
  int dx;
  int dy;
} circles[NB_CIRCLES] = {0};


int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart;
  int frameTime;
  int dist, greyLevel;

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window   *window   = SDL_CreateWindow("Metaballs", SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED, SIZE, SIZE, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  for (int i=0; i<NB_CIRCLES; i++) {
    circles[i].x  = rand()%SIZE;
    circles[i].y  = rand()%SIZE;
    circles[i].r  = rand()%(RADIUS_MAX-RADIUS_MIN) + RADIUS_MIN;
    circles[i].dx = rand()%(CIRCLE_SPEED) + 1;
    circles[i].dy = rand()%(CIRCLE_SPEED) + 1;
  }

  while(isRunning) {
    frameStart = SDL_GetTicks();
    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    for (int i=0; i<NB_CIRCLES; i++) {
      circles[i].x += circles[i].dx;
      circles[i].y += circles[i].dy;
      if (circles[i].x < 0 || circles[i].x > SIZE) circles[i].dx *= -1;
      if (circles[i].y < 0 || circles[i].y > SIZE) circles[i].dy *= -1;
    }

    for(int x=0; x<SIZE; x++) {
      for(int y=0; y<SIZE; y++) {
        greyLevel=0;
        for(int i=0; i<NB_CIRCLES; i++) {
          dist = sqrt((pow(x - circles[i].x, 2) + pow( y - circles[i].y, 2)));
          if (dist) greyLevel += (circles[i].r * 90) / dist;
          else greyLevel += 255;
        }
        if (greyLevel > 255) greyLevel = 255;
        SDL_SetRenderDrawColor(renderer, greyLevel, greyLevel, greyLevel, ALPHA);
        SDL_RenderDrawPoint(renderer, x, y);
      }
    }

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
