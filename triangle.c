#include <stdlib.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS 600


typedef struct triangle_t {
  SDL_Point a,b,c;
} SDL_Triangle;

void SDL_RenderDrawTriangle(SDL_Renderer *renderer, SDL_Triangle *t) {
  SDL_RenderDrawLine(renderer, t->a.x, t->a.y, t->b.x, t->b.y);
  SDL_RenderDrawLine(renderer, t->b.x, t->b.y, t->c.x, t->c.y);
  SDL_RenderDrawLine(renderer, t->c.x, t->c.y, t->a.x, t->a.y);
}

void SDL_RenderFillTriangle(SDL_Renderer *renderer, SDL_Triangle *t) {

  // sort the 3 points to have A above B and C under B
  SDL_Point *A=&t->a, *B=&t->b, *C=&t->c;
  if (A->y > B->y) { A=B; B=&t->a; }
  if (C->y <= A->y) { C=B; B=A; A=&t->c; }
  else if (C->y < B->y)  { C=B; B=&t->c; }

  // calculate the slopes from A to B, A to C and B to C
	float dx1 = (B->y>A->y) ? (float)(B->x-A->x) / (float)(B->y-A->y) : 0,
	      dx2 = (C->y>A->y) ? (float)(C->x-A->x) / (float)(C->y-A->y) : 0,
	      dx3 = (C->y>B->y) ? (float)(C->x-B->x) / (float)(C->y-B->y) : 0;

  float Sx, Ex, y; // Startx, Endx and y from Ay, to By and then to Cy

  // fill from A to B
	for (y=A->y, Sx=A->x, Ex=A->x; y<=B->y; y++, Sx+=dx2, Ex+=dx1)
    SDL_RenderDrawLine(renderer, Sx, y, Ex, y);
  // fill from B to C
	for (Ex=B->x ; y<=C->y; y++, Sx+=dx2, Ex+=dx3)
    SDL_RenderDrawLine(renderer, Sx, y, Ex, y);
}

int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Triangles", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_Rect canvas = {0, 0, SIZE, SIZE};
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  SDL_Triangle triangle;

  while(isRunning) {
    frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 32);
    SDL_RenderFillRect(renderer, &canvas);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);

    triangle = (SDL_Triangle) {
        .a.x = rand()%(SIZE - 60) + 30,
        .a.y = rand()%(SIZE - 60) + 30,
        .b.x = rand()%(SIZE - 60) + 30,
        .b.y = rand()%(SIZE - 60) + 30,
        .c.x = rand()%(SIZE - 60) + 30,
        .c.y = rand()%(SIZE - 60) + 30
    };

    SDL_RenderFillTriangle(renderer, &triangle);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawTriangle(renderer, &triangle);

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
