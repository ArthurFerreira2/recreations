#include <SDL2/SDL.h>

int main(int argc, char* args[]){

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  SDL_Window  *window  = SDL_CreateWindow("Lorenz Attractor", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, 256, 256, 0);
  SDL_Surface *winSurf = SDL_GetWindowSurface(window);
  Uint32      *pixels  = winSurf->pixels;

  SDL_Event event;
  SDL_bool isRunning = SDL_TRUE;

  const float s=10, b=8/3, r=28, dt=0.002;
  float x=0, y=1, z=1.05;
  float nx, ny;

  while (isRunning) {

	  nx = x + s*(y-x)*dt;
	  ny = y + (r*x-y-x*z)*dt;
	  z  = z + (x*y-b*z)*dt;
    x  = nx;
    y  = ny;

  pixels[(int)(x*4 + 128) + (int)(z*4) * 256] = SDL_MapRGBA(winSurf->format, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_UpdateWindowSurface(window);
    while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) isRunning = SDL_FALSE;
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
