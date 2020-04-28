// gravity, 3 corps
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>

static const int width = 800;
static const int height = 800;

struct corp{
  double m, x, y, w, h, vx, vy, nvx, nvy;
};

struct corp corp1 = {5000,  200, 200, 5,  5,   .1,  .1,  0, 0 };
struct corp corp2 = {10000, 600, 200, 10, 10,  -.1, .3,  0, 0 };
struct corp corp3 = {10000, 400, 600, 10, 10,  0,   -.4, 0, 0 };

double dx2, dy2;
static const double G =0.0000006674;
SDL_Rect Rect;

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO );
  SDL_Window *window = SDL_CreateWindow("gravity", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

  bool running = true;
  SDL_Event event;

  while(running) {
    while(SDL_PollEvent(&event))
      if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
        running = false;


    // calculs sur 1
    // effet de 2 sur 1
    dx2 = (corp1.x-corp2.x) * (corp1.x-corp2.x);
    dy2 = (corp1.y-corp2.y) * (corp1.y-corp2.y);
    if (corp1.x > corp2.x)
      corp1.nvx = - corp1.m*corp2.m*G/(dx2+dy2);
    else
      corp1.nvx = corp1.m*corp2.m*G/(dx2+dy2);
    if (corp1.y > corp2.y)
      corp1.nvy = - corp1.m*corp2.m*G/(dx2+dy2);
    else
      corp1.nvy = corp1.m*corp2.m*G/(dx2+dy2);
    //printf("corp1.dx2 : %g\n", dx2);
    //printf("corp1.dy2 : %g\n", dy2);
    //printf("corp1.nvx : %g\n", corp1.nvx);
    //printf("corp1.nvy : %g\n", corp1.nvy);

    // effet de 3 sur 1
    dx2 = (corp1.x-corp3.x) * (corp1.x-corp3.x);
    dy2 = (corp1.y-corp3.y) * (corp1.y-corp3.y);
    if (corp1.x > corp3.x)
      corp1.nvx -= corp1.m*corp3.m*G/(dx2+dy2);
    else
      corp1.nvx += corp1.m*corp3.m*G/(dx2+dy2);
    if (corp1.y > corp3.y)
      corp1.nvy -= corp1.m*corp3.m*G/(dx2+dy2);
    else
      corp1.nvy += corp1.m*corp3.m*G/(dx2+dy2);
    //printf("corp1.dx2 : %g\n", dx2);
    //printf("corp1.dy2 : %g\n", dy2);
    //printf("corp1.nvx : %g\n", corp1.nvx);
    //printf("corp1.nvy : %g\n", corp1.nvy);


    // calculs sur 2
    // effet de 1 sur 2
    dx2 = (corp1.x-corp2.x) * (corp1.x-corp2.x);
    dy2 = (corp1.y-corp2.y) * (corp1.y-corp2.y);
    if (corp2.x > corp1.x)
      corp2.nvx = - corp2.m*corp1.m*G/(dx2+dy2);
    else
      corp2.nvx = corp2.m*corp1.m*G/(dx2+dy2);
    if (corp2.y > corp1.y)
      corp2.nvy = - corp2.m*corp1.m*G/(dx2+dy2);
    else
      corp2.nvy = corp2.m*corp1.m*G/(dx2+dy2);
    //printf("corp2.dx2 : %g\n", dx2);
    //printf("corp2.dy2 : %g\n", dy2);
    //printf("corp2.nvx : %g\n", corp2.nvx);
    //printf("corp2.nvy : %g\n", corp2.nvy);

    // effet de 3 sur 2
    dx2 = (corp3.x-corp2.x) * (corp3.x-corp2.x);
    dy2 = (corp3.y-corp2.y) * (corp3.y-corp2.y);
    if (corp2.x > corp3.x)
      corp2.nvx -= corp2.m*corp3.m*G/(dx2+dy2);
    else
      corp2.nvx += corp2.m*corp3.m*G/(dx2+dy2);
    if (corp2.y > corp3.y)
      corp2.nvy -= corp2.m*corp3.m*G/(dx2+dy2);
    else
      corp2.nvy += corp2.m*corp3.m*G/(dx2+dy2);
    //printf("corp2.dx2 : %g\n", dx2);
    //printf("corp2.dy2 : %g\n", dy2);
    //printf("corp2.nvx : %g\n", corp2.nvx);
    //printf("corp2.nvy : %g\n", corp2.nvy);


    // calculs sur 3
    // effet de 2 sur 3
    dx2 = (corp3.x-corp2.x) * (corp3.x-corp2.x);
    dy2 = (corp3.y-corp2.y) * (corp3.y-corp2.y);
    if (corp3.x > corp2.x)
      corp3.nvx = - corp3.m*corp2.m*G/(dx2+dy2);
    else
      corp3.nvx = corp3.m*corp2.m*G/(dx2+dy2);
    if (corp3.y > corp2.y)
      corp3.nvy = - corp3.m*corp2.m*G/(dx2+dy2);
    else
      corp3.nvy = corp3.m*corp2.m*G/(dx2+dy2);
    //printf("corp3.dx2 : %g\n", dx2);
    //printf("corp3.dy2 : %g\n", dy2);
    //printf("corp3.nvx : %g\n", corp3.nvx);
    //printf("corp3.nvy : %g\n", corp3.nvy);

    // effet de 1 sur 3
    dx2 = (corp1.x-corp3.x) * (corp1.x-corp3.x);
    dy2 = (corp1.y-corp3.y) * (corp1.y-corp3.y);
    if (corp3.x > corp1.x)
      corp3.nvx -= corp3.m*corp1.m*G/(dx2+dy2);
    else
      corp3.nvx += corp3.m*corp1.m*G/(dx2+dy2);
    if (corp3.y > corp1.y)
      corp3.nvy -= corp3.m*corp1.m*G/(dx2+dy2);
    else
      corp3.nvy += corp3.m*corp1.m*G/(dx2+dy2);
    //printf("corp3.dx2 : %g\n", dx2);
    //printf("corp3.dy2 : %g\n", dy2);
    //printf("corp3.nvx : %g\n", corp3.nvx);
    //printf("corp3.nvy : %g\n", corp3.nvy);


    corp1.vx += corp1.nvx;
    corp1.vy += corp1.nvy;
    corp2.vx += corp2.nvx;
    corp2.vy += corp2.nvy;
    corp3.vx += corp3.nvx;
    corp3.vy += corp3.nvy;

    corp1.x += corp1.vx;
    corp1.y += corp1.vy;
    corp2.x += corp2.vx;
    corp2.y += corp2.vy;
    corp3.x += corp3.vx;
    corp3.y += corp3.vy;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Rect.x = (int)corp1.x;
    Rect.y = (int)corp1.y;
    Rect.w = (int)corp1.w;
    Rect.h = (int)corp1.h;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &Rect );

    Rect.x = (int)corp2.x;
    Rect.y = (int)corp2.y;
    Rect.w = (int)corp2.w;
    Rect.h = (int)corp2.h;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &Rect );

    Rect.x = (int)corp3.x;
    Rect.y = (int)corp3.y;
    Rect.w = (int)corp3.w;
    Rect.h = (int)corp3.h;
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &Rect );

    SDL_RenderPresent(renderer);
    //SDL_Delay(10);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
