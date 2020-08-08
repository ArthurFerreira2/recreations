// https://lodev.org/cgtutor/plasma.html

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define FPS 60


typedef struct rgbColor_t {
    int r;
    int g;
    int b;
} rgbColor;


typedef struct hsvColor_t {
    int h;
    int s;
    int v;
} hsvColor;


void hsv2rgb(hsvColor hsv, rgbColor* rgb) {
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0) {
        rgb->r = hsv.v;
        rgb->g = hsv.v;
        rgb->b = hsv.v;
        return;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6;

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region) {
        case 0:
            rgb->r = hsv.v; rgb->g = t; rgb->b = p;
            break;
        case 1:
            rgb->r = q; rgb->g = hsv.v; rgb->b = p;
            break;
        case 2:
            rgb->r = p; rgb->g = hsv.v; rgb->b = t;
            break;
        case 3:
            rgb->r = p; rgb->g = q; rgb->b = hsv.v;
            break;
        case 4:
            rgb->r = t; rgb->g = p; rgb->b = hsv.v;
            break;
        default:
            rgb->r = hsv.v; rgb->g = p; rgb->b = q;
            break;
    }
    return;
}



int main(int argc, char **argv) {

  const int frameDelay = 1000 / FPS;
  Uint32 frameStart, frameTime;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("plasma", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, SIZE, SIZE, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_bool isRunning = SDL_TRUE;
  SDL_Event event;

  hsvColor hsv = {0, 255, 255};
  rgbColor rgb;
  Uint8 plasma[SIZE][SIZE];
  Uint8 R[256], G[256], B[256];
  Uint8 idx, paletteShift = 0;

  for(hsv.h=0; hsv.h<256; hsv.h++) {
    hsv2rgb(hsv, &rgb );
    R[hsv.h] = rgb.r;
    G[hsv.h] = rgb.g;
    B[hsv.h] = rgb.b;
  }

  for (int x=0; x<SIZE; x++)
    for (int y=0; y<SIZE; y++)
      plasma[y][x] = (int)(((128+(128*sin(x/32.0))) + (128+(128*cos(y/32.0)))
                          + (128+(128*sin(sqrt((x*x+y*y))/32.0))))/4);

  while(isRunning) {

    frameStart = SDL_GetTicks();

    while(SDL_PollEvent(&event))
      if (event.type==SDL_QUIT || event.type==SDL_KEYDOWN) isRunning=SDL_FALSE;

    paletteShift++;
    for (int x=0; x<SIZE; x++)
      for (int y=0; y<SIZE; y++) {
        idx = plasma[y][x] + paletteShift;
        SDL_SetRenderDrawColor(renderer, R[idx], G[idx], B[idx], SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(renderer, x, y);
      }

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  exit(EXIT_SUCCESS);
}
