#include <stdlib.h>
#include <SDL2/SDL.h>

struct walker_t {
  Uint8 x, y, R, G, B;
} walker[25];

int main(int argc, char* args[]) {

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Walkers2", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 256, 256, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  for(int i=0; i<25; i++) {
    walker[i].R = rand()%256;
    walker[i].G = rand()%256;
    walker[i].B = rand()%256;
  }

  while (isRunning) {
    for(int i=0; i<25; i++) {  // the walker can go 8 different directions
      walker[i].x += rand()%3 - 1;
      walker[i].y += rand()%3 - 1;

      SDL_SetRenderDrawColor(renderer, walker[i].R, walker[i].G, walker[i].B, 255);
      SDL_RenderDrawPoint(renderer, walker[i].x, walker[i].y);
    }

    SDL_RenderPresent(renderer);
    while (SDL_PollEvent(&event)) if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
