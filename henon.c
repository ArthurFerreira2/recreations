// https://experiences.math.cnrs.fr/Iterations-de-l-application.html

#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS  600



inline double frand() {
  return (double)rand() / (double)RAND_MAX;
}

int main(int argc, char **argv) {

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window=SDL_CreateWindow("Henon", SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;
  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  double x=1, y=0.2, x0=frand()*2-1, y0=frand()*2-1;
  double a=1.4, b=0.3;

  while(isRunning) {
    frameStart=SDL_GetTicks();

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    x0=frand()*2-1;
    y0=frand()*2-1;
    for (int i=0; i<10; i++) {
      x= y0 + 1 - a*x0*x0;
      y= b * x0;
      x0=x;
      y0=y;
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(renderer, x*SIZE/4+SIZE/2, y*SIZE+SIZE/2);

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
