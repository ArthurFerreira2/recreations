#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define CIRCLESNUM 800
#define RADIUSMAX 25
#define RADIUSMIN 1


typedef struct circle_t {
  int x, y, r;
} SDL_Circle;


inline SDL_bool collide(SDL_Circle *c1, SDL_Circle *c2) {
  return ((c2->x-c1->x)*(c2->x-c1->x)+(c2->y-c1->y)*(c2->y-c1->y) <= (c2->r+c1->r)*(c2->r+c1->r));
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

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Circle Packing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Event event;
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  SDL_Circle circles[CIRCLESNUM] = {{0}};
  int tentative = 0, radius = RADIUSMAX;

  for (int i=0; i<CIRCLESNUM; i++) {
    if (tentative > 10000/radius) {
      if (--radius < RADIUSMIN) break;
      else tentative=0;
    }
    circles[i].r = radius;
    circles[i].x = (rand() % (SIZE-2*circles[i].r))+circles[i].r;
    circles[i].y = (rand() % (SIZE-2*circles[i].r))+circles[i].r;
    for (int j=0; j<i; j++)
      if (collide(&circles[i], &circles[j])) {
        i--;
        tentative++;
        break;
      }
    SDL_SetRenderDrawColor(renderer, rand()%55+200, rand()%55+200, rand()%55+200, SDL_ALPHA_OPAQUE);
    SDL_RenderFillCircle(renderer, &circles[i]);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawCircle(renderer, &circles[i]);
    SDL_RenderPresent(renderer);
  }


  while(SDL_TRUE)
    if (SDL_PollEvent(&event) && (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)) break;
    else SDL_Delay(256);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
