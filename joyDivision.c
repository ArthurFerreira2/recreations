#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#define SIZE 256

int main(int argc, char *args[]) {

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window  *window  = SDL_CreateWindow("Joy Division", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, SIZE, SIZE, 0);
  SDL_Surface *winSurf = SDL_GetWindowSurface(window);
  Uint32      *pixels  = winSurf->pixels;
  SDL_Event    event;

  int y   = SIZE/4;   // altitude
  int wl  = SIZE/16;  // wavelength
  int amp = SIZE/8;   // amplitude
  float a = (float)rand() / (float)RAND_MAX;
  float b = (float)rand() / (float)RAND_MAX;
  float f;

  int max[SIZE];
  for (int i=0; i<SIZE; i++) max[i] =SIZE;

  for (int l=SIZE-2*amp; l>amp; l-=amp/4) {
    y = l;
    for (int x = SIZE/4; x < SIZE*3/4; x++) {
      if (x % wl == 0) {
        a = b;
        b = (float)rand() / (float)RAND_MAX;
        y = (int)(l + a*amp);
      }
      else {
        f = (1 - cos((float)(x % wl) / wl * 3.141592)) * .5;
        y = l + (int)((a * (1 - f) + b * f)  * amp);
      }
      if (y < max[x]) {
        max[x] = y;
        pixels[x + y * SIZE] = SDL_MapRGBA(winSurf->format, 255, 255, 255, SDL_ALPHA_OPAQUE);
      }
    }
  }
  SDL_UpdateWindowSurface(window);

  while(SDL_TRUE)
    if (SDL_PollEvent(&event) && (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)) break;
    else SDL_Delay(256);

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
