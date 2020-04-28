#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS  60

int main(int argc, char **argv) {

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window=SDL_CreateWindow("Lissajous2", SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED, SIZE, SIZE, 0);
  SDL_Renderer *renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_Rect canvas={0, 0, SIZE, SIZE};
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  char title[50];
  const int frameDelay=1000/FPS;
  Uint32 frameStart, frameTime;

  const float Amp = SIZE/2.5;
  float theta=0, offset=.01;
  int  p=1, q=1;

  while(isRunning) {
    frameStart=SDL_GetTicks();

    while(SDL_PollEvent(&event)) {
      if (event.type==SDL_KEYDOWN) {
        if (event.key.keysym.sym==SDLK_q) p++;
        if (event.key.keysym.sym==SDLK_w) q++;
        if (event.key.keysym.sym==SDLK_e) offset+=.01;
        if (event.key.keysym.sym==SDLK_d) offset-=.01;
        if((event.key.keysym.sym==SDLK_a) && (--p<0)) p=0;
        if((event.key.keysym.sym==SDLK_s) && (--q<0)) q=0;
        if (event.key.keysym.sym==SDLK_ESCAPE) isRunning=SDL_FALSE;
      }
      if (event.type==SDL_QUIT) isRunning=SDL_FALSE;
    }

    SDL_SetRenderDrawColor(renderer, 30, 30, 35, 96);
    SDL_RenderFillRect(renderer, &canvas);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);

    sprintf(title, "Lissa p=%d  q=%d", p, q);
    SDL_SetWindowTitle(window, title);

    theta += offset;
    for (float t=.0; t<=6.28318; t+=.001) // de zéro à 2PI
      SDL_RenderDrawPoint(renderer, Amp*sin(p*t)+SIZE/2, Amp*sin(q*t+theta)+SIZE/2);

    frameTime=SDL_GetTicks()-frameStart;
    if (frameDelay>frameTime) SDL_Delay(frameDelay-frameTime);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
