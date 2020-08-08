#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS 600

typedef struct circle_t {
  int x, y, r;
} SDL_Circle;

inline SDL_bool collide(SDL_Circle *c1, SDL_Circle *c2) {
  return ((c2->x-c1->x)*(c2->x-c1->x)+(c2->y-c1->y)*(c2->y-c1->y) < (c1->r+c2->r)*(c1->r+c2->r));
}

void SDL_RenderDrawCircle(SDL_Renderer *renderer, SDL_Circle *c) {
  int x=0, y=c->r, p=1-c->r;

  for (x=0; x<=y; x++) {
    if (p<0) p+=2*x ;
    else {
      y-=1;
      p+=2*(x-y);
    }
    SDL_RenderDrawPoint(renderer, c->x+x, c->y-y);
    SDL_RenderDrawPoint(renderer, c->x-x, c->y-y);
    SDL_RenderDrawPoint(renderer, c->x+x, c->y+y);
    SDL_RenderDrawPoint(renderer, c->x-x, c->y+y);
    SDL_RenderDrawPoint(renderer, c->x+y, c->y-x);
    SDL_RenderDrawPoint(renderer, c->x-y, c->y-x);
    SDL_RenderDrawPoint(renderer, c->x+y, c->y+x);
    SDL_RenderDrawPoint(renderer, c->x-y, c->y+x);
  }
}

void SDL_RenderFillCircle(SDL_Renderer *renderer, SDL_Circle *c) {
  int x=0, y=c->r, p=1-c->r;

  for (x=0; x<=y; x++) {
    if (p<0) p+=2*x ;
    else {
      y-=1;
      p+=2*(x-y);
    }
    SDL_RenderDrawLine(renderer, c->x+x, c->y-y, c->x-x, c->y-y);
    SDL_RenderDrawLine(renderer, c->x+x, c->y+y, c->x-x, c->y+y);
    SDL_RenderDrawLine(renderer, c->x+y, c->y-x, c->x-y, c->y-x);
    SDL_RenderDrawLine(renderer, c->x+y, c->y+x, c->x-y, c->y+x);
  }
}

int main(int argc, char **argv) {
  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Circles", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_Rect canvas = {0, 0, SIZE, SIZE};
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  SDL_Circle circle;

  while(isRunning) {
    frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 10);
    SDL_RenderFillRect(renderer, &canvas);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);

    circle.x = rand()%(SIZE-100)+50;
    circle.y = rand()%(SIZE-100)+50;
    circle.r = rand()%(SIZE/10);

    SDL_RenderFillCircle(renderer, &circle);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawCircle(renderer, &circle);


    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
