#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

int main(int argc, char* args[]){

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Gaussian Distribution", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 256, 256, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Event event;

  SDL_Rect rect = {0, 255, 2, 0};

  for(int i=0; i<128; i++){
    rect.h  = (int)((float)(-rand()%256) * fabsf(1.0/(i - 64)));
    rect.x = i*2;
    SDL_RenderFillRect(renderer, &rect);
  }

  SDL_RenderPresent(renderer);

  while(SDL_TRUE)
    if (SDL_PollEvent(&event) && (event.type==SDL_QUIT \
        || event.type==SDL_KEYDOWN)) break;
    else SDL_Delay(256);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  exit(EXIT_SUCCESS);
}
