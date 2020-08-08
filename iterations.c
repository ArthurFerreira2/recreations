// https://experiences.math.cnrs.fr/Iterations-de-l-application.html

#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS  30


int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window=SDL_CreateWindow("iterations", SDL_WINDOWPOS_CENTERED,
                     SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_Rect canvas={0, 0, SIZE, SIZE};
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  float i=0.2, r=3.7;
  int x0=0, y0=SIZE/2, x=0, y=0;


  while(isRunning) {
    frameStart=SDL_GetTicks();

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    SDL_SetRenderDrawColor(renderer, 30, 30, 35, 10);
    SDL_RenderFillRect(renderer, &canvas);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);

    x+=4;
    if (x>=SIZE) {
      x=x0=0;
      SDL_SetRenderDrawColor(renderer, 30, 30, 35, 192);
      SDL_RenderFillRect(renderer, &canvas);
      SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
    }

    i=r*i*(1.0-i);
    y=i*SIZE/2 + SIZE/5;
    SDL_RenderDrawLine(renderer, x0, y0, x, y);
    x0=x;
    y0=y;

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
