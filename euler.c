#include <math.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define DELTA_A 0.5  // play with these two values.
#define LENGHT 2.0

int main(int argc, char **argv) {

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window   *window   = SDL_CreateWindow("Euler Spiral", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  long double x = SIZE / 10;
  long double y = 9 * SIZE / 10;
  long double nx, ny;
  long double angle = 0;
  long double a = 0.0;
  unsigned char t = 0;

  while(isRunning) {
    a += DELTA_A;
    angle += a;

    nx = x + cos(angle) * LENGHT;
    ny = y + sin(angle) * LENGHT;
    SDL_RenderDrawLine(renderer, (int)x, (int)y, (int)nx, (int)ny);
    x = nx;
    y = ny;

    if (++t == 0)  // only render wvery 256 frames
      SDL_RenderPresent(renderer);

    while(SDL_PollEvent(&event)) if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
