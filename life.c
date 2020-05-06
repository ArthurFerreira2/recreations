#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define SIZE 256

int main(int argc, char **argv) {

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window   *window   = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED,
                                             SDL_WINDOWPOS_CENTERED, SIZE, SIZE, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Event event;

  int x, y, neighbors;
  char table1[SIZE][SIZE] = {0};
  char table2[SIZE][SIZE] = {0};
  char (*cur)[SIZE] = table1;
  char (*new)[SIZE] = table2;
  char (*tmp)[SIZE] = NULL;

  for (x=0; x<SIZE*SIZE; x++)
    cur[rand()%SIZE][rand()%SIZE] = 1;

  while(SDL_TRUE) {

    for (x=0; x<SIZE; x++)
      for (y=0; y<SIZE; y++) {
        neighbors = 0;
        if (x>0 && y>0 && x<SIZE-1 && y<SIZE-1)
          neighbors = cur[x-1][y-1] + cur[x-1][y  ] + cur[x-1][y+1] +
                      cur[x  ][y-1]                 + cur[x  ][y+1] +
                      cur[x+1][y-1] + cur[x+1][y  ] + cur[x+1][y+1];

        if ((cur[x][y]==0 && neighbors==3) ||
           ((cur[x][y]==1 && (neighbors==2 || neighbors==3)))) {
          new[x][y] = 1;
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        else {
          new[x][y] = 0;
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
        SDL_RenderDrawPoint(renderer, x, y);
      }

    tmp = cur;
    cur = new;
    new = tmp;

    SDL_RenderPresent(renderer);
    if (SDL_PollEvent(&event) && (event.type==SDL_QUIT || event.type==SDL_KEYDOWN)) break;
    else SDL_Delay(256);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
