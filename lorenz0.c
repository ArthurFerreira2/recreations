#include <SDL2/SDL.h>

int main(int argc, char* args[]){

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  SDL_Window *window = SDL_CreateWindow("Lorenz Attractor", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 256, 256, 0);
  SDL_Surface *winSurf= SDL_GetWindowSurface(window);
  Uint32 *pixels = winSurf->pixels;
  SDL_Event event;
  const float s=10, b=8/3, r=28, dt=0.002;
  float x=0, y=1, z=1.05;
  float nx, ny;

  while (SDL_TRUE) {
	  nx = x + s*(y-x)*dt;
	  ny = y + (r*x-y-x*z)*dt;
	  z  = z + (x*y-b*z)*dt;
    x  = nx;
    y  = ny;
    pixels[(int)(x*4+128)+(int)(z*4)*256] = SDL_MapRGBA(winSurf->format,255,255,255,255);

    SDL_UpdateWindowSurface(window);
    if (SDL_PollEvent(&event) && (event.type==SDL_QUIT || event.type==SDL_KEYDOWN)) break;
    else SDL_Delay(20);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
