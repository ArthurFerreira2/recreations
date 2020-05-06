#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

int main(int argc, char *args[]) {

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window  *window  = SDL_CreateWindow("Perlin Noise", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 256, 0);
  SDL_Surface *winSurf = SDL_GetWindowSurface(window);
  Uint32      *pixels  = winSurf->pixels;
  SDL_Event    event;

  int y   = 64;  // altitude
  int wl  = 16;   // wavelength
  int amp = 64;   // amplitude
  float a = (float)rand() / (float)RAND_MAX;
  float b = (float)rand() / (float)RAND_MAX;
  float f;

  for (int x = 0; x < 256; x++) {
    if (x % wl == 0) {
      a = b;
      b = (float)rand() / (float)RAND_MAX;
      y = (int)(64 + a * amp);
    }
    else {
      f = (1 - cos((float)(x % wl) / wl * 3.141592)) * .5;
      y = 64 + (int)((a * (1 - f) + b * f)  * amp);
    }
    pixels[x + y * 256] = SDL_MapRGBA(winSurf->format, 255, 255, 255, SDL_ALPHA_OPAQUE);
  }

  SDL_UpdateWindowSurface(window);

  while(SDL_TRUE)
    if (SDL_PollEvent(&event) && (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)) break;
    else SDL_Delay(256);

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
