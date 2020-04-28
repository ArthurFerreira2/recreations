#include <math.h>
#include <SDL2/SDL.h>

int main(int argc, char* args[]){

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window  *window  = SDL_CreateWindow("rosace", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 256, 0);
  SDL_Surface *winSurf = SDL_GetWindowSurface(window);
  Uint32 *pixels = winSurf->pixels;
  SDL_Event event;

  Uint8 x, y;
  float angle1 = .0;
  float angle2 = .0;
  float radius = 24;
  Uint8 greyLevel;

  for (int i = 0; i < 10000; i++) {
    angle1 += 0.001;
    angle2 += 0.01;
    radius += cos(angle2);

    x = (Uint8)(cos(angle1) * radius + 128);
    y = (Uint8)(sin(angle1) * radius + 128);
    greyLevel = (Uint8)(cos(angle2) * 255);

    pixels[x + y * 256] = SDL_MapRGBA(winSurf->format, greyLevel, greyLevel, greyLevel, SDL_ALPHA_OPAQUE);
  }

  SDL_UpdateWindowSurface(window);

  while(SDL_TRUE)
    if (SDL_PollEvent(&event) && (event.type==SDL_QUIT || event.type==SDL_KEYDOWN)) break;
    else SDL_Delay(256);

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
