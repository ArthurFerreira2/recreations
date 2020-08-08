#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS 1000


typedef struct shape1_t {
  int nbEdges;
  SDL_Point *edges;
} SDL_Shape1;

typedef struct shape2_t {
  int nbEdges;
  SDL_Point *edges;
  SDL_Point center;
} SDL_Shape2;



void SDL_RenderDrawShape1(SDL_Renderer *renderer, SDL_Shape1 *s) {
  SDL_RenderDrawLines(renderer, s->edges, s->nbEdges);
  SDL_RenderDrawLine(renderer, s->edges[0].x, s->edges[0].y, s->edges[s->nbEdges-1].x, s->edges[s->nbEdges-1].y);
}

void SDL_RenderDrawShape2(SDL_Renderer *renderer, SDL_Shape2 *s) {
  int i;
  for (i=0; i<s->nbEdges-1; i++)
    SDL_RenderDrawLine(renderer, s->edges[i].x   + s->center.x , s->edges[i].y  + s->center.y,
                                 s->edges[i+1].x + s->center.x, s->edges[i+1].y + s->center.y);

  SDL_RenderDrawLine(renderer, s->edges[i].x + s->center.x, s->edges[i].y + s->center.y,
                               s->edges[0].x   + s->center.x, s->edges[0].y   + s->center.y);
}

int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Shapes", SDL_WINDOWPOS_CENTERED,
                      SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_Rect canvas = {0, 0, SIZE, SIZE};
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  SDL_Shape1 shape1;
  shape1.nbEdges = 5;
  shape1.edges   = malloc(shape1.nbEdges*sizeof(SDL_Point));

  SDL_Shape2 shape2;
  shape2.nbEdges  = 5;
  shape2.edges    = malloc(shape2.nbEdges*sizeof(SDL_Point));
  shape2.center.x = SIZE/2;
  shape2.center.y = SIZE/2;

  while(isRunning) {
    frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 192);
    SDL_RenderFillRect(renderer, &canvas);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);

    for (int i=0; i<shape1.nbEdges; i++) {
      shape1.edges[i].x = rand()%(SIZE - 60) + 30;
      shape1.edges[i].y = rand()%(SIZE - 60) + 30;
    }
    SDL_RenderDrawShape1(renderer, &shape1);

    for (int i=0; i<shape2.nbEdges; i++) {
      shape2.edges[i].x = (rand()%(SIZE/2)) - (SIZE/4);
      shape2.edges[i].y = (rand()%(SIZE/2)) - (SIZE/4);
    }
    SDL_RenderDrawShape2(renderer, &shape2);

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
