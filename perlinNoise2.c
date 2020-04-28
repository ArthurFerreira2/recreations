// experiment with perlin noise

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#define WIDTH 512
#define HEIGHT 300
#define PI 3.14159265359



inline float rand_fl() {
  return (float)rand() / (float)RAND_MAX;
}


float interpolate(float pa, float pb, float px) {  // first point, second point and px is the gradiant (a value between 0 and 1)
  float f = (1 - cos(px * PI)) * 0.5;
  return (pa * (1 - f) + pb * f);
}


int main(int argc, char *args[]) {

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window   *window   = SDL_CreateWindow("Pelin Noise", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_bool isRunning     = SDL_TRUE;
  SDL_Event event;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


  float y   = HEIGHT * 2 / 3;   // altitude
  float amp = 96;               // amplitude
  int   wl  = 32;               // wavelength == # of waves across the WIDTH
  float a   = rand_fl();
  float b   = rand_fl();

  for (int x = 0; x < WIDTH; x++) {
    if (x % wl == 0){
      a = b;
      b = rand_fl();
      y = HEIGHT * 2 / 3 + a * amp;
    }
    else
      y = HEIGHT * 2 / 3 + interpolate(a, b, (float)(x % wl) / wl) * amp;
    SDL_RenderDrawPoint(renderer, x, (int)y);
  }

  SDL_RenderPresent(renderer);


  while (isRunning) {
    while (SDL_PollEvent(&event))
      if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
        isRunning = SDL_FALSE;
    SDL_Delay(200);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
