#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#define PI 3.14159265359
#define SIZE 256
#define FPS 60
#define CENTER_X SIZE/2
#define CENTER_Y SIZE/2
#define RADIUS 200
#define RES 300


int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  float blob[RES] = {0};
  float blobblob[RES] = {0};
  float p0x, p0y, p1x, p1y, p2x, p2y;
  float deltaA, angle, offsetA = 0;
  SDL_Rect canvas  = {0, 0, SIZE, SIZE};

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window   *window   = SDL_CreateWindow("HYPNOSE", SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED, SIZE, SIZE, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  for (int i = 0; i < RES; i++)
    blob[i] = ((float)rand() / (float)RAND_MAX) * 80 - 160;

  while(isRunning) {
    frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 10);
    SDL_RenderFillRect(renderer, &canvas);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    for (int i = 0; i < RES; i++)
      blobblob[i] = ((float)rand() / (float)RAND_MAX) * 20 - 40;

    offsetA += 0.001;
    if (offsetA > 2 * PI ) offsetA = 0;
    deltaA = 2 * PI / RES;

    p0x = CENTER_X + (RADIUS + blob[0] + blobblob[0]) * cos(offsetA);
    p0y = CENTER_Y + (RADIUS + blob[0] + blobblob[0]) * sin(offsetA);
    p2x = p0x;
    p2y = p0y;

    for (int i = 1; i < RES; i++) {
      angle = i * deltaA + offsetA;
      p1x = p2x;
      p1y = p2y;
      p2x = CENTER_X + (RADIUS + blob[i] + blobblob[i]) * cos(angle);
      p2y = CENTER_Y + (RADIUS + blob[i] + blobblob[i]) * sin(angle);
      SDL_RenderDrawLine(renderer, (int)p1x, (int)p1y, (int)p2x, (int)p2y);
    }
    SDL_RenderDrawLine(renderer, (int)p2x, (int)p2y, (int)p0x, (int)p0y);

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime)
      SDL_Delay(frameDelay - frameTime);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
