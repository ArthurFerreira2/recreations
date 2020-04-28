// https://www.rosettacode.org/wiki/Fractal_tree#C

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#define PI 3.1415926536
#define SIZE           256 // size of window
#define SCALE          5   // determines how quickly branches shrink (higher value means faster shrinking)
#define BRANCHES       17  // number of branches (depth of the tree)
#define ROTATION_SCALE .75 // determines how slowly the angle between branches shrinks (higher value = slower shrinking)
#define INITLENGTH SIZE/16 // length of first branch, proportionnal to the window size


inline float randF(){
  return (float)rand() / (float)RAND_MAX;
}


void draw_tree(SDL_Renderer *renderer, float offsetx, float offsety,
     float directionx, float directiony, float size, float rotation, int depth) {

  SDL_SetRenderDrawColor(renderer, 0, 200/(depth+1)+50 , 0, 255);
  SDL_RenderDrawLine(renderer, (int)offsetx, (int)offsety,
  (int)(offsetx + directionx * size), (int)(offsety + directiony * size));

  if (depth > 0) {  // draw left branch
    draw_tree(renderer,
        offsetx + directionx * size, offsety + directiony * size,
        directionx * cos(rotation) + directiony * sin(rotation),
        directionx * -sin(rotation) + directiony * cos(rotation),
        size * randF() / SCALE + size * (SCALE - 1) / SCALE,
        rotation * ROTATION_SCALE, depth - 1);

    draw_tree(renderer,  // draw right branch
        offsetx + directionx * size, offsety + directiony * size,
        directionx * cos(-rotation) + directiony * sin(-rotation),
        directionx * -sin(-rotation) + directiony * cos(-rotation),
        size * randF() / SCALE + size * (SCALE - 1) / SCALE,
        rotation * ROTATION_SCALE, depth - 1);
  }
}


int main(int argc, char **argv) {

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Fractal Trees", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SIZE, SIZE, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Event event;

  draw_tree(renderer, SIZE/2, SIZE-10, .0, -1, INITLENGTH, PI/8, BRANCHES);
  SDL_RenderPresent(renderer);

  while(SDL_TRUE) {
    if (SDL_PollEvent(&event) && (event.type==SDL_QUIT || event.type==SDL_KEYDOWN)) break;
    else SDL_Delay(256);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
