#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define PI   3.14159265359
#define FPS  30

int main(int argc, char **argv) {
  float t, x, y, ox, oy;
  const float A = SIZE/2.5, B = SIZE/2.5;
  float greyLevel = 255;
  float p = 1, q = 1, o = 0;
  float offset = .03;

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window   *window   = SDL_CreateWindow("Lissajous", SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  while(isRunning) {
    frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&event)) {
      if (event.type==SDL_KEYDOWN) {
        if (event.key.keysym.sym==SDLK_q) p++;
        if (event.key.keysym.sym==SDLK_w) q++;
        if (event.key.keysym.sym==SDLK_e) offset+=.01;
        if (event.key.keysym.sym==SDLK_d) offset-=.01;
        if((event.key.keysym.sym==SDLK_a) && (--p<0)) p=0;
        if((event.key.keysym.sym==SDLK_s) && (--q<0)) q=0;
        if (event.key.keysym.sym==SDLK_ESCAPE) isRunning=SDL_FALSE;
      }
      if (event.type==SDL_QUIT) isRunning=SDL_FALSE;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    o += offset;
    x = A * sin(0) + SIZE / 2;
    y = B * sin(o) + SIZE / 2;

    for (t = .01; t < 2*PI; t += .01) { // 628 iterations
      ox = x;
      oy = y;
      x  = A * sin(p * t)     + SIZE / 2;
      y  = B * sin(q * t + o) + SIZE / 2;
      greyLevel -= .32;              // 628 / 200 grey levels
      if (greyLevel < 55) greyLevel = 255; // 200 grey levels
      SDL_SetRenderDrawColor(renderer, greyLevel, greyLevel, greyLevel, 255);
      SDL_RenderDrawLine(renderer, ox, oy, x, y);
    }

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
