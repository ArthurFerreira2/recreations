#include <math.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define PI 3.1415926535
#define FPS 60

void z(SDL_Renderer *renderer, float a, float b, float e, float d, float t) {
  if(d > 1) {
    a += d * sin(e);
    b -= d * cos(e);
    SDL_RenderDrawPoint(renderer, a, b);
    e -= 2 * sin(t);
    d *= .7;
    z(renderer, a, b, e, d, t);
    e += 2 * sin(t * t);
    z(renderer, a, b, e, d, t);
  }
}

int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window   *window   = SDL_CreateWindow("fractal Panic", SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED, SIZE, SIZE, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  while(isRunning) {
    frameStart = SDL_GetTicks();
    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    SDL_SetRenderDrawColor(renderer, 000, 000, 000, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    z(renderer, SIZE/2, SIZE*3/4, 0, SIZE/5, fmodf((float)frameStart/5000, 2*PI)-PI);

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
