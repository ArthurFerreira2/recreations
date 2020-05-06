#include <stdlib.h>
#include <SDL2/SDL.h>

struct walker_t {
  Uint8 x, y, R, G, B;
} walker[25];

int main(int argc, char* args[]) {

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Walkers", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 256, 256, SDL_WINDOW_OPENGL);
  SDL_Surface *winSurf = SDL_GetWindowSurface(window);
  Uint32 *pixels = winSurf->pixels;
  SDL_Event event;

  for(int i=0; i<25; i++) {
    walker[i].R = rand()%256;
    walker[i].G = rand()%256;
    walker[i].B = rand()%256;
  }

  while (SDL_TRUE) {
    for(int i=0; i<25; i++) {
      walker[i].x += rand()%3 - 1;
      walker[i].y += rand()%3 - 1;

      pixels[walker[i].x + (walker[i].y<<8)] = SDL_MapRGBA(winSurf->format, walker[i].R, walker[i].G, walker[i].B, SDL_ALPHA_OPAQUE);
    }

    SDL_UpdateWindowSurface(window);
    if (SDL_PollEvent(&event) && (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)) break;
    else SDL_Delay(20);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
