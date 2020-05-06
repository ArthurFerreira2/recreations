#include <SDL2/SDL.h>

int main(int argc, char **argv) {

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window  *window  = SDL_CreateWindow("Munching Squares", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 256, 0);
  SDL_Surface *winSurf = SDL_GetWindowSurface(window);
  Uint32      *pixels  = winSurf->pixels;
  SDL_Event    event;

  for (int x = 0; x < 256; x++)
    for (int y = 0; y < 256; y++)
      pixels[x + y * 256] = (x<<16) + ((x^y)<<8) + y ;
      // pixels[x + y * 256] = SDL_MapRGBA(winSurf->format, x, x^y, y, SDL_ALPHA_OPAQUE);

  SDL_UpdateWindowSurface(window);

  while(SDL_TRUE)
    if (SDL_PollEvent(&event) && (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)) break;
    else SDL_Delay(256);

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
