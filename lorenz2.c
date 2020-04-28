#include <SDL2/SDL.h>

typedef struct greyedPoint_t{
  int x, y;
  unsigned char greyLevel;
} greyedPoint;


int main(int argc, char* args[]){
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window   *window   = SDL_CreateWindow("Lorenz Attractor", SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED, 256, 256, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  float s=10, b=8/3, r=28, dt=0.002;
  float x=0, y=1, z=1.05;
  float nx, ny;
  unsigned char currentPoint=0;
  greyedPoint lorenz[256]={{ 0 }};

  while (isRunning) {

	  nx = x + s*(y-x)*dt;
	  ny = y + (r*x-y-x*z)*dt;
	  z  = z + (x*y-b*z)*dt;
    x  = nx;
    y  = ny;

    lorenz[currentPoint].x=(int)(x*4 + 128);
    lorenz[currentPoint].y=(int)(z*4);
    lorenz[currentPoint++].greyLevel=255;

    for(int i=0; i<256; i++) {
      SDL_SetRenderDrawColor(renderer, lorenz[i].greyLevel, lorenz[i].greyLevel, lorenz[i].greyLevel, 255);
      SDL_RenderDrawPoint(renderer, lorenz[i].x, lorenz[i].y);
      if (lorenz[i].greyLevel > 50) lorenz[i].greyLevel--;
    }
    SDL_RenderPresent(renderer);
    while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) isRunning = SDL_FALSE;
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
