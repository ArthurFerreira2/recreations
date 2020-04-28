#include <SDL2/SDL.h>

int main(int argc, char* args[]){

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window  *window  = SDL_CreateWindow("Mandelbrot set", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 256, 0);
  SDL_Surface *winSurf = SDL_GetWindowSurface(window);
  Uint32      *pixels  = winSurf->pixels;
  SDL_Event    event;

  const int max_iter = 40;
  const float zoom = 2.5;
  const float colorFactor  = 256 / max_iter;
  const float widthFactor  = 256 / zoom;
  const float heightFactor = 256 / zoom;
  float cr, ci, zr, zi, tmp;
  int x, y, iteration, color;

  for (x=0; x<256; x++) {
    for (y=0; y<256/2; y++) {
      cr = x / widthFactor  - 2 ;
      ci = y / heightFactor - 1.25;
      zr = 0;
      zi = 0;

      for (iteration=0; iteration<max_iter; iteration++) {
        tmp = zr;
        zr = zr*zr - zi*zi + cr;
        zi = 2*tmp*zi  + ci;
        if ((zr*zr + zi*zi) >= 4) break;
      }

      color = (int)(iteration*colorFactor);
      pixels[x + y * 256] = SDL_MapRGBA(winSurf->format, color, color, color, SDL_ALPHA_OPAQUE);
      pixels[x + (255 - y) * 256] = SDL_MapRGBA(winSurf->format, color, color, color, SDL_ALPHA_OPAQUE);
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
