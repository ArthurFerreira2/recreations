#include <SDL2/SDL.h>

int main(int argc, char* args[]){

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  SDL_Window   *window   = SDL_CreateWindow("Lorenz Attractor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 256, 256, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_Rect canvas  = {0, 0, 256, 256};

  SDL_Event event;

  const float s=10, b=8/3, r=28, dt=0.002;
  float x=0, y=1, z=1.05;
  float nx, ny;

  while (SDL_TRUE) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
    SDL_RenderFillRect(renderer, &canvas);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

	  nx = x + s*(y-x)*dt;
	  ny = y + (r*x-y-x*z)*dt;
	  z  = z + (x*y-b*z)*dt;
    x  = nx;
    y  = ny;

    SDL_RenderDrawPoint(renderer, (int)(x*4 + 128), (int)(z*4));
    SDL_RenderPresent(renderer);

    if (SDL_PollEvent(&event) && (event.type==SDL_QUIT || event.type==SDL_KEYDOWN)) break;
    else SDL_Delay(20);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
