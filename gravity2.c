#include <SDL2/SDL.h>

int main(int argc, char **argv) {

  SDL_Init(SDL_INIT_VIDEO );
  SDL_Window *window = SDL_CreateWindow("gravity2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 256, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  struct corp{
    double     m,   x,   y,   vx,   vy, nvx, nvy ;
  } corp1 = {350,  85,  85,  .01,  .01,   0,   0 },
    corp2 = {350, 160,  85, -.01,    0,   0,   0 },
    corp3 = {350, 128, 170,    0, -.01,   0,   0 };

  double D2, F, G =.0000000674;

  while(isRunning) {
    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

  // effet de 2 sur 1
    D2 = ((corp1.x-corp2.x)*(corp1.x-corp2.x)+(corp1.y-corp2.y)*(corp1.y-corp2.y));
    F = corp1.m*corp2.m*G/D2;
    corp1.nvx = (corp1.x > corp2.x) ? -F : F;
    corp1.nvy = (corp1.y > corp2.y) ? -F : F;
  // effet de 1 sur 2
    corp2.nvx = (corp2.x > corp1.x) ? -F : F;
    corp2.nvy = (corp2.y > corp1.y) ? -F : F;
  // effet de 1 sur 3
    D2 = ((corp1.x-corp3.x)*(corp1.x-corp3.x)+(corp1.y-corp3.y)*(corp1.y-corp3.y));
    F = corp3.m*corp1.m*G/D2;
    corp3.nvx = (corp3.x > corp1.x) ? -F : F;
    corp3.nvy = (corp3.y > corp1.y) ? -F : F;
  // effet de 3 sur 1
    corp1.nvx -= (corp1.x > corp3.x) ? F : -F;
    corp1.nvy -= (corp1.y > corp3.y) ? F : -F;
  // effet de 3 sur 2
    D2 = ((corp3.x-corp2.x)*(corp3.x-corp2.x)+(corp3.y-corp2.y)*(corp3.y-corp2.y));
    F = corp2.m*corp3.m*G/D2;
    corp2.nvx -= (corp2.x > corp3.x) ? F : -F;
    corp2.nvy -= (corp2.y > corp3.y) ? F : -F;
  // effet de 2 sur 3
    corp3.nvx -= (corp3.x > corp2.x) ? F : -F;
    corp3.nvy -= (corp3.y > corp2.y) ? F : -F;

  // mise à jour des vitesses
    corp1.vx += corp1.nvx;
    corp1.vy += corp1.nvy;
    corp2.vx += corp2.nvx;
    corp2.vy += corp2.nvy;
    corp3.vx += corp3.nvx;
    corp3.vy += corp3.nvy;

  // mise à jour des positions
    corp1.x += corp1.vx;
    corp1.y += corp1.vy;
    corp2.x += corp2.vx;
    corp2.y += corp2.vy;
    corp3.x += corp3.vx;
    corp3.y += corp3.vy;

  // affichage
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawPoint(renderer, corp1.x, corp1.y);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawPoint(renderer, corp2.x, corp2.y);
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawPoint(renderer, corp3.x, corp3.y);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
