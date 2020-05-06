#include <math.h>
#include <SDL2/SDL.h>


#define SIZE 256
#define LEN (SIZE-(SIZE/5))
#define PI 3.1415926536
#define theta PI/3


void koch(SDL_Renderer *renderer, float p1x, float p1y, float p2x, float p2y, int depth) {
  if (depth>0){
    float p3x = (2 * p1x + p2x) / 3;
    float p3y = (2 * p1y + p2y) / 3;
		float p5x = (2 * p2x + p1x) / 3;
    float p5y = (2 * p2y + p1y) / 3;
		float p4x = p3x + (p5x - p3x) * cos(theta) + (p5y - p3y) * sin(theta);
    float p4y = p3y - (p5x - p3x) * sin(theta) + (p5y - p3y) * cos(theta);

		koch(renderer, p1x, p1y, p3x, p3y, depth-1);
		koch(renderer, p3x, p3y, p4x, p4y, depth-1);
		koch(renderer, p4x, p4y, p5x, p5y, depth-1);
		koch(renderer, p5x, p5y, p2x, p2y, depth-1);
  }
  else SDL_RenderDrawLine(renderer, p1x, p1y, p2x, p2y);
}


int main(int argc, char **argv) {

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Koch", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, SIZE, SIZE, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Event event;

  float p1x = (SIZE - LEN) / 2;
  float p1y = (SIZE - LEN * sin(theta)) / 1.1;
  float p2x = p1x + LEN;
  float p2y = p1y;
  float p3x = p1x+cos(theta)*LEN;
  float p3y = p1y+sin(theta)*LEN;

  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 32);
  koch(renderer, p1x, p1y, p2x, p2y, 4);
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 32);
  koch(renderer, p2x, p2y, p3x, p3y, 4);
  SDL_SetRenderDrawColor(renderer, 0, 255, 255, 32);
  koch(renderer, p3x, p3y, p1x, p1y, 4);

  SDL_RenderPresent(renderer);

  while(SDL_TRUE)
    if (SDL_PollEvent(&event) && (event.type==SDL_QUIT || event.type==SDL_KEYDOWN)) break;
    else SDL_Delay(256);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
