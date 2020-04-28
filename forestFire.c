#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define P rand()%100<1      // chances an empty cell becomes a tree
#define F rand()%1000000<1  // chances a tree spontaneously ignites

enum cells {empty, tree, fire};

int main(int argc, char **argv) {
  int x, y;
  int field1[SIZE][SIZE] = {0};
  int field2[SIZE][SIZE] = {0};
  int (*cur)[SIZE] = field1;
  int (*new)[SIZE] = field2;
  int (*tmp)[SIZE] = NULL;

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window   *window   = SDL_CreateWindow("Forest Fire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                            SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  while(isRunning) {
    while(SDL_PollEvent(&event)) if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;
    // https://en.wikipedia.org/wiki/Forest-fire_model
    for (x=1; x<SIZE-1; x++)
      for (y=1; y<SIZE-1; y++) {
        // A burning cell turns into an empty cell
        if (cur[x][y] == fire) {
          new[x][y] = empty;
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
        else
        // empty space fills with a tree with probability P
        if (cur[x][y] == empty) {
          if (P) {
            new[x][y] = tree;
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
          }
          else {
            new[x][y] = empty;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
          }
        }
        else
        // A tree ignites with probability F even if no neighbor is burning
        if (F) {
          new[x][y] = fire;
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        else
        // A tree will burn if at least one of its 4 neighbors is burning
        if (cur[x-1][y]==fire || cur[x][y-1]==fire || cur[x][y+1]==fire || cur[x+1][y]==fire) {
          new[x][y] = fire;
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        else {
          new[x][y] = tree;
          SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        }

        SDL_RenderDrawPoint(renderer, x, y);
      }

    tmp = cur;
    cur = new;
    new = tmp;

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
