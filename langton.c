#include <SDL2/SDL.h>

#define SIZE 256


int main(int argc, char **argv) {

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Langton's ant", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Event event;

  char world[SIZE][SIZE] = {0};
  enum directions {up=0, left=1, down=2, right=3} dir = up;
  SDL_Rect ant = {SIZE/2, SIZE/2, 2, 2};

  for (int i = 0; i < 11200; i++) {
    if (world[ant.x][ant.y]) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
      world[ant.x][ant.y] = 0;
      dir = (dir + 1) % 4;
    }
    else {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
      world[ant.x][ant.y] = 1;
      dir = (dir - 1) % 4;
    }

    SDL_RenderFillRect(renderer, &ant);

    switch (dir) {
      case    up: ant.y -=3 ; break;
      case  down: ant.y +=3 ; break;
      case  left: ant.x -=3 ; break;
      case right: ant.x +=3 ; break;
    }
  }

  SDL_RenderPresent(renderer);

  while(SDL_TRUE)
    if (SDL_PollEvent(&event) && (event.type==SDL_QUIT || event.type==SDL_KEYDOWN)) break;
    else SDL_Delay(256);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
