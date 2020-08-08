// https://en.wikipedia.org/wiki/Rose_(mathematics)

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SIZE 256

int main(int argc, char **argv) {

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window=SDL_CreateWindow("rose", SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED, SIZE, SIZE, 0);
  SDL_Renderer *renderer=SDL_CreateRenderer(window, -1, 0 );
  // welcome screen
  SDL_Surface *surface = SDL_LoadBMP("rose-title.bmp");
  SDL_BlitSurface(surface, NULL, SDL_GetWindowSurface(window), NULL);
  SDL_UpdateWindowSurface(window);

  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;
  char windowTitle[50];
  const double radius = SIZE/2.5;
  double k=1, n=1, d=1;

  while(isRunning) {
    SDL_Delay(250);

    while(SDL_PollEvent(&event)) {
      if (event.type==SDL_QUIT) isRunning=SDL_FALSE;
      if (event.type==SDL_KEYDOWN) {
        if (event.key.keysym.sym==SDLK_ESCAPE) isRunning=SDL_FALSE;
        if (event.key.keysym.sym==SDLK_q) n++;
        if (event.key.keysym.sym==SDLK_w) d++;
        if((event.key.keysym.sym==SDLK_a) && (--n<=0)) n=1;
        if((event.key.keysym.sym==SDLK_s) && (--d<=0)) d=1;
        k=n/d;
        sprintf(windowTitle, "rose k=%.0f/%.0f", n, d);
        SDL_SetWindowTitle(window, windowTitle);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
        for (double t=.0; t<=(n+d)*12.56; t+=.005)
          SDL_RenderDrawPoint(renderer, radius*cos(k*t)*cos(t) + SIZE/2,
                                        radius*cos(k*t)*sin(t) + SIZE/2);
        SDL_RenderPresent(renderer);
      }
    }
  }
  SDL_FreeSurface(surface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
