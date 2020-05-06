#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define WIDTH 256
#define HEIGHT 256
#define SQUARESNUM 5000
#define SIZE 20


SDL_bool collide(SDL_Rect r1, SDL_Rect r2){
  if (r1.x + r1.w < r2.x || r1.x > r2.x + r2.w) return SDL_FALSE;
  if (r1.y + r1.h < r2.y || r1.y > r2.y + r2.h) return SDL_FALSE;
  return SDL_TRUE;
}


int main(int argc, char **argv) {
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Event event;
  SDL_Rect squares[SQUARESNUM] = {{0}};

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
  SDL_SetWindowTitle(window, "noOverlap");

  for (int i=0; i<SQUARESNUM; i++) {
    squares[i].x = rand() % (WIDTH  + SIZE) - SIZE;
    squares[i].y = rand() % (HEIGHT + SIZE) - SIZE;
    squares[i].h = rand() % SIZE;
    squares[i].w = squares[i].h;
    for (int j=0; j<i; j++)
      if (collide(squares[i], squares[j])) {
        i--;
        break;
      }
    SDL_SetRenderDrawColor(renderer, rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &squares[i]);
  }

  SDL_RenderPresent(renderer);

  while(SDL_TRUE)
    if (SDL_PollEvent(&event) && (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)) break;
    else SDL_Delay(256);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
